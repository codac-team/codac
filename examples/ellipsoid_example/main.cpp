#include <codac>

using namespace std;
using namespace codac2;

int main()
{
    Vector mu({1.3,2.7});
    Matrix G({{2.3,0.5},{0.5,1.7}});
    Ellipsoid e1(mu, G);

    Matrix A({{0.,0.5},{-0.5,0.9}});
    Vector b({1.3,2.1});

    Ellipsoid e2 = linear_mapping(e1, A, b);

    cout << "Linear Mapping" << endl;
    cout << "Initial ellipsoid e1:" << endl;
    cout << e1 << endl;

    cout << "Image ellipsoid e2:" << endl;
    cout << e2 << endl;

    cout << "non Linear Mapping" << endl;
    AnalyticFunction h {
            {x,y}, vec(x[0]+0.5*sin(x[1]),-0.5*sin(x[0])+0.9)};
    Ellipsoid e3 = nonlinear_mapping(e1, f);
}
