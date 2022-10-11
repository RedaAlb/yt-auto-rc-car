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