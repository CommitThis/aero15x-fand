#include "change_detector.hpp"


using aero15x::state;

aero15x::change_detector::change_detector()
    : m_state_idx{0}
{}

auto aero15x::change_detector::change_needed(int temperature) -> bool
{
    auto change = false;
    auto current_state = states[m_state_idx];
    if (temperature >= current_state.up && m_state_idx != 0) {
        --m_state_idx;
        change = true;
    }
    else if (temperature < current_state.down && m_state_idx <
        states.size() - 1) {
        ++m_state_idx;
        change = true;
    }
    return change;
}

auto aero15x::change_detector::get_current_state() -> state
{
    return states[m_state_idx];
}


