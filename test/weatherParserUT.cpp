#include "gtest/gtest.h"

#include <algorithm>
#include <fstream>
#include <regex>
#include <tuple>
#include <optional>

struct _WeatherDataOfDay {
    _WeatherDataOfDay(int day, int min, int max) : dayNumber(day), minTemperature(min), maxTemperature(max) {}
    int dayNumber;
    int minTemperature;
    int maxTemperature;
};
using WeatherDataOfDay = std::optional<_WeatherDataOfDay>;

static const std::regex weatherDataRegex(R"(^\s*(\d+)\s*(\d+)\s*(\d+).*)");

class WeatherParser {
public:
    WeatherParser() = default;
    explicit WeatherParser(const std::string& filename) : fileHandle_(filename, std::ios_base::in) {}

    bool isFileOpen()
    {
        return fileHandle_.is_open();
    }

    int getDataFromMatchGroup(const std::ssub_match& group)
    {
        return std::stoi(group.str());
    }

    WeatherDataOfDay getWeatherDataFromLine(const std::string& lineContent)
    {
        std::smatch matchResult;
        if (std::regex_match(lineContent, matchResult, weatherDataRegex)) {
            return _WeatherDataOfDay(
                getDataFromMatchGroup(matchResult[1]), getDataFromMatchGroup(matchResult[2]),
                getDataFromMatchGroup(matchResult[3]));
        }
        return {};
    }

private:
    std::ifstream fileHandle_;
};

static const std::string DATA_FILE = "/home/legend/weather.dat";

TEST(WeatherParserTest, ReadFile)
{
    WeatherParser parser(DATA_FILE);
    EXPECT_TRUE(parser.isFileOpen());
}

TEST(WeatherParserTest, GetDataFromMatchResult)
{
    WeatherParser parser;
    const std::string testStr = "1122";
    const std::regex regexStr(R"((\d+))");
    std::smatch matchResult;
    std::regex_match(testStr, matchResult, regexStr);
    auto result = parser.getDataFromMatchGroup(matchResult[0]);

    EXPECT_EQ(result, 1122);
}

TEST(WeatherParserTest, GetWeatherDataFromString)
{
    const std::string test =
        "   1  88    59    74          53.8       0.00 F       280  9.6 270  17  1.6  93 23 1004.5";
    WeatherParser parser;
    auto result = parser.getWeatherDataFromLine(test);
    EXPECT_TRUE(result.has_value());

    auto [day, min, max] = result.value();
    EXPECT_EQ(day, 1);
    EXPECT_EQ(min, 88);
    EXPECT_EQ(max, 59);
}
// ? open data file
// ? skip the first two line
// ? fetch weather data from one line
// ? record day number, calculate the temperature spread
// ? fetch all lines
// ? return day number with the smallest temperature spread