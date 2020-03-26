#include <Dynamixel2Arduino.h>

#define DXL_SERIAL Serial1
#define DEBUG_SERIAL Serial
const uint8_t DXL_DIR_PIN = 28;

const uint8_t DXL_ID = 1;
const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

// motion parameters
float min_pos = 0;
float max_pos = 0;
const float rpm = 15;
const float omega_millis = 2 * PI * 15 / 60 * 1e-3;

/**
 * Establishes the connunication to the motor. If already connected, nothing
 * happens.
 */
void init_dxl() {
  if (dxl.ping(DXL_ID)) {
    return;
  }
  while (!dxl.ping(DXL_ID)) {
    DEBUG_SERIAL.println("connecting dynamixel...");
    dxl.begin(57600);
    delay(100);
  }
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  DEBUG_SERIAL.println("success!");

  // switch to postion control, defensive inital, min & max position
  float position = dxl.getPresentPosition(DXL_ID, UNIT_DEGREE);
  min_pos = position;
  max_pos = position;
  dxl.setGoalPosition(DXL_ID, position, UNIT_DEGREE);
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_CURRENT_BASED_POSITION);
  dxl.torqueOn(DXL_ID);
  DEBUG_SERIAL.println("ready");
}

/**
 * Checks if the button is pressed, set limits with disabled torque.
 */
void update_limits() {
  // read the state of the pushbutton value:
  int buttonState = digitalRead(BOARD_BUTTON_PIN);
  float position = dxl.getPresentPosition(DXL_ID, UNIT_DEGREE);
  if (buttonState == HIGH) {
    // disable torque and set limits
    dxl.torqueOff(DXL_ID);
    if (position < min_pos) {
      min_pos = position;
      DEBUG_SERIAL.printf("minimal postion updated to %f\n", position);
    }
    if (position > max_pos) {
      max_pos = position;
      DEBUG_SERIAL.printf("maximal postion updated to %f\n", position);
    }
  }
  if (buttonState == LOW) {
    dxl.torqueOn(DXL_ID);
  }
}

void setup() {
  DEBUG_SERIAL.begin(115200);
  // Use button as input
  pinMode(BOARD_BUTTON_PIN, INPUT_PULLDOWN);
}

void loop() {
  // reconnect motor if not connected
  init_dxl();
  // set limits when button is pressed
  update_limits();

  // Limit torque
  dxl.setGoalCurrent(DXL_ID, 25, UNIT_PERCENT);
  // TODO handle overflows after 50 days
  float goal_pos = 0.5 * (min_pos + max_pos) +
                   0.5 * (max_pos - min_pos) * sin(millis() * omega_millis);
  dxl.setGoalPosition(DXL_ID, goal_pos, UNIT_DEGREE);
}