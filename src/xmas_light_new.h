#include <vector>
#include <array>

constexpr uint LIGHT_NUM = 1000;

constexpr uint MAX_BRIGHTNESS = 1000;
using Brightness = uint;

struct LightBrightness
{
    LightBrightness(){};
    LightBrightness(uint x) : value(x){};

    operator uint() const
    {
        return value;
    }

    const static uint CLOSE;
    const static uint OPEN;

    const static int OPEN_DELTA;
    const static int CLOSE_DELTA;
    const static int SWITCH_DELTA;
    uint value{0};
};

const uint LightBrightness::CLOSE = 0;
const uint LightBrightness::OPEN = 1;

const int LightBrightness::OPEN_DELTA = 1;
const int LightBrightness::CLOSE_DELTA = -1;
const int LightBrightness::SWITCH_DELTA = 2;

class LightManager
{
public:
    struct Position
    {
        uint x;
        uint y;
    };

    struct Range
    {
        Position start;
        Position end;
    };

    using LightGrid = std::array<std::array<LightBrightness, LIGHT_NUM>, LIGHT_NUM>;
    using RangeState = std::vector<LightBrightness>;

    LightManager() : lightMatrix_() {}

    LightBrightness getLightState(uint x, uint y) const
    {
        validateInput(x, y);
        return lightMatrix_[x][y];
    }

    void setLightState(uint x, uint y, LightBrightness state)
    {
        validateInput(x, y);
        lightMatrix_[x][y] = state;
    }

    void modifyLightState(uint x, uint y, int delta)
    {
        auto newBrightness = calcValidBrightness(getLightState(x, y), delta);
        setLightState(x, y, newBrightness);
    }

    void openLight(uint x, uint y)
    {
        validateInput(x, y);
        modifyLightState(x, y, LightBrightness::OPEN_DELTA);
    }

    void closeLight(uint x, uint y)
    {
        validateInput(x, y);
        modifyLightState(x, y, LightBrightness::CLOSE_DELTA);
    }

    void switchLight(uint x, uint y)
    {
        validateInput(x, y);
        modifyLightState(x,y,LightBrightness::SWITCH_DELTA);
    }

    RangeState getLightStateWithRange(const Range &range)
    {
        return getLightStateWithRange(range.start, range.end);
    }
    RangeState getLightStateWithRange(const Position &start, const Position &end)
    {
        validateInput(start);
        validateInput(end);

        RangeState states;
        states.reserve(getRangeSize(start, end));

        iterateRange(start, end, [&states, this](uint x, uint y) {
            states.push_back(getLightState(x, y));
        });

        return states;
    }

    void setLightStateWithRange(const Range &range, LightBrightness state)
    {
        setLightStateWithRange(range.start, range.end, state);
    }
    void setLightStateWithRange(const Position &start, const Position &end, LightBrightness state)
    {
        validateInput(start);
        validateInput(end);

        iterateRange(start, end, [state, this](uint x, uint y) {
            setLightState(x, y, state);
        });
    }

    void switchLightWithRange(const Position &start, const Position &end)
    {
        validateInput(start);
        validateInput(end);

        iterateRange(start, end, [this](uint x, uint y) {
            switchLight(x, y);
        });
    }

    void openLightWithRange(const Position &start, const Position &end)
    {
        validateInput(start);
        validateInput(end);

        iterateRange(start, end, [this](uint x, uint y) {
            openLight(x, y);
        });
    }

    void closeLightWithRange(const Position &start, const Position &end)
    {
        validateInput(start);
        validateInput(end);

        iterateRange(start, end, [this](uint x, uint y) {
            closeLight(x, y);
        });
    }

    uint countOpenLight()
    {
        uint count = 0;
        iterateRange({0, 0}, {LIGHT_NUM - 1, LIGHT_NUM - 1}, [&count, this](uint x, uint y) {
            if (isOpen({x, y}))
            {
                count++;
            }
        });

        return count;
    }

    uint countBrightness(){
        uint count = 0;
        iterateRange({0,0}, {LIGHT_NUM-1, LIGHT_NUM-1},[&count,this](uint x, uint y){
            count += getLightState(x,y);
       });

       return count;
    }

    template <typename Lambda>
    void iterateRange(const Position &start, const Position &end, Lambda func)
    {
        for (auto rowIndex = start.x; rowIndex <= end.x; rowIndex++)
        {
            for (auto colIndex = start.y; colIndex <= end.y; colIndex++)
            {
                func(rowIndex, colIndex);
            }
        }
    }

    bool isOpen(const Position &p) const
    {
        return getLightState(p.x, p.y) >= LightBrightness::OPEN;
    }

private:
    LightBrightness calcValidBrightness(LightBrightness oldBrightness, int delta) const
    {
        auto newBrightness = static_cast<int>(oldBrightness) + delta;
        if (newBrightness < 0)
        {
            return 0;
        }
        else if (newBrightness > 1000)
        {
            return 1000;
        }

        return newBrightness;
    }

    size_t getRangeSize(const Position &start, const Position &end) const noexcept
    {
        return (end.x - start.x + 1) * (end.y - start.y + 1);
    }

    bool checkOutOfRange(uint x, uint y) const
    {
        return x >= LIGHT_NUM || y >= LIGHT_NUM;
    }

    void validateInput(int x, int y) const
    {
        if (checkOutOfRange(x, y))
        {
            throw std::out_of_range("light position is invalid");
        }
    }

    void validateInput(const Position &x) const
    {
        validateInput(x.x, x.y);
    }

private:
    LightGrid lightMatrix_;
};