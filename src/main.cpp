#include <Arduino.h>
#include "common/Timer.h"
#include "quectel/QuectelGSMManager.h"
#include "GSMController.h"
#include "GPIOController.h"

GSMController gsmController;
GPIOController gpioController;

void setup() {
    Serial.begin(115200);

    unsigned long serialStart = millis();
    while (!Serial) {
        if (((millis() - serialStart) > (5000))) {
            break;
        }
    }

    Serial.println("Setup");
    gpioController.Init();

    // TODO: Replace by your sim pin
    gsmController.Start("0000");
}

//
// Main UI Loop
//
void loop() {
    Timer::Loop();
    gsmController.Loop();

    yield();
}