# Autonomous RC Car

I initially created this project as my bachelor's and master's thesis project, where that version can be found [here](https://github.com/RedaAlb/old-auto-rc-car).

However, I am re-creating the project from scratch in this repo, with 3D printed parts to improve on it and to continue learning from it.

The original paper of the project when I submitted this project for grading can be found [here](MSc_AI_AutoRCCar_paper.pdf). Although the paper doesn't directly relate to this newly re-created version of the project, it still contains useful information.


# Arduino

All Arduino sketches can be found in the `/arduino` directory. For all the components and wiring used, please refer to the corresponding sections below.

## Transmitter (Attiny88)

- `/arduino/Attiny88_Transmitter/`
- This is for the micro-controller that is used in the joystick controller, which is the remote controller for the car when controlled manually.
- Ideally you want to choose a micro-controller that has low current consumption so the battery lasts longer and is smaller to fit in the joystick housing. Here the Attiny88 is used with the AttinyCore board manager (see below how to upload sketches to the Attiny88). 

### Libraries needed

All libraries can be easily installed using the Arduino IDE library manager.

- `RF24` by TMRh20, Avamander.


## Receiver (ESP32)

- `/arduino/ESP32_Receiver/`
- This is the micro-controller that will be on-board on the car.
- The used and recommended micro-controller for this is the ESP32.

### Libraries needed

All libraries can be easily installed using the Arduino IDE library manager.

- `RF24` by TMRh20, Avamander.
- `ESP32Servo` by Kevin Harrington.


# How to upload sketches to Attiny MCs

- The Attiny MCs can be programmed just like any Arduino and it's very easy to upload sketches to them.
- You just need a USBASP adapter, then simply make the connections as shown in the image below.

![](readme_imgs/usbasp.png)

- You will need to install the Attiny boards using the [AttinyCore](https://github.com/SpenceKonde/ATTinyCore) board manager by placing this link `http://drazzy.com/package_drazzy.com_index.json` to your "Additional Boards Manager URLs", which can be accessed in `File > Preferences`.
- From `Tools > Board`, choose your Attiny board with the no bootloader option.
- Then go to `Tools > Programmer` and choose `USBasp (ATTinyCore)`, and then press `Tools > Burn Bootloader`.
- Then to simply upload code, use `Sketch > Upload Using Programmer`.
- You can use stripboards with some wires to make uploading code to the Attiny MCs more convenient:

![](/readme_imgs/attiny_stripboards.png)


# Components

- [ESP32](https://amzn.eu/d/0U2EgUz), this is used as the on-board micro-controller on the car.
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


# Wiring

## Transmitter (joystick controller, Attiny88)

![](/readme_imgs/breadboard_transmitter.png)

## Receiver (car, ESP32)

![](/readme_imgs/breadboard_receiver.png)