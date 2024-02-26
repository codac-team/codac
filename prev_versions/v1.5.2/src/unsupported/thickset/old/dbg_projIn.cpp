#include "ibex_IntervalVector.h"
#include "ibex_Function.h"

#include "paving.h"
#include "pavingVisitor.h"
#include "vibes.h"
#include "InclTest.h"

using ibex::IntervalVector;
//using ibex::BoolInterval;
using ibex::ExtBoolean;
using ibex::Paving;

using namespace ibex;

class PdcIn : public ibex::InclTest{
public:
    PdcIn(IntervalVector& _Y) : InclTest(_Y.size()), Y(_Y){

    };
    ExtBoolean test(const ibex::IntervalVector &box){
        if ((box & Y).is_empty())
                return ibex::OUT;
        else if (box.is_subset(Y))
            return ibex::IN;
        else
            return ibex::MAYBE;
    }

private:
    IntervalVector Y;
};

class PdcFIn : public ibex::InclTest{
public:
    PdcFIn(Function &f, IntervalVector& itv): InclTest(2), f(f), itv(itv){}

    ExtBoolean test(const IntervalVector& X){
        IntervalVector tmp = f.eval_vector(X);
        this->is_enable = true;
        if (tmp.is_subset(itv) ){
            return IN;
        }else if (tmp.is_disjoint(itv) ){
            return OUT;
        } else {
            this->is_enable = true;
            return MAYBE;
        }
    }

    IntervalVector itv;
    Function& f;

};


class ToVibes: public PavingVisitor{
public:
    ToVibes(double max,const std::string& figureName): frame(2), figureName(figureName){

        frame = IntervalVector(2, max*Interval(-1, 1));
    }

    void visit_leaf(const IntervalVector &box, ExtBoolean status){

        IntervalVector framebox= box & frame;
        std::string color;
        if (status == IN)
            color="[#FF000048]";
        else if (status == OUT)
            color="[#0000FF48]";
        else if (status == MAYBE)
            color="[#FFFF0048]";
        else
            color="[g]";

        vibes::drawBox(framebox[0].lb(), framebox[0].ub(), framebox[1].lb(), framebox[1].ub(), color);
    }
    void pre_visit(){
        vibes::newFigure(figureName);
        vibes::setFigureProperties(vibesParams("x",0,"y",220,"width",400,"height",400));
    }

    void post_visit(){
        vibes::axisAuto();
    }

private:
    IntervalVector frame;
    std::string figureName;
};

int main(){
//    IntervalVector X0(2, Interval(-30, 30));
//    IntervalVector YA(2, Interval(-10, 10));
//    IntervalVector YB(2, Interval(-5, 15));
//    Paving A(X0, ibex::YES);
//    Paving B(X0, ibex::YES);

//    PdcIn pdcA(YA);
//    PdcIn pdcB(YB);

    vibes::beginDrawing();
    Function f("x[2]", "x[0]^2 + 3*x[0]*x[1] + x[1]^2");
    IntervalVector y(1, Interval(-1, 4));
    PdcFIn pdc(f, y);
    IntervalVector X0(2, Interval(-10,10));
    Paving A(X0, pdc, 0.02);
    A.Reunite();
    ToVibes VA(40, "A");
    A.visit(VA);

    IntervalVector X(2);
    X[0] = Interval(-3.2225, -2.4);
    X[1] = Interval(6.6725, 10);

    X[0] = Interval(-0.5, 1.00263);
    X[1] = Interval(-3.7225, -1);

    X[0] = Interval(-3.2225, -2.45);
    X[1] = Interval(6.6725, 8.16988);

    vibes::drawBox(X[0].lb(), X[0].ub(), X[1].lb(), X[1].ub(), "b");
    A.projIn(X, 0, true);

//    B.Sivia(pdcB, ibex::And, 0.1);
//    ToVibes VB(40, "B");
//    B.visit(VB);

//    A.Sivia2(pdcB, ibex::Inter, 0.1);
//    ToVibes VC(40, "C");

//    A.visit(VC);



    vibes::endDrawing();


}



