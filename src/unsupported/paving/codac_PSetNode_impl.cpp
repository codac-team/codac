/* ============================================================================
 *  IBEX-Robotics - PSetNode class
 * ============================================================================
 *  Copyright : Simon Rohou, Benoit Desrochers
 *  License   : This program can be distributed under the terms of
 *              the MIT license. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou, Benoit Desrochers
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */



#include <utility>
#include <codac_PSetNode.h>

namespace pyibex {

PSetNode::PSetNode(const ibex::IntervalVector& box):
  m_box_in(box), m_box_out(box), m_left(nullptr), m_right(nullptr) {}

PSetNode::PSetNode(const ibex::IntervalVector& box_in, const ibex::IntervalVector& box_out):
  m_box_in(box_in), m_box_out(box_out), m_left(nullptr), m_right(nullptr) {}

PSetNode::PSetNode(const PSetNode& n) :  m_box_in(n.m_box_in), m_box_out(n.m_box_out), m_left(nullptr), m_right(nullptr) {
  if (n.m_left)
    m_left = new PSetNode(*n.m_left);  // recursively call copy constructor
  if (n.m_right)
    m_right = new PSetNode(*n.m_right);  // recursively call copy constructor
}


PSetNode::PSetNode(PSetNode&& other): m_box_in(other.m_box_in), m_box_out(other.m_box_out){
  m_left =  other.m_left; other.m_left = nullptr;
  m_right = other.m_right; other.m_right = nullptr;
}

PSetNode::~PSetNode()
{
  if(m_left != nullptr)
    delete m_left;

  if(m_right != nullptr)
    delete m_right;
}

PSetNode &PSetNode::operator=(const PSetNode &&other){
  if (&other == this){
      return *this;
  }
  m_left = other.m_left;
  m_right = other.m_right;
  m_box_in = other.m_box_in;
  m_box_out = other.m_box_out;
  return *this;
}

ibex::IntervalVector& PSetNode::getBoxOut()
{
  return m_box_out;
}

ibex::IntervalVector& PSetNode::getBoxIn()
{
  return m_box_in;
}

bool PSetNode::isLeaf() const
{
  return m_left == nullptr && m_right == nullptr;
}

int PSetNode::height() const
{
  if(isLeaf())
    return 1;

  return 1 + std::max(m_left->height(), m_right->height());
}

PSetNode* PSetNode::left() const
{
  return m_left;
}

PSetNode* PSetNode::right() const
{
  return m_right;
}

//
// void PSetNode::setValue(V value, bool propagate_to_leafs)
// {
//   m_box_in = value;
//   // if(propagate_to_leafs && !isLeaf())
//   // {
//   //   m_left->setValue(value);
//   //   m_right->setValue(value);
//   // }
// }

void PSetNode::removeNode(){
  bool bl = m_left->is_empty();
  bool br = m_right->is_empty();
  if (bl && br){
    delete m_left; m_left=nullptr;
    delete m_right; m_right=nullptr;
  } else if (!bl && br){
    PSetNode* tmp = m_left;
    delete m_right;
    m_right = tmp->m_right;
    m_left = tmp->m_left;
    tmp->m_left = nullptr; tmp->m_right = nullptr;
    delete tmp;
  } else if (bl && !br){
    PSetNode* tmp = m_right;
    delete m_left;
    m_left=m_right->m_left;
    m_right = m_right->m_right;
    tmp->m_left = nullptr; tmp->m_right = nullptr;
    delete tmp;
  } else {
    assert(false);
  }
}

bool PSetNode::is_empty(){
  return (m_box_in & m_box_out).is_empty();
}

bool PSetNode::reunite()
{
  static int id = 0;
  bool has_been_simplified = false;
  if(isLeaf()) return false;
  if(!isLeaf())
  {
    // std::cerr << " ID " << id++ << " " << this << "\n";
    has_been_simplified |= m_left->reunite();
    has_been_simplified |= m_right->reunite();
    IntervalVector *cXin, *cXout;
    IntervalVector X0 = m_box_in | m_box_out;
    int nin = X0.diff(m_box_in, cXin);
    int nout = X0.diff(m_box_out, cXout);

    m_box_in = m_left->m_box_in | m_right->m_box_in;
    for( int i = 0; i < nout; i++){
      m_box_in |= cXout[i];
    }

    m_box_out = m_left->m_box_out | m_right->m_box_out;
    for( int i = 0; i < nin; i++){
      m_box_out |= cXin[i];
    }
    delete[] cXin;
    delete[] cXout;

		// std::cerr << m_box_out << " "  << m_box_in <<  " " << X << "\n";
    if (m_left->is_empty() || m_right->is_empty()){
      removeNode();
    } else {
			IntervalVector X(m_box_in & m_box_out);
      m_left->m_box_in &= X;
      m_left->m_box_out &= X;
      m_right->m_box_in &= X;
      m_right->m_box_out &= X;
    }

    // if (!isLeaf()){
    //   assert(!m_box_in.is_empty());
    //   assert(!m_box_out.is_empty());
		// 	IntervalVector X(m_box_in & m_box_out);
		// 	IntervalVector X1(m_left->m_box_in & m_left->m_box_out);
		// 	IntervalVector X2(m_right->m_box_in & m_right->m_box_out);
    //
    //   assert(X == (X1 | X2));
    //
    // }
  }

  return has_been_simplified;
}


ibex::BoolInterval PSetNode::isInside(const ibex::Vector& p){
    bool bin = this->m_box_in.contains(p);
    bool bout = this->m_box_out.contains(p);

    if(bin && !bout){
      return ibex::NO;
    } else if (!bin && bout){
      return ibex::YES;
    } else if (bin & bout){
      if (this->isLeaf()) return ibex::MAYBE;
      else {
        if (left()->m_box_in.contains(p) || left()->m_box_out.contains(p)){
          return left()->isInside(p);
        } else if (right()->m_box_in.contains(p) || right()->m_box_out.contains(p)){
          return right()->isInside(p);
        } else {
          std::cerr << " Underterminated case !!!!\n";
        }
      }
    } else if (!bin && !bout){
      // std::cerr << "WARNING !bin && !bout\n";
      return ibex::EMPTY_BOOL;
    }
    return ibex::MAYBE;
}

void PSetNode::bisect(ibex::Bsc &bisector)
{
  if(isLeaf())
  {
    IntervalVector X(m_box_out & m_box_in);
    std::pair<ibex::IntervalVector,ibex::IntervalVector> boxes = bisector.bisect(X);
    m_left = new PSetNode(boxes.first);
    m_right = new PSetNode(boxes.second);
  }
}

void PSetNode::bisect_around(ibex::Bsc &bisector, IntervalVector& XX){
      //
      // X = self.xin & self.xout
      // LX = X.diff(XX)
      // if(len(LX) == 0):
      //     print("[len(LX) == 0]", X, XX)
      //     exit()
      // if len(LX) == 1:
      //     X1, X2 = XX, LX[0]
      // else:
      //     X1, X2 = bisector.bisect(X)
      //     if not (XX.is_subset(X1) or XX.is_subset(X2)):
      //         VX1 = reduce(lambda x, y: x*y, (XX & X1).diam(), 1)
      //         VX2 = reduce(lambda x, y: x*y, (XX & X2).diam(), 1)
      //         if VX1 > VX2:
      //             X1 = X1 | XX
      //             X2 = None if (X & X1).is_empty() else X.diff(X1)[0]
      //         else:
      //             X1 = X2 | XX
      //             X2 = None if (X & X1).is_empty() else X.diff(X1)[0]
      //     elif (XX.is_subset(X2)){
      //       X1, X2 = X2, X1
      //     } else {
      //       print("ERROR !!!")
      //       exit(-1)
      //     }
      // old_left, old_right = self.left, self.right
      // if (!X2.is_empty()){
      //     self.left = pySetNode(X1)
      //     self.right = pySetNode(X2)
      //     self.left.left = old_left
      //     self.left.right = old_right
      // }
      //
      // return;
}

void PSetNode::bisect_max(ibex::Bsc &bisector){
      IntervalVector X(m_box_in & m_box_out);
      IntervalVector XX(left()->m_box_in | left()->m_box_out | right()->m_box_in | right()->m_box_out);
      if (X == XX)  return;
      if (X.is_subset(XX)) return;
      if (!XX.is_subset(X)){
        XX &= X;
      }
      return bisect_around(bisector, XX);
}



int PSetNode::countLeaves(){
  if (isLeaf())
    return 1;
  return left()->countLeaves() + right()->countLeaves();

}

void PSetNode::remove_children(){
  delete m_left; m_left = nullptr;
  delete m_right; m_right = nullptr;
}



void PSetNode::clear(){
  if(m_left != nullptr)
    delete m_left;
  if(m_right != nullptr)
    delete m_right;
  m_left=nullptr;
  m_right=nullptr;
}


void PSetNode::save(std::ofstream& of)
{
  int size = m_box_out.size();
  // of.write((char*)(&m_box_in), sizeof(m_box_in));
  // save Xout
  of.write((char*)(&size), sizeof(size));
  for (int i = 0; i < size; i++){
    double lb = m_box_out[i].lb();
    double ub = m_box_out[i].ub();
    of.write((char*)(&lb), sizeof(double));
    of.write((char*)(&ub), sizeof(double));
  }
  // Save Xin
  for (int i = 0; i < size; i++){
    double lb = m_box_in[i].lb();
    double ub = m_box_in[i].ub();
    of.write((char*)(&lb), sizeof(double));
    of.write((char*)(&ub), sizeof(double));
  }
  bool has_children = !isLeaf();
  of.write((char*)(&has_children), sizeof(bool));
  if (has_children){
    m_left->save(of);
    m_right->save(of);
  }
}


PSetNode* PSetNode::load(std::ifstream& infile)
{
  int size;
  // V value;
  bool has_children;

  infile.read((char*)(&size), sizeof(size));
  auto bounds_in = new double[size][2];
  auto bounds_out = new double[size][2];
  infile.read((char*)(&bounds_out), 2*size*sizeof(double));
  infile.read((char*)(&bounds_in), 2*size*sizeof(double));
  infile.read((char*)(&has_children), sizeof(has_children));
  PSetNode* node = new PSetNode(IntervalVector(size, bounds_in), IntervalVector(size, bounds_out) );
  delete[] bounds_in;
  delete[] bounds_out;
  if (has_children) {
    node->m_left = PSetNode::load(infile);
    node->m_right = PSetNode::load(infile);
  }
  return node;
}

} // end namespace pybiex
