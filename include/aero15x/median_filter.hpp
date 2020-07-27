#pragma once

#include <algorithm>
#include <array>
#include <utility>

namespace aero15x {

template <std::size_t Size>
class median_filter
{
public:

    median_filter(std::array<double, Size> initial)
        : m_buffer{initial}
    {}

    auto filter(double value) -> double
    {
        /* Rotate buffer */
        for (auto ii = Size; ii != 0; --ii) {
            m_buffer[ii] = m_buffer[ii-1];
        }
        m_buffer[0] = value;

        auto sorted = std::array<double, Size>{m_buffer};
        std::sort(sorted.begin(), sorted.end());

        static constexpr auto half_point = Size / 2;
        if constexpr (Size % 2 == 0) {
            return (sorted[half_point] + sorted[half_point - 1]) / 2.0;
        }
        else {
            return sorted[half_point];
        }
    }

private:
    std::array<double, Size> m_buffer;
    
};


}