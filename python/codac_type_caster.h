#ifndef __CODAC_TYPE_CASTER_H__
#define __CODAC_TYPE_CASTER_H__

#include "codac_Vector.h"
#include "ibex_Array.h"
#include <pybind11/pybind11.h>

#ifdef PYBIND11_NAMESPACE_BEGIN
PYBIND11_NAMESPACE_BEGIN(pybind11)
PYBIND11_NAMESPACE_BEGIN(detail)
#else
NAMESPACE_BEGIN(pybind11)
NAMESPACE_BEGIN(detail)
#endif // PYBIND11_NAMESPACE_BEGIN

template <> struct type_caster<codac::Vector>
{
  public:

    PYBIND11_TYPE_CASTER(codac::Vector, _("Vector"));

    type_caster<codac::Vector>() : value(1) { }
    bool load(handle src_hdl, bool convert)
    {
      PyObject *src = src_hdl.ptr();

      if(!PyList_Check(src) && !PyTuple_Check(src))
        return false;

      size_t size = (size_t) (PyList_Check(src) ? PyList_GET_SIZE(src) : PyTuple_GET_SIZE(src));
      type_caster<double> conv;
      value.resize(size);

      for(size_t i = 0; i < size; ++i)
      {
        if(PyList_Check(src))
        {
          if(!conv.load(PyList_GetItem(src, (ssize_t)i), convert))
            return false;
        }

        else
        {
          if(!conv.load(PyTuple_GetItem(src, (ssize_t)i), convert))
            return false;
        }

        value[i] = (double) conv;
      }
      
      return true;
    }

    static handle cast(const codac::Vector &src, return_value_policy policy, handle parent)
    {
      list l(src.size());

      for(size_t i = 0; i < static_cast<pybind11::size_t>(src.size()); i++)
      {
        auto value_ = reinterpret_steal<object>(type_caster<double>::cast(src[i], policy, parent));
        if(!value_)
          return handle();

        PyList_SET_ITEM(l.ptr(), i, value_.release().ptr()); // steals a reference
      }

      return l.release();
    }
};

template <typename Value> struct type_caster<ibex::Array<Value>>
{
  typedef type_caster<Value> value_conv;

  public:

    PYBIND11_TYPE_CASTER(ibex::Array<Value>, _("Array<Value>"));

    bool load(handle src_hdl, bool convert)
    {
      PyObject *src = src_hdl.ptr();

      if(!isinstance<list>(src))
        return false;
      
      auto l = reinterpret_borrow<list>(src);
      size_t size = (size_t) PyList_GET_SIZE(src);
      value.resize(size);
      value_conv conv;
      
      for(size_t i = 0; i < size; ++i)
      {
        if(!conv.load(PyList_GetItem(src, (ssize_t)i), convert))
          return false;
        
        value.set_ref(i, *(Value*) conv);
      }
      
      return true;
    }

    static handle cast(const ibex::Array<Value> &src, return_value_policy policy, handle parent)
    {
      list l(src.size());
      return l.release();
    }
};

#ifdef PYBIND11_NAMESPACE_END
PYBIND11_NAMESPACE_END(detail)
PYBIND11_NAMESPACE_END(pybind11)
#else
NAMESPACE_END(detail)
NAMESPACE_END(pybind11)
#endif // PYBIND11_NAMESPACE_END

#endif // __PYIBEX_TYPE_CASTER_H__