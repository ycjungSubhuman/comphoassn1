#pragma once

#include "pyramid.hpp"
#include <cmath>
#include <stdexcept>

namespace compho {
template <typename T>
T Pyramid::scale_down_value(size_t delta_level, T value) const {
    T result = value;
    for (size_t i = 0; i < delta_level; i++) {
        result /= typename T::value_type(m_factor_scale);
    }
    return result;
}

template <typename T>
T Pyramid::scale_up_value(size_t delta_level, T value) const {
    T result = value;
    for (size_t i = 0; i < delta_level; i++) {
        result *= typename T::value_type(m_factor_scale);
    }
    return result;
}
} // namespace compho
