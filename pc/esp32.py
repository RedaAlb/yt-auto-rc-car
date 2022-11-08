import socket


HOST_IP = "192.168.0.21"  # PC ip address.
ESP32_PORT = 5010

LOG_PREFIX = "ESP32 -"

BUFFER_SIZE = 1024    


esp32_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
esp32_socket.bind((HOST_IP, ESP32_PORT))
print(LOG_PREFIX, f"UDP connection opened on {HOST_IP}:{ESP32_PORT}...")


# This will automatically be set when something is received from the ESP32,
# so it can be used to send data to the ESP32.
esp32_address = None


def recv_esp32_data():
    packet = esp32_socket.recvfrom(BUFFER_SIZE)

    if packet:
        # Once something is received from the ESP32, save the esp address if not set already.
        global esp32_address
        if esp32_address is None:
            esp32_address = packet[1]

        data_received = list(packet[0])
        print(LOG_PREFIX, "Received:", data_received)

        return data_received


def send_to_esp32(data):
    data_bytes = bytes(data)

    try:
        esp32_socket.sendto(data_bytes, esp32_address)
    except TypeError as error:
        print(LOG_PREFIX, "ESP32 address was not set, error:", error)

