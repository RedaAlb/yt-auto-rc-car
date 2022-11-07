import socket
import struct

import numpy as np
import cv2


HOST_IP = "192.168.0.21"  # PC ip address.
CAM_PORT = 5000

UDP_BYTE_LIMIT = 65507

LOG_PREFIX = "Raspberry Host -"


host = (HOST_IP, CAM_PORT)

cam_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
cam_socket.bind(host)
print(LOG_PREFIX, f"UDP connection opened on {HOST_IP}:{CAM_PORT}...")


def recv_img_bytes():
    """Receive image as bytes from the raspberry pi using the UDP connection.

    Returns:
        bytes/None: Image bytes if successfully received, otherwise None if not successful.
    """

    img_bytes = b""  # This will hold the frame/image in bytes coming from the Pi.

    # Getting the size (number of bytes) of the image to be received, which is an int32 (4 bytes).
    try:
        img_num_bytes = struct.unpack("<L", cam_socket.recvfrom(4)[0])[0]
    except:
        print(LOG_PREFIX, "Failed to receive image number of bytes.")
        return None


    # Receive image in one chunk if image size is under the udp bytes limit.
    if img_num_bytes < UDP_BYTE_LIMIT:
        try:
            img_bytes = cam_socket.recvfrom(img_num_bytes)[0]
            return img_bytes
        except:
            print(LOG_PREFIX, "Full frame was not received successfully.")
            return None
    # Receive in two chunks if image size exceeds the udp byte limit.
    else:
        half1_num_bytes = img_num_bytes // 2
        half2_num_bytes = img_num_bytes - half1_num_bytes

        try:
            half1 = cam_socket.recvfrom(half1_num_bytes)[0]
            half2 = cam_socket.recvfrom(half2_num_bytes)[0]

            # Combine both halves to make the full image.
            img_bytes = half1 + half2
            return img_bytes
        except:
            print(LOG_PREFIX, "Two frame halves were not received successfully.")
            return None


def get_frame():
    """Converts the image bytes to an actual image/frame (numpy array).

    Returns:
        numpy.ndarray: The frame in numpy array format.
    """

    img_bytes = recv_img_bytes()

    # Skip frame if image data was not successfully received.
    if img_bytes is None:
        return None
    
    # Converting image bytes to an actual image/frame.
    np_arr = np.frombuffer(img_bytes, np.uint8)
    frame = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)

    # Ensuring the decoding was successfull (no corruption in image data).
    if frame is None:
        return None

    return frame