#ifndef __PYIBEX_TYPE_CASTER_H__
#define __PYIBEX_TYPE_CASTER_H__

#include "ibex_Vector.h"
#include "ibex_Array.h"
#include "ibex_Ctc.h"
#include <pybind11/pybind11.h>

NAMESPACE_BEGIN(pybind11)
NAMESPACE_BEGIN(detail)

using ibex::Vector;
using ibex::Array;

template <> struct type_caster<ibex::Vector> {
public:
    PYBIND11_TYPE_CASTER(ibex::Vector, _("Vector"));

    type_caster<ibex::Vector>() :  value(1) { }
    bool load(handle src_hdl, bool convert) {
        PyObject *src = src_hdl.ptr();
        if (!PyList_Check(src) && !PyTuple_Check(src))
            return false;
        size_t size = (size_t) ( PyList_Check(src) ? PyList_GET_SIZE(src) : PyTuple_GET_SIZE(src) );
        type_caster<double> conv;
        value.resize(size);
        for (size_t i=0; i<size; ++i) {
            if (PyList_Check(src)) {
              if (!conv.load(PyList_GetItem(src, (ssize_t) i), convert))
                  return false;
            } else {
              if (!conv.load(PyTuple_GetItem(src, (ssize_t) i), convert))
                  return false;
            }
            value[i] = (double) conv;
        }
        return true;
    }

    static handle cast(const ibex::Vector &src, return_value_policy policy, handle parent) {
        list l(src.size());

        for (size_t i = 0; i < static_cast<pybind11::size_t>(src.size()); i++){
            auto value_ = reinterpret_steal<object>(type_caster<double>::cast(src[i], policy, parent));
            if (!value_) {
                return handle();
            }
            PyList_SET_ITEM(l.ptr(), i, value_.release().ptr()); // steals a reference
        }
        return l.release();
    }
};

template <typename Value> struct type_caster<ibex::Array<Value> > {
    typedef type_caster<Value> value_conv;
public:
    PYBIND11_TYPE_CASTER(ibex::Array<Value>, _("Array<Value>"));

    bool load(handle src_hdl, bool convert) {
      PyObject *src = src_hdl.ptr();
      if (!isinstance<list>(src))
            return false;
        auto l = reinterpret_borrow<list>(src);
        size_t size = (size_t) PyList_GET_SIZE(src);
        value.resize(size);
        value_conv conv;
        for (size_t i=0; i<size; ++i) {
            if (!conv.load(PyList_GetItem(src, (ssize_t) i), convert))
                return false;
            value.set_ref(i, *(Value*) conv);
        }
        return true;
    }

    static handle cast(const ibex::Array<Value> &src, return_value_policy policy, handle parent) {
        list l(src.size());
        return l.release();
    }
};

NAMESPACE_END(detail)
NAMESPACE_END(pybind11)

#endif // __PYIBEX_TYPE_CASTER_H__
