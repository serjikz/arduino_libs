#pragma once
#include <WString.h>

class Sensor {
	String _id;

public:
	Sensor(String id);
	virtual void Init() = 0;
	bool IsInited() const { return _isInited; }

protected:
	const String& GetID() const;
	String GetSensorNotInitializedMsg() const;

	void InitLog(const char* msg, bool isOk);

protected:
	bool _isInited = false;

};