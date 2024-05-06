#pragma once
#include "settings.h"
#include <Arduino.h>
#include "common/Timer.h"
#include "common/Util.h"

#define INVALID_PIN 0xFF
// Pin value read interval in micro seconds
#define PINS_REFRESH_DELAY 10000

#if defined(INPUT_INVERSION)
#define INPUT_HIGH LOW
#else
#define INPUT_HIGH HIGH
#endif

enum GPIO_IN {
    OPTION_1,
    OPTION_2,
    OPTION_3,
    OPTION_4,

    GPIO_IN_TOTAL
};

enum GPIO_OUT {
    RELAY_1,
    RELAY_2,
    RELAY_3,
    RELAY_4,

    GPIO_OUT_TOTAL
};

class GPIOController: public ITimerCallback
{
private:
    bool isInitialized = false;
    uint8_t registerState = 0;
    Timer updateTimer;

    uint8_t prevDebugState;
    void PrintPinStates(uint8_t currentState);

    uint8_t RealInPin(GPIO_IN pin);
    uint8_t RealOutPin(GPIO_OUT pin);

    //bool lockOpen = false;

    void ReadValues();
    /* data */
public:
    GPIOController(/* args */);
    ~GPIOController();

    void Init();
    void OnTimerComplete(Timer * timer) override;

    void SetPinVal(GPIO_OUT pin, uint8_t val);
    uint8_t GetPinVal(GPIO_IN pin);
    uint8_t GetOutPinVal(GPIO_OUT pin);

    uint8_t GetRegisterState();
};
