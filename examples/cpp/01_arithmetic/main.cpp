/* ============================================================================
 *  tubex-lib - Arithmetic on tubes: simple example
 *
 *  Example from the paper "Guaranteed Computation of Robot Trajectories"
 *  Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 *
 *  Here we consider the following tubes:
 *    [x](·)
 *    [y](·)
 *    [a](·) = [x](·) + [y](·)
 *    [b](·) = sin([x](·))
 *    [c](·) = integ([x](tau)dtau)
 *    [d](·) = |[y](·)|
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#include "tubex.h"

using namespace std;
using namespace ibex;
using namespace tubex;

class TubeNode
{
  public:

    TubeNode()
    {

    }

    virtual void test()
    {
      cout << "TubeNode::test()" << endl;
    }

    virtual void test2()
    {
      first->test();
      second->test();
    }

    TubeNode *first, *second;
};

class TubeSlice : public TubeNode
{
  public:

    TubeSlice() : TubeNode()
    {
      m_a = 2;
    }

    void test()
    {
      //TubeNode::test();
      cout << "TubeSlice::test()" << endl;
    }

    int m_a;
};

class Tube : public TubeNode
{

};

int main(int argc, char *argv[])
{
  TubeNode test;
  test.first = new TubeSlice();
  test.second = new TubeNode();
  test.test2();
  return 1;

  //cout << ((TubeSlice*)test.first)->m_a << endl;
  test.first->test();

  TubeSlice test2;
  test2.test();

  return 0;
}