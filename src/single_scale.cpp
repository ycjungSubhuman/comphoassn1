#include <algorithm>
#include <cmath>
#include <iostream>
#include <mutex>
#include <omp.h>
#include <opencv2/core.hpp>
#include <tuple>
#include <vector>

#include "single_scale.hpp"
#include <opencv2/imgproc.hpp>

namespace compho {

SingleScalePairAlignAlgorithm::SingleScalePairAlignAlgorithm(
    cv::Vec2i pert, std::shared_ptr<MatchingScore> score)
    : m_pert(std::move(pert)), m_score(std::move(score)) {}

namespace {
/**
 * Get overlapping portion of fixed and moving images given global translation
 */
void overlap_translated(int delta_x, int delta_y, const cv::Mat1f &fixed,
                        const cv::Mat1f &moving, cv::Mat1f &fixed_overlapped,
                        cv::Mat1f &moving_overlapped) {
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

cv::Vec2i SingleScalePairAlignAlgorithm::align_pair(const cv::Mat1f &fixed,
                                                    const cv::Mat1f &moving) {

    std::vector<std::pair<double, cv::Vec2i>> score_disp;

    const int tot_cnt = (2 * m_pert[0] + 1) * (2 * m_pert[1] + 1);
    int cnt;
    std::mutex mutex_score_disp;
    cnt = 0;

    auto get_score = [this, &fixed, &moving](int delta_x,
                                             int delta_y) -> double {
        cv::Mat1f fixed_overlapped;
        cv::Mat1f moving_overlapped;
        overlap_translated(delta_x, delta_y, fixed, moving, fixed_overlapped,
                           moving_overlapped);

        return (*this->m_score)(fixed_overlapped, moving_overlapped);
    };

#pragma omp parallel for
    for (int delta_x = -m_pert[0]; delta_x <= m_pert[0]; delta_x++) {
        for (int delta_y = -m_pert[1]; delta_y <= m_pert[1]; delta_y++) {
            double score = get_score(delta_x, delta_y);

            mutex_score_disp.lock();
            score_disp.emplace_back(score, cv::Vec2i(delta_x, delta_y));
            cnt++;
            std::cout << "Tried " << cnt << " out of " << tot_cnt << std::endl;
            mutex_score_disp.unlock();
        }
    }

    std::sort(score_disp.begin(), score_disp.end(),
              [](const auto &p1, const auto &p2) -> bool {
                  return p1.first < p2.first;
              });

    return score_disp.back().second;
}

} // namespace compho
