#pragma once

class RGYDiodsInformer {

	enum State {
		None = 0,
		RedLight = 1,
		YellowLight,
		GreenLight,
		GreenBlink,
		RedBlink,
		Alarm
	};

public:

	RGYDiodsInformer(int redDiodPin, int yellowDiodPin, int greenDiodPin) 
		:_redPin(redDiodPin),
		_yellowPin(yellowDiodPin),
		_greenPin(greenDiodPin)
	{};

	void Start() {
		if (_redPin > 0 && _yellowPin > 0 && _greenPin > 0) 
		{
			pinMode(_redPin, OUTPUT);
    		pinMode(_yellowPin, OUTPUT);
    		pinMode(_greenPin, OUTPUT);
			digitalWrite(_yellowPin, LOW);
			digitalWrite(_greenPin, LOW);
			_inited = true;
			_startTime = millis();
			// Enable a red diod
			digitalWrite(_redPin, HIGH);
			_state = State::RedLight;
		}
		else
		{
			_inited = false;
		}
	}

	void Update() 
	{
		if (_inited) 
		{
			switch (_state) {
				case State::RedLight:
					if (millis() > COLOR_SWITCH_DT) {
						digitalWrite(_redPin, LOW);
						digitalWrite(_yellowPin, HIGH);
						_state = State::YellowLight;
					}
				break;
				case State::YellowLight:
					if (millis() > 2 * COLOR_SWITCH_DT) {
						digitalWrite(_yellowPin, LOW);
						digitalWrite(_greenPin, HIGH);
						_state = State::GreenLight;
					}
				break;
				case State::GreenLight:
					if (millis() > 3 * COLOR_SWITCH_DT) {
						digitalWrite(_greenPin, LOW);
						_state = State::None;
						_isStartEffectCompleted = true;
					}
				break;
				case State::GreenBlink:
					if (millis() - _blinkStartTime >= BLINK_DT)
					{
						digitalWrite(_greenPin, LOW);
						_state = State::None;
					}
				break;
				case State::RedBlink:
					if (millis() - _blinkStartTime >= BLINK_DT)
					{
						digitalWrite(_redPin, LOW);
						_state = State::None;
					}
				break;
				case State::Alarm:
					if (millis() - _alarmTime >= BLINK_DT / 2)
					{
						digitalWrite(_redPin, LOW);
						digitalWrite(_yellowPin, HIGH);
					}
					if (millis() - _alarmTime >= BLINK_DT)
					{
						digitalWrite(_redPin, HIGH);
						digitalWrite(_yellowPin, LOW);
						_alarmTime = millis();
					}
				break;
				default:
				break;
			}
		}
	}

	bool IsStartEffectCompleted() const
	{
		return _isStartEffectCompleted;
	}

	void BlinkGreenLight() 
	{
		if (_isStartEffectCompleted && _state != State::Alarm) 
		{
			_state = State::GreenBlink;
			_blinkStartTime = millis();
			digitalWrite(_greenPin, HIGH);
		}
	}

	void BlinkRedLight() 
	{
		if (_isStartEffectCompleted) 
		{
			_state = State::RedBlink;
			_blinkStartTime = millis();
			digitalWrite(_redPin, HIGH);
		}
	}

	void StartAlarm()
	{
		if (_isStartEffectCompleted) 
		{
			_state = State::Alarm;
			_alarmTime = millis();
			digitalWrite(_greenPin, LOW);
			digitalWrite(_redPin, HIGH);
			digitalWrite(_yellowPin, LOW);
		}
	}

	void TryStopAlarm()
	{
		if (_state == State::Alarm) {
			_state = State::GreenBlink;
			digitalWrite(_redPin, LOW);
			digitalWrite(_yellowPin, LOW);
		}
	}

protected:
	bool _inited = false;
	bool _isStartEffectCompleted= false;
	unsigned long _startTime = 0;
	unsigned long _blinkStartTime = 0;
	unsigned long _alarmTime = 0;
	const unsigned int COLOR_SWITCH_DT = 3000;
	const unsigned int BLINK_DT = 1000;
	int _redPin = 0;
	int _yellowPin = 0;
	int _greenPin = 0;
	State _state = State::None;
};
