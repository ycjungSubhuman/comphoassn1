#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

namespace compho {
/**
 * A binary matching cost function for single-channel images
 */
class MatchingScore {
  public:
    virtual double operator()(const cv::Mat1f &a, const cv::Mat1f &b) = 0;
};

class SumOfSquare : public MatchingScore {
  public:
    double operator()(const cv::Mat1f &a, const cv::Mat1f &b) override;
};

class NormalizedCrossCorrelation : public MatchingScore {
  public:
    double operator()(const cv::Mat1f &a, const cv::Mat1f &b) override;
};

class ZeroMeanNormalizedCrossCorrelation : public MatchingScore {
  public:
    double operator()(const cv::Mat1f &a, const cv::Mat1f &b) override;
};
} // namespace compho
