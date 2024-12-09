#pragma once
#include <Arduino.h>

class Button
{
private:
    unsigned long button_last_change;
    unsigned long debounce_time;

    short button_pin;
    bool button_changed;
    bool button_base_value;
    bool button_last_state;
    bool current_stable_state;
    bool pullup;

    Button(short pin = -1, unsigned long time = 50, bool base_value = HIGH)
        : button_pin(pin),
          debounce_time(time),
          button_base_value(base_value),
          button_last_state(base_value),
          current_stable_state(base_value) {}

public:
    void setup(short button_pin, unsigned long debounce_time, bool button_base_value)
    {
        pullup ? pinMode(button_pin, INPUT_PULLUP) : pinMode(button_pin, INPUT);
        this->button_pin = button_pin;
        this->debounce_time = debounce_time;
        this->button_base_value = button_base_value;
        this->button_last_state = button_base_value;
    }

    void begin(bool pullup = true)
    {
        pinMode(button_pin, pullup ? INPUT_PULLUP : INPUT);
        this->pullup = pullup;
    }

    bool wasPressed()
    {
        bool current_state = digitalRead(this->button_pin);
        unsigned long now = millis();

        //read inputs
        if (current_state != this->button_last_state)
        {
            now = this->button_last_change;
            this->button_changed = true;
        }

        // debounce function
        if (this->button_changed && (now - this->button_last_change >= this->debounce_time))
        {
            this->button_changed = false;
            this->button_last_state = current_state;
        }

        // check stable state
        if (current_state != current_stable_state)
        {
            current_stable_state = current_state;
            return (current_state != button_base_value);
        }
    }

    bool isPressed()
    {
        return digitalRead(button_pin) != button_base_value;
    }
};
class Pause
{
private:
    unsigned long milliSec = 0;

public:
    bool wait(unsigned long milliSec)
    {
        if (millis() - this->milliSec >= milliSec)
        {
            this->milliSec = millis();
            return true;
        }
    }
};
