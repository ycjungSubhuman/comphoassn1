#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

namespace compho {
/**
 * An abstract class for image-aligning algorithm
 */
class AlignAlgorithm {
  public:
    /**
     * Given a list of images, return a list of global translation
     *
     * @param images                target images. Referene is selected as the
     *                              image on index 0
     *
     * @return                      list of translation vectors for each image
     */
    virtual std::vector<cv::Vec2i>
    align(const std::vector<cv::Mat1f> &images) = 0;
};

/**
 * An abstract class for pair-wise image-aligning algorithm
 */
class PairwiseAlignAlgorithm : public AlignAlgorithm {
  public:
    std::vector<cv::Vec2i> align(const std::vector<cv::Mat1f> &images) override;

    /**
     * Get a translation vector for moving image
     *
     * @param fixed             fixed reference image.
     * @param moving            moving image.
     */
    virtual cv::Vec2i align_pair(const cv::Mat1f &fixed,
                                 const cv::Mat1f &moving) = 0;
};
} // namespace compho
