#include <algorithm>
#include <cmath>
#include <iostream>
#include <opencv2/core.hpp>
#include <tuple>
#include <vector>

#include "single_scale.hpp"
#include <opencv2/imgproc.hpp>

namespace compho {

SingleScalePairAlignAlgorithm::SingleScalePairAlignAlgorithm(
    cv::Vec2i pert, std::shared_ptr<MatchingCost> cost)
    : m_pert(std::move(pert)), m_cost(std::move(cost)) {}

namespace {
/**
 * Get overlapping portion of fixed and moving images given global translation
 */
void overlap_translated(int delta_x, int delta_y, const cv::Mat &fixed,
                        const cv::Mat &moving, cv::Mat &fixed_overlapped,
                        cv::Mat &moving_overlapped) {
    cv::Rect rect_fixed(0, 0, fixed.size().width, fixed.size().height);
    cv::Rect rect_moved(delta_x, delta_y, fixed.size().width,
                        fixed.size().height);
    cv::Rect rect_neg_moved(-delta_x, -delta_y, fixed.size().width,
                            fixed.size().height);
    cv::Rect rect_fixed_intersection = rect_fixed & rect_moved;
    cv::Rect rect_moving_intersection = rect_fixed & rect_neg_moved;

    fixed_overlapped = fixed(rect_fixed_intersection);
    moving_overlapped = moving(rect_moving_intersection);
}
} // namespace

cv::Vec2i SingleScalePairAlignAlgorithm::align_pair(const cv::Mat &fixed,
                                                    const cv::Mat &moving) {

    std::vector<std::pair<double, cv::Vec2i>> cost_disp;

    for (int delta_x = -m_pert[0]; delta_x <= m_pert[0]; delta_x++) {
        for (int delta_y = -m_pert[1]; delta_y <= m_pert[1]; delta_y++) {
            cv::Mat fixed_overlapped;
            cv::Mat moving_overlapped;
            overlap_translated(delta_x, delta_y, fixed, moving,
                               fixed_overlapped, moving_overlapped);

            double cost = (*m_cost)(fixed_overlapped, moving_overlapped);
            cost_disp.emplace_back(cost, cv::Vec2i(delta_x, delta_y));
        }
    }

    std::sort(cost_disp.begin(), cost_disp.end(),
              [](const auto &p1, const auto &p2) -> bool {
                  return p1.first < p2.first;
              });

    return cost_disp.back().second;
}

} // namespace compho
