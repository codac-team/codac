
/** 
 *  ile
 *  CtcEval documentation header 
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  uthor     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
 
 const char* DOCS_CTCEVAL=R"_docs(/**
* \class CtcEval
* \brief \f$\mathcal{C}_\textrm{eval}\f$ that contracts a tube \f$[y](\cdot)\f$ with
*        respect to its derivative tube \f$[w](\cdot)\f$ and a measurement \f$[t]\times[z]\f$
*        according to the constraints \f$z=y(t)\f$ and \f$\dot{y}(\cdot)=w(\cdot)\f$
* \note  This contractor is denoted \f$\mathcal{C}_\textrm{eval}\f$ in the literature.
*/
)_docs";

const char* DOCS_CTCEVAL_CTCEVAL=R"_docs(/**
* \brief Creates a contractor object \f$\mathcal{C}_\textrm{eval}\f$
*/
)_docs";

const char* DOCS_CTCEVAL_CONTRACT_VECTOR_ABSTRACTDOMAIN=R"_docs(
)_docs";

const char* DOCS_CTCEVAL_CONTRACT_DOUBLE_INTERVAL_TUBE_TUBE=R"_docs(/**
* \brief \f$\mathcal{C}_\textrm{eval}\big(t,[z],[y](\cdot),[w](\cdot)\big)\f$:
*        contracts the tube \f$[y](\cdot)\f$ and the evaluation \f$[z]\f$.
*
* \note The slicing of \f$[y](\cdot)\f$ and \f$[w](\cdot)\f$ may be changed.
*
* \param t the date \f$t\f$ of the evaluation
* \param z the bounded evaluation \f$[z]\f$
* \param y the scalar tube \f$[y](\cdot)\f$
* \param w the scalar derivative tube \f$[w](\cdot)\f$
*/
)_docs";

const char* DOCS_CTCEVAL_CONTRACT_INTERVAL_INTERVAL_TUBE_TUBE=R"_docs(/**
* \brief \f$\mathcal{C}_\textrm{eval}\big([t],[z],[y](\cdot),[w](\cdot)\big)\f$:
*        contracts the tube \f$[y](\cdot)\f$ and the evaluation \f$[t]\times[z]\f$.
*
* \note The slicing of \f$[y](\cdot)\f$ and \f$[w](\cdot)\f$ may be changed.
*
* \param t the uncertain domain \f$[t]\f$ of the evaluation
* \param z the bounded evaluation \f$[z]\f$
* \param y the scalar tube \f$[y](\cdot)\f$
* \param w the scalar derivative tube \f$[w](\cdot)\f$
*/
)_docs";

const char* DOCS_CTCEVAL_CONTRACT_INTERVAL_INTERVAL_TUBE_TUBE1=R"_docs(/**
* \brief \f$\mathcal{C}_\textrm{eval}\big([t],[z],[y](\cdot),[w](\cdot)\big)\f$:
*        contracts the tube \f$[y](\cdot)\f$ only.
*
* \param t the uncertain domain \f$[t]\f$ of the evaluation
* \param z the bounded evaluation \f$[z]\f$
* \param y the scalar tube \f$[y](\cdot)\f$
* \param w the scalar derivative tube \f$[w](\cdot)\f$
*/
)_docs";

const char* DOCS_CTCEVAL_CONTRACT_DOUBLE_INTERVALVECTOR_TUBEVECTOR_TUBEVECTOR=R"_docs(/**
* \brief \f$\mathcal{C}_\textrm{eval}\big(t,[\mathbf{z}],[\mathbf{y}](\cdot),[\mathbf{w}](\cdot)\big)\f$:
*        contracts the tube \f$[\mathbf{y}](\cdot)\f$ and the evaluation \f$[\mathbf{z}]\f$.
*
* \note The slicing of \f$[\mathbf{y}](\cdot)\f$ and \f$[\mathbf{w}](\cdot)\f$ may be changed.
*
* \param t the date \f$t\f$ of the evaluation
* \param z the bounded evaluation \f$[\mathbf{z}]\f$
* \param y the n-dimensional tube \f$[\mathbf{y}](\cdot)\f$
* \param w the n-dimensional derivative tube \f$[\mathbf{w}](\cdot)\f$
*/
)_docs";

