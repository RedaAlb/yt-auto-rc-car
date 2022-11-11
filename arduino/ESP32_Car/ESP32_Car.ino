#include <WiFi.h>
#include <WiFiUdp.h>
#include "RF24.h"
#include "ESP32Servo.h"


const char WIFI_SSID[] = "wifi_name";
const char WIFI_PASS[] = "wifi_password";

const char PC_IP[] = "192.168.0.21";  // Local pc address.
const int PORT = 5010;


// Radio nRF24L01 module.
const int CSN_PIN = 21;
const int CE_PIN = 22;
const byte ADDRESS[6] = "RCCAR";


// Controls pins for the motor drivers.
const int MOTORS_LEFT_FWD_PIN = 26;   // Left motors forward
const int MOTORS_LEFT_BCK_PIN = 25;   // Left motors backwards
const int MOTORS_RIGHT_FWD_PIN = 33;  // Right motors forward
const int MOTORS_RIGHT_BCK_PIN = 32;  // Right motors backwards

const int STEERING_SERVO_PIN = 17;
const int STEERING_SERVO_ANGLE_MID = 90;  // The middle angle for the servo, resting position.
const int STEERING_SERVO_ANGLE_LIMIT = 50;  // Steering max angle allowed.


// PWM channels to control the motors.
// 2-3 pwm channels are for the left motors (forward and backwards), 4-5 for the right motors.
const int PWM_CHANNELS[] = { 2, 3, 4, 5 };
const int PWM_FREQ = 500;
const int PWM_RES = 8;


const int JOYSTICK_RESTING_VALUE = 127;


// This array holds the data that is received from the remote controller (RC).
// The order and length of this array has to match with the array in the transmitter (Attiny88).
// Order: joystickLX, joystickLY, joystickRX, joystickRY, btnLeft, btnRight.
const int NUM_OF_RC_DATA_ITEMS = 6;
byte rcDataArray[NUM_OF_RC_DATA_ITEMS];

// This array holds the received values from the pc.
const int ITEMS_TO_RECV = 3;  // Number of items in list/array receiving from pc.
byte receivedPcValues[ITEMS_TO_RECV];


WiFiUDP udp;

RF24 radio(CE_PIN, CSN_PIN);
Servo steeringServo;


void setup() {
    Serial.begin(115200);

    connectToWiFi();
    udp.begin(PORT);

    initRadio();
    initPwm();

    steeringServo.write(STEERING_SERVO_ANGLE_MID);
    steeringServo.attach(STEERING_SERVO_PIN);
}


void loop() {
    receiveDataFromPc();

    if (!radio.available()) {
        return;
    }

    // Read data from the remote controller.
    radio.read(&rcDataArray, sizeof(rcDataArray));

    sendDataToPc(rcDataArray);

    handleVerticalMovement();
    handleSteering();
}


/// @brief Connect the ESP32 to the WiFi.
void connectToWiFi() {
    Serial.print("Connecting to ");
    Serial.print(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi Connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}


/// @brief Initialise radio nrf module to receive data from rc.
void initRadio() {
    if (!radio.begin()) {
        while (1) {}
    }

    radio.openReadingPipe(1, ADDRESS);
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_2MBPS);

    radio.startListening();
}


/// @brief Initialise pwm channels for motor controls.
void initPwm() {
    int numOfChannels = sizeof(PWM_CHANNELS) / sizeof(PWM_CHANNELS[0]);

    for (int i = 0; i < numOfChannels; i++) {
        int channel = PWM_CHANNELS[i];
        ledcSetup(channel, PWM_FREQ, PWM_RES);
    }

    ledcAttachPin(MOTORS_LEFT_FWD_PIN, PWM_CHANNELS[0]);
    ledcAttachPin(MOTORS_LEFT_BCK_PIN, PWM_CHANNELS[1]);
    ledcAttachPin(MOTORS_RIGHT_FWD_PIN, PWM_CHANNELS[2]);
    ledcAttachPin(MOTORS_RIGHT_BCK_PIN, PWM_CHANNELS[3]);
}


/// @brief Based on the position of the left joystick, move car forwards or backwards.
void handleVerticalMovement() {
    int joystickLeftY = rcDataArray[1];

    if (joystickLeftY > JOYSTICK_RESTING_VALUE) {
        int speed = map(joystickLeftY, JOYSTICK_RESTING_VALUE + 1, 255, 0, 255);
        forward(speed);
    }
    else if (joystickLeftY < JOYSTICK_RESTING_VALUE) {
        int speed = map(joystickLeftY, JOYSTICK_RESTING_VALUE - 1, 0, 0, 255);
        backward(speed);
    }
    else {
        stopCar();
    }
}


/// @brief Based on the position of the right joystick, control car steering servo.
void handleSteering() {
    int joystickRightX = rcDataArray[2];

    int minSteeringAngle = STEERING_SERVO_ANGLE_MID - STEERING_SERVO_ANGLE_LIMIT;
    int maxSteeringAngle = STEERING_SERVO_ANGLE_MID + STEERING_SERVO_ANGLE_LIMIT;

    int servoValue = map(joystickRightX, 0, 255, minSteeringAngle, maxSteeringAngle);

    steeringServo.write(servoValue);
}


/// @brief Send data to the PC using the UDP connection.
/// @param dataToSend The data byte array to send to the PC.
void sendDataToPc(byte dataToSend[]) {
    udp.beginPacket(PC_IP, PORT);
    udp.write(dataToSend, NUM_OF_RC_DATA_ITEMS);
    udp.endPacket();
}


/// @brief Updates receivedPcValues array with values received.
void receiveDataFromPc() {
    udp.parsePacket();

    if (udp.read(receivedPcValues, ITEMS_TO_RECV) > 0) {
        printArrayValues(receivedPcValues, ITEMS_TO_RECV, " ");
    }
}


/// @brief Prints array values to serial.
/// @param array The array to print values of.
/// @param arrayLength Array length.
/// @param spacer The spacer that will be in-between each value when printed.
void printArrayValues(byte array[], int arrayLength, String spacer) {
    for (int i = 0; i < arrayLength; i++) {
        int value = array[i];

        Serial.print(value);
        Serial.print(spacer);
    }

    Serial.println("");
}


/// @brief Move car forwards.
/// @param speed Speed of the car.
void forward(int speed) {
    ledcWrite(PWM_CHANNELS[0], speed);
    ledcWrite(PWM_CHANNELS[1], 0);
    ledcWrite(PWM_CHANNELS[2], speed);
    ledcWrite(PWM_CHANNELS[3], 0);
}


/// @brief Move car backwards.
/// @param speed Speed of the car.
void backward(int speed) {
    ledcWrite(PWM_CHANNELS[0], 0);
    ledcWrite(PWM_CHANNELS[1], speed);
    ledcWrite(PWM_CHANNELS[2], 0);
    ledcWrite(PWM_CHANNELS[3], speed);
}


/// @brief Stop the car.
void stopCar() {
    ledcWrite(PWM_CHANNELS[0], 0);
    ledcWrite(PWM_CHANNELS[1], 0);
    ledcWrite(PWM_CHANNELS[2], 0);
    ledcWrite(PWM_CHANNELS[3], 0);
}
