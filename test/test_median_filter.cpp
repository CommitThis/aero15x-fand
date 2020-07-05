#include <aero15x/aero15x.hpp>
#include <aero15x/median_filter.hpp>

#include <gtest/gtest.h>


TEST(Aero15xSuite, CanConstructMedianFilter)
{
    auto median = aero15x::median_filter<3>{{}};
}


TEST(Aero15xSuite, MedianFilterIgnoresTransientStage1)
{
    auto median = aero15x::median_filter<3>{{100, 100, 100}};
    auto expected_filtered_result = 100;
    {
        auto result = median.filter(9999);
        EXPECT_EQ(expected_filtered_result, result);
    }
    {
        auto result = median.filter(100);
        EXPECT_EQ(expected_filtered_result, result);
    }
    {
        auto result = median.filter(100);
        EXPECT_EQ(expected_filtered_result, result);
    }
}

TEST(Aero15xSuite, MedianFilterIgnoresTransientStage2)
{
    auto median = aero15x::median_filter<3>{{100, 100, 100}};
    auto expected_filtered_result = 100;
    {
        auto result = median.filter(100);
        EXPECT_EQ(expected_filtered_result, result);
    }
    {
        auto result = median.filter(9999);
        EXPECT_EQ(expected_filtered_result, result);
    }
    {
        auto result = median.filter(100);
        EXPECT_EQ(expected_filtered_result, result);
    }
}


TEST(Aero15xSuite, MedianFilterIgnoresTransientStage3)
{
    auto median = aero15x::median_filter<3>{{100, 100, 100}};
    auto expected_filtered_result = 100;
    {
        auto result = median.filter(100);
        EXPECT_EQ(expected_filtered_result, result);
    }
    {
        auto result = median.filter(100);
        EXPECT_EQ(expected_filtered_result, result);
    }
    {
        auto result = median.filter(9999);
        EXPECT_EQ(expected_filtered_result, result);
    }
}
