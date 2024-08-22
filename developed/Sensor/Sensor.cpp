#include "Sensor.h"
#include <Arduino.h>

Sensor::Sensor(String id)
	:_id(id)
	, _isInited(false)
{
}

const String& Sensor::GetID() const
{
	return _id;
}

String Sensor::GetSensorNotInitializedMsg() const
{
	return String("Sensor " + _id + " not initialized");
}

void Sensor::InitLog(const char* msg, bool isOk)
{	
	if (isOk) {
        Serial.print("[I] ");
	} else {
		Serial.print("[E] ");
	}
	Serial.print(GetID());
	Serial.print("::Init: ");
	Serial.print(msg);
	if (isOk) {
		Serial.println(" done.");
	} else {
		Serial.println(" failed.");
	}     
}

