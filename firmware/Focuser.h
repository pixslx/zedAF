#pragma once

#include <TMC2130Stepper.h>
#include <AccelStepper.h>
#include <Adafruit_BME280.h>

class Focuser : public AccelStepper {
  public:
    Focuser();

    void setup();
    void setTargetPosition(long targetPosition);
    long getTargetPosition();
    int getCurrentSpeedDivider();
    void setCurrentSpeedDivider(int currentSpeedDivider);
    bool isHalfStep();
    void setHalfStep(bool halfStep);
    void moveToTargetPosition();
    void readTemperature();
    float getTemperature();
    bool isTempCompensation();
    void setTempCompensation(bool tempCompensation);
    float getTempCoefficient();
    void setTempCoefficient(float tempCoefficient);
    void setTempCalibration(float tempCalibration);

  private:
    static Focuser* instance;
    
    TMC2130Stepper stepperDriver;
    Adafruit_BME280 tempSensor;
    
    HardwareTimer timer1;
    HardwareTimer timer2;
    
    float currentTemperature;
    int currentSpeedDivider;
    long targetPosition;
    float targetPositionTemperature;
    bool halfStep;
    bool tempCompensation;
    float tempCoefficient;
    float tempCalibration;
  
    void setupPins();
    void setupTimers();
    void setupStepper();
    void setupSensor();
    void setupVariables();
    
    static void handleMovementTimer();
    static void handleTempCompensationTimer();
    
    static void moveInward();
    static void moveOutward();
    static void step();
};
