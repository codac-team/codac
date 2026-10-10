/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Paving.h>

using namespace std;
using namespace codac2;

TEST_CASE("Paving tree structure after bisection")
{
  PavingOut p(IntervalVector({{-1.,1.},{-1.,1.}}));
  auto root = p.tree();

  REQUIRE(root->is_leaf());
  CHECK(root->top() == nullptr);

  root->bisect();
  REQUIRE_FALSE(root->is_leaf());
  REQUIRE(root->left() != nullptr);
  REQUIRE(root->right() != nullptr);
  CHECK(root->left()->top() == root);
  CHECK(root->right()->top() == root);
  CHECK(root->left()->is_leaf());
  CHECK(root->right()->is_leaf());

  root->left()->bisect();
  REQUIRE(root->left()->left() != nullptr);
  CHECK(root->left()->left()->top() == root->left());
}

TEST_CASE("Paving nodes do not leak through a parent/child reference cycle")
{
  std::weak_ptr<PavingOut_Node> w_root, w_left, w_left_left;

  {
    PavingOut p(IntervalVector({{-1.,1.},{-1.,1.}}));
    auto root = p.tree();
    root->bisect();
    root->left()->bisect();

    w_root = root;
    w_left = root->left();
    w_left_left = root->left()->left();

    // Sanity check: while 'p' (and so the whole tree) is alive, every
    // node must still resolve through its weak_ptr.
    REQUIRE_FALSE(w_root.expired());
    REQUIRE_FALSE(w_left.expired());
    REQUIRE_FALSE(w_left_left.expired());
  }
  // 'p' is destroyed here. Before the fix, every node below the root
  // would still be kept alive by its child's (owning) _top pointing back
  // up through the cycle, so none of the weak_ptr's below would expire.

  CHECK(w_root.expired());
  CHECK(w_left.expired());
  CHECK(w_left_left.expired());
}