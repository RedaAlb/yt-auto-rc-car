import socket


HOST_IP = "192.168.0.21"  # PC ip address.
ESP32_IP = "192.168.0.75"  # Obtain this from ESP32 serial monitor on boot.
ESP32_PORT = 5010

LOG_PREFIX = "ESP32 -"

BYTES_TO_RECV = 6  # Number of bytes to receive from the ESP32 (length of array).


esp32_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
esp32_socket.bind((HOST_IP, ESP32_PORT))
esp32_socket.settimeout(0.001)
print(LOG_PREFIX, f"UDP connection opened on {HOST_IP}:{ESP32_PORT}...")


def recv_esp32_data():
    """Receive data from the ESP32 using the udp connection.

    Returns:
        list/None: A list containing all data received, otherwise None if no data was received.
    """
    try:
        packet = esp32_socket.recvfrom(BYTES_TO_RECV)
    except TimeoutError:
        return

    if packet:
        data_received = list(packet[0])
        return data_received
    else:
        return None


def send_data_esp32(data):
    """Send data to the ESP32 using the udp connection.

    Args:
        data (list): A list containing all the data to send to the ESP32.
    """
    data_bytes = bytes(data)

    try:
        esp32_socket.sendto(data_bytes, (ESP32_IP, ESP32_PORT))
    except:
        print(LOG_PREFIX, "Failed to send to ESP32, possibly incorrect ip or port.")

