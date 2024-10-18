import codac
print('Codac version: ', codac.__version__)
from codac import *
import timeit

# Comparisons of two distance contractors

# Using manual forward / backward algorithm

time_sum = 0

for i in range(0, 100000):
  x = IntervalVector([[2,2.5],[0,0.5],[1.5,2.5],[4,11],[7,8]])
  start = timeit.default_timer()
  ctc.dist.contract(x) # contractor from Codac
  time_sum += timeit.default_timer() - start
  #if i == 0: print(x)

print('[ Manual ctc   ] Time:', time_sum)


# Using automatic HC-Revise algorithm (IBEX)

ctc_f = CtcFunction(
    Function("a[2]", "b[2]", "d",
             "d - sqrt((a[0]-b[0])^2+(a[1]-b[1])^2)"))

time_sum = 0

for i in range(0, 100000):
  x = IntervalVector([[2,2.5],[0,0.5],[1.5,2.5],[4,11],[7,8]])
  start = timeit.default_timer()
  ctc_f.contract(x)
  time_sum += timeit.default_timer() - start
  #if i == 0: print(x)

print('[ HC4 IBEX ctc ] Time:', time_sum)