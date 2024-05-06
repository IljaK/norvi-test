#include "GPIOController.h"

GPIOController::GPIOController(/* args */): updateTimer(this)
{
}

GPIOController::~GPIOController()
{
}

void GPIOController::Init()
{

    pinMode(INPUT_PIN_1, INPUT);
    pinMode(INPUT_PIN_2, INPUT);
    pinMode(INPUT_PIN_3, INPUT);
    pinMode(INPUT_PIN_4, INPUT);


    pinMode(OUTPUT_PIN_1, OUTPUT);
    digitalWrite(OUTPUT_PIN_1, LOW);

    pinMode(OUTPUT_PIN_2, OUTPUT);
    digitalWrite(OUTPUT_PIN_2, LOW);

    pinMode(OUTPUT_PIN_3, OUTPUT);
    digitalWrite(OUTPUT_PIN_3, LOW);

    pinMode(OUTPUT_PIN_4, OUTPUT);
    digitalWrite(OUTPUT_PIN_4, LOW);

    isInitialized = true;

    ReadValues();

    if (prevDebugState == 0) {
        PrintPinStates(prevDebugState);
    }

}


void GPIOController::OnTimerComplete(Timer * timer)
{
    if (&updateTimer == timer) {
        ReadValues();
    }
}

void GPIOController::ReadValues() {

    updateTimer.StartMicros(PINS_REFRESH_DELAY);
    uint8_t currentState = 0;
    for(uint8_t i = 0; i < GPIO_IN_TOTAL; i++) {
        currentState = setBitOfByte(currentState, GetPinVal((GPIO_IN)i) ,i);
    }
    if (prevDebugState != currentState) {
        PrintPinStates(currentState);
    }

}

void GPIOController::PrintPinStates(uint8_t currentState) 
{
    Serial.print("GPIO IN: ");
    for(uint8_t i = 0; i < GPIO_IN_TOTAL; i++) {
        if (i > 0) {
            Serial.print('-');
        }
        Serial.print((int)getBitFromByte(prevDebugState, i));
    }
    if (prevDebugState != currentState) {
        Serial.print("->");
        for(uint8_t i = 0; i < GPIO_IN_TOTAL; i++) {
            if (i > 0) {
                Serial.print('-');
            }
            Serial.print((int)getBitFromByte(currentState, i));
        }
    }
    prevDebugState = currentState;
    Serial.println();
}

uint8_t GPIOController::RealInPin(GPIO_IN pin)
{
    switch (pin)
    {
    case OPTION_1:
        return INPUT_PIN_1;
    case OPTION_2:
        return INPUT_PIN_2;
    case OPTION_3:
        return INPUT_PIN_3;
    case OPTION_4:
        return INPUT_PIN_4;

    case GPIO_IN_TOTAL:
        // Always skip!
        break;
    default:
        break;
    }
    return INVALID_PIN;
}

uint8_t GPIOController::RealOutPin(GPIO_OUT pin)
{
    switch (pin)
    {
    case RELAY_1:
        return OUTPUT_PIN_1;
    case RELAY_2:
        return OUTPUT_PIN_2;
    case RELAY_3:
        return OUTPUT_PIN_3;
    case RELAY_4:
        return OUTPUT_PIN_4;
    case GPIO_OUT_TOTAL:
        // Always skip!
        break;
    default:
        break;
    }
    return INVALID_PIN;
}

void GPIOController::SetPinVal(GPIO_OUT pin, uint8_t val)
{
    if (!isInitialized) return;
    uint8_t realPin = RealOutPin(pin);
    if (realPin == INVALID_PIN) return;

    registerState = setBitOfByte(registerState, val, pin);
    digitalWrite(realPin, val);

}

uint8_t GPIOController::GetPinVal(GPIO_IN pin)
{
    if (!isInitialized) return 0;
    uint8_t realPin = RealInPin(pin);

    if (realPin == INVALID_PIN) return 0;

    return digitalRead(realPin) == INPUT_HIGH;
}

uint8_t GPIOController::GetOutPinVal(GPIO_OUT pin)
{
    if (!isInitialized) return 0;
    return getBitFromByte(registerState, pin);
}

uint8_t GPIOController::GetRegisterState()
{
    return registerState;
}