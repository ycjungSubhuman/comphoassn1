#pragma once

#include <memory>

#include "align.hpp"
#include "match_score.hpp"

namespace compho {
/**
 * Image-aligning with raw image perturbation. No scaling is applied
 */
class SingleScalePairAlignAlgorithm : public PairwiseAlignAlgorithm {
  public:
    /**
     * @param pert            pertervation radius in pixel units.
     *                        each element should be positive values.
     *
     * @param fn_score        matching score calculator object
     */
    SingleScalePairAlignAlgorithm(cv::Vec2i pert,
                                  std::shared_ptr<MatchingScore> fn_score,
                                  cv::Vec2i offset = cv::Vec2i(0, 0));

    /**
     * The result from this function does not have offset in it.
     * Add offset to the result to get absolute displacement value.
     */
    cv::Vec2i align_pair(const cv::Mat1f &fixed,
                         const cv::Mat1f &moving) override;

  private:
    cv::Vec2i m_pert;
    std::shared_ptr<MatchingScore> m_fn_score;
    cv::Vec2i m_offset;
};
} // namespace compho
