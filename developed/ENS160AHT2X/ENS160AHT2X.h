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
        _aht_humidity = _aht->getHumiditySensor();
        _aht_temp = _aht->getTemperatureSensor();

        _isInited = isEns160Inited && isAHTInited;
    }

    String GetTemperature() 
    {
        if (_aht_temp && _isInited) {
            sensors_event_t temp;
            _aht_temp->getEvent(&temp);
            return String("Temp=" ) + String(temp.temperature) + String(char(223)) + String("C");
        }
        else {
            return Super::GetSensorNotInitializedMsg();
        }
    }

    String GetHumidity() 
    {
        if (_aht_humidity && _isInited) {
            sensors_event_t humidity;
            _aht_humidity->getEvent(&humidity);
            return String("Humid=" ) + String(humidity.relative_humidity) + String("%");
        }
        else {
            return Super::GetSensorNotInitializedMsg();
        }
    }

private:
    Adafruit_AHTX0* _aht;    
    ScioSense_ENS160* _ens160;
    Adafruit_Sensor* _aht_humidity;
    Adafruit_Sensor* _aht_temp;
};
