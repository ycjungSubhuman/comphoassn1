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
     * @param pert            pertervation size in pixel units.
     *                        Each element should be positive values.
     */
    SingleScalePairAlignAlgorithm(cv::Vec2i pert,
                                  std::shared_ptr<MatchingScore> score);

    /**
     * Get a translation vector for moving image
     *
     * @param fixed             fixed reference image.
     * @param moving            moving image.
     */
    cv::Vec2i align_pair(const cv::Mat1f &fixed,
                         const cv::Mat1f &moving) override;

  private:
    cv::Vec2i m_pert;
    std::shared_ptr<MatchingScore> m_score;
};
} // namespace compho
