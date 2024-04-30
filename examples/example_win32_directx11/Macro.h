#pragma once
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>

class Macro
{
public:
    bool first_person, third_person = false;

    bool first_person_active = false;
    bool third_person_active = false;

    bool sleep_twice = false;

    int first_person_delay_ms = 8;
    int third_person_delay_ms = 12;

    short first_person_key = 113;
    short third_person_key = 6;

    bool toggle = false; // Toggle or Hold key

    void loop();

private:
    void pressKey(CHAR keyParam) {
        SHORT key;
        UINT mappedkey;
        INPUT input = { 0 };
        key = VkKeyScan(keyParam);
        mappedkey = MapVirtualKey(LOBYTE(key), 0);
        input.type = INPUT_KEYBOARD;
        input.ki.dwFlags = KEYEVENTF_SCANCODE;
        input.ki.wScan = mappedkey;
        SendInput(1, &input, sizeof(input));
        Sleep(10);
        input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(input));
    }

    void scrollUp()
    {
        INPUT input = { 0 };
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;
        input.mi.mouseData = 120;
        SendInput(1, &input, sizeof(input));
    }

    void scrollDown()
    {
        INPUT input = { 0 };
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;
        input.mi.mouseData = -120;
        SendInput(1, &input, sizeof(input));
    }
};

