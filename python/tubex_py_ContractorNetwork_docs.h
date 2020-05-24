
/** 
 *  ile
 *  ContractorNetwork documentation header 
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  uthor     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
 
 const char* DOCS_CONTRACTORNETWORK=R"_docs(/**
* \class ContractorNetwork
* \brief Graph of contractors and domains that model a problem in the constraint
*        programming framework. Heterogeneous domains can be handled in the same
*        network, which allows to deal with a wide variety of problems such as
*        non-linear equations, differential systems, delays or inter-temporal
*        equations.
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_CONTRACTORNETWORK=R"_docs(/// \name Definition
/// @{/**
* \brief Creates an empty Contractor Network (CN)
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_NB_CTC=R"_docs(/**
* \brief Returns the number of contractors the graph is made of
*
* \return integer
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_NB_DOM=R"_docs(/**
* \brief Returns the number of domains the graph is made of
*
* \return integer
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_EMPTINESS=R"_docs(/**
* \brief Returns `true` if one of the domains is empty, `false` otherwise
*
* \return emptiness test
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_CREATE_DOM_INTERVAL=R"_docs(/// @}
/// \name Building the network (domains, contractors)
/// @{/**
* \brief Creates an Interval intermediate variable with a reference kept in the 
*        ContractorNetwork. The variable will be kept alive outside its definition
*        block. This allows for instance to add contraints involving intermediate
*        variables inside iteration loops. The variable exists in the CN but cannot
*        be reached outside its definition block. Its deallocation is handled by
*        the CN.
*
* \param i initial domain providing information on the type and initial set of values
* \return a reference to the created Interval domain variable
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_CREATE_DOM_INTERVALVECTOR=R"_docs(/**
* \brief Creates an IntervalVector intermediate variable with a reference kept in the 
*        ContractorNetwork. The variable will be kept alive outside its definition
*        block. This allows for instance to add contraints involving intermediate
*        variables inside iteration loops. The variable exists in the CN but cannot
*        be reached outside its definition block. Its deallocation is handled by
*        the CN.
*
* \param iv initial domain providing information on the type and initial set of values
* \return a reference to the created IntervalVector domain variable
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_CREATE_DOM_TUBE=R"_docs(/**
* \brief Creates a Tube intermediate variable with a reference kept in the 
*        ContractorNetwork. The variable will be kept alive outside its definition
*        block. This allows for instance to add contraints involving intermediate
*        variables inside iteration loops. The variable exists in the CN but cannot
*        be reached outside its definition block. Its deallocation is handled by
*        the CN.
*
* \param t initial domain providing information on the type and initial set of values
* \return a reference to the created Tube domain variable
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_CREATE_DOM_TUBEVECTOR=R"_docs(/**
* \brief Creates a TubeVector intermediate variable with a reference kept in the 
*        ContractorNetwork. The variable will be kept alive outside its definition
*        block. This allows for instance to add contraints involving intermediate
*        variables inside iteration loops. The variable exists in the CN but cannot
*        be reached outside its definition block. Its deallocation is handled by
*        the CN.
*
* \param tv initial domain providing information on the type and initial set of values
* \return a reference to the created TubeVector domain variable
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_SUBVECTOR_VECTOR_INT_INT=R"_docs(/**
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
)_docs";

const char* DOCS_CONTRACTORNETWORK_SUBVECTOR_INTERVALVECTOR_INT_INT=R"_docs(/**
* \brief Creates a subvector of an IntervalVector domain
*
* \param iv IntervalVector domain
* \param start_index first component index of the subvector to be returned
* \param end_index last component index of the subvector to be returned
* \return a reference to the created IntervalVector extracted from the IntervalVector domain
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_ADD_CTC_VECTOR_DOMAIN_=R"_docs(/**
* \brief Adds to the graph a static contractor (inherited from Ctc class) with related Domains
*
* \note If tubes are involved in the domain list, they must share the same slicing and tdomain.
*       The static contractor will be applied on each slice of these tubes.
*
* \param static_ctc ibex::Ctc contractor object
* \param v_domains a vector of abstract domains (Interval, Slice, Tube, etc.)
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_ADD_DYNCTC_VECTOR_DOMAIN_=R"_docs(/**
* \brief Adds to the graph a dynamic contractor (inherited from DynCtc class) with related Domains
*
* \note If tubes are involved in the domain list, they must share the same slicing and tdomain.
*       If the contractor is not "inter-temporal", then it will be applied independently on each
*       slice of these tubes.
*
* \param dyn_ctc DynCtc contractor object
* \param v_domains a vector of abstract domains (Interval, Slice, Tube, etc.)
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_ADD_DATA_TUBE_DOUBLE_INTERVAL=R"_docs(/**
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
)_docs";

const char* DOCS_CONTRACTORNETWORK_ADD_DATA_TUBEVECTOR_DOUBLE_INTERVALVECTOR=R"_docs(/**
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
)_docs";

const char* DOCS_CONTRACTORNETWORK_CONTRACT_BOOL=R"_docs(/// @}
/// \name Contraction process
/// @{/**
* \brief Launch the contraction process
*
* Contractions are performed until a fixed point has been reached on the whole graph.
*
* \param verbose verbose mode, `false` by default
* \return the computation time in seconds
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_CONTRACT_DURING_DOUBLE_BOOL=R"_docs(/**
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
)_docs";

const char* DOCS_CONTRACTORNETWORK_SET_FIXEDPOINT_RATIO_FLOAT=R"_docs(/**
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
)_docs";

const char* DOCS_CONTRACTORNETWORK_TRIGGER_ALL_CONTRACTORS=R"_docs(/**
* \brief Triggers on all contractors involved in the graph.
*
* This method can be used to reset the propagation process when domains have been updated
* externaly: outside the ContractorNetwork.
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_NB_CTC_IN_STACK=R"_docs(/**
* \brief Returns the number of contractors that are waiting for process.
*
* If the propagation process reached a fixed point, then this number is zero.
*
* \return number of active contractors
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_SET_NAME_DOMAIN_STRING=R"_docs(/// @}
/// \name Visualization
/// @{/**
* \brief Sets a string name for a given Domain
*
* \param dom domain to name
* \param name string of characters
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_SET_NAME_CTC_STRING=R"_docs(/**
* \brief Sets a string name for a given static contractor
*
* \param ctc static contractor (inherited from Ctc) to name
* \param name string of characters
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_SET_NAME_DYNCTC_STRING=R"_docs(/**
* \brief Sets a string name for a given dynamic contractor
*
* \param ctc dynamic contractor (inherited from DynCtc) to name
* \param name string of characters
*/
)_docs";

const char* DOCS_CONTRACTORNETWORK_PRINT_DOT_GRAPH_STRING_STRING=R"_docs(/**
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
)_docs";

const char* DOCS_CONTRACTORNETWORK_OPERATOR_OSTREAM_CONTRACTORNETWORK=R"_docs(/**
* \brief Displays a synthesis of this ContractorNetwork
*
* \param str ostream
* \param cn ContractorNetwork to be displayed
* \return ostream
*/
)_docs";

