#include <algorithm>
#include <cmath>
#include <iostream>
#include <mutex>
#include <omp.h>
#include <opencv2/core.hpp>
#include <tuple>
#include <vector>

#include "single_scale.hpp"
#include "util.hpp"
#include <opencv2/imgproc.hpp>

namespace compho {

SingleScalePairAlignAlgorithm::SingleScalePairAlignAlgorithm(
    cv::Vec2i pert, std::shared_ptr<MatchingScore> fn_score, cv::Vec2i offset)
    : m_pert(std::move(pert)), m_fn_score(std::move(fn_score)),
      m_offset(std::move(offset)) {}

cv::Vec2i SingleScalePairAlignAlgorithm::align_pair(const cv::Mat1f &fixed,
                                                    const cv::Mat1f &moving) {

    std::vector<std::pair<double, cv::Vec2i>> score_disp;

    std::mutex mutex_score_disp;
    const int tot_cnt = (2 * m_pert[0] + 1) * (2 * m_pert[1] + 1);
    int cnt;
    cnt = 0;
    auto calc_score = [this, &fixed, &moving, &score_disp, &mutex_score_disp,
                       &cnt, tot_cnt](int delta_x, int delta_y) -> void {
        cv::Mat1f fixed_overlapped;
        cv::Mat1f moving_overlapped;
        overlap_translated(delta_x + m_offset[0], delta_y + m_offset[1], fixed,
                           moving, fixed_overlapped, moving_overlapped);

        double score = (*this->m_fn_score)(fixed_overlapped, moving_overlapped);

        mutex_score_disp.lock();
        score_disp.emplace_back(score, cv::Vec2i(delta_x, delta_y));
        cnt++;
        std::cout << "Tried " << cnt << " out of " << tot_cnt << std::endl;
        mutex_score_disp.unlock();
    };

#pragma omp parallel for
    for (int delta_x = -m_pert[0]; delta_x <= m_pert[0]; delta_x++) {
        for (int delta_y = -m_pert[1]; delta_y <= m_pert[1]; delta_y++) {
            calc_score(delta_x, delta_y);
        }
    }

    std::sort(score_disp.begin(), score_disp.end(),
              [](const auto &p1, const auto &p2) -> bool {
                  return p1.first < p2.first;
              });

    return score_disp.back().second;
}

} // namespace compho
