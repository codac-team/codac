/* ============================================================================
 *  tubex-lib - Tube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Tube.h"
#include "tubex_Exception.h"
#include "tubex_DomainException.h"
#include "tubex_TubeSerialization.h"
#include "tubex_TrajectorySerialization.h"
#include "tubex_CtcDeriv.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    Tube::Tube(const Interval& domain, const Interval& codomain) : TubeTree(domain, codomain)
    {

    }
    
    Tube::Tube(const Interval& domain, double timestep, const Interval& codomain) : TubeTree(domain, codomain)
    {
      if(timestep < 0.)
        throw Exception("Tube constructor", "invalid timestep");

      else if(timestep == 0.)
      {
        // then the tube is defined as one single slice
      }

      else if(timestep < domain.diam())
      {
        double lb, ub = domain.lb();
        vector<double> v_bounds; // a vector of slices is created only once
        do
        {
          lb = ub; // we guarantee all slices are adjacent
          ub = lb + timestep;
          if(ub < domain.ub()) v_bounds.push_back(ub);
        } while(ub < domain.ub());

        sample(v_bounds);
      }
    }
    
    Tube::Tube(const Interval& domain, double timestep, const Function& function) : Tube(domain, timestep)
    {
      TubeSlice *slice = getFirstSlice();
      while(slice != NULL)
      {
        IntervalVector iv_domain(1, slice->domain());
        slice->set(function.eval(iv_domain));
        slice = slice->nextSlice();
      }
    }

    Tube::Tube(const Tube& x, const Interval& codomain) : TubeTree(x, codomain)
    {

    }

    Tube::Tube(const Trajectory& traj, double thickness, double timestep) : Tube(traj.domain(), timestep, Interval::EMPTY_SET)
    {
      *this |= traj;
      inflate(thickness);
    }

    Tube::Tube(const Trajectory& lb, const Trajectory& ub, double timestep) : Tube(lb.domain(), timestep, Interval::EMPTY_SET)
    {
      *this |= lb;
      *this |= ub;
    }

    Tube::Tube(const string& binary_file_name) : TubeTree(Interval(0.))
    {
      vector<Trajectory> v_trajs;
      deserialize(binary_file_name, v_trajs);
    }
    
    Tube::Tube(const string& binary_file_name, Trajectory& traj) : TubeTree(Interval(0.))
    {
      vector<Trajectory> v_trajs;
      deserialize(binary_file_name, v_trajs);

      if(v_trajs.size() == 0)
        throw Exception("Tube constructor", "unable to deserialize a Trajectory");

      traj = v_trajs[0];
    }

    Tube::Tube(const string& binary_file_name, vector<Trajectory>& v_trajs) : TubeTree(Interval(0.))
    {
      deserialize(binary_file_name, v_trajs);
      if(v_trajs.size() == 0)
        throw Exception("Tube constructor", "unable to deserialize a Trajectory");
    }
    
    Tube Tube::primitive(const Interval& initial_value) const
    {
      Tube primitive(*this, Interval::ALL_REALS);
      primitive.set(primitive.domain().lb(), initial_value);
      primitive.ctcFwd(*this);
      return primitive;
    }
    
    // Bisection


    // String
    
    ostream& operator<<(ostream& str, const Tube& x)
    {
      str << "Tube " << x.domain() << "↦" << x.codomain() << flush;
      return str;
    }

    // Integration

    Interval Tube::integral(double t) const
    {
      return integral(Interval(t));
    }

    Interval Tube::integral(const Interval& t) const
    {
      pair<Interval,Interval> partial_ti = partialIntegral(t);
      return Interval(partial_ti.first.lb(), partial_ti.second.ub());
    }

    Interval Tube::integral(const Interval& t1, const Interval& t2) const
    {
      pair<Interval,Interval> integral_t1 = partialIntegral(t1);
      pair<Interval,Interval> integral_t2 = partialIntegral(t2);
      double lb = (integral_t2.first - integral_t1.first).lb();
      double ub = (integral_t2.second - integral_t1.second).ub();
      return Interval(min(lb, ub), max(lb, ub));
    }

    pair<Interval,Interval> Tube::partialIntegral(const Interval& t) const
    {
      checkPartialPrimitive();
      
      int index_lb = input2index(t.lb());
      int index_ub = input2index(t.ub());

      Interval integral_lb = Interval::EMPTY_SET;
      Interval integral_ub = Interval::EMPTY_SET;

      Interval intv_t_lb = sliceDomain(index_lb);
      Interval intv_t_ub = sliceDomain(index_ub);

      // Part A
      {
        pair<Interval,Interval> partial_primitive_first = getSlice(index_lb)->getPartialPrimitiveValue();
        Interval primitive_lb = Interval(partial_primitive_first.first.lb(), partial_primitive_first.second.ub());

        Interval y_first = (*this)[index_lb];
        Interval ta1 = Interval(intv_t_lb.lb(), t.lb());
        Interval ta2 = Interval(intv_t_lb.lb(), min(t.ub(), intv_t_lb.ub()));
        Interval tb1 = Interval(t.lb(), intv_t_lb.ub());
        Interval tb2 = Interval(min(t.ub(), intv_t_lb.ub()), intv_t_lb.ub());

        if(y_first.lb() < 0)
          integral_lb |= Interval(primitive_lb.lb() - y_first.lb() * tb2.diam(),
                                  primitive_lb.lb() - y_first.lb() * tb1.diam());

        else if(y_first.lb() > 0)
          integral_lb |= Interval(primitive_lb.lb() + y_first.lb() * ta1.diam(),
                                  primitive_lb.lb() + y_first.lb() * ta2.diam());

        if(y_first.ub() < 0)
          integral_ub |= Interval(primitive_lb.ub() + y_first.ub() * ta2.diam(),
                                  primitive_lb.ub() + y_first.ub() * ta1.diam());

        else if(y_first.ub() > 0)
          integral_ub |= Interval(primitive_lb.ub() - y_first.ub() * tb1.diam(),
                                  primitive_lb.ub() - y_first.ub() * tb2.diam());
      }

      // Part B
      if(index_ub - index_lb > 1)
      {
        pair<Interval,Interval> partial_primitive = getPartialPrimitiveValue(Interval(intv_t_lb.ub(), intv_t_ub.lb()));
        integral_lb |= partial_primitive.first;
        integral_ub |= partial_primitive.second;
      }

      // Part C
      if(index_lb != index_ub)
      {
        pair<Interval,Interval> partial_primitive_second = getSlice(index_ub)->getPartialPrimitiveValue();
        Interval primitive_ub = Interval(partial_primitive_second.first.lb(), partial_primitive_second.second.ub());

        Interval y_second = (*this)[index_ub];
        Interval ta = Interval(intv_t_ub.lb(), t.ub());
        Interval tb1 = intv_t_ub;
        Interval tb2 = Interval(t.ub(), intv_t_ub.ub());

        if(y_second.lb() < 0)
          integral_lb |= Interval(primitive_ub.lb() - y_second.lb() * tb2.diam(),
                                  primitive_ub.lb() - y_second.lb() * tb1.diam());

        else if(y_second.lb() > 0)
          integral_lb |= Interval(primitive_ub.lb(),
                                  primitive_ub.lb() + y_second.lb() * ta.diam());

        if(y_second.ub() < 0)
          integral_ub |= Interval(primitive_ub.ub() + y_second.ub() * ta.diam(),
                                  primitive_ub.ub());

        else if(y_second.ub() > 0)
          integral_ub |= Interval(primitive_ub.ub() - y_second.ub() * tb1.diam(),
                                  primitive_ub.ub() - y_second.ub() * tb2.diam());
      }

      return make_pair(integral_lb, integral_ub);
    }

    pair<Interval,Interval> Tube::partialIntegral(const Interval& t1, const Interval& t2) const
    {
      pair<Interval,Interval> integral_t1 = partialIntegral(t1);
      pair<Interval,Interval> integral_t2 = partialIntegral(t2);
      return make_pair((integral_t2.first - integral_t1.first),
                       (integral_t2.second - integral_t1.second));
    }

    // Contractors

    bool Tube::ctcFwd(const Tube& derivative)
    {
      CtcDeriv ctc;
      return ctc.contractFwd(*this, derivative);
    }

    bool Tube::ctcBwd(const Tube& derivative)
    {
      CtcDeriv ctc;
      return ctc.contractBwd(*this, derivative);
    }

    bool Tube::ctcFwdBwd(const Tube& derivative)
    {
      CtcDeriv ctc;
      return ctc.contract(*this, derivative);
    }
      
    // Serialization

    /*
      Tube binary files structure (VERSION 2)
        - minimal storage
        - format: [tube]
                  [int_nb_trajectories]
                  [traj1]
                  [traj2]
                  ...
    */

    void Tube::serialize(const string& binary_file_name, int version_number) const
    {
      vector<Trajectory> v_trajs;
      serialize(binary_file_name, v_trajs, version_number);
    }

    void Tube::serialize(const string& binary_file_name, const Trajectory& traj, int version_number) const
    {
      vector<Trajectory> v_trajs;
      v_trajs.push_back(traj);
      serialize(binary_file_name, v_trajs, version_number);
    }
    
    void Tube::serialize(const string& binary_file_name, const vector<Trajectory>& v_trajs, int version_number) const
    {
      ofstream bin_file(binary_file_name.c_str(), ios::out | ios::binary);

      if(!bin_file.is_open())
        throw Exception("Tube::serialize()", "error while writing file \"" + binary_file_name + "\"");

      serializeTube(bin_file, *this, version_number);

      int nb_trajs = v_trajs.size();
      bin_file.write((const char*)&nb_trajs, sizeof(int));
      for(int i = 0 ; i < v_trajs.size() ; i++)
        serializeTrajectory(bin_file, v_trajs[i], version_number);

      bin_file.close();
    }

  // Protected methods

    // Integration

    void Tube::checkPartialPrimitive() const
    {
      // Warning: this method can only be called from the root (Tube class)
      // (because computation starts from 0)

      if(m_primitive_update_needed)
      {
        Interval sum_max = Interval(0);

        TubeSlice *slice = getFirstSlice();
        while(slice != NULL)
        {
          double dt = slice->domain().diam();
          Interval slice_codomain = slice->codomain();
          Interval integral_value = sum_max + slice_codomain * Interval(0., dt);
          slice->m_partial_primitive = make_pair(Interval(integral_value.lb(), integral_value.lb() + fabs(slice_codomain.lb() * dt)),
                                                 Interval(integral_value.ub() - fabs(slice_codomain.ub() * dt), integral_value.ub()));
          slice->m_primitive_update_needed = true;
          sum_max += slice_codomain * dt;
          slice = slice->nextSlice();
        }

        TubeTree::checkPartialPrimitive(); // updating nodes from leafs information
      }
    }

    // Serialization

    void Tube::deserialize(const string& binary_file_name, vector<Trajectory>& v_trajs)
    {
      ifstream bin_file(binary_file_name.c_str(), ios::in | ios::binary);

      if(!bin_file.is_open())
        throw Exception("Tube::deserialize()", "error while opening file \"" + binary_file_name + "\"");

      deserializeTube(bin_file, *this);

      int nb_trajs;
      bin_file.read((char*)&nb_trajs, sizeof(int));
      for(int i = 0 ; i < nb_trajs ; i++)
      {
        Trajectory traj;
        deserializeTrajectory(bin_file, traj);
        v_trajs.push_back(traj);
      }

      bin_file.close();
    }
}