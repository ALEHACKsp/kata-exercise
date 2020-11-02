#include <vector>
#include <array>

constexpr uint LIGHT_NUM = 1000;
enum class LightState
{
    CLOSE = 0,
    OPEN
};

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

    using LightGrid = std::array<std::array<LightState, LIGHT_NUM>, LIGHT_NUM>;
    using RangeState = std::vector<LightState>;

    constexpr LightManager() : lightMatrix_({LightState::CLOSE}) {}

    LightState getLightState(uint x, uint y) const
    {
        validateInput(x, y);
        return lightMatrix_[x][y];
    }

    void setLightState(uint x, uint y, LightState state)
    {
        validateInput(x, y);
        lightMatrix_[x][y] = state;
    }

    void openLight(uint x, uint y)
    {
        validateInput(x, y);
        setLightState(x, y, LightState::OPEN);
    }

    void closeLight(uint x, uint y)
    {
        validateInput(x, y);
        setLightState(x, y, LightState::CLOSE);
    }

    void switchLight(uint x, uint y)
    {
        validateInput(x, y);

        auto oldState = getLightState(x, y);
        if (oldState == LightState::CLOSE)
        {
            openLight(x, y);
        }
        else
        {
            closeLight(x, y);
        }
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

    void setLightStateWithRange(const Range &range, LightState state)
    {
        setLightStateWithRange(range.start, range.end, state);
    }
    void setLightStateWithRange(const Position &start, const Position &end, LightState state)
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
        setLightStateWithRange(start, end, LightState::OPEN);
    }

    void closeLightWithRange(const Position &start, const Position &end)
    {
        setLightStateWithRange(start, end, LightState::CLOSE);
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
        return getLightState(p.x, p.y) == LightState::OPEN;
    }

private:
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