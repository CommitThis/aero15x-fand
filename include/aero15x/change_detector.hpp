#pragma once

#include "aero15x.hpp"

#include <array>
#include <cstdint>

namespace aero15x {

struct state
{
    constexpr state(int up, int down, std::size_t speed)
        : up{up}
        , down{down}
        , speed{speed}
    {
    }
    int up;
    int down;
    std::size_t speed;
};

inline auto operator==(state const & lhs, state const & rhs)
{
    return lhs.up == rhs.up && lhs.down == rhs.down && lhs.speed == rhs.speed;
}


constexpr static auto state_max    = state{255, 71,   fan_speed_max};
constexpr static auto state_high   = state{80,  61,   195};
constexpr static auto state_medium = state{70,  51,   160};
constexpr static auto state_low    = state{60,  45,   125};
constexpr static auto state_min    = state{50,  -255, fan_speed_min};
constexpr static auto number_of_states = 5;

constexpr static std::array<state, number_of_states> states {
        state_max,
        state_high,
        state_medium,
        state_low,
        state_min
    };

class change_detector
{
public:
    change_detector();
    auto change_needed(int temperature) -> bool;
    auto get_current_state() -> state;
private:
    std::size_t m_state_idx = 0;
};

}