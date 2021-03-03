/** 
 *  \file
 *  CtcLohner class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Auguste Bourgois
 *  \copyright  Copyright 2020 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CTCLOHNER_H__
#define __CODAC_CTCLOHNER_H__

#include "codac_DynCtc.h"
#include "codac_TFnc.h"
#include "codac_Slice.h"


namespace codac {

/**
 * \class CtcLohner
 *
 * \brief \f$\mathcal{C}_\textrm{lohner}\f$ that contracts a tube \f$[\mathbf{x}](\cdot)\f$ according
 *        to a differential constraint \f$\dot{\mathbf{x}}=\mathbf{f}(\mathbf{x})\f$
 */
class CtcLohner : public codac::DynCtc {
public:

  /**
   * \brief Creates a contractor object \f$\mathcal{C}_\textrm{lohner}\f$
   *
   * \param f function corresponding to the differential constraint \f$\dot{\mathbf{x}}=\mathbf{f}(\mathbf{x})\f$
   * \param contractions number of contractions of the global enclosure by the estimated local enclosure
   * \param eps inflation parameter for the global enclosure
   */
  explicit CtcLohner(const Function &f, int contractions = 5, double eps = 0.1);

  /**
   * \brief Contracts the tube with respect to the specified differential constraint, either forward, backward (or both) in time
   *
   * @param tube tube to contract
   * @param t_propa direction of contraction
   */
  void contract(codac::TubeVector &tube, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);

  /**
   * \brief Contracts the tube with respect to the specified differential constraint, either forward, backward (or both) in time
   *
   * @param tube tube to contract
   * @param t_propa direction of contraction
   */
  void contract(codac::Tube &tube, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);

  /*
   * \brief Contracts a set of abstract domains
   *
   * This method makes the contractor available in the CN framework.
   *
   * \param v_domains vector of Domain pointers
   */
  void contract(std::vector<codac::Domain *> &v_domains) override;

protected:
  Function m_f; //!< forward function
  int contractions; //!< number of contractions of the global enclosure by the estimated local enclosure
  int dim; //!< dimension of the state vector
  double eps; //!< inflation parameter for the global enclosure

  static const std::string m_ctc_name; //!< class name (mainly used for CN Exceptions)
  static std::vector<std::string> m_str_expected_doms; //!< allowed domains signatures (mainly used for CN Exceptions)
  friend class ContractorNetwork;
};

} // namespace codac

#endif