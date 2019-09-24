#pragma once

#include "align.hpp"
#include "match_score.hpp"
#include "pyramid.hpp"
#include "single_scale.hpp"

namespace compho {

/**
 * Image-aligning with multi-scale image perturbation.
 */
class MultiScalePairAlignAlgorithm : public PairwiseAlignAlgorithm {
  public:
    /**
     * @param pert                  perturbation radius to be used in all image
     *                              scale
     *
     * @param fn_score              matching score calculator object
     *
     * @param num_level             number of levels in image pyramid
     *
     * @param factor_scale          scaling factor for each level of pyramid
     */
    MultiScalePairAlignAlgorithm(cv::Vec2i pert,
                                 std::shared_ptr<MatchingScore> fn_score,
                                 size_t num_level = 4, size_t factor_scale = 2);

    cv::Vec2i align_pair(const cv::Mat1f &fixed,
                         const cv::Mat1f &moving) override;

  private:
    cv::Vec2i m_pert;
    std::shared_ptr<MatchingScore> m_fn_score;
    size_t m_num_level;
    size_t m_factor_scale;
};
} // namespace compho
