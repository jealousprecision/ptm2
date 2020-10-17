#pragma once

enum class State : uint8_t
{
    Blinking = 0,
    On = 1,
    Off = 2
};

constexpr uint8_t STATE_SIZE = 3u;

inline State nextState(State state)
{
    return static_cast<State>( (static_cast<uint8_t>(state) + 1) % STATE_SIZE);
}
