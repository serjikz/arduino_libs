#pragma once
#include "Sensor.h"
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <ScioSense_ENS160.h>

namespace ENS160AHT2X_INTERNAL {
    Adafruit_AHTX0 aht;
    ScioSense_ENS160 ens160(ENS160_I2CADDR_1);
}

class ENS160AHT2X : public Sensor {
    typedef Sensor Super;

public:

    ENS160AHT2X(String id)
        : Super(id)
        , _aht(&ENS160AHT2X_INTERNAL::aht)
        , _ens160(&ENS160AHT2X_INTERNAL::ens160)
    {}

    virtual void Init() {
        _ens160->begin();
        bool isEns160Inited = _ens160->available();
        InitLog("ENS160 init", isEns160Inited);
        bool isEnsStdModeSetted = _ens160->setMode(ENS160_OPMODE_STD);
        InitLog("ENS160 Standard mode init", isEnsStdModeSetted);

        bool isAHTInited = _aht->begin();
        InitLog("AHT init", isAHTInited);
    }

private:
    void InitLog(const char* msg, bool isOk)
    {
         if (isOk) {
            Serial.print("[I]");
        } else {
            Serial.print("[E]");
        }
        Serial.print(" ENS160AHT2X::Init: ");
        Serial.print(msg);
        if (isOk) {
            Serial.println(" done.");
        } else {
            Serial.println(" failed.");
        }     
    }

private:
    Adafruit_AHTX0* _aht;
    ScioSense_ENS160* _ens160;
};