const char* DOCS_CTCEVAL_CONTRACT_INTERVAL_INTERVALVECTOR_TUBEVECTOR_TUBEVECTOR=R"_docs(/**
* \brief \f$\mathcal{C}_\textrm{eval}\big([t],[\mathbf{z}],[\mathbf{y}](\cdot),[\mathbf{w}](\cdot)\big)\f$:
*        contracts the tube \f$[\mathbf{y}](\cdot)\f$ and the evaluation \f$[t]\times[\mathbf{z}]\f$.
*
* \note The slicing of \f$[\mathbf{y}](\cdot)\f$ and \f$[\mathbf{w}](\cdot)\f$ may be changed.
*
* \param t the uncertain domain \f$[t]\f$ of the evaluation
* \param z the bounded evaluation \f$[\mathbf{z}]\f$
* \param y the n-dimensional tube \f$[\mathbf{y}](\cdot)\f$
* \param w the n-dimensional derivative tube \f$[\mathbf{w}](\cdot)\f$
*/
)_docs";

const char* DOCS_CTCEVAL_CONTRACT_INTERVAL_INTERVALVECTOR_TUBEVECTOR_TUBEVECTOR1=R"_docs(/**
* \brief \f$\mathcal{C}_\textrm{eval}\big([t],[\mathbf{z}],[\mathbf{y}](\cdot),[\mathbf{w}](\cdot)\big)\f$:
*        contracts the tube \f$[\mathbf{y}](\cdot)\f$ only.
*
* \note The slicing of \f$[\mathbf{y}](\cdot)\f$ and \f$[\mathbf{w}](\cdot)\f$ may be changed.
*
* \param t the uncertain domain \f$[t]\f$ of the evaluation
* \param z the bounded evaluation \f$[\mathbf{z}]\f$
* \param y the n-dimensional tube \f$[\mathbf{y}](\cdot)\f$
* \param w the n-dimensional derivative tube \f$[\mathbf{w}](\cdot)\f$
*/
)_docs";

const char* DOCS_CTCEVAL_CONTRACT_INTERVAL_INTERVAL_TUBE=R"_docs(/**
* \brief \f$\mathcal{C}_\textrm{eval}\big([t],[z],[y](\cdot)\big)\f$:
*        contracts the evaluation \f$[t]\times[z]\f$ only.
*
* \param t the uncertain domain \f$[t]\f$ of the evaluation
* \param z the bounded evaluation \f$[z]\f$
* \param y the scalar tube \f$[y](\cdot)\f$
*/
)_docs";

const char* DOCS_CTCEVAL_CONTRACT_INTERVAL_INTERVALVECTOR_TUBEVECTOR=R"_docs(/**
* \brief \f$\mathcal{C}_\textrm{eval}\big([t],[\mathbf{z}],[\mathbf{y}](\cdot)\big)\f$:
*        contracts the evaluation \f$[t]\times[\mathbf{z}]\f$ only.
*
* \param t the uncertain domain \f$[t]\f$ of the evaluation
* \param z the bounded evaluation \f$[\mathbf{z}]\f$
* \param y the n-dimensional tube \f$[\mathbf{y}](\cdot)\f$
*/
)_docs";

const char* DOCS_CTCEVAL_ENABLE_TEMPORAL_PROPAGATION_BOOL=R"_docs(/**
* \brief Enables a forward/backward temporal propagation of the contraction
*
* \note If disabled, then the contraction will only affect the slices over \f$[t]\f$.
*
* \param enable_propagation if true, the contractions will be propagated as far as possible across \f$[t_0,t_f]\f$
*/
)_docs";

