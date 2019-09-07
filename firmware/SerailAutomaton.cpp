#include "SerialAutomaton.h"
#include "Focuser.h"
#include "constants.h"

SerialAutomaton::SerialAutomaton(Focuser* focuser) : _focuser(focuser) {}

void SerialAutomaton::loop() {
  if(Serial.available() > 0) {
    *serialBufferP = Serial.read();
    if(*serialBufferP == '#') {
      *serialBufferP = 0;
      processCommand(serialBuffer);
      memset(serialBuffer, 0, sizeof(serialBuffer));
      serialBufferP = serialBuffer;
    } else if(serialBufferP != serialBuffer || *serialBufferP == ':') serialBufferP++;
  }
}

void SerialAutomaton::processCommand(char command[]) {
  char* commandP = command;
  char output[20] = {0};

  switch(*commandP) {
    case(':'):
      commandP++;
      switch(*commandP) {

        // GET COMMANDS ------------------------------------------------------------
        case 'G':
          commandP++;
          switch(*commandP) {
            case 'P':
              sprintf(output, "%08lX#", _focuser->currentPosition());
              Serial.print(output);
              break;
            case 'N':
              sprintf(output, "%08lX#", _focuser->getTargetPosition());
              Serial.print(output);
              break;
            case 'D':
              sprintf(output, "%02X#", _focuser->getCurrentSpeedDivider());
              Serial.print(output);
              break;
            case 'H':
              if(_focuser->isHalfStep()) Serial.print("FF#");
              else Serial.print("00#");
              break;
            case 'I':
              if(_focuser->isRunning()) Serial.print("01#");
              else Serial.print("00#");
              break;
            case 'T':
              sprintf(output, "%04X#", (int)round(_focuser->getTemperature() * 2.0));
              Serial.print(output);
              break;
            case 'C':
              sprintf(output, "%04X#", (int)round(_focuser->getTempCoefficient() * 2.0));
              Serial.print(output);
              break;
            case 'V':
              Serial.print(VERSION + "#");
              break;
          }
          break;

        // SET COMMANDS ------------------------------------------------------------
        case 'S':
          commandP++;
          switch(*commandP) {
            case 'N':
              _focuser->setTargetPosition(strtol(++commandP, NULL, 16));
              break;
            case 'P':
              _focuser->setCurrentPosition(strtol(++commandP, NULL, 16));
              break;
            case 'D':
              _focuser->setCurrentSpeedDivider(strtol(++commandP, NULL, 16));
              break;
            case 'F':
              _focuser->setHalfStep(false);
              break;
            case 'H':
              _focuser->setHalfStep(true);
              break;
            case 'C':
              _focuser->setTempCoefficient((float)strtol(++commandP, NULL, 16) / 2.0);
              break;
          }
          break;

        // MOTOR COMMANDS ------------------------------------------------------------
        case 'F':
          commandP++;
          switch(*commandP) {
            case 'G': 
              _focuser->moveToTargetPosition();
              break;
            case 'Q':
              _focuser->stop();
              break;
          }
          break;

        // TEMPERATURE COMMANDS ------------------------------------------------------------
        case 'P':
          commandP++;
          switch(*commandP) {
            case 'O': 
              _focuser->setTempCalibration((float)strtol(++commandP, NULL, 16) / 2.0);
              break;
          }
          break;
        case '+':
          _focuser->setTempCompensation(true);
          break;
        case '-':
          _focuser->setTempCompensation(false);
          break;
        case 'C':
          _focuser->readTemperature();
          break;
      }
      break;
  }
  
}
