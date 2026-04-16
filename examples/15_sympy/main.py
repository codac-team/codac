from codac import *

x = ScalarVar()
y = ScalarVar()
f = AnalyticFunction([x,y], x*y + sin(x))

dfdx = sympy_partial_diff(f, x)
dfdy = sympy_partial_diff(f, y)

print(dfdx)
print(dfdy)