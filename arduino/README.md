# Arduino Sketches

## /ESP32_Car
For the car on-board ESP32 micro-controller.

Communicates with the PC using a udp connection, and with the remote controller using radio signals.

### Key changes to make
- `WIFI_SSID` and `WIFI_PASS`, your WiFi credentials.
- `PC_IP`, the PC local ip address.
- The pin numbers to match your wiring (any constant variable with "PIN" in the name).

### Libraries required
All libraries can be easily installed using the Arduino IDE library manager.

- `RF24` by TMRh20, Avamander.
- `ESP32Servo` by Kevin Harrington.


## /Attiny88_RC
For the Attiny88 micro-controller that is in the remote controller (RC).

Ideally you want to choose a micro-controller that has low current consumption so the battery lasts longer and is small to fit in the RC housing, the Attiny88 is perfect for this.

See below how to upload sketches to the Attiny88.

### Key changes to make
- The pin numbers to match your wiring (any constant variable with "PIN" in the name).

### Libraries required
- `RF24` by TMRh20, Avamander.


# Wiring
## Receiver (car, ESP32)
![](/readme_imgs/breadboard_receiver.png)

## Transmitter (remote controller, Attiny88)
![](/readme_imgs/breadboard_transmitter.png)


# How to upload sketches to Attiny MCs
- The Attiny MCs can be programmed just like any Arduino and it's very easy to upload sketches to them.
- You just need a USBASP adapter, then simply make the connections as shown in the image below.

![](/readme_imgs/usbasp.png)

- You will need to install the Attiny boards using the [AttinyCore](https://github.com/SpenceKonde/ATTinyCore) board manager by placing this link `http://drazzy.com/package_drazzy.com_index.json` to your "Additional Boards Manager URLs", which can be accessed in `File > Preferences` on the Arduino IDE.
- From `Tools > Board`, choose your Attiny board with the no bootloader option.
- Then go to `Tools > Programmer` and choose `USBasp (ATTinyCore)`, and then press `Tools > Burn Bootloader`.
- Then to simply upload code, use `Sketch > Upload Using Programmer`.
- You can use stripboards with some wires to make uploading code to the Attiny MCs more convenient:

![](/readme_imgs/attiny_stripboards.png)