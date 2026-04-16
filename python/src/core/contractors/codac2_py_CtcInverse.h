/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <vector>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <codac2_CtcInverse.h>
#include <codac2_CtcInverseNotIn.h>
#include <codac2_analytic_variables.h>
#include <codac2_AnalyticExprWrapper.h>

#include "codac2_py_Ctc.h"
#include "codac2_py_CtcInverse_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_CtcInverseNotIn_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_Ctc_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


template<typename C>
void export_CtcInverse(py::module& m,
                       const std::string& export_name,
                       py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  using T = typename C::OutputType;
  using D = typename T::Domain;

  py::class_<C> exported_ctc(m, export_name.c_str(), pyctc, CTCINVERSE_MAIN);
  exported_ctc

    .def(py::init(
        [](const AnalyticFunction<T>& f, const D& y, bool with_centered_form)
        {
          return std::make_unique<C>(unaryize_function(f), y, with_centered_form);
        }
      ),
      CTCINVERSE_Y_X0X_CTCINVERSE_CONST_ANALYTICFUNCTION_OUTPUTTYPE_REF_CONST_Y_REF_BOOL_BOOL,
      "f"_a, "y"_a, "with_centered_form"_a = true)
  ;

  if constexpr(std::is_same_v<T,VectorType>) // contractors are only associated with interval vectors
  {
    exported_ctc
    .def(py::init(
        [](const AnalyticFunction<T>& f, const CtcBase<IntervalVector>& c, bool with_centered_form)
        {
          return std::make_unique<C>(unaryize_function(f), c.copy(), with_centered_form);
        }
      ),
      CTCINVERSE_Y_X0X_CTCINVERSE_CONST_ANALYTICFUNCTION_OUTPUTTYPE_REF_CONST_C_REF_BOOL_BOOL,
      "f"_a, "c"_a, "with_centered_form"_a = true);
  }

  CONTRACT_METHODS(exported_ctc, C,
    VOID_CTCINVERSE_Y_X0X_CONTRACT_X0_REF_X_REF_VARIADIC_CONST)

  // In order to deal with variadic arguments (contractors on mixed/various domains)
  auto def_variadic = [](const C& c, py::args xs) -> py::object
  {
    auto return_args = [&](py::args xs_) -> py::object
    {
      if(xs_.size() == 1)
        return py::reinterpret_borrow<py::object>(xs_[0]);

      py::tuple out(xs_.size());
      for(size_t i = 0 ; i < xs_.size() ; ++i)
        out[i] = py::reinterpret_borrow<py::object>(xs_[i]);
      return out;
    };

    if(xs.size() == 0)
      assert_release(false && "contract() expects at least one argument");

    // Fast paths already consistent with the typed overloads

    if(xs.size() == 1 && py::isinstance<IntervalVector>(xs[0]))
    {
      auto& x = xs[0].cast<IntervalVector&>();
      as_ctc_base<C>(c).contract(x);
      return py::cast(x, py::return_value_policy::reference);
    }

    if(xs.size() == 1 && py::isinstance<SlicedTube<IntervalVector>>(xs[0]))
    {
      auto& x = xs[0].cast<SlicedTube<IntervalVector>&>();
      as_ctc_base<C>(c).contract(x);
      return py::cast(x, py::return_value_policy::reference);
    }

    // Detecting whether we are in "domains" mode or "tubes" mode

    bool has_domain_args = false;
    [[maybe_unused]] bool has_tube_args = false;

    for(size_t i = 0 ; i < xs.size() ; ++i)
    {
      py::handle h = xs[i];

      if(py::isinstance<Interval>(h) || py::isinstance<IntervalVector>(h))
        has_domain_args = true;
      else if(py::isinstance<SlicedTube<Interval>>(h) || py::isinstance<SlicedTube<IntervalVector>>(h))
        has_tube_args = true;
      else
      {
        assert_release(false && "contract(): variadic mode only supports "
          "Interval / IntervalVector / SlicedTube<Interval> / SlicedTube<IntervalVector>");
      }
    }

    assert_release(!(has_domain_args && has_tube_args)
      && "contract(): cannot mix Interval domains and SlicedTube arguments in the same call");

    // Domain mode: Interval / IntervalVector

    if(has_domain_args)
    {
      Index total_size = 0;
      for(size_t i = 0 ; i < xs.size() ; ++i)
      {
        py::handle h = xs[i];

        if(py::isinstance<Interval>(h))
          total_size += 1;
        else
          total_size += h.cast<IntervalVector&>().size();
      }

      assert_release(total_size == as_ctc_base<C>(c).size()
        && "contract(): wrong total input dimension");

      IntervalVector total(total_size);

      Index offset = 0;
      for(size_t i = 0 ; i < xs.size() ; ++i)
      {
        py::handle h = xs[i];

        if(py::isinstance<Interval>(h))
        {
          total[offset] = h.cast<Interval&>();
          ++offset;
        }
        else
        {
          auto& x = h.cast<IntervalVector&>();
          total.put(offset, x);
          offset += x.size();
        }
      }

      as_ctc_base<C>(c).contract(total);

      offset = 0;
      for(size_t i = 0 ; i < xs.size() ; ++i)
      {
        py::handle h = xs[i];

        if(py::isinstance<Interval>(h))
        {
          h.cast<Interval&>() &= total[offset];
          ++offset;
        }
        else
        {
          auto& x = h.cast<IntervalVector&>();
          x &= total.subvector(offset, offset + x.size() - 1);
          offset += x.size();
        }
      }

      return return_args(xs);
    }

    // Tube mode: SlicedTube<Interval> / SlicedTube<IntervalVector>

    auto first = xs[0];

    std::shared_ptr<TDomain> tdomain;
    if(py::isinstance<SlicedTube<Interval>>(first))
      tdomain = first.cast<SlicedTube<Interval>&>().tdomain();
    else
      tdomain = first.cast<SlicedTube<IntervalVector>&>().tdomain();

    Index total_size = 0;
    for(size_t i = 0 ; i < xs.size() ; ++i)
    {
      assert_release([&](){
        py::handle h = xs[i];
        if(py::isinstance<SlicedTube<Interval>>(h))
        {
          [[maybe_unused]] auto& x = h.cast<SlicedTube<Interval>&>();
          if(!TDomain::are_same(tdomain, x.tdomain()))
            return false;
          total_size += 1;
        }
        else
        {
          [[maybe_unused]] auto& x = h.cast<SlicedTube<IntervalVector>&>();
          if(!TDomain::are_same(tdomain, x.tdomain()))
            return false;
          total_size += x.size();
        }
        return true;
      }() && "contract(): all SlicedTube arguments must share the same tdomain");
    }

    assert_release(total_size == as_ctc_base<C>(c).size()
      && "contract(): wrong total input dimension");

    SlicedTube<IntervalVector> total(tdomain, IntervalVector(total_size));

    for(auto it = tdomain->begin() ; it != tdomain->end() ; ++it)
    {
      Index offset = 0;

      for(size_t i = 0 ; i < xs.size() ; ++i)
      {
        py::handle h = xs[i];

        if(py::isinstance<SlicedTube<Interval>>(h))
        {
          auto& x = h.cast<SlicedTube<Interval>&>();
          total.slice(it)->codomain()[offset] = x.slice(it)->codomain();
          ++offset;
        }
        else
        {
          auto& x = h.cast<SlicedTube<IntervalVector>&>();
          total.slice(it)->codomain().put(offset, x.slice(it)->codomain());
          offset += x.size();
        }
      }
    }

    as_ctc_base<C>(c).contract(total);

    Index offset = 0;
    for(size_t i = 0 ; i < xs.size() ; ++i)
    {
      py::handle h = xs[i];

      if(py::isinstance<SlicedTube<Interval>>(h))
      {
        auto& x = h.cast<SlicedTube<Interval>&>();
        x &= total[offset];
        ++offset;
      }
      else
      {
        auto& x = h.cast<SlicedTube<IntervalVector>&>();
        x &= total.subvector(offset, offset + x.size() - 1);
        offset += x.size();
      }
    }

    return return_args(xs);
  };

  exported_ctc.def("contract", def_variadic,
    VOID_CTCINVERSE_Y_X0X_CONTRACT_X0_REF_X_REF_VARIADIC_CONST);

  // Deprecated
  exported_ctc.def("contract_tube", [&def_variadic](const C& c, py::args xs) -> py::object
    {
      std::cout << "CtcInverse.contract_tube(...) is deprecated; \
        use CtcInverse.contract(...) with SlicedTube arguments instead" << std::endl;
      return def_variadic(c,xs);
    },
    VOID_CTCINVERSE_Y_X0X_CONTRACT_X0_REF_X_REF_VARIADIC_CONST);

  // Disabled for CtcInverseNotInt
  if constexpr(std::is_same_v<C,CtcInverse<Interval>> || std::is_same_v<C,CtcInverse<IntervalVector>>)
  {
    exported_ctc.def("fnc", &C::fnc,
      CONST_ANALYTICFUNCTION_TYPENAME_EXPRTYPE_Y_TYPE_CTCINVERSE_Y_X0X__F);
  }
}