# Autonomous RC Car

I initially created this project as my master's thesis project, where that version can be found [here](https://github.com/RedaAlb/old-auto-rc-car).

However, I am re-creating the project from scratch in this repo, with 3D printed parts and re-designed from the ground up.

The original paper of the project when I submitted this project for grading (87%) can be found [here](MSc_AI_AutoRCCar_paper.pdf). Although the paper does not directly relate to this new re-created version of the project, it still contains useful information.

- [Project video part 1](https://youtu.be/-9yZNdQbg1g), creating the foundation, a remote controlled car (no autonomy).


# Structure

- `/pc`, contains files that needs to be and run on the pc.
- `/raspberry`, contains files that needs to be and run on the raspberry pi.
- `/arduino`, contains files that needs to be and run on an Arduino.
- `/3d_printing`, 3D printing models.
- `/readme_imgs`, images for read me files.

For more info, refer to the corresponding readme file in each directory.


# Project components

- [ESP32](https://amzn.eu/d/0U2EgUz), this is used as the on-board micro-controller on the car.
- [Raspberry Pi 4 B](https://www.raspberrypi.com/products/raspberry-pi-4-model-b/), to capture and stream camera feed to the PC. Could be the Raspberry Pi 3 as well.
- [PiCamera v2.1](https://amzn.eu/d/9oRUHw3), the on-board camera on the car.
- [Attiny88](https://uk.rs-online.com/web/p/microcontrollers/1331690), this is the micro-controller that is used in the joystick controller to control the car manually.
- [2S lipo battery](https://amzn.eu/d/9X1NhUL), used to power the car.
- 18650 battery, used to power the joystick controller.
- [18650 TP4056 charging board](https://amzn.eu/d/iKM8bIU), used to charge the 18650 battery.
- [MT3608 step up converter](https://amzn.eu/d/ggu3ih6), to convert the 18650 battery ~3.7V to a constant 5V.
- [nRF24L01](https://amzn.eu/d/3ZHSqjr), used for the wireless radio communication between the transmitter(Attiny88) and the receiver(ESP32).
- [MCP1700 (3V)](https://uk.rs-online.com/web/p/voltage-regulators/0403888), to convert the 5V from the MT3608 to power the nRF24L01 module.
- [TT motors](https://amzn.eu/d/18QKg2z) (x4), used for the 4 wheels of the car.
- [L298N Mini](https://amzn.eu/d/cQw7e1T) (x2), the DC motor controller for the TT motors (one controls two motors).
- [Servo motor](https://amzn.eu/d/5htsI2w), used for the car steering.
- [Joystick](https://shop.pimoroni.com/products/mini-analog-joystick?variant=28289831047) (x2), used to control the servo motor for the car steering when controlling it manually.
- [LD1095(5v)](https://uk.rs-online.com/web/p/voltage-regulators/6869690) voltage regulator (x2), one for the ESP32, and one for the servo motor.
- 10µF capacitors (x4), two for each of the LD1095 voltage regulators.
- [Switches](https://amzn.eu/d/4lvAWpm) (x2), one to turn on and off the car and other for the joystick.
- [Buttons](https://www.cricklewoodelectronics.com/Red-Top-Push-to-Make-Miiniature-Switch.html) (x2), for the joystick controller.