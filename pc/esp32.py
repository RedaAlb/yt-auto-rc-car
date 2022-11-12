import socket
import threading 
from queue import Queue

import log


HOST_IP = "192.168.0.21"  # PC ip address.
ESP32_IP = "192.168.0.75"  # Obtain this from ESP32 serial monitor on boot.
ESP32_PORT = 5010

BYTES_TO_RECV = 6  # Number of bytes to receive from the ESP32 (length of array).

LOG = True  # Enable or disable logging.


logger = log.create_logger("ESP32")
log.set_logger_display(logger, LOG)


esp32_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
esp32_socket.bind((HOST_IP, ESP32_PORT))
esp32_socket.settimeout(0.001)
logger.info(f"UDP connection opened on {HOST_IP}:{ESP32_PORT}...")


# These queues are used to pass data between the main thread (main.py) and the ESP32 thread.
recv_queue = Queue()  # For data received from the ESP32.
send_queue = Queue()  # For data to be sent to the ESP32.


def start_thread():
    """Start a thread for the ESP32 communication to not interfere with camera feed (main thread).
    """
    esp32_thread = threading.Thread(target=thread_target, args=((recv_queue, send_queue)))
    esp32_thread.start()


def thread_target(recv_queue, send_queue):
    """The thread target function for this ESP32 module.

    Args:
        recv_queue (Queue): Used to share data between threads for data received from the ESP32.
        send_queue (Queue): Used to share data between threads to be sent to the ESP32.
    """
    while True:
        esp32_data_recv = recv_esp32_data()
        if esp32_data_recv is not None:
            recv_queue.queue.clear()
            recv_queue.put(esp32_data_recv)

        if not send_queue.empty():
            esp32_data_to_send = send_queue.get()
            
            # For thread termination.
            if(esp32_data_to_send == 0):
                break

            send_data_esp32(esp32_data_to_send)


def get_queue_value():
    """Get value from the thread queue which is the received value from the ESP32.

    Returns:
        list/None: A list containing all data received, otherwise None if no data was received.
    """
    if not recv_queue.empty():
        esp32_data_recv = recv_queue.get()
        return esp32_data_recv
    else:
        return None


def set_queue_value(data):
    """Set the value of the thread queue to send to the ESP32.

    Args:
        data (list): A list containing all the data to send to the ESP32.
    """
    send_queue.queue.clear()
    send_queue.put(data)


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
        logger.warning("Failed to send to ESP32, possibly incorrect ip or port.")
