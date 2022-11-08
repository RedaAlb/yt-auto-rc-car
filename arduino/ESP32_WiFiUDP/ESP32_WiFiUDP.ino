#include <WiFi.h>
#include <WiFiUdp.h>


const char WIFI_SSID[] = "wifi_name";
const char WIFI_PASS[] = "wifi_password";

const char PC_IP[] = "192.168.0.21";  // Local pc address.
const int PORT = 5010;

// Number of values receiving from the PC (number of items in list/array).
const int BYTES_TO_RECV = 3;


WiFiUDP udp;

byte dataToSend[] = { 1, 2, 3 };  // Sample data.
int dataToSendLength = sizeof(dataToSend) / sizeof(dataToSend[0]);

// This array will hold the received values from the host.
byte receivedValues[BYTES_TO_RECV];


void setup() {
  Serial.begin(115200);

  connectToWiFi();
}


void loop() {
  sendDataToPc(dataToSend);

  receiveDataFromPc();

  delay(33);
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


/// @brief Send data to the PC using the UDP connection.
/// @param dataToSend The data byte array to send to the PC.
void sendDataToPc(byte dataToSend[]) {
  udp.beginPacket(PC_IP, PORT);
  udp.write(dataToSend, dataToSendLength);
  udp.endPacket();
}


/// @brief Updates receivedValues array with values received.
void receiveDataFromPc() {
  udp.parsePacket();

  if (udp.read(receivedValues, BYTES_TO_RECV) > 0) {
    printArrayValues(receivedValues, BYTES_TO_RECV, " ");
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