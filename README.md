# Dynamixel Respirator Open Source
This is the source code for the Dynamixel servo based setup.
The motor is controlled directly via an OpenCM 9.04 board (costs only ~10-15€).
Using a servo enables a closed loop design of the bag compression.

# Upload Code
Setup the development environment as described [here](http://emanual.robotis.com/docs/en/parts/controller/opencm904/#development-environment).
Connect the board to the computer via an USB port and open the main.ino file in the Arduino IDE.
Click the upload button and a confirmation should note the success.

# Operating Instructions
1. Connect the OpenCM 9.04 board to a 12V power supply and the motor to the Dynamixel bus.
2. The motor will hold the position it had when it was powered on.
3. To adjust the limits, hold the user button which will disable the torque.
4. Move the levers to the desired limits and release the button.
5. The levers move in a sine trajectory.


# Hardware and Assembly Instructions
- [Thingiverse](https://www.thingiverse.com/thing:4236312)
- [Instructions](https://coresponse.github.io/blog/markdowninstruction/concept/2020/03/24/concept_cable_winch_v0.html)
