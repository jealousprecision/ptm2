#pragma once

class Button
{
public:
    constexpr static unsigned long STATE_CHANGE_THRESHOLD = 30;  // in ms

    void updateState(bool currentState, unsigned long currentTime)
    {
        if (currentState != lastKnownState_)
        {
            lastKnownState_ = currentState;
            lastKnownStateChangeTime_ = currentTime;
        }

        // if state_ is not currentState
        // and currentState was the same at minimum STATE_CHANGE_THRESHOLD time
        // change state of a button
        if (currentState != state_
            && currentTime - lastKnownStateChangeTime_ > STATE_CHANGE_THRESHOLD)
        {
            state_ = currentState;

            // if current state was off and is changed to on,
            // then toogle pressedEvent
            if (state_ == false)
            {
                pressedEvent_ = true;
            }
        }
    }

    bool popWasPressed()
    {
        if (pressedEvent_)
        {
            pressedEvent_ = false;
            return true;
        }

        return false;
    }


private:
    bool state_ = false;
    bool pressedEvent_ = false;

    bool lastKnownState_ = false;
    unsigned long lastKnownStateChangeTime_ = 0;  // in ms
};
