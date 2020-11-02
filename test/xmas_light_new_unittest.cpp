#include "gtest/gtest.h"
#include "xmas_light_new.h"

TEST(BasicOperations, GetLightByPosition)
{
    LightManager mgr;
    EXPECT_EQ(mgr.getLightState(0, 0), LightBrightness::CLOSE);
    EXPECT_EQ(mgr.getLightState(444, 444), LightBrightness::CLOSE);
    EXPECT_EQ(mgr.getLightState(999, 999), LightBrightness::CLOSE);
}

TEST(BasicOperations, SetLightByPosition)
{
    LightManager mgr;
    mgr.setLightState(0, 0, LightBrightness::OPEN);
    EXPECT_EQ(mgr.getLightState(0, 0), LightBrightness::OPEN);

    mgr.setLightState(0, 0, LightBrightness::CLOSE);
    EXPECT_EQ(mgr.getLightState(0, 0), LightBrightness::CLOSE);

    mgr.setLightState(999, 999, LightBrightness::OPEN);
    EXPECT_EQ(mgr.getLightState(999, 999), LightBrightness::OPEN);
}

TEST(BasicOperations, OpenLightWhichClosed)
{
    LightManager mgr;
    mgr.setLightState(111, 111, LightBrightness::CLOSE);

    mgr.openLight(111, 111);
    EXPECT_EQ(mgr.getLightState(111, 111), LightBrightness::OPEN);
}

TEST(BasicOperations, OpenLightWhichOpened)
{
    LightManager mgr;

    mgr.openLight(111, 111);
    mgr.openLight(111, 111);
    EXPECT_EQ(mgr.getLightState(111, 111), 2);
}

TEST(BasicOperations, OpenLightWhichReachLimit)
{
    LightManager mgr;

    mgr.setLightState(111,111,1000u);
    mgr.openLight(111,111);
    EXPECT_EQ(mgr.getLightState(111, 111), 1000);
}

TEST(BasicOperations, CloseLight)
{
    LightManager mgr;

    mgr.openLight(111, 111);
    EXPECT_EQ(mgr.getLightState(111, 111), LightBrightness::OPEN);

    mgr.closeLight(111, 111);
    EXPECT_EQ(mgr.getLightState(111, 111), LightBrightness::CLOSE);
}

TEST(BasicOperations, SwitchLightWhichClosed)
{
    LightManager mgr;

    mgr.switchLight(111, 111);
    EXPECT_EQ(mgr.getLightState(111, 111),2);
}

TEST(BasicOperations, SwitchLightWhichOpened)
{
    LightManager mgr;

    mgr.openLight(111, 111);
    mgr.switchLight(111, 111);
    EXPECT_EQ(mgr.getLightState(111, 111), 3);
}

TEST(BasicOperations, ThrowOutofLowerBound)
{
    LightManager mgr;

    EXPECT_THROW(mgr.getLightState(-1, 0), std::out_of_range);
    EXPECT_THROW(mgr.setLightState(-1, 0, LightBrightness::CLOSE), std::out_of_range);
    EXPECT_THROW(mgr.openLight(-1, 0), std::out_of_range);
    EXPECT_THROW(mgr.closeLight(-1, 0), std::out_of_range);
    EXPECT_THROW(mgr.switchLight(0, -1), std::out_of_range);
}

TEST(BasicOperations, ThrowOutofUpperBound)
{
    LightManager mgr;

    EXPECT_THROW(mgr.getLightState(1000, 0), std::out_of_range);
    EXPECT_THROW(mgr.setLightState(0, 1000, LightBrightness::CLOSE), std::out_of_range);
    EXPECT_THROW(mgr.openLight(0, 1000), std::out_of_range);
    EXPECT_THROW(mgr.closeLight(0, 1111), std::out_of_range);
    EXPECT_THROW(mgr.switchLight(0, 1000), std::out_of_range);
}

TEST(RangeOperations, GetRangeLightsState)
{
    LightManager mgr;

    mgr.openLight(0, 0);
    mgr.openLight(1, 1);
    mgr.openLight(1, 0);

    auto rangeEqual = [](const auto &rangeStates) -> bool {
        return rangeStates[0] == LightBrightness::OPEN &&
               rangeStates[1] == LightBrightness::CLOSE &&
               rangeStates[2] == LightBrightness::OPEN &&
               rangeStates[3] == LightBrightness::OPEN;
    };

    LightManager::RangeState rangeStates = mgr.getLightStateWithRange({0, 0}, {1, 1});
    EXPECT_PRED1(rangeEqual, rangeStates);
}

TEST(RangeOperations, SetRangeLightState)
{
    LightManager mgr;

    mgr.setLightStateWithRange({0, 0}, {1, 1}, LightBrightness::OPEN);

    auto rangeEqual = [](const auto &rangeStates) -> bool {
        return rangeStates[0] == LightBrightness::OPEN &&
               rangeStates[1] == LightBrightness::OPEN &&
               rangeStates[2] == LightBrightness::OPEN &&
               rangeStates[3] == LightBrightness::OPEN;
    };
    EXPECT_PRED1(rangeEqual, mgr.getLightStateWithRange({0, 0}, {1, 1}));
}

TEST(RangeOperations, OpenLightWithRange)
{
    LightManager mgr;

    mgr.openLightWithRange({1, 1}, {2, 2});

    auto rangeEqual = [](const auto &rangeStates) -> bool {
        return rangeStates[0] == LightBrightness::OPEN &&
               rangeStates[1] == LightBrightness::OPEN &&
               rangeStates[2] == LightBrightness::OPEN &&
               rangeStates[3] == LightBrightness::OPEN;
    };
    EXPECT_PRED1(rangeEqual, mgr.getLightStateWithRange({1, 1}, {2, 2}));
}

TEST(RangeOperations, CloseLightWithRange)
{
    LightManager mgr;

    mgr.openLight(0, 1);
    mgr.openLight(1, 1);

    mgr.closeLightWithRange({0, 0}, {1, 1});
    auto rangeEqual = [](const auto &rangeStates) -> bool {
        return rangeStates[0] == LightBrightness::CLOSE &&
               rangeStates[1] == LightBrightness::CLOSE &&
               rangeStates[2] == LightBrightness::CLOSE &&
               rangeStates[3] == LightBrightness::CLOSE;
    };
    EXPECT_PRED1(rangeEqual, mgr.getLightStateWithRange({0, 0}, {1, 1}));
}

TEST(RangeOperations, SwitchLightWithRange)
{
    LightManager mgr;

    mgr.openLightWithRange({0, 0}, {1, 0});

    mgr.switchLightWithRange({0, 0}, {1, 1});
    auto rangeEqual = [](const auto &rangeStates) -> bool {
        return rangeStates[0] == 3 &&
               rangeStates[1] == 2 &&
               rangeStates[2] == 3 &&
               rangeStates[3] == 2;
    };
    EXPECT_PRED1(rangeEqual, mgr.getLightStateWithRange({0, 0}, {1, 1}));
}

TEST(CountOperations, CountOpenLight)
{
    LightManager mgr;

    mgr.openLightWithRange({0, 0}, {1, 1});
    mgr.openLight(3, 3);

    auto count = mgr.countOpenLight();
    EXPECT_EQ(count, 5);
}