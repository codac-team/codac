/** 
 *  CAPD to Tubex
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Julien Damers
 *  \copyright  Copyright 2020 Tubex
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_capd2tubex.h"
#include "tubex_Exception.h"
#include "capd/capdlib.h"

using namespace std;
using namespace ibex;
using namespace tubex;

namespace tubex
{
     // namespace tubex


    string tubexFnc2capdString(const TFunction& f)
    {

        int a_capd_dim = f.nb_vars();
        int a_ibex_dim = a_capd_dim+1;

        IntervalVector a_ibex(a_ibex_dim);
        vector<IntervalVector> ibex_curve;

        // Generate the string that CAPD will process to compute
        string capd_string ="time:t;var:";
        string function_string = f.expr().substr(1, f.expr().size() - 2); // removing outside parentheses
        const char comma = ',';
        const char semicolon = ';';
        replace(function_string.begin(),function_string.end(),semicolon,comma);
        for(int i=0;i<a_capd_dim-1; i++)
        {
            capd_string.append(f.arg_name(i));
            capd_string.append(",");

        }
        capd_string.append(f.arg_name(a_capd_dim-1));
        capd_string.append(";fun:");
        try
        {
            if (function_string.size()>2)
            {
                capd_string.append(function_string);
                capd_string.append(";");

            }
        }
        catch(exception& e)
        {
            cout << "\n\nException caught!\n" << e.what() << endl << endl;
        }

        return (capd_string);

    }



    vector<IntervalVector> capd2ibex(const Interval& domain, capd::IMap& vectorField, const IntervalVector& x0,
                                           const double& timestep)
    {
        int a_capd_dim = x0.size();
        int a_ibex_dim = a_capd_dim+1;

        IntervalVector a_ibex(a_ibex_dim);
        vector<IntervalVector> ibex_curve;


        try
        {
            // CAPD processing

            // The solver uses high order enclosure method to verify the existence
            // of the solution.
            // The order will be set to 20.
            capd::IOdeSolver solver(vectorField,20);

            // Set a fixed integration step if needed
            if (timestep!=0)
            {
                solver.setStep(timestep);
            }
            capd::ITimeMap timeMap(solver);

            // This is our initial condition
            capd::IVector a_capd(a_capd_dim);
            for (int i = 0; i<a_capd_dim; i++)
            {
                a_capd[i] = capd::interval(x0[i].lb(),x0[i].ub());
            }
            // define a doubleton representation of the interval vector x
            capd::C0Rect2Set s(a_capd);

            // Here we start to integrate. The time of integration is set to the value given in the json file.
            double T=domain.ub();
            timeMap.stopAfterStep(true);
            capd::interval prevTime(0.);

            do
            {
                timeMap(T,s);
                capd::interval stepMade = solver.getStep();
                //cout << "\nstep made: " << stepMade;
                // This is how we can extract an information
                // about the trajectory between time steps.
                // The type CurveType is a function defined
                // on the interval [0,stepMade].
                // It can be evaluated at a point (or interval).
                // The curve can be also differentiated wrt to time.
                // We can also extract from it the 1-st order derivatives wrt.
                const capd::IOdeSolver::SolutionCurve& curve = solver.getCurve();
                capd::interval domain = capd::interval(0,1)*stepMade;

                // Here we use a uniform grid of last time step made
                // to enclose the trajectory between time steps.
                // You can use your own favorite subdivision, perhaps nonuniform,
                // depending on the problem you want to solve.
                int grid=2;
                for(int i=0;i<grid;++i)
                {
                    capd::interval subsetOfDomain = capd::interval(i,i+1)*stepMade/grid;
                    // The above interval does not need to be a subset of domain.
                    // This is due to rounding to floating point numbers.
                    // We take the intersection with the domain.
                    intersection(domain,subsetOfDomain,subsetOfDomain);
                    // Here we evaluated curve at the interval subsetOfDomain.
                    // v will contain rigorous bound for the trajectory for this time interval.
                    capd::IVector v = curve(subsetOfDomain);
                    capd::interval currentTime = prevTime + subsetOfDomain;

                    // Converting capd box into an IBEX one
                    a_ibex[0] = Interval(currentTime.left().leftBound(),currentTime.right().rightBound());
                    for (int i=0; i< a_capd_dim; i++)
                    {
                        a_ibex[i+1] = Interval(v[i].left().leftBound(),v[i].right().rightBound());
                    }
                    // Adding our computed box to the curve in IBEX format
                    ibex_curve.push_back(a_ibex);

                }
                prevTime = timeMap.getCurrentTime();
            }while(!timeMap.completed());

        }

        catch(exception& e)
        {
            cout << "\n\nException caught!\n" << e.what() << endl << endl;
        }
        return(ibex_curve);

    }


    TubeVector ibex2tubex(vector<IntervalVector>& ibex_curve)
    {
        /*
         * To create a tube we need to assert that for each slice, the end of the slice
         * is equal to the beginning of the next slice. This function first verify if
         * for each box of the ibex_curve the condition mentioned above is verified. If
         * it is the box is directly converted into a slice. If not, the current box and the next
         * one are processed to verify this condition.
         */

        /*
         * Initialize two vectors that will store time domain and the interval on the other dimensions
         * for each slice. v_domains[i] + v_codomains[i] = slice[i] of the tube
         */
        vector<Interval> v_domains; // Store the time interval of each slice of the tube
        vector<IntervalVector> v_codomains; // Store the other dimensions of the slice of the tube


        IntervalVector current_box(ibex_curve[0].size());
        IntervalVector mid_box(ibex_curve[0].size());

        for(size_t i=0; i<ibex_curve.size()-1;i++)
        {   
            IntervalVector current_box = ibex_curve[i];

            if((current_box[0].ub() == ibex_curve[i+1][0].lb()))
            {
                /*
                 * |###############||###########|
                 * | current_box[0]||next box[0]|
                 * |###############||###########|
                 *
                 *  condition verified? Ok
                 *
                 * --> convert to slice directly
                 */
                v_domains.push_back(current_box[0]);
                v_codomains.push_back(current_box.subvector(1,current_box.size()-1));
            }
            else
            {
                /*
                 * |##########################|
                 * |      current_box[0]      |
                 * |##########################|
                 *                 |#######################|
                 *                 |      next box[0]      |
                 *                 |#######################|
                 *
                 *  condition verified? No
                 *
                 * --> Truncating boxes and creating a link in the middle
                 * |##############||##########||###########|
                 * |current_box[0]||mid_box[0]||next box[0]|
                 * |##############||##########||###########|
                 *
                 * --> convert current_box and mid_box to slices
                 */


                mid_box[0] = current_box[0]&ibex_curve[i+1][0]; // storing time dimension of the mid box
                for (int k=1;k<mid_box.size();k++)
                {
                    // Ensure that we keep the convex hull so that our computation is guaranteed (may add pessimism)
                    mid_box[k] = current_box[k]|ibex_curve[i+1][k];

                }
                current_box[0] = Interval(current_box[0].lb(), mid_box[0].lb());
                ibex_curve[i+1][0] = Interval(mid_box[0].ub(),ibex_curve[i+1][0].ub());

                v_domains.push_back(current_box[0]);
                v_codomains.push_back(current_box.subvector(1,current_box.size()-1));
                v_domains.push_back(mid_box[0]);
                v_codomains.push_back(mid_box.subvector(1,current_box.size()-1));
            }

        }
        v_domains.push_back(ibex_curve[ibex_curve.size()-1][0]);
        v_codomains.push_back(ibex_curve[ibex_curve.size()-1].subvector(1,ibex_curve[ibex_curve.size()-1].size()-1));

        // Creating our Tube Vector object thanks to the info previously stored
        TubeVector output_tube(v_domains,v_codomains);
        return(output_tube);
    }


    TubeVector capd2tubex(const Interval& domain, const TFunction& f, const IntervalVector& x0, const double timestep)
    {
        string capd_string = tubexFnc2capdString(f);
        try
        {
            capd::IMap vectorField(capd_string);
            vector<IntervalVector> ibex_curve = capd2ibex(domain, vectorField, x0, timestep);
            return(ibex2tubex(ibex_curve));

        }
        catch(exception& e)
        {
            throw Exception("capd2tubex", e.what());
        }

    }

    // TODO add version of capd2tubex that can handle parameters

}

