#pragma once
#include <LiquidCrystal_I2C.h>
#include <EncButton.h>

LiquidCrystal_I2C LCDMain(0x27, 16, 2);
EncButton<EB_TICK, PIND5> btnMain;

namespace Display {
    static const int SENSORS_COUNT = 6;
}

namespace Display {
    class LCD1602Dislay {
    public:

        LCD1602Dislay(String id)
            : _id(id)
            , _inited(false)
            , _lcdMain(&LCDMain)
            , _btn(&btnMain)
        {}

        bool IsInited() const { return _inited; }

        void Init() {
            if (!_lcdMain) {
                return;
            }
            _lcdMain->init();
            _inited = true;
            _lcdMain->backlight();
            _lcdMain->setCursor(3, 0);
            _lcdMain->print("Loading...");
        }

        void ShowErrMsg() 
        {
            _lcdMain->setCursor(5, 0);
            _lcdMain->print("Error");
            _lcdMain->setCursor(1, 1);
            _lcdMain->print("Check wiring!");
        }

        void Clear() 
        {
            if (_inited) {
                _lcdMain->clear();
            }
        }

        void Print(size_t charPos, size_t rowNumber, const String str) {
            if (_inited) {
                _lcdMain->setCursor(charPos, rowNumber);
                _lcdMain->print(str);
            }
        }

        void Update() {
            _btn->tick();
            if (_btn->isClick()) {
                Clear();
                Serial.println("Tapped");
                _firstRowIdx += 2;
                _firstRowIdx %= SENSORS_COUNT;        
                Serial.print("FirstRow=");        
                Serial.println(_firstRowIdx);
            }
        }

        void UpdateValues(String vals[SENSORS_COUNT]) {
            for (int i = 0; i < SENSORS_COUNT; ++i) {
                _vals[i] = vals[i];
            }
        }

        void ShowStoragedData() {
            Print(0, 0, _vals[_firstRowIdx]);
            int secondLineData = (_firstRowIdx + 1) % SENSORS_COUNT;
            Print(0, 1, _vals[secondLineData]);
        }

    private:
        String _id;
        bool _inited;
        LiquidCrystal_I2C* _lcdMain;
        EncButton<EB_TICK, PIND5>* _btn;

        String _vals[SENSORS_COUNT];
        int _firstRowIdx = 0;
    };
}

namespace Display {
    LCD1602Dislay LCD1602("Display.LCD1602");
}
