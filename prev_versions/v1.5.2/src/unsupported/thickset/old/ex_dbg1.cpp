#include "ibex.h"
#include "paving.h"
#include "vibes.h"

using ibex::IntervalVector;
using ibex::BoolInterval;
using ibex::Paving;
using namespace ibex;
class PdcIn : public ibex::Pdc{
public:
    PdcIn(IntervalVector& _Y) : Pdc(_Y.size()), Y(_Y){

    };
    BoolInterval test(const ibex::IntervalVector &box){
        if ((box & Y).is_empty())
                return ibex::NO;
        else if (box.is_subset(Y))
            return ibex::YES;
        else
            return ibex::MAYBE;
    }

private:
    IntervalVector Y;

};


class ToVibes: public SetVisitor{
public:
    ToVibes(double max,const string& figureName): frame(2), figureName(figureName){

        frame = IntervalVector(2, max*Interval(-1, 1));
    }

    void visit_leaf(const IntervalVector &box, BoolInterval status){

        IntervalVector framebox= box & frame;
        string color;
        if (status == YES)
            color="[#FF000048]";
        else if (status == NO)
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
    string figureName;
};

int main(){
    IntervalVector X0(2, Interval(-30, 30));
    IntervalVector YA(2, Interval(-10, 10));
    IntervalVector YB(2, Interval(-5, 15));
    Paving A(X0, ibex::YES);
    Paving B(X0, ibex::YES);

    PdcIn pdcA(YA);
    PdcIn pdcB(YB);

    vibes::beginDrawing();

    A.Sivia(pdcA, ibex::And, 0.1);
    ToVibes VA(40, "A");
    A.visit(VA);

    B.Sivia(pdcB, ibex::And, 0.1);
    ToVibes VB(40, "B");
    B.visit(VB);

    A.Sivia2(pdcB, ibex::Inter, 0.1);
    ToVibes VC(40, "C");

    A.visit(VC);



    vibes::endDrawing();

//        B = Paving(X0, YES)
//        B.Sivia(pdcB, ibex.op_And, 0.1)
//        B.visit(ToVibes(40, "B"))

//        # P = IntervalVector([[-4,-2], [0,0]])
//        P = IntervalVector([[-1,-1], [0, 0]])
//        f = Function("x", "y", "(x + %s, y + %s)"%(P[0], P[1]))
//        # X0 = IntervalVector([[-29, 31], [-30, 30]])

//        # exit(-1)
//        # pdc = pyPdcTransform(f, pyPdcInSet(A))
//        pdc = pyPdcInSet(A)


//        B.Sivia2(pdc, ibex.op_Inter, 0.1)
//        B.visit(ToVibes(30, "C2"))
//        # vibes.drawCircle(3,0, 1, 'g')
//        # vibes.drawCircle(-3,0, 1, 'r')

//        # TA = Paving(X0, pdc, 0.1)
//        # TA.visit(ToVibes(40, 'TA'))
//        # C = Paving(X0, MAYBE)
//        # op_binaire(C, TA, B, ibex.op_Inter)
//        # C.visit(ToVibes(40, "C2"))

//        vibes.drawBox(-10, 10, -10, 10, 'r')
//        vibes.drawBox(-5, 15, -5, 15, 'r')

}
