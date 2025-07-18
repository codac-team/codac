#include <codac>
using namespace codac2;

int main()
{
  // [A-q2-beg]
  Vector x_truth({2,1,PI/6});
  Vector y_truth({6,PI/6});
  Vector m_truth({5,6});
  // [A-q2-end]

  // [A-q3-beg]
  IntervalVector x = cart_prod(Interval(-oo,oo),Interval(-oo,oo),x_truth[2]);
  IntervalVector m = IntervalVector(m_truth).inflate(0.2);
  IntervalVector y = IntervalVector(y_truth).inflate(Vector({0.3,0.1}));
  // [A-q3-end]

  DefaultFigure::draw_box(m, Color::red());
  DefaultFigure::draw_tank(x_truth, 1, {Color::black(),Color::yellow()});
  // [A-q5-beg]
  DefaultFigure::draw_pie(x_truth.subvector(0,1), y[0],     x_truth[2]+y[1], Color::red());
  DefaultFigure::draw_pie(x_truth.subvector(0,1), y[0] | 0, x_truth[2]+y[1], Color::light_gray());
  // [A-q5-end]

  // [A-q6-beg]
  CtcPolar ctc_polar;

  VectorVar v1(2), v2(3), v3(2);
  AnalyticFunction f_minus({v1,v2,v3}, {
      v1[0]-v2[0]-v3[0],
      v1[1]-v2[1]-v3[1]
  });
  CtcInverse<IntervalVector, // f_minus output type
    IntervalVector,IntervalVector,IntervalVector // f_minus inputs (i.e. contractor inputs)
    > ctc_minus(f_minus, {0,0});

  ScalarVar s1, s2, s3;
  AnalyticFunction f_plus({s1,s2,s3}, s1+s2-s3);
  CtcInverse<
    Interval, // f_plus output type
    Interval,Interval,Interval // f_plus inputs (i.e. contractor inputs)
    > ctc_plus(f_plus, Interval(0.));
  // [A-q6-end]

  // [A-q7-beg]
  Interval a;
  IntervalVector d(2);
  // [A-q7-end]

  // [A-q8-beg]
  // Either with a smart order of contractor calls:
  
  ctc_plus.contract(x[2],y[1],a);
  ctc_polar.contract(d[0],d[1],y[0],a);
  ctc_minus.contract(m,x,d);

  // Or using a fixpoint method:
  
  fixpoint([&](){
    ctc_plus.contract(x[2],y[1],a);
    ctc_polar.contract(d[0],d[1],y[0],a);
    ctc_minus.contract(m,x,d);
  }, x,y,m);
  // [A-q8-end]

  // [A-q9-beg]
  std::cout << x << std::endl;
  DefaultFigure::draw_box(x); // does not display anything if unbounded
  // [A-q9-end]
}