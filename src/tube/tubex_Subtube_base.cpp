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

      Subtube::Subtube(const Subtube& x)
      {
        *this = x;
      }
      
      Subtube::Subtube(const Subtube& x, const Interval& value)
      {
        *this = x;
        set(value);
      }
      
      Subtube::~Subtube()
      {
        if(m_first_subtube != NULL) delete m_first_subtube;
        if(m_second_subtube != NULL) delete m_second_subtube;
      }
      
      const Interval& Subtube::domain() const
      {
        return m_domain;
      }  

    // Slices structure

      int Subtube::nbSlices() const
      {
        // todo: has to be updated?
        return m_slices_number;
      }
      
      const IntervalVector Subtube::sliceBox(int slice_id) const
      {
        IntervalVector box(2);
        const Subtube *slice = getSlice(slice_id);
        box[0] = slice->domain();
        box[1] = slice->image();
        return box;
      }
      
      bool Subtube::isSlice() const
      {
        return m_slices_number == 1;
      }
      
      Subtube* Subtube::getSlice(int slice_id)
      {
        DomainException::check(*this, slice_id);

        if(isSlice())
          return this;

        else
        {
          if(slice_id >= m_first_subtube->nbSlices())
            return m_second_subtube->getSlice(slice_id - m_first_subtube->nbSlices());
          else
            return m_first_subtube->getSlice(slice_id);
        }
      }
      
      const Subtube* Subtube::getSlice(int slice_id) const
      {
        return const_cast<Subtube*>(getSlice(slice_id));
      }
      
      Subtube* Subtube::getSlice(double t)
      {
        return getSlice(input2index(t));
      }
      
      const Subtube* Subtube::getSlice(double t) const
      {
        return const_cast<Subtube*>(getSlice(t));
      }
      
      void Subtube::getSlices(vector<const Subtube*>& v_slices) const
      {
        if(isSlice())
          v_slices.push_back(this);

        else
        {
          m_first_subtube->getSlices(v_slices);
          m_second_subtube->getSlices(v_slices);
        }
      }
      
      int Subtube::input2index(double t) const
      {
        DomainException::check(*this, t);

        // todo: check the following
        //if(t == m_domain.ub())
        //  return nbSlices() - 1;

        if(isSlice())
          return 0;

        if(t < m_first_subtube->domain().ub())
          return m_first_subtube->input2index(t);

        else
          return m_second_subtube->input2index(t) + m_first_subtube->nbSlices();
      }
      
      double Subtube::index2input(int slice_id) const
      {
        DomainException::check(*this, slice_id);
        return getSlice(slice_id)->domain().lb();
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
      
      double Subtube::dist(const Subtube& x) const
      {
        return fabs(x.volume() - volume());
      }
      
      const Interval& Subtube::image() const
      {
        updateTree();
        return m_image;
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
      
      Interval Subtube::interpol(double t, const Subtube& derivative) const
      {
        StructureException::check(*this, derivative);
        EmptyException::check(derivative);

        int slice_id = input2index(t);
        Interval dom_ = sliceDomain(slice_id);
        Interval deriv_value = derivative[slice_id];
        return ((*this)[dom_.ub()] - (dom_.ub() - t) * deriv_value)
             & ((*this)[dom_.lb()] + (t - dom_.lb()) * deriv_value);
      }
      
      Interval Subtube::interpol(const Interval& t, const Subtube& derivative) const
      {
        pair<Interval,Interval> p_interpol = partialInterpol(t, derivative);
        return p_interpol.first | p_interpol.second;
      }
      
      pair<Interval,Interval> Subtube::partialInterpol(const Interval& t, const Subtube& derivative) const
      {
        Interval y_tlb = interpol(t.lb(), derivative);
        Interval y_tub = interpol(t.ub(), derivative);

        /* Dealing with infinity...
         *
         * In IBEX, the following is defined:
         *    Interval(NEG_INFINITY) = EMPTY_SET
         *    Interval(POS_INFINITY) = EMPTY_SET
         *    Interval(NEG_INFINITY,POS_INFINITY) = EMPTY_SET
         *
         * So, we have to detect infinite bounds
         * and work with dbl_max (max double).
         *
         * This is only a temporary solution.
         * [todo]
         */

          double dbl_max = std::numeric_limits<double>::max();

          Interval lb, ub;
          lb.set_empty(); ub.set_empty();

          lb |= y_tlb.lb() == NEG_INFINITY ? Interval(-dbl_max) : y_tlb.lb();
          ub |= y_tlb.ub() == POS_INFINITY ? Interval(dbl_max) : y_tlb.ub();

          lb |= y_tub.lb() == NEG_INFINITY ? Interval(-dbl_max) : y_tub.lb();
          ub |= y_tub.ub() == POS_INFINITY ? Interval(dbl_max) : y_tub.ub();

          // todo: check the following simplification:
          // lb &= dbl_max*Interval(-1.,1.);
          // ub &= dbl_max*Interval(-1.,1.);

        for(int i = min(nbSlices() - 1, input2index(t.lb()) + 1) ; i < max(0, input2index(t.ub()) - 1) ; i++)
        {
          pair<Interval,Interval> p_i = eval(sliceDomain(i));
          lb |= p_i.first;
          ub |= p_i.second;
        }

        return make_pair(lb, ub);
      }
      
      Interval Subtube::invert(const Interval& y, const Interval& t) const
      {
        if(!domain().intersects(t))
          return Interval::EMPTY_SET;

        else if(!image().intersects(y))
          return Interval::EMPTY_SET;

        else
        {
          if(isSlice())
            return t & m_domain;

          else
            return m_first_subtube->invert(y, t) | m_second_subtube->invert(y, t);
        }
      }
      
      void Subtube::invert(const Interval& y, vector<Interval> &v_t, const Interval& t) const
      {
        return invert(y, v_t, t, true);
      }
      
      double Subtube::maxThickness() const
      {
        int id;
        return maxThickness(id);
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

      bool Subtube::isEmpty() const
      {
        return image().is_empty();
      }
      
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
          vector<Subtube*> this_nodes;
          vector<const Subtube*> x_nodes;
          getSubtubeNodes(this_nodes);
          x.getSubtubeNodes(x_nodes);

          for(int i = 0 ; i < this_nodes.size() ; i++)
            this_nodes[i]->unionWith_localUpdate(x_nodes[i]);
        }

        // todo: check the interest of the following
        flagFutureTreeUpdate();
        return *this;
      }
      
      Subtube& Subtube::operator|=(const Trajectory& x)
      {
        for(int i = 0 ; i < nbSlices() ; i++)
          inflate(x[sliceDomain(i)], i);
      }
      
      Subtube& Subtube::operator&=(const Subtube& x)
      {

      }

    // String

      ostream& operator<<(ostream& str, const Subtube& x)
      {

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

      void Subtube::createFromSlicesVector(const vector<Interval>& v_slices_domains, const Interval& value)
      {

      }
      
      const Subtube* Subtube::getFirstSubtube() const
      {

      }
          
      const Subtube* Subtube::getSecondSubtube() const
      {

      }
          
      void Subtube::getSubtubeNodes(vector<Subtube*> &v_nodes)
      {

      }
      
      void Subtube::getSubtubeNodes(vector<const Subtube*> &v_nodes) const
      {

      }
          
      void Subtube::updateTree() const
      {
        if(!m_tree_computation_needed)
          return;


      }
      
      void Subtube::flagFutureTreeUpdate(int slice_id) const
      {

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

      }
      
      void Subtube::intersectWith_localUpdate(const Subtube *x)
      {

      }
}