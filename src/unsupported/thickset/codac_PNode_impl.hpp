/* ============================================================================
 *  IBEX-Robotics - PNode class
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
#include <array>

template <typename V>
PNode<V>::PNode(const codac::IntervalVector& box, V value):
	m_value(value), m_box(box), m_left(nullptr), m_right(nullptr) {}

template <typename V>
PNode<V>::PNode(const PNode& n) :  m_value(n.m_value), m_box(n.m_box), m_left(nullptr), m_right(nullptr) {
  if (n.m_left)
    m_left = new PNode(*n.m_left);  // recursively call copy constructor
  if (n.m_right)
    m_right = new PNode(*n.m_right);  // recursively call copy constructor
}

template<typename V>
PNode<V>::PNode(PNode&& other): m_value(other.m_value), m_box(other.m_box){
	m_left =  other.m_left; other.m_left = nullptr;
	m_right = other.m_right; other.m_right = nullptr;
}

template <typename V>
PNode<V>::~PNode()
{
  if(m_left != nullptr)
    delete m_left;

  if(m_right != nullptr)
    delete m_right;
}

template <typename V>
PNode<V> &PNode<V>::operator=(const PNode<V> &&other){
	if (&other == this){
			return *this;
	}
	m_left = other.m_left;
	m_right = other.m_right;
	m_value = other.m_value;
	m_box = other.m_box;
	return *this;
}

template <typename V>
const codac::IntervalVector& PNode<V>::getBox() const
{
  return m_box;
}

template <typename V>
V PNode<V>::value() const
{
  return m_value;
}

template <typename V>
bool PNode<V>::isLeaf() const
{
  return m_left == nullptr && m_right == nullptr;
}

template <typename V>
int PNode<V>::height() const
{
  if(isLeaf())
    return 1;

  return 1 + std::max(m_left->height(), m_right->height());
}

template <typename V>
inline PNode<V>* PNode<V>::left() const
{
  return m_left;
}

template <typename V>
inline PNode<V>* PNode<V>::right() const
{
  return m_right;
}

template <typename V>
void PNode<V>::setValue(V value, bool propagate_to_leafs)
{
  m_value = value;
  // if(propagate_to_leafs && !isLeaf())
  // {
  //   m_left->setValue(value);
  //   m_right->setValue(value);
  // }
}

template <typename V>
int PNode<V>::getSubpavingsNumber(V value) const
{
  if(isLeaf())
    return (int)(m_value & value);

  else
    return m_left->getSubpavingsNumber(value) + m_right->getSubpavingsNumber(value);
}

template <typename V>
bool PNode<V>::reunite()
{
  bool has_been_simplified = false;

  if(!isLeaf())
  {
    has_been_simplified |= m_left->reunite();
    has_been_simplified |= m_right->reunite();

    if(m_left->isLeaf() && m_right->isLeaf() &&
       m_left->value() == m_right->value())
    {
      m_value = m_left->value();
      delete m_left;
      delete m_right;
      m_left = nullptr;
      m_right = nullptr;
      has_been_simplified = true;
    } else {
			m_value = m_left->value() | m_right->value();
  	}
	}

  return has_been_simplified;
}

template <typename V>
void PNode<V>::bisect(ibex::Bsc &bisector)
{
  if(isLeaf())
  {
    std::pair<codac::IntervalVector,codac::IntervalVector> boxes = bisector.bisect(m_box);
    m_left = new PNode(boxes.first, m_value);
    m_right = new PNode(boxes.second, m_value);
  }
}

template <typename V>
template<typename T>
void PNode<V>::visit(T & visitor){
	if(isLeaf())
		visitor.visit_leaf(getBox(), value());
	else{
		visitor.visit_node(getBox());
		left()->visit(visitor);
		right()->visit(visitor);
	}
}


template <typename V>
int PNode<V>::countLeaves(){
	if (isLeaf())
		return 1;
	return left()->countLeaves() + right()->countLeaves();

}

template <typename V>
void PNode<V>::remove_children(){
	delete m_left; m_left = nullptr;
	delete m_right; m_right = nullptr;
}


template <typename V >
void PNode<V>::clear(){
	if(m_left != nullptr)
    delete m_left;
  if(m_right != nullptr)
    delete m_right;
	m_left=nullptr;
	m_right=nullptr;
}

template< typename V>
void PNode<V>::save(std::ofstream& of)
{
	int size = m_box.size();
	of.write((char*)(&m_value), sizeof(m_value));
	of.write((char*)(&size), sizeof(size));
	for (int i = 0; i < size; i++){
		double lb = m_box[i].lb();
		double ub = m_box[i].ub();
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

template< typename V>
PNode<V>* PNode<V>::load(std::ifstream& infile)
{
	int size;
	V value;
	bool has_children;
	infile.read((char*)(&value), sizeof(value));
	infile.read((char*)(&size), sizeof(size));
	std::vector< std::array<double, 2> > bounds = std::vector< std::array<double, 2> >(size);
  	infile.read((char*)(&bounds[0][0]), 2*size*sizeof(double));
	infile.read((char*)(&has_children), sizeof(has_children));
	PNode<V>* node = new PNode<V>(IntervalVector(size, bounds[0][0]), value );
	if (has_children) {
		node->m_left = PNode<V>::load(infile);
		node->m_right = PNode<V>::load(infile);
	}
	return node;
}
