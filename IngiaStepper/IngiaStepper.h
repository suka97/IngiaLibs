#ifndef IngiaStepper_h
#define IngiaStepper_h

#include <stdint.h>
#include <Arduino.h>

class IngiaStepper
{
    public:
        typedef enum {
            BWD = -1,
            FWD = 1
        } motorDir_t;


    public:
        IngiaStepper(uint8_t pinStep, uint8_t pinDir) {
            _pinStep = pinStep;
            _pinDir = pinDir;
        }

        void begin();
        void move(int32_t relPos);
        void moveTo(int32_t absPos);
        void motorStopped() { _running = false; }
        boolean runToSpeed(float vel, motorDir_t dir);
        boolean isRunning() { return _running; }
        boolean run();
        void setPosition(int32_t pos) { _pos = pos; }
        int32_t getPos() { return _pos; }
        void setMaxSpeed(float vel);
        void setAcceleration(float accel);
        void setMinSpeed(float vel);


    private:
        uint8_t _pinStep, _pinDir;
        int32_t _pos, _nextPos, _accelStopsPos, _desaccelStartsPos;
        boolean _changingPulse, _running;
        float _currentVel, _minVel, _maxVel, _accel;
        int8_t _dir, _velInc;
        uint32_t 
            _microsWaitPulse, 
            _microsWaitAccel,
            _accelInterval,
            _lastMicrosVel, _lastMicrosAccel;

        uint32_t velToMicros(float vel) { return (uint32_t)(1000000L / vel); }
        void calculateNewAccelInterval(float minVel, float maxVel, float accel);
        void calculateAccelDesaccelPos();
};

#endif