#include "Macro.h"

void Macro::loop() {
    while (true) {
        this->first_person_active = false;
        this->third_person_active = false;

        if (first_person_key)
        {
            if (this->toggle)
            {
                if (GetAsyncKeyState(first_person_key) & 1)
                {
                    this->first_person_active = !this->first_person_active;
                }
            }
            else
            {
                if (GetAsyncKeyState(first_person_key))
                {
                    this->first_person_active = true;
                }
            }
        }
        if (third_person_key)
        {
            if (this->toggle)
            {
                if (GetAsyncKeyState(third_person_key))
                {
                    this->third_person_active = !this->third_person_active;
                }
            }
            else
            {
                if (GetAsyncKeyState(third_person_key))
                {
                    this->third_person_active = true;
                }
            }
        }


        if (this->first_person && this->first_person_active)
        {
            this->pressKey('i');
            std::this_thread::sleep_for(std::chrono::milliseconds(this->first_person_delay_ms));
            this->pressKey('o');

            if (this->sleep_twice)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(this->first_person_delay_ms));
            }
        }

        // Prevent both from being active at the same time
        else if (this->third_person && this->third_person_active)
        {
            this->pressKey('o');
            std::this_thread::sleep_for(std::chrono::microseconds(this->third_person_delay_ms));
            this->pressKey('i');

            if (this->sleep_twice)
            {
                std::this_thread::sleep_for(std::chrono::microseconds(this->third_person_delay_ms));
            }
        }
    }
}
