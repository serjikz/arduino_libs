#pragma once
#include "Sensor.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

static const size_t CS_PIN = 48;

Adafruit_BMP280 BMP280Adafruit(CS_PIN);

class BMP280Sensor : public Sensor {
    typedef Sensor Super;

public:

    BMP280Sensor(String id)
        :Super(id)
        , BAR_KOEF(0.00750062f)
        , _bmp(&BMP280Adafruit)
    {}

    String GetTemperature() {
        if (_bmp && _inited) {
            return String(_bmp->readTemperature());
        }
        else {
            return Super::GetSensorNotInitializedMsg();
        }
    }

    String GetPressure() {
        if (_bmp && _inited) {
            return String(BAR_KOEF * _bmp->readPressure());
        }
        else {
            return Super::GetSensorNotInitializedMsg();
        }
    }

    virtual void Init() {
        if (!_bmp) {
            return;
        }
        if (!_bmp->begin()) {
            _inited = false;
        }
        else {
            _inited = true;
            _bmp->setSampling(Adafruit_BMP280::MODE_NORMAL, // ����� ������
                Adafruit_BMP280::SAMPLING_X16, // �������� ���. �����������
                Adafruit_BMP280::SAMPLING_X16, // �������� ���. ��������
                Adafruit_BMP280::FILTER_X16, // ������� ����������
                Adafruit_BMP280::STANDBY_MS_4000); // ������ ����������, ����
        }
    }

private:
    const float BAR_KOEF;
    Adafruit_BMP280* _bmp;
};

namespace Sensors {
    BMP280Sensor BMP280("BMP");
}
