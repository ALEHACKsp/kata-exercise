#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>

namespace Weather {

static const std::regex weatherDataRegex(R"(^\s*(\d+)\s*(\d+)\s*(\d+).*)");

static const std::string DATA_FILE = "/home/legend/weather.dat";

struct _WeatherDataOfDay {
    _WeatherDataOfDay(int day, int min, int max) : dayNumber(day), minTemperature(min), maxTemperature(max) {}
    int dayNumber;
    int minTemperature;
    int maxTemperature;
};
using WeatherDataOfDay = std::optional<_WeatherDataOfDay>;

class WeatherParser {
public:
    WeatherParser() = default;
    explicit WeatherParser(const std::string& filename) : fileHandle_(filename, std::ios_base::in) {}

    bool isFileOpen()
    {
        return fileHandle_.is_open();
    }

    int getSmallestTempSpreadDay();

    WeatherDataOfDay getWeatherDataFromLine(const std::string& lineContent) const;

    int getDataFromMatchGroup(const std::ssub_match& group) const;

private:
    int getTemperatureSpread(const WeatherDataOfDay& data) const;

    void skipFirstTwoLines();

    void processDataLines();

    int minSpread = std::numeric_limits<int>::max();
    int minDay = -1;
    std::ifstream fileHandle_;
};

}  // namespace Weather