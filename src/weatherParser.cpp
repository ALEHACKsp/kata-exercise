#include "weatherParser.h"

using namespace Weather;

int WeatherParser::getDataFromMatchGroup(const std::ssub_match& group) const
{
    return std::stoi(group.str());
}

WeatherDataOfDay WeatherParser::getWeatherDataFromLine(const std::string& lineContent) const
{
    std::smatch matchResult;
    if (std::regex_match(lineContent, matchResult, weatherDataRegex)) {
        return _WeatherDataOfDay(
            getDataFromMatchGroup(matchResult[1]), getDataFromMatchGroup(matchResult[2]),
            getDataFromMatchGroup(matchResult[3]));
    }
    return {};
}

int WeatherParser::getSmallestTempSpreadDay()
{
    if (!isFileOpen()) {
        return -1;
    }

    skipFirstTwoLines();
    processDataLines();

    return minDay;
}

void WeatherParser::skipFirstTwoLines()
{
    std::string _;
    std::getline(fileHandle_, _);
    std::getline(fileHandle_, _);
}

void WeatherParser::processDataLines()
{
    std::string line;
    while (std::getline(fileHandle_, line)) {
        auto weatherData = getWeatherDataFromLine(line);
        if (weatherData.has_value()) {
            auto spread = getTemperatureSpread(weatherData);
            if (spread < minSpread) {
                minDay = (*weatherData).dayNumber;
                minSpread = spread;
            }
        }
    }
}

int WeatherParser::getTemperatureSpread(const WeatherDataOfDay& data) const
{
    return (*data).minTemperature - (*data).maxTemperature;
}