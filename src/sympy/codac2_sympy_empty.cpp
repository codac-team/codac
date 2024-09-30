#include <pybind11/embed.h>
namespace py = pybind11;

void foo()
{

  py::scoped_interpreter guard{}; // start the interpreter and keep it alive

  py::module_ sympy = py::module_::import("sympy");
  // srepr

  py::object x = sympy.attr("Symbol")('x');
  py::object mul = sympy.attr("Add")(x,3);
  mul = sympy.attr("Add")(mul,x);
  py::print(mul);
  //py::object f = sympy.attr("sympify")("Add(Pow(Symbol('x'), Integer(2)), Mul(Symbol('x'), Symbol('y')))");
  //f = sympy.attr("diff")(mul,"x");
  //py::print(f);
  py::print(mul.attr("func"));
  for(const auto& pi : mul.attr("args"))
    py::print(pi);
  //py::print(mul.attr("args")[0]);
}