#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <experimental/optional>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template<typename T>
using optional = std::experimental::optional<T>;

optional<float> Sqrt(float x)
{
    using std::experimental::nullopt;
    return x >= 0 ? optional<float>{std::sqrt(x)} : nullopt;
}

TEST(Sqrt, gives_some_value_for_positive_input)
{
    auto const val = Sqrt(6.);

    EXPECT_TRUE(!!val);
}

TEST(Sqrt, gives_none_value_for_negative_input)
{
    auto const val = Sqrt(-17.);

    EXPECT_FALSE(!!val);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int _main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
