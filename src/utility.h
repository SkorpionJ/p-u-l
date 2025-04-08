#pragma once
#include <Arduino.h>

// Button *******************************************************************************************
class Button
{
    private:

    unsigned long button_last_change;
    

    short button_pin;
    unsigned long debounce_time;
    bool button_changed;
    bool button_base_value;
    bool button_last_state;
    bool current_stable_state;
    bool pullup;
    bool current_state = false;

    bool f_changed = false;

    public:

    Button(short pin = -1, unsigned long time = 50, bool base_value = HIGH, bool pullup = true)
        : button_pin(pin),
          debounce_time(time),
          button_base_value(base_value),
          button_last_state(base_value),
          current_stable_state(base_value),
          pullup(pullup)
    {
    }

    void begin(short button_pin, unsigned long debounce_time, bool button_base_value, bool pullup = true)
    {
        pullup ? pinMode(button_pin, INPUT_PULLUP) : pinMode(button_pin, INPUT);
        this->button_pin = button_pin;
        this->debounce_time = debounce_time;
        this->button_base_value = button_base_value;
        this->button_last_state = button_base_value;
        this->current_stable_state = button_base_value;

        pinMode(button_pin, pullup ? INPUT_PULLUP : INPUT);
        this->pullup = pullup;
    }

    void update()
    {
        this->current_state = digitalRead(button_pin);

        if (this->f_changed)
            this->f_changed = false;

        unsigned long now = millis();

        // Check for state change
        if (current_state != button_last_state)
        {
            button_last_state = current_state;
            button_last_change = now;
            button_changed = true;
        }

        // Debounce check
        if (button_changed && (now - button_last_change >= debounce_time))
        {
            button_changed = false;

            // Check if stable state is different
            if (current_state != current_stable_state)
            {
                current_stable_state = current_state;
                this->f_changed = true;
            }
        }
    }

    bool changed()
    {
        return this->f_changed;
    }

    bool rose()
    {
        return changed() && isPressed();
    }

    bool fell()
    {
        return changed() && !isPressed();
    }

    bool isPressed()
    {
        return this->current_stable_state != button_base_value;
    }
};
// Pause *******************************************************************************************
class Pause
{
private:
    unsigned long previousMillis = 0;
    unsigned long previousMicros = 0;

public:
    bool wait_ms(unsigned long time_ms)
    {
        unsigned long currentMillis = millis();

        if (currentMillis - previousMillis >= time_ms)
        {
            previousMillis = currentMillis;
            return true;
        }
        return false;
    }

    bool wait_us(unsigned long time_us)
    {
        unsigned long currentMicros = micros();

        if (currentMicros - previousMicros >= time_us)
        {
            previousMicros = currentMicros;
            return true;
        }
        return false;
    }
};

