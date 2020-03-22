#include <Dynamixel2Arduino.h>

#define DXL_SERIAL Serial1
#define DEBUG_SERIAL Serial
const uint8_t DXL_DIR_PIN = 28;

const uint8_t DXL_ID = 1;
const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

// motion parameters
float zero_pos = 0;
const float max_pos = 210;
const float rpm = 15;
const float omega_millis = 2 * PI * 15 / 60 * 1e-3;

/**
 * Establishes the connunication to the motor. If already connected, nothing happens.
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

  // switch to current control
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_CURRENT);
  dxl.torqueOn(DXL_ID);

  // init position
  DEBUG_SERIAL.println("homing");
  dxl.setGoalCurrent(DXL_ID, 1, UNIT_PERCENT);
  delay(1000);
  zero_pos = dxl.getPresentPosition(DXL_ID, UNIT_DEGREE);

  // switch to postion control
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_CURRENT_BASED_POSITION);
  dxl.torqueOn(DXL_ID);
  DEBUG_SERIAL.println("ready");
}

void setup() { DEBUG_SERIAL.begin(115200); }

void loop() {
  // reconnect motor if not connected
  init_dxl();

  // Limit torque
  dxl.setGoalCurrent(DXL_ID, 25, UNIT_PERCENT);
  // TODO handle overflows
  float goal_pos = zero_pos + max_pos * (1 + sin(millis() * omega_millis)) * 0.5;
  dxl.setGoalPosition(DXL_ID, goal_pos, UNIT_DEGREE);
}