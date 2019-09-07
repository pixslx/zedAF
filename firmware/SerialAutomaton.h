#pragma once

#include "Focuser.h"

class SerialAutomaton {
  public:
    SerialAutomaton(Focuser* focuser);
    void loop();

  private:
    Focuser* _focuser;
  
    char serialBuffer[20] = {0};
    char* serialBufferP = serialBuffer;
    
    void processCommand(char command[]);
    
};
