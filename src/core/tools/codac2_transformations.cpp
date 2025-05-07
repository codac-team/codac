/** 
 *  codac2_transformations.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_transformations.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  Eigen::Affine2d affine_transformation(const std::vector<Vector>& src, const std::vector<Vector>& dst)
  {
    assert_release(src.size() == dst.size() && src.size() >= 2);
    assert_release(src[0].size() == dst[0].size() && dst[0].size() == 2);

    Index s = src[0].size(), n = src.size();
    Vector src_centroid = Vector::zero(s);
    Vector dst_centroid = Vector::zero(s);

    for(const auto& src_i : src)
    {
      assert_release(src_i.size() == s);
      src_centroid += src_i;
    }
    src_centroid /= n;

    for(const auto& dst_i : dst)
    {
      assert_release(dst_i.size() == s);
      dst_centroid += dst_i;
    }
    dst_centroid /= n;

    Matrix H = Matrix::zero(s,s); // familiar covariance matrix
    double src_variance = 0.0, dst_variance = 0.0;

    for(auto it_m1 = src.cbegin(), it_m2 = dst.begin();
        it_m1 != src.cend() || it_m2 != dst.cend(); )
    {
      H += (*it_m1 - src_centroid) * (*it_m2 - dst_centroid).transpose();
      src_variance += (*it_m1 - src_centroid).squaredNorm();
      dst_variance += (*it_m2 - dst_centroid).squaredNorm();
      it_m1++; it_m2++;
    }

    Eigen::JacobiSVD<Matrix> svd(H, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Matrix R = svd.matrixV() * svd.matrixU().transpose();

    double scale = std::sqrt(dst_variance / src_variance);

    Eigen::Affine2d transform = Eigen::Affine2d::Identity();
    transform.linear() = scale * R;
    transform.translation() = dst_centroid - scale * R * src_centroid;

    return transform;
  }

  Eigen::Affine2d affine_transformation(const SampledTraj<Vector>& src, const SampledTraj<Vector>& dst)
  {
    std::vector<Vector> v_src(src.nb_samples()), v_dst(dst.nb_samples());

    size_t i = 0;
    for(const auto& [t_i,src_i] : src)
      v_src[i++] = src_i;

    i = 0;
    for(const auto& [t_i,dst_i] : dst)
      v_dst[i++] = dst_i;

    return affine_transformation(v_src, v_dst);
  }
}