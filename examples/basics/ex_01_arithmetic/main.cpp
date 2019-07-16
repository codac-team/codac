
  #include "tubex.h"
  
  using namespace std;
  using namespace ibex;
  using namespace tubex;
  
  int main()
  {
    // Trajectory from a formula
    TrajectoryVector x(Interval(0.,10.),
                       tubex::Function("(cos(t);sin(t))"));

    // Discretized trajectory
    TrajectoryVector y(2);
    for(double t = 0. ; t <= 10. ; t+=0.6)
    {
      Vector val(2);
      val[0] = cos(t); val[1] = sin(t);
      y.set(val, t);
    }

    // Graphics:
    vibes::beginDrawing();
    VIBesFigTube fig("Trajectories");
    fig.set_properties(100, 100, 600, 300);

    // From a formula
    fig.add_trajectory(&y[0], "y_0", "gray"); // first component
    fig.add_trajectory(&y[1], "y_1", "gray"); // second component

    // From discrete values
    fig.add_trajectory(&x[0], "x_0", "red");  // first component
    fig.add_trajectory(&x[1], "x_1", "blue"); // second component

    fig.show();
    vibes::endDrawing();
  }