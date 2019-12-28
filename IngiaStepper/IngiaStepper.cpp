#include "IngiaStepper.h"

void IngiaStepper::begin() {
    pinMode(_pinStep, OUTPUT); digitalWrite(_pinStep, HIGH);
    pinMode(_pinDir, OUTPUT); digitalWrite(_pinDir, HIGH);

    //Serial.begin(9600);

    // defaults
    _minVel = 10; _maxVel = 1000; _accel = 500; 
    _changingPulse = false; _running = false;
    _pos = 0; _nextPos = 0;
}

void IngiaStepper::setMinSpeed(float vel) {
    _minVel = vel;
}

void IngiaStepper::setMaxSpeed(float vel) {
    _maxVel = vel;
}

void IngiaStepper::setAcceleration(float accel) {
    _accel = accel;
}

void IngiaStepper::move(int32_t relPos) {
    _nextPos = _pos + relPos;
    _dir = (relPos > 0) ? 1 : (-1);
}

void IngiaStepper::moveTo(int32_t absPos) {
    _nextPos = absPos;
    _dir = (absPos > _pos) ? 1 : (-1);
}

void IngiaStepper::calculateNewAccelInterval(float minVel, float maxVel, float accel) {
    float accelTime = (maxVel - minVel) / accel;
    _accelInterval = (uint32_t)( (minVel * accelTime) + (0.5 * accel * accelTime * accelTime) + 1 );
}

void IngiaStepper::calculateAccelDesaccelPos() {
    // si me da el largo para acelerar y desacelerar
    uint32_t delta = abs(_nextPos - _pos) / 2;
    if ( delta > _accelInterval ) {
        _accelStopsPos = _pos + _accelInterval * (uint32_t)(_dir);
        _desaccelStartsPos = _nextPos - _accelInterval * (uint32_t)(_dir);
    }
    // sino lo divido en 2
    else {
        _accelStopsPos = _pos + delta * (int32_t)(_dir);
        _desaccelStartsPos = _nextPos - delta * (int32_t)(_dir) + _dir;
    }
    /*
    Serial.print("_accelInterval     "); Serial.println(_accelInterval);
    Serial.print("_pos               "); Serial.println(_pos);
    Serial.print("_nextPos           "); Serial.println(_nextPos);
    Serial.print("_accelStopsPos     "); Serial.println(_accelStopsPos);
    Serial.print("_desaccelStartsPos "); Serial.println(_desaccelStartsPos);
    Serial.println("");
    */
}

boolean IngiaStepper::runToSpeed(float vel, motorDir_t dir) {
    if ( !_running ) {
        _velInc = 1;
        _currentVel = _minVel; _microsWaitPulse = velToMicros(_minVel);
        _microsWaitAccel = (uint32_t)(1000000L / _accel);
        digitalWrite(_pinDir, (dir == FWD)?HIGH:LOW);
        _running = true;
    }

    uint32_t _microsNow = micros();
    if ( (_microsNow - _lastMicrosVel) >= (_microsWaitPulse / 2) ) {
        if ( !_changingPulse ) {
            digitalWrite(_pinStep, HIGH);
            _pos += (int32_t)_dir;
            _changingPulse = true;
        }
        else {
            digitalWrite(_pinStep, LOW);
            _changingPulse = false;
        }
        _lastMicrosVel = _microsNow;
    }

    if ( (_velInc != 0) && ((_microsNow - _lastMicrosAccel) >= _microsWaitAccel) ) {
        if ( _currentVel < vel ) {
            _currentVel += _velInc;
            _microsWaitPulse = velToMicros(_currentVel);
        }
        else
            _velInc = 0;

        _lastMicrosAccel = _microsNow;
    }

    return ( _currentVel == vel );
}

boolean IngiaStepper::run() {
    if ( _pos == _nextPos ) {
        _running = false;
        digitalWrite(_pinStep, LOW);
        _changingPulse = false;
        return false;
    }

    if ( !_running ) {
        _velInc = 1;
        _currentVel = _minVel; _microsWaitPulse = velToMicros(_minVel);
        _microsWaitAccel = (uint32_t)(1000000L / _accel);
        calculateNewAccelInterval(_minVel, _maxVel, _accel);
        calculateAccelDesaccelPos();
        digitalWrite( _pinDir, (_dir > 0) ? HIGH : LOW );
        _running = true;
    }

    uint32_t _microsNow = micros();
    if ( (_microsNow - _lastMicrosVel) >= (_microsWaitPulse / 2) ) {
        if ( !_changingPulse ) {
            digitalWrite(_pinStep, HIGH);
            _pos += (int32_t)_dir;

            // change acceleration/desacceleration
            if ( _pos == _accelStopsPos )
                _velInc = 0;
            else if ( _pos == _desaccelStartsPos )
                _velInc = (-1);

            _changingPulse = true;
        }
        else {
            digitalWrite(_pinStep, LOW);
            _changingPulse = false;
        }
        _lastMicrosVel = _microsNow;
    }

    if ( (_velInc != 0) && ((_microsNow - _lastMicrosAccel) >= _microsWaitAccel) ) {
        // accelerating
        if ( _velInc > 0 ) {
            if ( _currentVel < _maxVel ) {
                _currentVel += _velInc;
                _microsWaitPulse = velToMicros(_currentVel);
            }
        }
        // desaccelerating
        else {
            if ( _currentVel > _minVel ) {
                _currentVel += _velInc;
                _microsWaitPulse = velToMicros(_currentVel);
            }
        }
        _lastMicrosAccel = _microsNow;
    }

    return true;
}