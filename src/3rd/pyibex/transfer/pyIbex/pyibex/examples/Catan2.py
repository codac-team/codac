from pyibex import Interval, tan, bwd_imod, atan
import math

def Cmod(x,y):
  xx = Interval(x)
  yy = Interval(y)
  bwd_imod(xx,yy,2*math.pi)
  return yy


def Catan2(x,y,th):
  iZeroPi2 = Interval(0)| Interval.PI/2.
  iPi2Pi = Interval.PI/2. | Interval.PI
  if x.is_empty() or y.is_empty() or th.is_empty():
    return Interval.EMPTY_SET, Interval.EMPTY_SET, Interval.EMPTY_SET
  if x.is_subset(Interval.POS_REALS) and y.is_subset(Interval.POS_REALS) and th.is_subset(iZeroPi2):
    th_tmp = Cmod(th, iZeroPi2)
    tan_lb = tan(Interval(th_tmp.lb()))
    tan_ub = Interval(1e10) if th_tmp.ub() == Interval.HALF_PI.ub() else tan(Interval(th_tmp.ub()))
    tanTh = tan_lb | tan_ub
    # xx, yy = Interval(x), Interval(y)
    # bwd_mul(yy, tan(th_tmp), xx)
    xx = x & y/tanTh#tan(th_tmp)
    yy = y & x*tanTh#tan(th_tmp)
    thr = th_tmp & atan(y/x)
    # print("RESULT : ",x, y, th_tmp, tan(th_tmp), tan_lb, tan_ub,  y/tanTh)
    if xx.is_empty() or yy.is_empty() or thr.is_empty():
      return Interval.EMPTY_SET, Interval.EMPTY_SET, Interval.EMPTY_SET
    # print(yy,xx,thr)
    bwd_imod(th, thr, 2*Interval.PI.ub())
    return xx, yy, thr

  else:
    # th_tmp = Interval(th)
    # bwd_imod( th_tmp, th  ,2*math.pi)
    x1 = x & Interval.POS_REALS
    y1 = y & Interval.POS_REALS
    th1 = Cmod(th,iZeroPi2)
    x1, y1, th1 = Catan2(x1, y1, th1)
    th11 = Interval(th)
    bwd_imod(th11, th1, 2*Interval.PI.ub())

    x2 = x & Interval.POS_REALS
    y2 = y & Interval.NEG_REALS
    th2 = -Cmod(th, -iZeroPi2)
    x2, y2, th2 = Catan2(x2, -y2, th2)
    th22 = Interval(th)
    bwd_imod(th22, -th2, 2*Interval.PI.ub())

    x3 = x & Interval.NEG_REALS
    y3 = y & Interval.NEG_REALS
    th3 = Interval.PI + Cmod(th, -iPi2Pi)
    x3, y3, th3 = Catan2(-x3, -y3, (th3 &  iZeroPi2) )
    th33 = Interval(th)
    bwd_imod(th33, th3 - Interval.PI, 2*Interval.PI.ub())

    #
    x4 = x & Interval.NEG_REALS
    y4 = y & Interval.POS_REALS
    th4 = Interval.PI - Cmod( th,iPi2Pi)
    x4, y4, th4 = Catan2(-x4, y4, ( th4 & iZeroPi2) )
    th44 = Interval(th)
    bwd_imod(th44, Interval.PI -  th4, 2*Interval.PI.ub())

    xx =   ( x1 | x2 | (-x3) | (-x4) )
    yy =   ( y1 | (-y2) | (-y3) | (y4) )
    thh =  ( th11 | (th22) | ( th33 ) | ( th44 ) )
    return xx,yy,thh
