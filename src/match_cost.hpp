#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

namespace compho {
/**
 * A binary matching cost function for single-channel images
 */
class MatchingCost {
  public:
    virtual double operator()(const cv::Mat &a, const cv::Mat &b) = 0;
};

class SumOfSquare : public MatchingCost {
  public:
    double operator()(const cv::Mat &a, const cv::Mat &b) override;
};

class NormalizedCrossCorrelation : public MatchingCost {
  public:
    double operator()(const cv::Mat &a, const cv::Mat &b) override;
};

class ZeroMeanNormalizedCrossCorrelation : public MatchingCost {
  public:
    double operator()(const cv::Mat &a, const cv::Mat &b) override;
};
} // namespace compho
