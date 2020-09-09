#include "gtest/gtest.h"
#include "weatherParser.h"

using namespace Weather;

static const std::string firstDataLine =
    "   1  88    59    74          53.8       0.00 F       280  9.6 270  17  1.6  93 23 1004.5";
static const std::string secondDataLine =
    "   2  79    63    71          46.5       0.00         330  8.7 340  23  3.3  70 28 1004.5";

TEST(WeatherDataRegexMatch, GetDataFromMatchResult)
{
    WeatherParser parser;
    const std::string testStr = "1122";
    const std::regex regexStr(R"((\d+))");
    std::smatch matchResult;
    std::regex_match(testStr, matchResult, regexStr);
    auto result = parser.getDataFromMatchGroup(matchResult[0]);

    EXPECT_EQ(result, 1122);
}

TEST(WeatherDataRegexMatch, GetWeatherDataFromString)
{
    WeatherParser parser;
    auto result = parser.getWeatherDataFromLine(firstDataLine);
    EXPECT_TRUE(result.has_value());

    auto [day, min, max] = result.value();
    EXPECT_EQ(day, 1);
    EXPECT_EQ(min, 88);
    EXPECT_EQ(max, 59);
}

class WeatherParserFileOperation : public ::testing::Test {
public:
    WeatherParserFileOperation() : parser_(DATA_FILE) {}

    WeatherParser parser_;
};

TEST_F(WeatherParserFileOperation, ReadFile)
{
    EXPECT_TRUE(parser_.isFileOpen());
}

TEST_F(WeatherParserFileOperation, getSmallestDay){
    auto result = parser_.getSmallestTempSpreadDay();
    EXPECT_EQ(result, 14);
}

// ? open data file
// ? skip the first two line
// ? fetch weather data from one line
// ? record day number, calculate the temperature spread
// ? fetch all lines
// ? return day number with the smallest temperature spread