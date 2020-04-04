
/** 
 *  ile
 *  CtcDeriv documentation header 
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  uthor     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
 
 const char* DOCS_CTCDERIV=R"_docs(/**
* \class CtcDeriv
* \brief \f$\mathcal{C}_{\frac{d}{dt}}\f$ that contracts a tube \f$[x](\cdot)\f$ with respect
*        to its derivative tube \f$[v](\cdot)\f$ according to the constraint \f$\dot{x}(\cdot)=v(\cdot)\f$
* \note  This contractor is called \f$\mathcal{C}_{\frac{d}{dt}}\f$ in the literature.
* \todo  Contract the tube \f$[v](\cdot)\f$ if \f$[x](\cdot)\f$ degenerated?
*/
)_docs";

const char* DOCS_CTCDERIV_CTCDERIV=R"_docs(/**
* \brief Creates a contractor object \f$\mathcal{C}_{\frac{d}{dt}}\f$
*/
)_docs";

const char* DOCS_CTCDERIV_CONTRACT_VECTOR_ABSTRACTDOMAIN=R"_docs(
)_docs";

const char* DOCS_CTCDERIV_CONTRACT_TUBE_TUBE_TIMEPROPAG=R"_docs(/**
* \brief \f$\mathcal{C}_{\frac{d}{dt}}\big([x](\cdot),[v](\cdot)\big)\f$:
*        contracts the tube \f$[x](\cdot)\f$ with respect to its derivative \f$[v](\cdot)\f$.
*
* \pre \f$[x](\cdot)\f$ and \f$[v](\cdot)\f$ must share the same slicing and domain.
*
* \param x the scalar tube \f$[x](\cdot)\f$
* \param v the scalar derivative tube \f$[v](\cdot)\f$
* \param t_propa an optional temporal way of propagation
*                (forward or backward in time, both ways by default)
*/
)_docs";

const char* DOCS_CTCDERIV_CONTRACT_TUBEVECTOR_TUBEVECTOR_TIMEPROPAG=R"_docs(/**
* \brief \f$\mathcal{C}_{\frac{d}{dt}}\big([\mathbf{x}](\cdot),[\mathbf{v}](\cdot)\big)\f$:
*        contracts the tube \f$[\mathbf{x}](\cdot)\f$ with respect to its derivative \f$[\mathbf{v}](\cdot)\f$.
*
* \pre \f$[\mathbf{x}](\cdot)\f$ and \f$[\mathbf{v}](\cdot)\f$ must share the same dimension, slicing and domain.
*
* \param x the n-dimensional tube \f$[\mathbf{x}](\cdot)\f$
* \param v the n-dimensional derivative tube \f$[\mathbf{v}](\cdot)\f$
* \param t_propa an optional temporal way of propagation
*                (forward or backward in time, both ways by default)
*/
)_docs";

const char* DOCS_CTCDERIV_CONTRACT_SLICE_SLICE_TIMEPROPAG=R"_docs(/**
* \brief \f$\mathcal{C}_{\frac{d}{dt}}\big(\llbracket x\rrbracket(\cdot),\llbracket v\rrbracket(\cdot)\big)\f$:
*        contracts the slice \f$\llbracket x\rrbracket(\cdot)\f$ with respect to its derivative \f$\llbracket v\rrbracket(\cdot)\f$.
*
* \pre \f$\llbracket x\rrbracket(\cdot)\f$ and \f$\llbracket v\rrbracket(\cdot)\f$ must share the same domain.
*
* \param x the slice \f$\llbracket x\rrbracket(\cdot)\f$
* \param v the derivative slice \f$\llbracket v\rrbracket(\cdot)\f$
* \param t_propa an optional temporal way of propagation
*                (forward or backward in time, both ways by default)
*/
)_docs";

