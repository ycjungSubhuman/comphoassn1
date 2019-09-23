#include <cmath>

#include "match_score.hpp"
namespace compho {

double SumOfSquare::operator()(const cv::Mat1f &a, const cv::Mat1f &b) {
    cv::Mat1f diff = a - b;
    return -cv::sum(diff.mul(diff))[0] /
           (diff.size().width * diff.size().height);
}

double NormalizedCrossCorrelation::operator()(const cv::Mat1f &a,
                                              const cv::Mat1f &b) {
    double denom = cv::sum(a.mul(b))[0];
    double nom = std::sqrt(sum(a.mul(a))[0] * cv::sum(b.mul(b))[0]);
    double correlation = denom / nom;
    assert(std::isfinite(correlation));
    return correlation;
}

double ZeroMeanNormalizedCrossCorrelation::operator()(const cv::Mat1f &a,
                                                      const cv::Mat1f &b) {
    NormalizedCrossCorrelation ncc;
    return ncc(a - cv::mean(a), b - cv::mean(b));
}
} // namespace compho
