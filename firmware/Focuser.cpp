#include "Focuser.h"
#include "constants.h"

Focuser* Focuser::instance = 0;

Focuser::Focuser() :
  AccelStepper(moveInward, moveOutward),
  stepperDriver(SD_EN_PIN, SD_DIR_PIN, SD_STEP_PIN, SD_CS_PIN),
  tempSensor(BMP_CS_PIN),
  timer1(1),
  timer2(2)
{
  instance = this;
}

void Focuser::setup() {
  setupPins();
  setupStepper();
  setupSensor();
  setupVariables();
  setupTimers();
}

void Focuser::setupPins() {
  pinMode(SD_EN_PIN, OUTPUT);
  pinMode(SD_DIR_PIN, OUTPUT);
  pinMode(SD_STEP_PIN, OUTPUT);
  pinMode(SD_CS_PIN, OUTPUT);
  pinMode(BMP_CS_PIN, OUTPUT);
}

void Focuser::setupTimers() {
  timer1.pause();
  timer1.setPeriod(1000000 / MAX_SPEED);
  timer1.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
  timer1.setCompare(TIMER_CH1, 1);
  timer1.attachInterrupt(TIMER_CH1, handleMovementTimer);
  timer1.refresh();
  timer1.resume();
  
  timer2.pause();
  timer2.setPeriod(50000000);
  timer2.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
  timer2.setCompare(TIMER_CH1, 1);
  timer2.attachInterrupt(TIMER_CH1, handleTempCompensationTimer);
  timer2.refresh();
  timer2.resume();
}

void Focuser::setupStepper() {
  setAcceleration(ACCELERATION);
  setMaxSpeed(MAX_SPEED / INITIAL_SPEED_DIVIDER);
  
  stepperDriver.begin();
  stepperDriver.setCurrent(300, 0.11, 0.5);
  stepperDriver.stealthChop(1);
  if(INITIAL_HALF_STEP) stepperDriver.microsteps(FULL_STEP_MICROSTEPS * 2);
  else stepperDriver.microsteps(FULL_STEP_MICROSTEPS);
  stepperDriver.interpolate(1);
  stepperDriver.double_edge_step(1);
  
  digitalWrite(SD_EN_PIN, LOW);
}

void Focuser::setupSensor() {
  tempSensor.begin();
  tempSensor.setSampling(Adafruit_BME280::MODE_NORMAL, 
                  Adafruit_BME280::SAMPLING_X1,
                  Adafruit_BME280::SAMPLING_NONE,
                  Adafruit_BME280::SAMPLING_NONE,
                  Adafruit_BME280::FILTER_X16,
                  Adafruit_BME280::STANDBY_MS_1000);
}

void Focuser::setupVariables() {
  currentSpeedDivider = INITIAL_SPEED_DIVIDER;
  halfStep = INITIAL_HALF_STEP;
  tempCompensation = INITIAL_TEMP_COMPENSATION;
  targetPosition = currentPosition();
}

void Focuser::handleMovementTimer() {
  instance->run();
}

void Focuser::handleTempCompensationTimer() {
  if(instance->tempCompensation) {
    int targetPositionDiff = (instance->currentTemperature - instance->targetPositionTemperature) * (instance->tempCoefficient / instance->tempCalibration);
    instance->moveTo(instance->targetPosition + targetPositionDiff);
  }
}

void Focuser::moveInward() {
  instance->stepperDriver.shaft_dir(INWARD_DIRECTION);
  instance->step();
}

void Focuser::moveOutward() {
  instance->stepperDriver.shaft_dir(OUTWARD_DIRECTION);
  instance->step();
}

void Focuser::step() {
  digitalWrite(SD_STEP_PIN, !digitalRead(SD_STEP_PIN));
}

void Focuser::setTargetPosition(long targetPosition) {
  this->targetPosition = targetPosition;
  targetPositionTemperature = currentTemperature;
}

long Focuser::getTargetPosition() {
  return targetPosition;
}

int Focuser::getCurrentSpeedDivider() {
  return currentSpeedDivider;
}

void Focuser::setCurrentSpeedDivider(int currentSpeedDivider) {
  this->currentSpeedDivider = currentSpeedDivider;
  setMaxSpeed(MAX_SPEED / currentSpeedDivider);  
}

bool Focuser::isHalfStep() {
  return halfStep;
}

void Focuser::setHalfStep(bool halfStep) {
  this->halfStep = halfStep;
  if(halfStep) stepperDriver.microsteps(FULL_STEP_MICROSTEPS * 2);
  else stepperDriver.microsteps(FULL_STEP_MICROSTEPS);
}

void Focuser::moveToTargetPosition() {
  moveTo(targetPosition);
}

void Focuser::readTemperature() {
  currentTemperature = tempSensor.readTemperature(); 
}

float Focuser::getTemperature() {
  return currentTemperature;
}

bool Focuser::isTempCompensation() {
  return tempCompensation;
}

void Focuser::setTempCompensation(bool tempCompensation) {
  this->tempCompensation = tempCompensation;
}

float Focuser::getTempCoefficient() {
  return tempCoefficient / tempCalibration;
}

void Focuser::setTempCoefficient(float tempCoefficient) {
  this->tempCoefficient = tempCoefficient;
}

void Focuser::setTempCalibration(float tempCalibration) {
  this->tempCalibration = tempCalibration;
}
