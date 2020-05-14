
/** 
 *  ile
 *  Ctc documentation header 
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  uthor     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
 

const char* DOCS_TIMEPROPAG=R"_docs(
 /**
   * \enum TPropagation
   * \brief Specifies the temporal propagation way (forward or backward in time)
   */
)_docs";

const char* DOCS_CTC=R"_docs(/**
* \class Ctc
* \brief Contractor interface
*/
)_docs";

const char* DOCS_CTC_CTC=R"_docs(/**
* \brief Creates a contractor
*/
)_docs";

const char* DOCS_CTC_CONTRACT_VECTOR_ABSTRACTDOMAIN=R"_docs(
)_docs";

const char* DOCS_CTC_PRESERVE_SLICING_BOOL=R"_docs(/**
* \brief Specifies whether the contractor can impact the tube's slicing or not
*
* \param preserve if true, the slicing will remain the same
*/
)_docs";

const char* DOCS_CTC_SET_FAST_MODE_BOOL=R"_docs(/**
* \brief Specifies an optional fast mode of contraction
*
* \note For some contractors, pessimistic but faster contractions can be achieved.
*
* \param fast_mode if true, fast mode enabled
*/
)_docs";

const char* DOCS_CTC_RESTRICT_TDOMAIN_INTERVAL=R"_docs(/**
* \brief Limits the temporal domain of contractions
*
* \param domain Interval domain of restriction \f$[t_1,t_2]\f$
*/
)_docs";

