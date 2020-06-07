/** 
 *  \file
 *  ContractorNetwork class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CONTRACTORNETWORK_H__
#define __TUBEX_CONTRACTORNETWORK_H__

#include <deque>
#include <initializer_list>
#include "ibex_Ctc.h"
#include "tubex_DynCtc.h"
#include "tubex_Domain.h"
#include "tubex_Contractor.h"
#include "tubex_CtcDeriv.h"

namespace ibex
{
  class Ctc;
}

namespace tubex
{
  class Domain;
  class Contractor;
  class DynCtc;
  class CtcDeriv;

  /**
   * \class ContractorNetwork
   * \brief Graph of contractors and domains that model a problem in the constraint
   *        programming framework. Heterogeneous domains can be handled in the same
   *        network, which allows to deal with a wide variety of problems such as
   *        non-linear equations, differential systems, delays or inter-temporal
   *        equations.
   */
  class ContractorNetwork
  {
    public:

      /// \name Definition
      /// @{

      /**
       * \brief Creates an empty Contractor Network (CN)
       */
      ContractorNetwork();

      /**
       * \brief ContractorNetwork destructor
       */
      ~ContractorNetwork();

      /**
       * \brief Returns the number of contractors the graph is made of
       *
       * \return integer
       */
      int nb_ctc() const;

      /**
       * \brief Returns the number of domains the graph is made of
       *
       * \return integer
       */
      int nb_dom() const;

      /**
       * \brief Returns `true` if one of the domains is empty, `false` otherwise
       *
       * \return emptiness test
       */
      bool emptiness() const;

      /// @}
      /// \name Building the network (domains, contractors)
      /// @{

      /**
       * \brief Creates an Interval intermediate variable with a reference kept in the 
       *        ContractorNetwork. The variable will be kept alive outside its definition
       *        block. This allows for instance to add constraints involving intermediate
       *        variables inside iteration loops. The variable exists in the CN but cannot
       *        be reached outside its definition block. Its deallocation is handled by
       *        the CN.
       *
       * \param i initial domain providing information on the type and initial set of values
       * \return a reference to the created Interval domain variable
       */
      ibex::Interval& create_dom(const ibex::Interval& i);

      /**
       * \brief Creates an IntervalVector intermediate variable with a reference kept in the 
       *        ContractorNetwork. The variable will be kept alive outside its definition
       *        block. This allows for instance to add constraints involving intermediate
       *        variables inside iteration loops. The variable exists in the CN but cannot
       *        be reached outside its definition block. Its deallocation is handled by
       *        the CN.
       *
       * \param iv initial domain providing information on the type and initial set of values
       * \return a reference to the created IntervalVector domain variable
       */
      ibex::IntervalVector& create_dom(const ibex::IntervalVector& iv);

      /**
       * \brief Creates a Tube intermediate variable with a reference kept in the 
       *        ContractorNetwork. The variable will be kept alive outside its definition
       *        block. This allows for instance to add constraints involving intermediate
       *        variables inside iteration loops. The variable exists in the CN but cannot
       *        be reached outside its definition block. Its deallocation is handled by
       *        the CN.
       *
       * \param t initial domain providing information on the type and initial set of values
       * \return a reference to the created Tube domain variable
       */
      Tube& create_dom(const Tube& t);

      /**
       * \brief Creates a TubeVector intermediate variable with a reference kept in the 
       *        ContractorNetwork. The variable will be kept alive outside its definition
       *        block. This allows for instance to add constraints involving intermediate
       *        variables inside iteration loops. The variable exists in the CN but cannot
       *        be reached outside its definition block. Its deallocation is handled by
       *        the CN.
       *
       * \param tv initial domain providing information on the type and initial set of values
       * \return a reference to the created TubeVector domain variable
       */
      TubeVector& create_dom(const TubeVector& tv);

      /**
       * \brief Creates a subvector of a Vector domain
       *
       * \note Even if the input vector is a Vector object, the created domain is a set
       *       enclosing the subvector.
       *
       * \param v Vector domain
       * \param start_index first component index of the subvector to be returned
       * \param end_index last component index of the subvector to be returned
       * \return a reference to the created IntervalVector extracted from the Vector domain
       */
      ibex::IntervalVector& subvector(ibex::Vector& v, int start_index, int end_index);

      /**
       * \brief Creates a subvector of an IntervalVector domain
       *
       * \param iv IntervalVector domain
       * \param start_index first component index of the subvector to be returned
       * \param end_index last component index of the subvector to be returned
       * \return a reference to the created IntervalVector extracted from the IntervalVector domain
       */
      ibex::IntervalVector& subvector(ibex::IntervalVector& iv, int start_index, int end_index);

      // not yet suported: /**
      // not yet suported:  * \brief Creates a subvector of a TubeVector domain
      // not yet suported:  *
      // not yet suported:  * \param tv TubeVector domain
      // not yet suported:  * \param start_index first component index of the subvector to be returned
      // not yet suported:  * \param end_index last component index of the subvector to be returned
      // not yet suported:  * \return a reference to the created TubeVector extracted from the TubeVector domain
      // not yet suported:  */
      // not yet suported: TubeVector& subvector(TubeVector& tv, int start_index, int end_index);

      /**
       * \brief Adds to the graph a static contractor (inherited from Ctc class) with related Domains
       *
       * \note If tubes are involved in the domain list, they must share the same slicing and tdomain.
       *       The static contractor will be applied on each slice of these tubes.
       *
       * \param static_ctc ibex::Ctc contractor object
       * \param v_domains a vector of abstract domains (Interval, Slice, Tube, etc.)
       */
      void add(ibex::Ctc& static_ctc, const std::vector<Domain>& v_domains);

      /**
       * \brief Adds to the graph a dynamic contractor (inherited from DynCtc class) with related Domains
       *
       * \note If tubes are involved in the domain list, they must share the same slicing and tdomain.
       *       If the contractor is not "inter-temporal", then it will be applied independently on each
       *       slice of these tubes.
       *
       * \param dyn_ctc DynCtc contractor object
       * \param v_domains a vector of abstract domains (Interval, Slice, Tube, etc.)
       */
      void add(DynCtc& dyn_ctc, const std::vector<Domain>& v_domains);

      /**
       * \brief Adds continuous data \f$[y]\f$ to a tube \f$[x](\cdot)\f$ at \f$t\f$ (used for realtime applications).
       *
       * In this case, the tube represents an enclosure of a trajectory of measurements that are given in realtime.
       *
       * t must increase at each call of this method.
       * Slices of the tube will be contracted only if provided data completely cover their tdomain.
       * Contracted slices will trigger related contractors, allowing constraint propagation in the graph.
       *
       * \param x the tube \f$[x](\cdot)\f$ to be contracted with continuous data
       * \param t time of measurement \f$t\f$ 
       * \param y bounded measurement, equivalent to the set \f$[x](t)=[y]\f$ 
       */
      void add_data(Tube& x, double t, const ibex::Interval& y);

      /**
       * \brief Adds continuous data \f$[\mathbf{y}]\f$ to a tube \f$[\mathbf{x}](\cdot)\f$ at \f$t\f$ (used for realtime applications).
       *
       * In this case, the tube vector represents an enclosure of a trajectory of vector measurements that are given in realtime.
       *
       * t must increase at each call of this method.
       * Slices of the tube will be contracted only if provided data completely cover their tdomain.
       * Contracted slices will trigger related contractors, allowing constraint propagation in the graph.
       *
       * \param x the tube \f$[\mathbf{x}](\cdot)\f$ to be contracted with continuous data
       * \param t time of measurement \f$t\f$ 
       * \param y bounded measurement, equivalent to the set \f$[\mathbf{x}](t)=[\mathbf{y}]\f$ 
       */
      void add_data(TubeVector& x, double t, const ibex::IntervalVector& y);

      /// @}
      /// \name Contraction process
      /// @{

      /**
       * \brief Launch the contraction process
       *
       * Contractions are performed until a fixed point has been reached on the whole graph.
       *
       * \param verbose verbose mode, `false` by default
       * \return the computation time in seconds
       */
      double contract(bool verbose = false);

      /**
       * \brief Launch the contraction process and stops after \f$dt\f$ seconds
       *
       * Contractions are performed until a fixed point has been obtained on the whole graph,
       * or if the computation time limit has been reached.
       *
       * Note that the computation time may slightly exceed \f$dt\f$.
       *
       * \param dt allowed computation time
       * \param verbose verbose mode, `false` by default
       * \return the computation time in seconds
       */
      double contract_during(double dt, bool verbose = false);

      /**
       * \brief Sets the fixed point ratio defining the end of the propagation process.
       *
       * The ratio \f$r\f$ is a percentage of contraction impact computed on the volume of the domains.
       * For a given domain submitted to a contractor, if the contraction impact after contraction is 
       * more than the defined ratio, then the propagation is performed on other contractors related to
       * this domain. Otherwise, the propagation process stops.
       *
       * \f$r=0\f$ means that the propagation is performed up to a fixed point defined by the floating-point
       * precision value. \f$r=0.1\f$ means that the propagation will be done only if the domain has been
       * contracted by 10%.
       *
       * \param r ratio of contraction, \f$r\in[0,1]\f$
       */
      void set_fixedpoint_ratio(float r);

      /**
       * \brief Triggers on all contractors involved in the graph.
       *
       * This method can be used to reset the propagation process when domains have been updated
       * externally: outside the ContractorNetwork.
       */
      void trigger_all_contractors();

      /**
       * \brief Returns the number of contractors that are waiting for process.
       *
       * If the propagation process reached a fixed point, then this number is zero.
       *
       * \return number of active contractors
       */
      int nb_ctc_in_stack() const;

      /// @}
      /// \name Visualization
      /// @{

      /**
       * \brief Sets a string name for a given Domain
       *
       * \param dom domain to name
       * \param name string of characters
       */
      void set_name(Domain dom, const std::string& name);

      /**
       * \brief Sets a string name for a given static contractor
       *
       * \param ctc static contractor (inherited from Ctc) to name
       * \param name string of characters
       */
      void set_name(ibex::Ctc& ctc, const std::string& name);

      /**
       * \brief Sets a string name for a given dynamic contractor
       *
       * \param ctc dynamic contractor (inherited from DynCtc) to name
       * \param name string of characters
       */
      void set_name(DynCtc& ctc, const std::string& name);

      /**
       * \brief Generates a dot graph for visualization in PDF file format, by using dot2tex
       *
       * \param cn_name name of the graph (and rendered file)
       * \param layer_model custom layer model for rendering (dot2tex)
       *        * dot - hierarchical or layered drawings of directed graphs
       *        * neato - attempts to minimize a global energy function, which is equivalent to statistical multi-dimensional scaling
       *        * fdp - layouts similar to those of neato, but does this by reducing forces rather than working with energy
       *        * sfdp - multiscale version of fdp for the layout of large graphs
       *        * twopi - radial layouts, nodes are placed on concentric circles depending their distance from a given root node
       *        * circo - circular layout, suitable for certain diagrams of multiple cyclic structures
       * \return system command success
       */
      int print_dot_graph(const std::string& cn_name = "cn", const std::string& layer_model = "fdp") const;

      /**
       * \brief Displays a synthesis of this ContractorNetwork
       *
       * \param str ostream
       * \param cn ContractorNetwork to be displayed
       * \return ostream
       */
      friend std::ostream& operator<<(std::ostream& str, const ContractorNetwork& cn);

      /// @}

    protected:

      /**
       * \brief Adds an abstract Domain to the graph
       *
       * If an equal Domain already exists in the graph, then a pointer to it is returned, 
       * and the `ad` object is not added. Otherwise, `ad` is added and its pointer is
       * returned.
       *
       * \param ad abstract Domain object
       * \return the pointer to the related Domain object in the graph
       */
      Domain* add_dom(const Domain& ad);

      /**
       * \brief Adds an abstract Contractor to the graph
       *
       * If an equal Contractor already exists in the graph, then a pointer to it is returned, 
       * and the `ac` object is not added. Otherwise, `ac` is added and its pointer is
       * returned.
       *
       * \param ac abstract Contractor object
       * \return the pointer to the related Contractor object in the graph
       */
      Contractor* add_ctc(const Contractor& ac);

      /**
       * \brief Adds a Contractor object in the queue of active contractors
       *
       * \param ac Contractor to be added
       * \param ctc_deque queue of contractors
       */
      void add_ctc_to_queue(Contractor *ac, std::deque<Contractor*>& ctc_deque);

      /**
       * \brief Triggers on the contractors related to the given Domain
       *
       * \param dom pointer to the Domain
       * \param ctc_to_avoid optional pointer to a Contractor to not activate
       */
      void trigger_ctc_related_to_dom(Domain *dom, Contractor *ctc_to_avoid = NULL);

    protected:

      std::vector<Contractor*> m_v_ctc; //!< vector of pointers to the abstract Contractor objects the graph is made of
      std::vector<Domain*> m_v_domains; //!< vector of pointers to the abstract Domain objects the graph is made of
      std::deque<Contractor*> m_deque; //!< queue of active contractors

      float m_fixedpoint_ratio = 0.0001; //!< fixed point ratio for propagation limit
      double m_contraction_duration_max = std::numeric_limits<double>::infinity(); //!< computation time limit

      CtcDeriv *m_ctc_deriv = NULL; //!< optional pointer to a CtcDeriv object that can be automatically added in the graph
      std::list<std::pair<Domain*,Domain*> > m_domains_related_to_ctcderiv;

      friend class Domain;
  };
}

#endif