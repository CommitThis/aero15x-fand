#include <aero15x/aero15x.hpp>
#include <aero15x/change_detector.hpp>

#include <gtest/gtest.h>


TEST(Aero15xSuite, CanConstructChangeDetector)
{
    auto cd = aero15x::change_detector{};
}

TEST(Aero15xSuite, SpeedDoesNotIncreasePastMax)
{
    auto cd = aero15x::change_detector{};
    cd.change_needed(500);
    auto current_speed = cd.get_current_state().speed;
    EXPECT_EQ(aero15x::fan_speed_max, current_speed);
}

TEST(Aero15xSuite, StartsAtMaxState)
{
    auto cd = aero15x::change_detector{};
    auto current_state = cd.get_current_state();
    auto max_state = aero15x::states[0];
    EXPECT_EQ(max_state, current_state);
}


TEST(Aero15xSuite, TraversesAllStatesBeforeReachingMin)
{
    auto cd = aero15x::change_detector{};

    auto current_state = cd.get_current_state();
    EXPECT_EQ(current_state, aero15x::states[0]);

    cd.change_needed(-100);    
    current_state = cd.get_current_state();
    EXPECT_EQ(current_state, aero15x::states[1]);
    
    cd.change_needed(-100);    
    current_state = cd.get_current_state();
    EXPECT_EQ(current_state, aero15x::states[2]);

    cd.change_needed(-100);    
    current_state = cd.get_current_state();
    EXPECT_EQ(current_state, aero15x::states[3]);

    cd.change_needed(-100);    
    current_state = cd.get_current_state();
    EXPECT_EQ(current_state, aero15x::states[4]);
    
    auto current_speed = cd.get_current_state().speed;
    EXPECT_EQ(aero15x::fan_speed_min, current_speed);
}

TEST(Aero15xSuite, TraversesAllStatesBeforeReachingMax)
{
    auto cd = aero15x::change_detector{};

    for (auto ii = 0ull; ii != aero15x::number_of_states; ++ii) {
        cd.change_needed(0);
    }

    auto current_state = cd.get_current_state();
    EXPECT_EQ(aero15x::fan_speed_min, current_state.speed);
    
    cd.change_needed(100);    
    current_state = cd.get_current_state();
    EXPECT_EQ(current_state, aero15x::states[3]);
    
    cd.change_needed(100);    
    current_state = cd.get_current_state();
    EXPECT_EQ(current_state, aero15x::states[2]);

    cd.change_needed(100);    
    current_state = cd.get_current_state();
    EXPECT_EQ(current_state, aero15x::states[1]);

    cd.change_needed(100);    
    current_state = cd.get_current_state();
    EXPECT_EQ(current_state, aero15x::states[0]);

}
