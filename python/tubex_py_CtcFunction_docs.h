
/** 
 *  ile
 *  CtcFunction documentation header 
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  uthor     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
 
 const char* DOCS_CTCFUNCTION=R"_docs(/**
* \class CtcFunction
* \brief Generic static \f$\mathcal{C}\f$ that contracts a box \f$[\mathbf{x}]\f$ or a tube \f$[\mathbf{x}](\cdot)\f$
*        according to the constraint \f$\mathbf{f}(\mathbf{x})=\mathbf{0}\f$ or \f$\mathbf{f}(\mathbf{x})\in[\mathbf{y}]\f$.
*        It stands on the CtcFwdBwd of IBEX (HC4Revise) combined with a Ctc3BCid.
*/
)_docs";

const char* DOCS_CTCFUNCTION_CTCFUNCTION_CHAR_CHAR=R"_docs(
)_docs";

const char* DOCS_CTCFUNCTION_CTCFUNCTION_CHAR_CHAR_CHAR=R"_docs(
)_docs";

const char* DOCS_CTCFUNCTION_CTCFUNCTION_CHAR_CHAR_CHAR_CHAR=R"_docs(
)_docs";

const char* DOCS_CTCFUNCTION_CTCFUNCTION_CHAR_CHAR_CHAR_CHAR_CHAR=R"_docs(
)_docs";

const char* DOCS_CTCFUNCTION_CTCFUNCTION_CHAR_CHAR_CHAR_CHAR_CHAR_CHAR=R"_docs(
)_docs";

const char* DOCS_CTCFUNCTION_CTCFUNCTION_CHAR_CHAR_CHAR_CHAR_CHAR_CHAR_CHAR=R"_docs(
)_docs";

const char* DOCS_CTCFUNCTION_CTCFUNCTION_FUNCTION=R"_docs(
)_docs";

const char* DOCS_CTCFUNCTION_CTCFUNCTION_FUNCTION_DOMAIN=R"_docs(
)_docs";

const char* DOCS_CTCFUNCTION_CTCFUNCTION_FUNCTION_INTERVAL=R"_docs(
)_docs";

const char* DOCS_CTCFUNCTION_CTCFUNCTION_FUNCTION_INTERVALVECTOR=R"_docs(
)_docs";

const char* DOCS_CTCFUNCTION_CONTRACT_INTERVALVECTOR=R"_docs(/**
* \brief \f$\mathcal{C}\big([\mathbf{x}]\big)\f$
*
* \param x the n-dimensional box \f$[\mathbf{x}]\f$ to be contracted
*/
)_docs";

const char* DOCS_CTCFUNCTION_CONTRACT_TUBEVECTOR=R"_docs(/**
* \brief \f$\mathcal{C}\big([\mathbf{x}](\cdot)\big)\f$
*
* \param x the n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ to be contracted
*/
)_docs";

const char* DOCS_CTCFUNCTION_CONTRACT_TUBE=R"_docs(/**
* \brief \f$\mathcal{C}\big([x_1](\cdot)\big)\f$
*
* \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
*/
)_docs";

const char* DOCS_CTCFUNCTION_CONTRACT_TUBE_TUBE=R"_docs(/**
* \brief \f$\mathcal{C}\big([x_1](\cdot),[x_2](\cdot)\big)\f$
*
* \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
* \param x2 the scalar tube \f$[x_2](\cdot)\f$ to be contracted
*/
)_docs";

const char* DOCS_CTCFUNCTION_CONTRACT_TUBE_TUBE_TUBE=R"_docs(/**
* \brief \f$\mathcal{C}\big([x_1](\cdot),[x_2](\cdot),[x_3](\cdot)\big)\f$
*
* \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
* \param x2 the scalar tube \f$[x_2](\cdot)\f$ to be contracted
* \param x3 the scalar tube \f$[x_3](\cdot)\f$ to be contracted
*/
)_docs";

const char* DOCS_CTCFUNCTION_CONTRACT_TUBE_TUBE_TUBE_TUBE=R"_docs(/**
* \brief \f$\mathcal{C}\big([x_1](\cdot),[x_2](\cdot),[x_3](\cdot),[x_4](\cdot)\big)\f$
*
* \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
* \param x2 the scalar tube \f$[x_2](\cdot)\f$ to be contracted
* \param x3 the scalar tube \f$[x_3](\cdot)\f$ to be contracted
* \param x4 the scalar tube \f$[x_4](\cdot)\f$ to be contracted
*/
)_docs";

const char* DOCS_CTCFUNCTION_CONTRACT_TUBE_TUBE_TUBE_TUBE_TUBE=R"_docs(/**
* \brief \f$\mathcal{C}\big([x_1](\cdot),[x_2](\cdot),[x_3](\cdot),[x_4](\cdot),[x_5](\cdot)\big)\f$
*
* \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
* \param x2 the scalar tube \f$[x_2](\cdot)\f$ to be contracted
* \param x3 the scalar tube \f$[x_3](\cdot)\f$ to be contracted
* \param x4 the scalar tube \f$[x_4](\cdot)\f$ to be contracted
* \param x5 the scalar tube \f$[x_5](\cdot)\f$ to be contracted
*/
)_docs";

const char* DOCS_CTCFUNCTION_CONTRACT_TUBE_TUBE_TUBE_TUBE_TUBE_TUBE=R"_docs(/**
* \brief \f$\mathcal{C}\big([x_1](\cdot),[x_2](\cdot),[x_3](\cdot),[x_4](\cdot),[x_5](\cdot),[x_6](\cdot)\big)\f$
*
* \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
* \param x2 the scalar tube \f$[x_2](\cdot)\f$ to be contracted
* \param x3 the scalar tube \f$[x_3](\cdot)\f$ to be contracted
* \param x4 the scalar tube \f$[x_4](\cdot)\f$ to be contracted
* \param x5 the scalar tube \f$[x_5](\cdot)\f$ to be contracted
* \param x6 the scalar tube \f$[x_6](\cdot)\f$ to be contracted
*/
)_docs";

const char* DOCS_CTCFUNCTION_CONTRACT_SLICE=R"_docs(/**
* \brief Contracts an array of slices (representing a slice vector)
*
* Propagates the contractions to the next slices
*
* \param v_x_slices the slices to be contracted
*/
)_docs";

