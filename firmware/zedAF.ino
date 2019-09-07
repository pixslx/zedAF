#include "Focuser.h"
#include "SerialAutomaton.h"
#include "constants.h"

Focuser focuser;
SerialAutomaton serialAutomaton(&focuser);

void setup() {
  focuser.setup();
}

void loop() {
  serialAutomaton.loop();  
}
