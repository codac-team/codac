/* ============================================================================
 *  tubex-lib - Subtube class (base methods)
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Subtube.h"
#include "ibex.h"
#include "exceptions/tubex_Exception.h"
#include "exceptions/tubex_DomainException.h"
#include "exceptions/tubex_EmptyException.h"
#include "exceptions/tubex_StructureException.h"
#include <iostream>
#include <limits>
#include <iomanip> // for setprecision()
#ifdef _OPENMP
  #include <omp.h> // for multithreading
#endif

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

      Subtube::Subtube()
      {
        
      }
      
      Subtube::Subtube(const Interval& domain, const Interval& value) : m_domain(domain), m_image(value)
      {
        if(domain.is_unbounded())
          throw Exception("Subtube constructor", "unbounded domain");
      }

      Subtube::Subtube(const Interval& domain, double timestep, const Interval& value) : m_domain(domain), m_image(value)
      {
        if(domain.is_unbounded())
          throw Exception("Subtube constructor", "unbounded domain");
        if(timestep <= 0.)
          throw Exception("Subtube constructor", "invalid timestep");
        sampleSlice(timestep);
      }

      Subtube::~Subtube()
      {
        if(m_first_subtube != NULL) delete m_first_subtube;
        if(m_second_subtube != NULL) delete m_second_subtube;
      }
      
      const Interval& Subtube::sliceDomain(int slice_id) const
      {
        return getSlice(slice_id)->domain();
      }
      
      const Interval& Subtube::sliceDomain(double t) const
      {
        return getSlice(t)->domain();
      }
      
      void Subtube::sampleSlice(double timestep)
      {
        if(!isSlice())
          throw Exception("Subtube::sampleSlice()", "already sampled");
        if(timestep < 0. || timestep >= domain().diam())
          throw Exception("Subtube::sampleSlice()", "invalid timestep");

        if(timestep == 0.) // default value: divide a slice into two identical parts
          timestep = domain().diam() / 2;

        double lb, ub = domain().lb();
        vector<Interval> v_slices_domains; // a vector of subslices
        do
        {
          lb = ub; // we guarantee all slices are adjacent
          ub = min(lb + timestep, domain().ub());
          v_slices_domains.push_back(Interval(lb, ub));
        } while(ub < domain().ub());

        sampleSlice(v_slices_domains);
      }

      void Subtube::sampleSlice(const vector<Interval>& v_slices_domains)
      {
        if(!isSlice())
          throw Exception("Subtube::sampleSlice()", "already sampled");
        if(v_slices_domains.size() == 0)
          throw Exception("Subtube::sampleSlice()", "invalid subdomains");

        vector<Interval> v_slices_domains_first, v_slices_domains_second;

        IntervalVector subdomain(1, domain());
        LargestFirst bisector(0., 0.5); // todo: use more generic class
        pair<IntervalVector,IntervalVector> p_subdomains = bisector.bisect(subdomain);

        // Slices are spread equitably in the tree, according to their domain
        for(int i = 0 ; i < v_slices_domains.size() ; i++)
        {
          if((v_slices_domains[i] & p_subdomains.first[0]).diam() >= (v_slices_domains[i] & p_subdomains.second[0]).diam())
            v_slices_domains_first.push_back(v_slices_domains[i]);

          else
            v_slices_domains_second.push_back(v_slices_domains[i]);
        }

        m_first_subtube = new Subtube(v_slices_domains_first, image());
        m_second_subtube = new Subtube(v_slices_domains_second, image());
        m_slices_number = v_slices_domains.size();
      }

    // Access values

      double Subtube::volume() const
      {
        updateTree();
        return m_volume;
      }
      
      const Interval& Subtube::operator[](int slice_id) const
      {
        // Write access is not allowed for this operator:
        // a further call to updateTree() is needed when values change,
        // this call cannot be garanteed with a direct access to m_image
        // For write access: use set()
        return getSlice(slice_id)->image();
      }
      
      Interval Subtube::operator[](double t) const
      {
        // Write access is not allowed for this operator:
        // a further call to updateTree() is needed when values change,
        // this call cannot be garanteed with a direct access to m_image
        // For write access: use set()
        int slice_id = input2index(t);
        Interval intv_t = getSlice(slice_id)->domain();
        Interval intv_y = (*this)[slice_id];
        if(t == intv_t.lb() && slice_id > 0) // on the boundary, between two slices
          return (*this)[slice_id - 1] & intv_y;
        return intv_y;
      }
      
      Interval Subtube::operator[](const Interval& t) const
      {
        // Write access is not allowed for this operator:
        // a further call to updateTree() is needed when values change,
        // this call cannot be garanteed with a direct access to m_image
        // For write access: use set()

        if(t.lb() == t.ub())
          return (*this)[t.lb()];

        Interval intersection = m_domain & t;

        if(intersection.is_empty())
          return Interval::EMPTY_SET;

        else if(isSlice() || t == m_domain || t.is_unbounded() || t.is_superset(m_domain))
          return image();

        else
        {
          Interval inter_firstsubtube = m_first_subtube->domain() & intersection;
          Interval inter_secondsubtube = m_second_subtube->domain() & intersection;

          if(inter_firstsubtube == inter_secondsubtube)
            return (*m_first_subtube)[inter_firstsubtube.lb()] & (*m_second_subtube)[inter_secondsubtube.lb()];
          // todo: test, replace by return (*this)[inter_firstsubtube.lb()];

          else if(inter_firstsubtube.lb() == inter_firstsubtube.ub()
                  && inter_secondsubtube.lb() != inter_secondsubtube.ub())
            return (*m_second_subtube)[inter_secondsubtube];

          else if(inter_firstsubtube.lb() != inter_firstsubtube.ub()
                  && inter_secondsubtube.lb() == inter_secondsubtube.ub())
            return (*m_first_subtube)[inter_firstsubtube];

          else
            return (*m_first_subtube)[inter_firstsubtube] | (*m_second_subtube)[inter_secondsubtube];
        }
      }
      
      const pair<Interval,Interval> Subtube::eval(const Interval& t) const
      {
        if(t.lb() == t.ub())
        {
          Interval front = (*this)[t.lb()];
          return make_pair(Interval(front.lb()), Interval(front.ub()));
        }

        Interval intersection = m_domain & t;

        if(intersection.is_empty())
          return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

        else if(isSlice() || t == m_domain || t.is_unbounded() || t.is_superset(m_domain))
        {
          updateTree();
          return m_enclosed_bounds; // pre-computed
        }

        else
        {
          Interval inter_firstsubtube = m_first_subtube->domain() & intersection;
          Interval inter_secondsubtube = m_second_subtube->domain() & intersection;

          if(inter_firstsubtube.lb() == inter_firstsubtube.ub() && inter_secondsubtube.lb() == inter_secondsubtube.ub())
            return make_pair((*m_first_subtube)[inter_firstsubtube.lb()] & (*m_second_subtube)[inter_secondsubtube.lb()],
                             (*m_first_subtube)[inter_firstsubtube.ub()] & (*m_second_subtube)[inter_secondsubtube.ub()]);
          // todo: check simplification:
          //{
          //  Interval front = (*m_first_subtube)[inter_firstsubtube.lb()];
          //  return make_pair(Interval(front.lb()), Interval(front.ub()));
          //}

          else if(inter_firstsubtube.is_empty() || inter_firstsubtube.lb() == inter_firstsubtube.ub())
            return m_second_subtube->eval(inter_secondsubtube);

          else if(inter_secondsubtube.is_empty() || inter_secondsubtube.lb() == inter_secondsubtube.ub())
            return m_first_subtube->eval(inter_firstsubtube);

          else
          {
            pair<Interval,Interval> ui_first = m_first_subtube->eval(inter_firstsubtube);
            pair<Interval,Interval> ui_second = m_second_subtube->eval(inter_secondsubtube);
            return make_pair(ui_first.first | ui_second.first, ui_first.second | ui_second.second);
          }
        }
      }
      
      double Subtube::maxThickness(int& first_id_max_thickness) const
      {
        first_id_max_thickness = -1;
        float thickness = std::numeric_limits<double>::quiet_NaN();
        for(int i = nbSlices()-1 ; i >= 0 ; i--) // usually, tubes are thicker at the end
          if((*this)[i].diam() > thickness || std::isnan(thickness))
          {
            thickness = (*this)[i].diam();
            first_id_max_thickness = i;
          }

        return thickness;
      }
      
    // Tests

      
      bool Subtube::operator==(const Subtube& x) const
      {
        return (isSlice() && x.isSlice() && image() == x.image() && domain() == x.domain())
            || (!isSlice() && *getFirstSubtube() == *x.getFirstSubtube() && *getSecondSubtube() == *x.getSecondSubtube());
      }
      
      bool Subtube::operator!=(const Subtube& x) const
      {
        return image() != x.image()
            || domain() != x.domain()
            || (!isSlice() && !x.isSlice() && (*getFirstSubtube() != *x.getFirstSubtube() || *getSecondSubtube() != *x.getSecondSubtube()));
      }

    // Setting values

      void Subtube::set(const Interval& y)
      {
        if(isSlice())
          m_image = y;

        else
        {
          m_first_subtube->set(y);
          m_second_subtube->set(y);
          flagFutureTreeUpdate();
        }
      }

      void Subtube::set(const Interval& y, int slice_id)
      {
        getSlice(slice_id)->set(y);
        flagFutureTreeUpdate(slice_id);
      }
      
      void Subtube::setEmpty()
      {
        set(Interval::EMPTY_SET);
      }
      
      bool Subtube::contract(const Interval& y, int slice_id)
      {
        Subtube *slice = getSlice(slice_id);
        Interval old_y = slice->m_image;
        Interval new_y = old_y & y;
        slice->set(new_y);
        return new_y.diam() < old_y.diam();
      }
      
      Subtube& Subtube::inflate(const Interval& y, int slice_id)
      {
        set(y | (*this)[slice_id], slice_id);
      }
      
      Subtube& Subtube::inflate(const Interval& y, double t)
      {
        int slice_id = input2index(t);
        inflate(y, slice_id);
        if(sliceDomain(slice_id).lb() == t && slice_id > 0)
          inflate(y, slice_id - 1);
      }
      
      Subtube& Subtube::inflate(double rad)
      {
        for(int i = 0 ; i < nbSlices() ; i++)
        {
          Interval old_y = (*this)[i];
          set(old_y.inflate(rad), i);
        }
        return *this;
      }

    // Operators

      Subtube& Subtube::operator=(const Subtube& x)
      {
        if(m_first_subtube != NULL) delete m_first_subtube;
        if(m_second_subtube != NULL) delete m_second_subtube;

        m_domain = x.domain();
        m_image = x.image();
        m_slices_number = x.nbSlices();
        m_enclosed_bounds = x.eval();
        m_volume = x.volume();
        m_tree_computation_needed = x.treeUpdateNeeded();
        m_primitive_computation_needed = x.primitiveUpdateNeeded();

        if(x.isSlice())
        {
          m_first_subtube = NULL;
          m_second_subtube = NULL;
        }

        else
        {
          m_first_subtube = new Subtube(*(x.getFirstSubtube()));
          m_second_subtube = new Subtube(*(x.getSecondSubtube()));
        }

        return *this;
      }
      
      Subtube& Subtube::operator|=(const Subtube& x)
      {
        // Case of tubes of different structure
        // todo: better check of different structure. The following is not reliable
        if(nbSlices() != x.nbSlices() || domain() != x.domain())
        {
          for(int i = 0 ; i < nbSlices() ; i++)
          {
            Interval this_t = sliceDomain(i);
            Interval this_y = (*this)[i];

            Interval x_t = x.domain() & this_t;

            if(!x_t.is_empty())
            {
              Interval x_y = x[x_t];
              set(this_y | x_y, i);
            }
          }
        }

        else // fast union
        {
          // todo: study the strategy of union: all nodes or slices only with further updates

          vector<Subtube*> this_nodes;
          vector<const Subtube*> x_nodes;
          getSubtubeNodes(this_nodes);
          x.getSubtubeNodes(x_nodes);

          for(int i = 0 ; i < this_nodes.size() ; i++)
            this_nodes[i]->unionWith_localUpdate(x_nodes[i]);
        }

        // todo: check the usefulness of the following
        flagFutureTreeUpdate();
        return *this;
      }
      
      Subtube& Subtube::operator|=(const Trajectory& x)
      {
        // todo: study the strategy of union: all nodes or slices only with further updates
          
        for(int i = 0 ; i < nbSlices() ; i++)
          inflate(x[sliceDomain(i)], i);
      }
      
      Subtube& Subtube::operator&=(const Subtube& x)
      {
        // todo: study the strategy of union: all nodes or slices only with further updates

        // Case of tubes of different structure
        // todo: better check of different structure. The following is not reliable
        if(nbSlices() != x.nbSlices() || domain() != x.domain())
        {
          for(int i = 0 ; i < nbSlices() ; i++)
          {
            Interval this_t = sliceDomain(i);
            Interval this_y = (*this)[i];

            Interval x_t = x.domain() & this_t;

            if(!x_t.is_empty())
            {
              Interval x_y = x[x_t];
              set(this_y & x_y, i);
            }
          }
        }

        else // fast union
        {
          // todo: study the strategy of union: all nodes or slices only with further updates

          vector<Subtube*> this_nodes;
          vector<const Subtube*> x_nodes;
          getSubtubeNodes(this_nodes);
          x.getSubtubeNodes(x_nodes);

          for(int i = 0 ; i < this_nodes.size() ; i++)
            this_nodes[i]->intersectWith_localUpdate(x_nodes[i]);
        }

        // todo: check the usefulness of the following
        flagFutureTreeUpdate();
        return *this;
      }

    // String

      ostream& operator<<(ostream& str, const Subtube& x)
      {
        cout << "Subtube: t=" << x.domain() << ", y=" << x.image() << flush;
        return str;
      }

  // Protected methods

    // Definition

      Subtube::Subtube(const vector<Interval>& v_slices_domains, const Interval& value) : m_image(value)
      {
        if(v_slices_domains.size() == 0)
          throw Exception("Subtube constructor", "invalid subdomains");

        m_domain.set_empty();
        for(int i = 0 ; i < v_slices_domains.size() ; i++)
          m_domain |= v_slices_domains[i];

        sampleSlice(v_slices_domains);
      }

    // Slices/tree structure
      
      const Subtube* Subtube::getFirstSubtube() const
      {
        return m_first_subtube;
      }
          
      const Subtube* Subtube::getSecondSubtube() const
      {
        return m_second_subtube;
      }
          
      void Subtube::updateTree() const
      {
        if(!treeUpdateNeeded())
          return;

        if(isSlice())
        {
          // todo: volume of sliceBox()?
          m_volume = m_domain.diam();

          if(m_image.is_empty()) // ibex::diam(EMPTY_SET) is not 0, todo: check this
            m_volume = 0.;

          else if(m_image.is_unbounded())
            m_volume = INFINITY;

          else
            m_volume *= m_image.diam();

          m_enclosed_bounds = make_pair(Interval(m_image.lb()), Interval(m_image.ub()));
        }

        else
        {
          m_first_subtube->updateTree();
          m_second_subtube->updateTree();
          
          m_image = m_first_subtube->image() | m_second_subtube->image();
          m_volume = m_first_subtube->volume() + m_second_subtube->volume();

          // Enclosed bounds
          pair<Interval,Interval> ui_past = m_first_subtube->eval();
          pair<Interval,Interval> ui_future = m_second_subtube->eval();
          m_enclosed_bounds = make_pair(ui_past.first | ui_future.first, ui_past.second | ui_future.second);
        }

        m_tree_computation_needed = false;
      }
      
      void Subtube::flagFutureTreeUpdate(int slice_id) const
      {
        m_tree_computation_needed = true;

        if(!isSlice())
        {
          if(slice_id == -1)
          {
            m_first_subtube->flagFutureTreeUpdate(-1);
            m_second_subtube->flagFutureTreeUpdate(-1);
          }

          else
          {
            DomainException::check(*this, slice_id);
            int mid_id = m_first_subtube->nbSlices();
            if(slice_id < mid_id) m_first_subtube->flagFutureTreeUpdate(slice_id);
            else m_second_subtube->flagFutureTreeUpdate(slice_id - mid_id);
          }
        }
        
        flagFuturePrimitiveComputation();
      }

      bool Subtube::treeUpdateNeeded() const
      {
        return m_tree_computation_needed;
      }
    
    // Access values

      void Subtube::invert(const Interval& y, vector<Interval> &v_t, const Interval& search_domain, bool concatenate_results) const
      {
        v_t.clear();
        Interval intv_t_ctc = invert(y, search_domain);

        if(!intv_t_ctc.is_empty())
        {
          pair<Interval,Interval> enc_bounds = eval(intv_t_ctc);

          if(!concatenate_results)
          {
            if(enc_bounds.first.ub() > y.ub() || enc_bounds.second.lb() < y.lb())
            {
              // Bisection is needed
              vector<Interval> v1;
              m_first_subtube->invert(y, v1, search_domain, false);
              v_t.insert(v_t.end(), v1.begin(), v1.end());
              vector<Interval> v2;
              m_second_subtube->invert(y, v2, search_domain, false);
              v_t.insert(v_t.end(), v2.begin(), v2.end());
            }

            else
              v_t.push_back(intv_t_ctc);
          }

          else
          {
            vector<Interval> v;
            invert(y, v, search_domain, false);

            // Concatenation (solutions may be adjacent)
            int i = 0;
            while(i < v.size())
            {
              int j = i;
              Interval merge = v[i];

              while(j + 1 < v.size() && v[j].ub() == v[j + 1].lb())
              {
                j ++;
                merge |= v[j];
              }

              v_t.push_back(merge);
              i = j + 1;
            }
          }
        }
      }
    
    // Operators

      void Subtube::unionWith_localUpdate(const Subtube *x)
      {
        if(treeUpdateNeeded())
          updateTree();
        
        m_image |= x->image();
        pair<Interval,Interval> eb1 = eval();
        pair<Interval,Interval> eb2 = x->eval();
        m_enclosed_bounds = make_pair(Interval(min(eb1.first.lb(), eb2.first.lb()), min(eb1.first.ub(), eb2.first.ub())),
                                      Interval(max(eb1.second.lb(), eb2.second.lb()), max(eb1.second.ub(), eb2.second.ub())));

        flagFuturePrimitiveComputation();
      }
      
      void Subtube::intersectWith_localUpdate(const Subtube *x)
      {
        if(treeUpdateNeeded())
          updateTree();
        
        m_image &= x->image();
        // Enclosed bounds cannot be computed on this level.
        // Synthesis has to be done from the root
        flagFutureTreeUpdate();
      }
}