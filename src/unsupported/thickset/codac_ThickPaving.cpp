//============================================================================
//                               P Y I B E X
// File        : ThickPaving.h
// Author(s)   : Benoit Desrochers, Luc Jaulin
// Copyright   : Benoit Desrochers
// License     : LGPLv3, See the LICENSE file
// Created     : Sep  15, 2015
//============================================================================

#include <codac_ThickPaving.h>
#include <vibes.h>

#include <functional>   // std::logical_and
#include <numeric>      // std::accumulate
#include <list>
#include <iostream>
#include <fstream>
#include <stdexcept>

using std::list;
using std::cerr;
using std::endl;

//#include "vibes.h"

//IntervalVector(*F_sivia) (const IntervalVector&);  // Global variable used in SIVIA;
//ThickPaving X_sivia;

namespace ibex {

ThickPaving::ThickPaving(int dim) : root(IntervalVector(dim), UNK), bisector(LargestFirst(0, 0.5)) { } ;

ThickPaving::ThickPaving(const IntervalVector& p,ThickBoolean b):
 	root(p, b), bisector(LargestFirst(0, 0.5)) { }

ThickPaving::ThickPaving(const IntervalVector& p,ThickBoolean b, const LargestFirst& bisector):
 	root(p, b), bisector(bisector) { }

ThickPaving::ThickPaving(const ThickPaving& p):
  root(p.root), bisector(p.bisector) {}

ThickPaving::ThickPaving(IntervalVector& p,ThickTest& pdc,double eps, BINARY_OP op, bool display):
  root(p, MAYBE), bisector(LargestFirst(0, 0.5)){
    if (display){
      this->Sivia_visu(pdc, eps, op);
    }else {
      this->Sivia(pdc, eps, op);
    }
}

ThickPaving::ThickPaving(IntervalVector& p,FuncTest& test,double eps, BINARY_OP op, bool display):
  root(p, MAYBE), bisector(LargestFirst(0, 0.5)){
    if (display){
      this->Sivia_visu(test, eps, op);
    }else {
      this->Sivia(test, eps, op);
    }
}


ThickPaving::ThickPaving(const std::string& filename) : bisector(LargestFirst(0, 0.5)), root(IntervalVector(1), UNK){
  std::ifstream myfile;
  myfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try{
    myfile.open(filename, std::ios::binary | std::ios::in);
  } catch (const std::exception & e){
    std::cerr << "Error while opening file: " << e.what() << std::endl;
    // throw e;
  }
  std::cout << "file " << filename << " open successfull !\n";
  // if (myfile.fail()){
  //
  // }
  root = std::move(*Node::load(myfile));
  myfile.close();
}

void ThickPaving::save(const std::string& filename){
  std::ofstream myfile;
  myfile.open(filename, std::ios::binary | std::ios::out);
  root.save(myfile);
  myfile.close();
}

//
void ThickPaving::ctcOutside(IntervalVector& box){
  if ( !box.is_subset(root.getBox()) )
    return;
  IntervalVector res(IntervalVector::empty(box.size()));
  std::list<Node*> L;
  L.push_back(&root);
  while (!L.empty()){
    Node* n=L.front();    L.pop_front();
    IntervalVector tmp = (n->getBox() & box);
    if ( !tmp.is_empty() && !tmp.is_flat() ){
      if (n->isLeaf()){
        if (n->value() != OUT){
          res = res | n->getBox();
        }
      } else {
        L.push_back(n->left());
        L.push_back(n->right());
      }
    }
  }
  box &= res;
}
//
void ThickPaving::ctcInside(IntervalVector& box){
  if ( !box.is_subset(root.getBox()) )
    return;
  IntervalVector res(IntervalVector::empty(box.size()));
  std::list<Node*> L;
  L.push_back(&root);
  while (!L.empty()){
    Node* n=L.front();    L.pop_front();
    IntervalVector tmp = (n->getBox() & box);
    if ( !tmp.is_empty() && !tmp.is_flat() ){
      if (n->isLeaf()){
        if (n->value() != IN){
          res = res | n->getBox();
        }
      } else {
        L.push_back(n->left());
        L.push_back(n->right());
      }
    }
  }
  box &= res;
}


void ThickPaving::ctcTransform(ThickPaving& B, IntervalVector& T){
  std::list<Node*> L;
  L.push_back(&root);
  while (!L.empty()){
    Node* n=L.front();    L.pop_front();
    if (n->isLeaf()){
      /*if (n->value() == IN){
        IntervalVector Y = T + n->getBox();
        B.ctcOutside(Y);
        T &= Y - n->getBox();
      } else */if (n->value() == IN){
          IntervalVector Y = T + n->getBox();
          B.ctcOutside(Y);
          T &= Y - n->getBox();
      }
    } else {
      L.push_back(n->left());
      L.push_back(n->right());
    }
  }
}


ThickPaving& ThickPaving::change(UNARY_OP& op){
  std::list<Node*> L;
  L.push_back(&root);
  while (!L.empty()){
    Node* n=L.front();    L.pop_front();
    if (n->isLeaf()){
      n->setValue(op(n->value() ));

    } else {
      L.push_back(n->left());
      L.push_back(n->right());
    }
  }
  this->Reunite();
  return *this;
}
//    ThickPaving::~ThickPaving ();

//----------------------------------------------------------------------
// ostream& operator<<(ostream& os, const ThickPaving& X)
// {
//     cout << "ThickPaving :"<<"\t dim="<< X.B.front().size() <<"\n";
//     /*for (int i=0;i<X.B.size();i++)
//        {
//     os <<i<< "\t : left= "<<X.left[i]<<",\t right= "<<X.right[i]<<",\t val= "<<X.val[i]<<", \t X=["<< X.B[i][1]<<","<< X.B[i][2]<<"] \n";
//        }*/
//     return (os);
// }
//----------------------------------------------------------------------
// ThickPaving& ThickPaving::Clear(ThickBoolean b)        // After Clear(), the ThickPaving contains a single IntervalVector.
// {>getBox()
//     IntervalVector P(B.front());
//     B.clear(); left.clear(); right.clear(); val.clear();// is_enable.clear();
//     B.push_back(P);
//     left.push_back(-1);
//     right.push_back(-1);
//     val.push_back(b);
//     //is_enable.push_back(true);
//     return (*this);
// }

void ThickPaving::visit(ThickVisitor &visitor)
{
    visitor.pre_visit(*this);
    root.visit(visitor);
		visitor.post_visit(*this);
}

//----------------------------------------------------------------------
ThickPaving& ThickPaving::Reunite()
{
		root.reunite();
    return (*this);
}


//----------------------------------------------------------------------
// int  Inside(ThickPaving& Z, const IntervalVector& X,int k=0)   // returns 0, if outside, 1 if inside, EMPTY if X is EMPTY, and MAYBE otherwize
// {
//     if (X.is_empty())
//         return ibex::EMPTY; // A cause de l'Union ci-dessous
// //    if (!X.is_subset(Z.B[k]))
// //        return MAYBE; // n'arrive jamais, sauf pour k=0
//     if (Z.val[k]!=MAYBE) return Z.val[k];
//     int kleft=Z.left[k];
//     int kright=Z.right[k];
//     if ((kleft!=-1)||(kright!=-1))
//     { IntervalVector Xleft =X & Z.B[kleft];
//         IntervalVector Xright=X & Z.B[kright];
//         int I1= Inside(Z,Xleft,kleft);
//         int I2= Inside(Z,Xright,kright);

//         return(ibex::Union(I1 ,I2));
//     }
//     return (MAYBE);
// }

//Afin de traiter un problème du type f-1(A) subset A, A in B
//faire un algo récursif du type  (A Verfier l'algo)
//Pour le Tester, inverser tout d'abord un pavage.
// std::pair<bool, ThickBoolean>  Inside(ThickPaving& Z, const IntervalVector& X,int k=0)   // returns 0, if outside, 1 if inside, empty if X is empty, and iperhaps otherwize
// {
  //   if (X.is_empty()){
  //       return std::make_pair(false, EMPTY); // A cause de l'Union ci-dessous
  //   }
  //   if (X.is_disjoint(Z.B[k]))
  //       return std::make_pair(false, MAYBE); // n'arrive jamais, sauf pour k=0
  //   // else if (!X.is_subset(Z.B[k]))
  //       // return std::make_pair(true, MAYBE); // n'arrive jamais, sauf pour k=0
  //   // if( X == Z.B[k])
  //       // return Z.val[k];
  //   if( Z.is_leaf(k) ){
  //       return std::make_pair(true, Z.val[k]);
  //   } else {
  //       int kleft=Z.left[k];
  //       int kright=Z.right[k];
  //       IntervalVector Xleft  = X & Z.B[kleft];
  //       if (Xleft.is_flat()) Xleft.set_empty();
  //       IntervalVector Xright = X & Z.B[kright];
  //       if (Xright.is_flat()) Xright.set_empty();
  //       std::pair<bool, ThickBoolean> I1= Inside(Z,Xleft,kleft);
  //       std::pair<bool, ThickBoolean> I2= Inside(Z,Xright,kright);
  //       return std::make_pair(I1.first | I2.first, opUnion2(I1.second,I2.second));
  //  }
// }

// Function return true if the thick box [[am], [ap]] is not included in [[bm], [bp]]
bool isThickIntersect(const IntervalVector& am, const IntervalVector& ap, const IntervalVector& bm, const IntervalVector& bp){
  IntervalVector tmp1 = bm - ap;
  IntervalVector tmp2 = am - bp;
  for (int i = 0 ; i < tmp1.size(); i++){
    if (tmp1[i].ub() > 0 || tmp2[i].ub() > 0)
      return false;
  }
  return true;
}


// Function return true if the thick box [[am], [ap]] is not included in box [b]
bool isThickIntersect(const IntervalVector& am, const IntervalVector& ap, const IntervalVector& b){
  return isThickIntersect(am, ap, IntervalVector(b.lb()), IntervalVector(b.ub()));
}


bool isThickInclude(const IntervalVector& am, const IntervalVector& ap, const IntervalVector& bm, const IntervalVector& bp){
  IntervalVector tmp1 = bm - am;
  IntervalVector tmp2 = ap - bp;
  for (int i = 0 ; i < tmp1.size(); i++){
    if (tmp1[i].ub() > 0 || tmp2[i].ub() > 0)
      return false;
  }
  return true;
}

// Function return true if the thick box [[am], [ap]] is not disjoint in [[bm], [bp]]
bool isThickNotInclude(const IntervalVector& am, const IntervalVector& ap, const IntervalVector& bm, const IntervalVector& bp){
  IntervalVector tmp1 = am - bm;
  IntervalVector tmp2 = bp - ap;
  // exist i such as tmp1[i] < 0 or tmp2[i] < 0
  for (int i = 0 ; i < tmp1.size(); i++){
    // std::cerr <<  "[" << i << "] " << tmp1[i].ub() << " || " << tmp2[i].ub() << "\n";
    if (tmp1[i].ub() < 0 || tmp2[i].ub() < 0)
      return true;
  }
  return false;
}

// Function return true if the thick box [[am], [ap]] is not disjoint in box [b]
bool isThickNotInclude(const IntervalVector& am, const IntervalVector& ap, const IntervalVector& b){
  return isThickNotInclude(am, ap, IntervalVector(b.lb()), IntervalVector(b.ub()));
}

ThickBoolean ThickPaving::contains(const IntervalVector& X){
  ThickBoolean res =  EMPTY;
  std::list<Node*> L;
  L.push_back(&root);
  while (!L.empty()){
    Node* n=L.front();    L.pop_front();
    IntervalVector tmp = (n->getBox() & X);
    if ( !tmp.is_empty() && !tmp.is_flat() ){
      if (n->isLeaf()){
        // std::cerr << "\t" << n->value() << " " << n->getBox() << " " << n->getBox().diam() << "\n";
        // if ( n->value() == UNK )
        //   res = res | MAYBE;
        // else
          res = res | n->value();
 			// 	if(res == UNK) break;
      } else {
        L.push_back(n->left());
        L.push_back(n->right());
      }
    }
  }
  if (!X.is_subset(root.getBox()) )
    return MAYBE | res;
  return res;
}

ThickBoolean ThickPaving::Inside2(const IntervalVector& X, std::vector<Node*>& lst){
  ThickBoolean res =  EMPTY;

  // if ( (root.getBox() & X).is_empty() ) return MAYBE;
  // if ( !X.is_subset(root.getBox() ) ) return UNK;

  std::list<Node*> L;
  L.push_back(&root);
  while (!L.empty()){
    Node* n=L.front();    L.pop_front();
    IntervalVector tmp = (n->getBox() & X);
    if ( !tmp.is_empty() && !tmp.is_flat() ){
      if (n->isLeaf()){
        res = res | n->value();
        lst.push_back(n);
      } else {
        L.push_back(n->left());
        L.push_back(n->right());
      }
    }
  }
  if(res == EMPTY){
    std::cerr << __LINE__ << "   res is empty, need to find a value to return\n";
    return OUT;
  }
  return res;

}


ThickBoolean ThickPaving::fastIntersection2(const IntervalVector& Xm, const IntervalVector& Xp, std::vector<Node*> &lst){
  bool intersect_in  = false;
  bool intersect_out = false;
  IntervalVector X = Xm | Xp;
  for( auto&& n : lst){
    IntervalVector tmp = (n->getBox() & X);
    if ( ( n->value() == IN || n->value() == MAYBE_IN || n->value() == MAYBE ) && intersect_in == false){
        intersect_in = isThickIntersect(Xm, Xp, tmp);
    } else if ( (n->value() == OUT || n->value() == MAYBE || n->value() == MAYBE_OUT) && intersect_out == false){
        intersect_out = isThickIntersect(Xm, Xp, tmp);
    }
    if(intersect_out && intersect_in){
        return MAYBE;
    }
  }
  if (intersect_in == true && intersect_out == false)
    return MAYBE_IN;
  else if (intersect_in == false && intersect_out == true)
    return MAYBE_OUT;
  return UNK;
}


bool projection_is_include(IntervalVector X, std::vector<IntervalVector> &lst, int i){
    std::sort(lst.begin(), lst.end(),
              [&i](const IntervalVector& a, const IntervalVector& b ) {
                return (a[i].lb() < b[i].lb());
            }
    );
    if (lst.empty()) return false;
    Interval c1, c2;
    int j = 0;
    for (auto &b : lst){
        if ( X[i].is_empty() ){
            return true;
        }
        int res = X[i].diff(b[i], c1, c2);
        X[i] = (X[i] & c1) | (X[i] & c2);
    }
    if ( X[i].is_empty() ){
        return true;
    }
    return false;
}

ThickBoolean ThickPaving::Xm_inter_Xp_inside_P(IntervalVector X, std::vector<Node*> &lst){
  ThickBoolean res = EMPTY;
  // if (lst.empty()) return UNK;
  for (const auto& n : lst){
    res = res | n->value();
  }
  assert(res != EMPTY);
  if (res == IN )
    return MAYBE_IN;
  if (res == OUT)
    return MAYBE_OUT;
  return res;
}

ThickBoolean isInBox(const IntervalVector& Xm, const IntervalVector& Xp, const IntervalVector& y){
    ThickBox X(Xm, Xp);
    if (X.superset().is_subset(y)){
      return IN;
    } else if (X.superset().is_disjoint(y)){
      return OUT;
    } else {
      bool b1 = X.intersects(y);
      bool b2 = X.is_not_subset(y);
      if ( b1 && b2 ) return MAYBE;
      if (!b1 && b2) return MAYBE_OUT;
      if (!b2 && b1) return MAYBE_IN;
    }
    return UNK;
}


ThickBoolean opMask(const ThickBoolean& a, const ThickBoolean& m){
  // std::cout << a << " " << m << "\n";
  // if(m == IN || is_singleton(a)) return a;

  // if ( is_singleton(a) ) return a;
  if (m == MAYBE) return MAYBE;
  if (a == IN &&  (m == MAYBE || m == MAYBE_OUT ) ) return MAYBE;
  if (a == IN &&  m == MAYBE_IN ) return MAYBE_IN;
  // if (m == MAYBE) return MAYBE;
  if (a == OUT && (m == MAYBE || m == MAYBE_OUT ) ) return MAYBE;
  if (a == OUT &&  m == MAYBE_IN ) return MAYBE_OUT;
  // if (m == MAYBE) return m; // && a == MAYBE_IN) return MAYBE;
  // else if (m == MAYBE && a == MAYBE_OUT) return MAYBE;
  // else if (m == MAYBE && a == UNK) return MAYBE;
  // else if (m == MAYBE_IN ) return a;
  // else if (m == MAYBE_OUT ) return MAYBE;
  // if ( a == MAYBE_IN && m == MAYBE) return MAYBE;
  // if ( a == MAYBE_OUT && m == MAYBE) return MAYBE;
  return UNK;
}


// std::tuple<vector<bool>, bool> get_innerThickInterval(const IntervalVector& Xm, const IntervalVector& Xp)
ThickBoolean  ThickPaving::Intersect(const IntervalVector& Xm, const IntervalVector& Xp)   // returns 0, if outside, 1 if inside, empty if X is empty, and iperhaps otherwize
{

  ThickBoolean inBox = isInBox(Xm, Xp, root.getBox());
  if (inBox == OUT) {
    return MAYBE;
  } else if (inBox == IN){
    return Intersect_fct(Xm, Xp);
  }

  //   // Compute new bounds
  // ThickBox tmp  = ThickBox(Xm, Xp).inter(ThickBox(root.getBox()));
  // std::cout << Xm << " " << Xp << "\n";
  // std::cout << tmp.lb() << " " << tmp.ub() << "\n";
  // std::cout << root.getBox() << " " << inBox << "\n***********************************************\n";
  // return Intersect_fct(tmp.lb(), tmp.ub());
  // return inBox;
  return opMask(Intersect_fct(Xm, Xp), inBox );


}




ThickBoolean  ThickPaving::Intersect_fct(const IntervalVector& Xm, const IntervalVector& Xp)   // returns 0, if outside, 1 if inside, empty if X is empty, and iperhaps otherwize
{
	size_t size = root.getBox().size();
  IntervalVector X = Xm | Xp;


  // firstly check if X is inside the paving
  // Keep track of boxes which intersect X
  std::vector<Node*> lst;
  ThickBoolean res0 = Inside2(X, lst);
  // if ( !X.is_subset(root.getBox()) ){
  //   // std::cout << "Here" << res0 << "\n";
  //   return (is_singleton(res0)) ? res0 : MAYBE | res0;
  // }
  if (is_singleton(res0))
    return res0;

  // std::cout << "Here Again !!!" << res0 << "\n";

  // Need to use Xm and Xp
  // If Xm and Xp intersect, we can conclude if
  // Xm & Xp is included in P
  if ( (Xm).intersects(Xp)){ // Xm and Xp intersect
    return Xm_inter_Xp_inside_P(Xm & Xp, lst);
  } else {
    // Check if Xm and Xp are completely disjoint
    IntervalVector X(size);
    std::vector<bool> dim_is_thin(size, true);
    for(size_t i = 0; i < size; i++){
      dim_is_thin[i] = ((Xm[i] & Xp[i]).is_empty());
      X[i] = (dim_is_thin[i] == false) ? (Xm[i] & Xp[i]) : Interval( Xm[i].ub(), Xp[i].lb() );
    } // end for
    bool res = std::accumulate(dim_is_thin.begin(), dim_is_thin.end(), true, std::logical_and<bool>() );

    if (res == true){ // Xm and Xp are disjoint
      // std::cerr << "Xm and Xp are disjoint\n";
      return fastIntersection2(Xm, Xp, lst);

    } else {


      // split lst into two lists :
      // l_in containing boxes which are inside P superset
      // l_out containing bixes which are outside P subset
      std::vector<IntervalVector> l_in;
      std::vector<IntervalVector> l_out;
      for (const auto& n : lst){
        IntervalVector tmp = X & n->getBox();
        if (tmp.is_empty()) continue;
        if ( n->value() == IN || n->value() == MAYBE_IN || n->value() == MAYBE){
          l_in.push_back(X & n->getBox());
        }
        if (n->value() == OUT || n->value() == MAYBE_OUT || n->value() == MAYBE){
          l_out.push_back(X & n->getBox());
        }
      }

      bool is_not_included = true;
      bool is_intersect = true;
      // std::cerr << X  << " " << Xm << " " << Xp << endl;
      for (size_t i = 0; i < dim_is_thin.size(); i++){
        if (dim_is_thin[i] == true) continue;
        is_not_included &= projection_is_include(X, l_in, i);
        is_intersect &= projection_is_include(X, l_out, i);
        // std::cerr << dim_is_thin[i] << " " << is_not_included << " " << is_intersect << " "
                  // << l_in.size() << " " << l_out.size() << "\n";


        // dim_is_thin[i] = b1 & b2;
      }
      if (is_not_included && is_intersect ) return MAYBE;
      else if (is_not_included && !is_intersect ) return MAYBE_IN;
      else if (!is_not_included && is_intersect ) return MAYBE_OUT;
    }
  }
  return UNK;
}


//----------------------------------------------------------------------
ThickPaving& ThickPaving::Sivia_visu(ThickTest &pdc, double eps, BINARY_OP op ){
	FuncTest f = [&pdc](const IntervalVector& X)->ThickBoolean {
    return pdc.test(X);
	};
	return Sivia_visu(f, eps, op);
}

ThickPaving& ThickPaving::Sivia_visu(FuncTest &test, double eps, BINARY_OP op )
{
    list<Node*> L;
    L.push_back(&root);
    int k = 0, j = 0;
    vibes::beginDrawing();
    vibes::newFigure("ThickPaving");
    vibes::setFigureProperties(vibesParams("x",0,"y",220,"width",400,"height",400));
    vibes::drawBox(root.getBox()[0].lb(), root.getBox()[0].ub(), root.getBox()[1].lb(), root.getBox()[1].ub(), "k");
    vibes::axisAuto();
    while (!L.empty()){
        k++;
        Node* n=L.front();    L.pop_front();
      //  vibes::drawBox(B[i][0].lb(), B[i][0].ub(), B[i][1].lb(), B[i][1].ub(), "k[g]");

        ThickBoolean testBi=test(n->getBox());
        ThickBoolean vali = op(n->value(),testBi);

        bool b1 = !is_singleton(vali);
        if (vali == EMPTY){
          if (n->getBox().max_diam()>eps){
            vali = UNK;
          } else {
            // std::cout << n->isLeaf() << " " << n->value() << " " << testBi << " " << vali << "\n";
          }
        }

				// On bisect quand :
				// 	- max_diazm > eps
				//  - vali n'est pas un singleton
        if( !is_singleton(vali) && (n->getBox().max_diam()>eps)){
            j++;
						if(n->isLeaf()){
							// cerr << "bisect " << n->value() << endl;
							n->bisect(bisector);
						}
            L.push_back(n->left());
            L.push_back(n->right());
        } else {
            const IntervalVector& B = n->getBox();
            if (vali == IN)
                vibes::drawBox(B[0].lb(), B[0].ub(), B[1].lb(), B[1].ub(), "[r]");
            else if (vali == OUT)
                vibes::drawBox(B[0].lb(), B[0].ub(), B[1].lb(), B[1].ub(), "[b]");
            else if (vali == MAYBE)
                vibes::drawBox(B[0].lb(), B[0].ub(), B[1].lb(), B[1].ub(), "[orange]");
            else
              vibes::drawBox(B[0].lb(), B[0].ub(), B[1].lb(), B[1].ub(), "[y]");

            // vali = (vali == UNK) ? MAYBE : vali;
						n->remove_children();
        }

        n->setValue(vali);
    }
    std::cerr << "k = " << k  << "|| j = " << j << std::endl;
    // Clean();
   // Reunite();
    vibes::endDrawing();
    return (*this);
}


// std::pair<bool, std::vector<IntervalVector> >  ThickPaving::Intersect(const IntervalVector& Xm, const IntervalVector& Xp)   // returns 0, if outside, 1 if inside, empty if X is empty, and iperhaps otherwize
// {

//   // IntervalVector X = Xm | Xp;
//   IntervalVector X(B[0].size());
//   std::vector<bool> dim_is_thin(B[0].size(), true); //
//   if ( (Xm).intersects(Xp)){ // Xm and Xp intersect
//     std::pair<bool, ThickBoolean> res =  contains(Xm & Xp);

//     std::cerr << "Xm and Xp intersect " << res.second << "\n";
//     if ( (res.second == MAYBE /*|| res.second == MAYBE_OUT*/ ) &&
//          (res.second != IN || res.second != MAYBE_IN) )
//        return std::make_pair(true, std::vector<IntervalVector>());
//     return std::make_pair(false, std::vector<IntervalVector>());

//   } else {
//     for(size_t i = 0; i < Xm.size(); i++){
//       if ( !(Xm[i] & Xp[i]).is_empty() ){
//         X[i] = Xm[i] & Xp[i];
//         dim_is_thin[i] = false;
//       } else {
//         X[i] = Interval( Xm[i].ub(), Xp[i].lb() );
//       }
//     } // end for
//     bool res = true;
//     for(auto b : dim_is_thin){
//       res &= b;
//     }
//     if (res == true){ // Xm and Xp are disjoint
//         std::cerr << "Xm and Xp are disjoint\n";
//         return fastIntersection(Xm, Xp);
//     }
//   }

//   std::vector<IntervalVector> l_in;
//   std::vector<IntervalVector> l_out;


//   list<int> L;
//   L.push_back(0);
//     // vibes::beginDrawing("/home/biop/.vibes.json");
//   vibes::selectFigure("Test");
//   while (!L.empty()){
//     int i=L.front();    L.pop_front();
//     IntervalVector tmp = (n->getBox() & X);
//     if ( !tmp.is_empty() && !tmp.is_flat() ){
//       if (is_leaf(i)){
//         if ( val[i] == IN || val[i] == MAYBE_IN || val[i] == MAYBE){
//           l_in.push_back(tmp);
//         } else if (val[i] == OUT || val[i] == MAYBE_OUT || val[i] == MAYBE){
//           l_out.push_back(tmp);
//         }
//       } else {
//         L.push_back(left[i]);
//         L.push_back(right[i]);
//       }
//     }
//   }

//   std::cerr << "************************************************************\n";
//   std::cerr << l_in.size()  << " " << l_in.size() << " " << X  << "\n" ;

//   for (size_t i = 0; i < dim_is_thin.size(); i++){
//     if (dim_is_thin[i] == true) continue;
//     bool b1 = projection_is_include(X, l_in, i);
//     bool b2 = projection_is_include(X, l_out, i);
//     std::cerr << dim_is_thin[i] << " " << b1 << " " << b2 << "\n";

//     dim_is_thin[i] = b1 & b2;
//   }
//   // std::cerr << endl;

//   bool res = true;
//   for(auto b : dim_is_thin){
//     res &= b;
//   }

//   vibes::selectFigure("CurrentThickPaving");
//   if (res == true)
//     return std::make_pair(true, l_out);
//   return std::make_pair(false, l_out);
// }


// std::pair<bool, ThickBoolean>  ThickPaving::Intersect(const IntervalVector& Xm, const IntervalVector& Xp)   // returns 0, if outside, 1 if inside, empty if X is empty, and iperhaps otherwize
// {
//     list<int> L;
//     L.push_back(0);
//     // vibes::beginDrawing("/home/biop/.vibes.json");
//     vibes::selectFigure("Test");
//     IntervalVector in(B[0].size(), Interval::EMPTY_SET);
//     IntervalVector maybe(B[0].size(), Interval::EMPTY_SET);
//     IntervalVector out(B[0].size(), Interval::EMPTY_SET);
//     bool disjoint = true;
//     bool notInclude = true;
//     IntervalVector X = Xm | Xp;
//     std:cerr << "************************************************************\n";
//     while (!L.empty()){
//       int i=L.front();    L.pop_front();
//       IntervalVector tmp = (B[i] & X);
//       if ( !tmp.is_empty() && !tmp.is_flat() ){
//         if (is_leaf(i)){
//           if ( val[i] == IN || val[i] == MAYBE_IN || val[i] == MAYBE){
//             std::cerr << tmp << "\n";
//             std::pair<IntervalVector, IntervalVector> box = getIntersect(Xm, Xp, tmp);
//             if ( ( box.first.is_flat() && !box.first.is_subset(box.second) )
//                 ||  ( box.second.is_flat() && !box.second.is_subset(box.first) ) ){
//               IntervalVector tmp = IntervalVector(box.first.ub()) | IntervalVector(box.second.lb());
//               // vibes::drawBox(tmp[0].lb(), tmp[0].ub(), tmp[1].lb(), tmp[1].ub(), vibesParams("format","[#00FFFF55]","group","cbox2"));
//               in |= tmp;
//               // vibes::drawBox(in[0].lb(), in[0].ub(), in[1].lb(), in[1].ub(), vibesParams("format","k[#F00FFF55]","group","cbox2"));
//             }
//           }
//         } else {
//           L.push_back(left[i]);
//           L.push_back(right[i]);
//         }
//       }
//     }
//     IntervalVector tmp = in;
//     if (in.is_empty()){
//       vibes::selectFigure("CurrentThickPaving");
//       return std::make_pair(false, OUT);
//     }
//     // vibes::drawBox(tmp[0].lb(), tmp[0].ub(), tmp[1].lb(), tmp[1].ub(),  "[#FFFF0044]" );
//     // vibes::drawBox(out[0].lb(), out[0].ub(), out[1].lb(), out[1].ub(),  "[#FF00FF44]" );
//     // std::cerr << tmp << std::endl;
//     // IntervalVector *res = NULL;
//     // int n = tmp.diff(out, res);
//     // disjoint = true;
//     // if (n > 0){
//     //   for(size_t i = 0; i < n; i++){
//     //     IntervalVector &tmp = res[i];
//     //     std::cerr << "[" << i << "] " << tmp << std::endl;
//     //     // vibes::drawBox(tmp[0].lb(), tmp[0].ub(), tmp[1].lb(), tmp[1].ub(),  "[#FF00FF44]" );
//     //     disjoint &= isThickIntersect(Xm, Xp, res[i]);
//     //   }
//     // } else {
//     //   disjoint = false;
//     // }
//     // delete[] res;
//     // for(size_t i = 0; i < in.size(); i++){
//     //   Interval c1, c2;
//     //   int n = tmp[i].diff(out[i],c1, c2);
//     //   if (n != 2){
//     //     in[i] = c1;
//     //   } else {
//     //     std::cerr << i << " " << n << " n == 2 \n";
//     //   }
//     // }
//     std::cerr << tmp << std::endl;
//     vibes::drawBox(tmp[0].lb(), tmp[0].ub(), tmp[1].lb(), tmp[1].ub(), vibesParams("format","[k]","group","cbox2"));
//     disjoint = isThickIntersect(Xm, Xp,tmp);
//     // notInclude = isThickNotInclude(Xm, Xp, in);
//     // vibes::endDrawing();
//     vibes::selectFigure("CurrentThickPaving");
//     if(disjoint && notInclude)
//       return std::make_pair(true, IN);
//     return std::make_pair(false, OUT);
// }


//----------------------------------------------------------------------
void ThickPaving::Contract_distance_gt_ThickPaving(double z, IntervalVector& X){
//Contract X with respect to distance to a subThickPaving lower than z.
  this->Contract_distance_gt_ThickPaving(root, z, X);
}


void ThickPaving::Contract_distance_gt_ThickPaving(Node& n, double z, IntervalVector& X) //Contract X with respect to distance to a subThickPaving lower than z.
{

   if (X.is_empty())  return;
   if (n.value() == IN)
       {X.set_empty();
         return;
   }                  // NE MARCHE PAS ! A REFAIRE !

   if ( n.isLeaf() || n.value() == OUT){
      Interval X1=X[0];
      Interval X2=X[1];
      Interval A1=(n.getBox())[0];
      Interval A2=(n.getBox())[1];
      Interval Z(0,z*z);    //X1^2+X2^2=Z
      Interval D1=X1-A1;
      Interval D2=X2-A2;
      Interval D12=sqr(D1);
      Interval D22=sqr(D2);
      Z &=  D12 + D22;
      ibex::bwd_add(Z, D12, D22);
      ibex::bwd_sqr(D22,D2);
      ibex::bwd_sqr(D12,D1);
      ibex::bwd_sub(D2,X2,A2);

      ibex::bwd_sub(D1,X1,A1);
      // X=IntervalVector(2);
      X[0] = X1; X[1] = X2;
      return;
   }
     IntervalVector X1(X);
     IntervalVector X2(X);
     Contract_distance_gt_ThickPaving(*n.left(),z,X1);
     Contract_distance_gt_ThickPaving(*n.right(),z,X2);
     X = X1 | X2;
     return;
}

// void ThickPaving::Contract_distance_gt_ThickPaving(Node& n, double z, IntervalVector& X) //Contract X with respect to distance to a subThickPaving lower than z.
// {
//
//    if (X.is_empty())  return;
//    if (A.val[k]==OUT)
//        {X.set_empty();return;}                  // NE MARCHE PAS ! A REFAIRE !
//    int kleft=A.left[k];
//    int kright=A.right[k];
//    if (((A.val[k]==MAYBE)&&(kleft==-1)&&(kright==-1))||(A.val[k]==IN))
//    {  Interval X1=X[0];
//       Interval X2=X[1];
//       Interval A1=(A.B[k])[0];
//       Interval A2=(A.B[k])[1];
//       Interval Z(0,z*z);    //X1^2+X2^2=Z
//       Interval D1=X1-A1;
//       Interval D2=X2-A2;
//       Interval D12=sqr(D1);
//       Interval D22=sqr(D2);
//       Z &=  D12 + D22;
//       bwd_add(Z, D12, D22);
//       bwd_sqr(D22,D2);
//       bwd_sqr(D12,D1);
//       bwd_sub(D2,X2,A2);
//       bwd_sub(D1,X1,A1);
//       // X=IntervalVector(2);
//       X[0] = X1; X[1] = X2;
//       return;
//    }
//      IntervalVector X1(X);
//      IntervalVector X2(X);
//      Contract_distance_gt_ThickPaving(A,z,X1,kleft);
//      Contract_distance_gt_ThickPaving(A,z,X2,kright);
//      X = X1 | X2;
//      return;
// }
//----------------------------------------------------------------------
// Operation between ThickPavings
void  op_binaire(ThickPaving& Z, ThickPaving& X,ThickPaving& Y, BINARY_OP op,int k,int i,int j)
{

    // if (k==0) {Z.B[0]=X.B[0]; Z.Clear(MAYBE);};
    // int vali = op(X.val[i],Y.val[j]);
    // if (X.val[i] == IN && Y.val[j] == OUT){
    //     std::cerr << "Error " << vali << " " << X.is_leaf(i) << " " << Y.is_leaf(j) << std::endl;
    // }
    // Z.val[k]=MAYBE;
		//
    // int ileft=X.left[i];
    // int jleft=Y.left[j];
    // int iright=X.right[i];
    // int jright=Y.right[j];
		//
		//
    // if( X.is_leaf(i) && Y.is_leaf(j)){
    //     Z.val[k] = op(X.val[i],Y.val[j]);
    // } else {
    //     Z.Expand(k);
    //     if (X.is_leaf(i)){
    //         X.Expand(i);
    //         // X.val[X.left[i]] = X.val[i];
    //         // X.val[X.right[i]] = X.val[i];
    //     }
    //     if( Y.is_leaf(j)){
    //         Y.Expand(j);
    //         // Y.val[Y.left[j]] = X.val[j];
    //         // Y.val[Y.right[j]] = X.val[j];
    //     }
    //     op_binaire(Z,X,Y,op,Z.left[k],X.left[i],Y.left[j]);
    //     op_binaire(Z,X,Y,op,Z.right[k],X.right[i],Y.right[j]);
		//
    // }
    // } else if (vali == MAYBE){
        // if ( X.is_leaf(i) && !Y.is_leaf(j) ){
        //     Z.Expand(k);
        //     op_binaire(Z,X,Y,op,Z.left[k],i,Y.left[j]);
        //     op_binaire(Z,X,Y,op,Z.right[k],i,Y.right[j]);
        // } else if ( !X.is_leaf(i) && Y.is_leaf(j) ){
        //     Z.Expand(k);
        //     op_binaire(Z,X,Y,op,Z.left[k],X.left[i],j);
        //     op_binaire(Z,X,Y,op,Z.right[k],X.right[i],j);
        // } else if ( !X.is_leaf(i) && !Y.is_leaf(j) ){
        //     Z.Expand(k);
        //     op_binaire(Z,X,Y,op,Z.left[k],ileft,jleft);
        //     op_binaire(Z,X,Y,op,Z.right[k],iright,jright);
        // }
    // }
    // if (((ileft!=-1)||(jleft!=-1))&&(vali==MAYBE))
    // { if (ileft==-1) {ileft=i; iright=i;}
    //     if (jleft==-1) {jleft=j; jright=j;}
    //     Z.Expand(k);
    //     op_binaire(Z,X,Y,ibex::Inter,Z.left[k],ileft,jleft);
    //     op_binaire(Z,X,Y,ibex::Inter,Z.right[k],iright,jright);
    // }
    // if (k==0) {Z.Clean();Z.Reunite();};
    return;
}

// Operation between ThickPavings
// binary op Z = X op Y
void  op_binaire(ThickPaving& Z, ThickPaving& X,ThickPaving& Y, BINARY_OP op){
		// if (X.root.getBox() != Y.root.getBox()) {
		// 	cerr << "X and Y must have the same root boxes !!!\n";
		// 	return;
		// }
    //
		// Z.clear();
    //
    //
	  // list<std::array<Node*, 3> > L = {&Z.root, &X.root, &Y.root};
    // // LZ.push_back(&Z.root);
		// // LX.push_back(&X.root);
		// // LY.push_back(&Y.root);
    //
    //
    //
    // while (!L.empty()){
    //     k++;
    //
    //     std::array<Node*, 3> nodes=L.front();    L.pop_front();
		// 		Node* nZ = nodes[0];
		// 		Node* nX = nodes[1];
		// 		Node* nY = nodes[2];
    //
		// 		nX->getBox() & nY->getBox()
    //
		// 		// cas 0 Box NX == Box Ny
    //
		// 		// Box XN in box NY
    //
		// 		// Box nY in Nx
    //
    //
    //
    //
    //     ThickBoolean testBi=test(nX->getBox());
    //     ThickBoolean vali = op(n->value(),testBi);
    //     bool b1 = !is_singleton(vali);
    //     bool b2 = !(is_singleton(testBi)
		// }
    //
    // if (k==0) {Z.B[0]=X.B[0]; Z.Clear(MAYBE);};
    // int vali = op(X.val[i],Y.val[j]);
    // if (X.val[i] == IN && Y.val[j] == OUT){
    //     std::cerr << "Error " << vali << " " << X.is_leaf(i) << " " << Y.is_leaf(j) << std::endl;
    // }
    // Z.val[k]=MAYBE;
    //
    // int ileft=X.left[i];
    // int jleft=Y.left[j];
    // int iright=X.right[i];
    // int jright=Y.right[j];
    //
    //
    // if( X.is_leaf(i) && Y.is_leaf(j)){
    //     Z.val[k] = op(X.val[i],Y.val[j]);
    // } else {
    //     Z.Expand(k);
    //     if (X.is_leaf(i)){
    //         X.Expand(i);
    //         // X.val[X.left[i]] = X.val[i];
    //         // X.val[X.right[i]] = X.val[i];
    //     }
    //     if( Y.is_leaf(j)){
    //         Y.Expand(j);
    //         // Y.val[Y.left[j]] = X.val[j];
    //         // Y.val[Y.right[j]] = X.val[j];
    //     }
    //     op_binaire(Z,X,Y,op,Z.left[k],X.left[i],Y.left[j]);
    //     op_binaire(Z,X,Y,op,Z.right[k],X.right[i],Y.right[j]);
    //
    // }
    // } else if (vali == MAYBE){
    //     if ( X.is_leaf(i) && !Y.is_leaf(j) ){
    //         Z.Expand(k);
    //         op_binaire(Z,X,Y,op,Z.left[k],i,Y.left[j]);
    //         op_binaire(Z,X,Y,op,Z.right[k],i,Y.right[j]);
    //     } else if ( !X.is_leaf(i) && Y.is_leaf(j) ){
    //         Z.Expand(k);
    //         op_binaire(Z,X,Y,op,Z.left[k],X.left[i],j);
    //         op_binaire(Z,X,Y,op,Z.right[k],X.right[i],j);
    //     } else if ( !X.is_leaf(i) && !Y.is_leaf(j) ){
    //         Z.Expand(k);
    //         op_binaire(Z,X,Y,op,Z.left[k],ileft,jleft);
    //         op_binaire(Z,X,Y,op,Z.right[k],iright,jright);
    //     }
    // }
    // if (((ileft!=-1)||(jleft!=-1))&&(vali==MAYBE))
    // { if (ileft==-1) {ileft=i; iright=i;}
    //     if (jleft==-1) {jleft=j; jright=j;}
    //     Z.Expand(k);
    //     op_binaire(Z,X,Y,ibex::Inter,Z.left[k],ileft,jleft);
    //     op_binaire(Z,X,Y,ibex::Inter,Z.right[k],iright,jright);
    // }

}


//----------------------------------------------------------------------
// Unary operation between ThickPavings
void  op_unaire(ThickPaving& Z, ThickPaving& X, UNARY_OP op,int k,int i)
{
  //   if (k==0) {Z.B[0]=X.B[0]; Z.Clear(MAYBE);};
  //   ThickBoolean vali = op(X.val[i]);
  //   Z.val[k]=vali;
  //   int ileft=X.left[i];
  //   int iright=X.right[i];
  //   if ((ileft!=-1)&&(vali==MAYBE))
  //   { Z.Expand(k);
  //       op_unaire(Z,X,op,Z.left[k],ileft);
  //       op_unaire(Z,X,op,Z.right[k],iright);
  //   }
  //  if (k==0) {Z.Clean();Z.Reunite();};
  //   return;
}



//----------------------------------------------------------------------
ThickPaving& ThickPaving::USivia(ThickTest &pdc, double eps)
{
    // int k = 0;
    // int j = 0;
    // list<int> L;
    // L.push_back(0);
    // while (!L.empty())
    // { int i=L.front();    L.pop_front();
    //     k++;
    //     // cerr << "Run test: ...";
    //     ThickBoolean testBi=pdc.test(B[i]);
    //     // cerr << "Done\n";
    //     ThickBoolean vali = opOr(val[i],testBi);
    //     // is_enable[i] = is_enable[i] | pdc.is_enable;
    //     if (vali!=MAYBE) // || ( is_enable[i] == false))
    //     {
    //       Remove_sons(i);
    //     }
    //     else
    //         if  ((B[i].max_diam()>eps)){// && (testBi==MAYBE)){    //Luc
    //             Expand(i);
    //             j++;
    //             L.push_back(left[i]);
    //             L.push_back(right[i]);
    //         }
    //     val[i]=vali;
    // };
    // std::cerr << "k = " << k  << "|| j = " << j << std::endl;
    return (*this);
}

// #include <unistd.h>

//----------------------------------------------------------------------
// ThickPaving ThickPaving::Sivia(ThickTest &pdc, BINARY_OP op, double eps)
// {
  //   int k = 0;
  //   int j = 0;
  //   list<int> L;
  //   L.push_back(0);
  //   vibes::beginDrawing();
  //   vibes::newFigure("CurrentThickPaving");
  //   vibes::setFigureProperties(vibesParams("x",0,"y",220,"width",400,"height",400));
  //   while (!L.empty())
  //   { int i=L.front();    L.pop_front();
  //       k++;
  //       ThickBoolean testBi=pdc.test(B[i]);
  //       // std::cerr << testBi << "/" << val[i] << " " << is_enable[i] << " "
  //                 // << pdc.is_enable << "\n";
	 //
  //       ThickBoolean vali = opInter(val[i],testBi);
  //       if (vali == EMPTY){
  //           val[i] = EMPTY;
  //           L.clear();
  //           continue;
  //       }
  //       // is_enable[i] = is_enable[i] | pdc.is_enable;
  //       // std::cerr << testBi << "/" << vali << " " << is_enable[i] << "\n";
  //       if (vali!=MAYBE) // || ( is_enable[i] == false))
  //       {
  //         Remove_sons(i);
  //         if (vali == IN)
  //           vibes::drawBox(B[i][0].lb(), B[i][0].ub(), B[i][1].lb(), B[i][1].ub(),  "[r]");
  //         else if (vali == OUT)
  //           vibes::drawBox(B[i][0].lb(), B[i][0].ub(), B[i][1].lb(), B[i][1].ub(),  "[b]");
  //         else if (vali == UNK)
  //           vibes::drawBox(B[i][0].lb(), B[i][0].ub(), B[i][1].lb(), B[i][1].ub(),  "[orange]");
  //       }
  //       else
  //           if  ((B[i].max_diam()>eps) && (testBi==MAYBE) ){ //&& pdc.is_enable == true)){    //Luc
  //               Expand(i);
  //               // std::cerr << "before isntall \n";
  //               // std::cerr << B[i] << std::endl;
  //               j++;
  //               L.push_back(left[i]);
  //               L.push_back(right[i]);
  //           }
  //       val[i]=vali;
  //       if(val[i] == MAYBE){//} && is_enable[i] == false){
  //         vibes::drawBox(B[i][0].lb(), B[i][0].ub(), B[i][1].lb(), B[i][1].ub(),  "[orange]");
  //       } else if (val[i] == MAYBE && B[i].max_diam()<=eps)
  //         vibes::drawBox(B[i][0].lb(), B[i][0].ub(), B[i][1].lb(), B[i][1].ub(),  "[y]");
  //     // usleep(5000);
  //   };
  //   std::cerr << "k = " << k  << "|| j = " << j << std::endl;
  //   vibes::endDrawing();
  //   // Clean();
  //  // Reunite();
    // return (*this);
// }

//----------------------------------------------------------------------
ThickPaving& ThickPaving::Sivia(ThickTest &pdc, double eps, BINARY_OP op ){
	FuncTest f = [&pdc](const IntervalVector& X)->ThickBoolean {
    return pdc.test(X);
	};
	return Sivia(f, eps, op);
}

ThickPaving& ThickPaving::Sivia(FuncTest &test, double eps, BINARY_OP op )
{
    list<Node*> L= {&root};
    // L.push_back(&root);
    int k = 0, j = 0;
    while (!L.empty()){
        k++;
        Node* n=L.front();    L.pop_front();

        ThickBoolean testBi=test(n->getBox());
        ThickBoolean vali = op(n->value(),testBi);
        // Uggly trick
        // if (n->isLeaf()&& testBi == MAYBE &&  ( n->value() == MAYBE_IN || n->value() == MAYBE_OUT)){
        //   vali = MAYBE; // cerr << testBi << " "  << vali << "\n";
        // }

        if  ( (vali == EMPTY) && (n->getBox().max_diam()>eps)){
            vali = UNK;
        } else {
            // std::cout << n->isLeaf() << " " << n->value() << " " << testBi << " " << vali << "\n";
        }

				// On bisect quand :
				// 	- max_diazm > eps
				//  - vali n'est pas un singleton
        if( !is_singleton(vali)  && (n->getBox().max_diam()>eps)){
            j++;
						if(n->isLeaf()){
							n->bisect(bisector);
						}
            L.push_back(n->left());
            L.push_back(n->right());
        } else {
            // vali = (vali == UNK) ? MAYBE : vali;
						n->remove_children();
        }
        n->setValue(vali);
    }
    std::cerr << "k = " << k  << "|| j = " << j << std::endl;
    return (*this);
}
// ThickPaving& ThickPaving::Sivia(FuncTest &test, double eps, BINARY_OP op )
// {
//     list<Node*> L;
//     L.push_back(&root);
//     int k = 0, j = 0;
//     // vibes::beginDrawing();
// //    vibes::newFigure("Test");
// //    vibes::setFigureProperties(vibesParams("x",0,"y",220,"width",400,"height",400));
// //    vibes::drawBox(B[0][0].lb(), B[0][0].ub(), B[0][1].lb(), B[0][1].ub(), "k[g]");
// //    vibes::axisAuto();
//     while (!L.empty()){
//         k++;
//         Node* n=L.front();    L.pop_front();
// //        vibes::drawBox(B[i][0].lb(), B[i][0].ub(), B[i][1].lb(), B[i][1].ub(), "k[g]");
//
//         ThickBoolean testBi=test(n->getBox());
//         ThickBoolean vali = op(n->value(),testBi);
//
//         bool b1 = !is_singleton(vali);
//         // b1 = (vali != MAYBE) ? true : b1;
//         // std::cerr << "[" << k << "] " << n->getBox() << " " << n->getBox().diam() << " " << n->value() << " " << testBi << " " << vali << "\n";
//         if (vali == EMPTY){
//           std::cout << n->isLeaf() << " " << n->value() << " " << testBi << " " << vali << "\n";
//         }
//         // bool b1 = !( vali == IN || vali == OUT);
//         // bool b11 = (n->value() == UNK) && !n->isLeaf();
//         // bool b12 = testBi == UNK;
//         // cout << b11 << " " << b12 << " " << n->isLeaf() << " " << n->value() << " " << testBi << " " << vali << "\n";
//         // if ( vali == MAYBE && ( b11^b12 )){
//         //   b1 = true;
//         //   vali = UNK;
//         // }
//         // if (n->isLeaf() && vali == MAYBE && testBi == MAYBE && n->value() == UNK){
//         //     vali = UNK;
//         // }
//         // bool b11 = !n->isLeaf() && (n->value() == UNK || n->value() == MAYBE) ; // n'est pas une feuiile et UNK
//         // if (!n->isLeaf())
//
//         // bool b1 = !( vali == IN || vali == OUT);
//         //
//         // bool b2 = (vali == MAYBE) && ( !b11 || (testBi == UNK) ) ;
//         //
//
//         // if (!b2 && (vali == MAYBE) ) b1 = true;
//         // if (b2) b1 = true;
//         // bool b2 = !(is_singleton(testBi) /*&& n->isLeaf()*/);
//
// 				// bool b1 = (vali == UNK);
//         // bool b2 = true; //(testBi == UNK);
//
//
// 				// std::cerr << "[" << k << "] " << n->getBox() << "\n";
// 				// std::cerr << n->value() << " " << testBi <<  " "  << vali << " " << b1 << " " << b2 << std::endl;
//
//         // bool b3 = (val[i] == MAYBE || val[i] == IN || val[i] == OUT) ;
//         // bool b1 = (val[i] == MAYBE && is_enable[i] == true) | (testBi == MAYBE && pdc.is_enable == true);
//         // is_enable[i] = is_enable[i] | pdc.is_enable;
//         // std::cerr << val[i] << " " << testBi << "\n";
//         // if(vali == EMPTY){
//         //     cerr << "error !!\n";
//         //     Clear(EMPTY);
//         //     val[0] = EMPTY;
//         //     break;
//         // }
//
// 				// On bisect quand :
// 				// 	- max_diazm > eps
// 				//  - vali n'est pas un singleton
//         if( (b1) && (n->getBox().max_diam()>eps)){
//             j++;
// 						if(n->isLeaf()){
// 							// cerr << "bisect " << n->value() << endl;
// 							n->bisect(bisector);
// 						}
//             L.push_back(n->left());
//             L.push_back(n->right());
//         } else {
//             // vali = (vali == UNK) ? MAYBE : vali;
// 						n->remove_children();
//         }
//
//         n->setValue(vali);
// //        if (vali!=MAYBE && (B[i].max_diam()<=eps)) {
// ////            if (vali == IN)
// ////                vibes::drawBox(B[i][0].lb(), B[i][0].ub(), B[i][1].lb(), B[i][1].ub(), "[r]");
// ////            else
// ////                vibes::drawBox(B[i][0].lb(), B[i][0].ub(), B[i][1].lb(), B[i][1].ub(), "[b]");
// //            Remove_sons(i);
// //        } else if ( (B[i].max_diam()>eps) && testBi == MAYBE ){
//
// //        }
// //        vibes::clearFigure();
//     }
//     std::cerr << "k = " << k  << "|| j = " << j << std::endl;
//     // Clean();
//    // Reunite();
//     // vibes::endDrawing();
//     return (*this);
// }


//------------------------------------------------------------------------------
ThickPaving& ThickPaving::FastSivia(ThickTest &pdc, double eps, BINARY_OP op ){
	FuncTest f = [&pdc](const IntervalVector& X)->ThickBoolean {
    return pdc.test(X);
	};
	return FastSivia(f, eps, op);
}

ThickPaving& ThickPaving::FastSivia(FuncTest &test, double eps, BINARY_OP op )
{
    list<Node*> L;
    L.push_back(&root);
    int k = 0, j = 0;
    while (!L.empty()){
        k++;
        Node* n=L.front();    L.pop_front();


        ThickBoolean testBi=test(n->getBox());
        ThickBoolean vali = op(n->value(),testBi);

        bool b1 = !is_singleton(vali);

        if (vali == IN || ( test(n->getBox().mid()) == IN) ){
        // if (vali == IN ) {
          // root.clear();
          // root.setValue(IN);
          n->setValue(IN);
          break;
        }


				// On bisect quand :
				// 	- max_diazm > eps
				//  - vali n'est pas un singleton
        if( (b1) && (n->getBox().max_diam()>eps)){
            j++;
						if(n->isLeaf()){
							// cerr << "bisect " << n->value() << endl;
							n->bisect(bisector);
						}
            L.push_back(n->left());
            L.push_back(n->right());
        } else {
            // vali = (vali == UNK) ? MAYBE : vali;
						n->remove_children();
        }

        n->setValue(vali);

    }
    std::cerr << "k = " << k  << "|| j = " << j << std::endl;

    return (*this);
}



//------------------------------------------------------------------------------
//----------------------  DOSEN T WORK AT ALL    -------------------------------
//------------------------------------------------------------------------------

ThickBoolean ThickPaving::erode(ThickTest &pdc, double eps, BINARY_OP op ){
	FuncTest f = [&pdc](const IntervalVector& X)->ThickBoolean {
    return pdc.test(X);
	};
	return erode(f, eps, op);
}


ThickBoolean ThickPaving::erode(FuncTest &test, double eps, BINARY_OP op){
  // TWO PASS VERSION
  list<Node*> L;
  L.push_back(&root);
  int k = 0, j = 0;
  bool find_in = false;
  // Pass1 on test
  while (!L.empty()){
    k++;
    Node* n=L.front();    L.pop_front();
    ThickBoolean vali = test(n->getBox());
    ThickBoolean vali_mid = (!is_singleton(vali)) ? test(n->getBox().mid()) : vali;
    if (opUpper(vali_mid) == IN){
      find_in = true;
      break;
    }
    bool b1 = !is_singleton(opUpper(vali));

    if( (b1) && (n->getBox().max_diam()>eps)){
        j++;
        if(n->isLeaf()){
          n->bisect(bisector);
        }
        L.push_back(n->left());
        L.push_back(n->right());
    } else {
        n->remove_children();
    }

    n->setValue(vali);

  } // end while
  std::cerr << "k = " << k  << "|| j = " << j << std::endl;
  if (!find_in && check_empty() == OUT){
    return IN;
  }

  L.clear(); L.push_back(&root);
  root.clear();
  // Second pass
  while (!L.empty()){
    k++;
    Node* n=L.front();    L.pop_front();
    // ThickBoolean vali;
    // if(!n->isLeaf()){
    //    vali = n->value();
    // } else {
    //    vali = ( is_singleton(n->value()) ) ? n->value() : test(n->getBox());
    // }

    ThickBoolean vali = test(n->getBox());
    ThickBoolean vali_mid = (is_singleton(vali)) ? vali : test(n->getBox().mid());
    if (vali == IN || opLower(vali_mid) == IN){
      n->setValue(IN);
      return OUT;
      // break;
    }
    bool b1 = !is_singleton(opLower(vali));

    if( (b1) && (n->getBox().max_diam()>eps)){
        j++;
        if(n->isLeaf()){
          n->bisect(bisector);
        }
        L.push_back(n->left());
        L.push_back(n->right());
    } else {
        n->remove_children();
    }

    n->setValue(vali);

  } // end while
  std::cerr << "k = " << k  << "|| j = " << j << std::endl;
  return UNK;

}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// ThickBoolean ThickPaving::erode(FuncTest &test, double eps, BINARY_OP op){
//   list<Node*> L;
//   L.push_back(&root);
//   int k = 0, j = 0;
//   bool test_ub = true;
//
//   while (!L.empty()){
//       k++;
//       Node* n=L.front();    L.pop_front();
//
//       ThickBoolean vali = test(n->getBox());
//       ThickBoolean vali_mid = (!is_singleton(vali)) ? test(n->getBox().mid()) : vali;
//
//       // ThickBoolean vali_lb = opUpper(vali);
//       // ThickBoolean vali_ub = opLower(vali);
//
//       //
//       if ( vali_mid == IN ){
//         n->setValue(IN);
//         return OUT;
//       }
//       if ( opUpper(vali_mid) == IN ){
//         // n->setValue(IN);
//         test_ub = false;
//         // break;
//       }
//
//       vali = (test_ub == false) ? opLower(vali) : vali;
//       // ThickBoolean vali = op(n->value(),testBi);
//       bool b1 = !is_singleton(vali);
//
//       // On bisect quand :
//       // 	- max_diazm > eps
//       //  - vali n'est pas un singleton
//       if( (b1) && (n->getBox().max_diam()>eps)){
//           j++;
//           if(n->isLeaf()){
//             // cerr << "bisect " << n->value() << endl;
//             n->bisect(bisector);
//           }
//           if (vali == MAYBE_IN){
//               L.push_front(n->left());
//               L.push_front(n->right());
//           } else {
//             L.push_back(n->left());
//             L.push_back(n->right());
//           }
//
//       } else {
//           // vali = (vali == UNK) ? MAYBE : vali;
//           n->remove_children();
//       }
//
//       n->setValue(vali);
//
//   }
//   std::cerr << "k = " << k  << "|| j = " << j << " " << test_ub << std::endl;
//   if (test_ub && check_empty() == OUT){
//     return IN;
//   }
//
//   return UNK;
// }

//
// ThickBoolean ThickPaving::erode_rec(FuncTest &test, double eps, Node* n){
//   ThickBoolean vali=test(n->getBox());
//   ThickBoolean vali_mid = (!is_singleton(vali)) ? test(n->getBox().mid()) : vali;
//
//   ThickBoolean vali_lb = opLower(vali);
//   ThickBoolean vali_ub = opUpper(vali);
//
//
//
// }
//----------------------------------------------------------------------
// ThickPaving  Sqcap(ThickPaving& X,ThickPaving& Y)
// {
    // ThickPaving Z(X.B[0].size());
    // op_binaire(Z,X,Y,ibex::Inter);
    // return (Z);
// }
//----------------------------------------------------------------------
void  Set_Contractor_Inter(ThickPaving& Z,ThickPaving& X,ThickPaving& Y)   //project with respect to Z = X inter Y
{    //op_binaire(Z,X,Y,Inter);  //Z=X inter Y

    //D= ThickPaving(X.B[0]);
    //op_binaire(Z,X,Y,Restrict);  //Z=X inter Y


    //ThickPaving Z1=Z;
    //     op_binaire(D,0,X,0,Y,0,0);     // and
    //     op_binaire(Z,0,Z1, 0,D,0,2);   // intersect
    //A FINIR
}

//----------------------------------------------------------------------
void  Set_Contractor_Overlap(ThickPaving& X,ThickPaving& Y)   //project with respect to X and Y overlap
{
    // ThickPaving XinterY(X.B[0].size());
    // op_binaire(XinterY,X,Y,And);
    // if (XinterY.val[0]==OUT)
    // {  X.Clear(EMPTY);  Y.Clear(EMPTY);}
}
//----------------------------------------------------------------------
ThickBoolean Overlap(ThickPaving& X,ThickPaving& Y)   //returns if  X and Y overlap
{
    // ThickPaving XinterY(X.B[0].size());
    // op_binaire(XinterY,X,Y,And);
    // if (XinterY.val[0]==OUT) return OUT;
    // for (int k=0;k<XinterY.B.size();k++)
    //     if (XinterY.val[0]==IN) return IN;
    return MAYBE;
}

//----------------------------------------------------------------------
// void  Set_Contractor_Subset(ThickPaving& X,ThickPaving& Y)   //project with respect to X \subset Y
// {
    // ThickPaving Z(X.B[0].size());
    // Solution 1
    //op_binaire(Z,X,Y,leq);  //X=X inter Y
    //X=Z;
    //op_binaire(Z,Y,X,geq);  //Y=X union Y
    //Y=Z ;
    //  ------
    // Solution 2
    // op_binaire(Z,X,Y,And);  //X=X inter Y
    // X=Sqcap(X,Z);
    // op_binaire(Z,Y,X,Or);  //Y=X union Y
    // Y=Sqcap(Y,Z);
// }
//----------------------------------------------------------------------
// void  Set_Contractor_Disjoint(ThickPaving& X,ThickPaving& Y)   //project with respect to X inter Y = EMPTY
// {
    // ThickPaving All(X.B[0],MAYBE);
    // ThickPaving Z1(X.B[0].size());
    // op_binaire(Z1,All,Y,Restrict);
    // ThickPaving Z2(X.B[0].size());
    // Z2=Sqcap(X,Z1);

    // ThickPaving Z3(X.B[0].size());
    // op_binaire(Z3,All,X,Restrict);
    // ThickPaving Z4(X.B[0].size());
    // Z4=Sqcap(Y,Z3);

    // X=Z2; Y=Z4;
// }

} // end namespace ibex
