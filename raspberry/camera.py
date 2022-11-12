import logging
import socket
import struct
import time

import picamera


HOST_IP = "192.168.0.21"  # PC ip address.
CAM_PORT = 5000

UDP_BYTE_LIMIT = 65507
TIME_TO_RECORD = 2000  # How long to keep cam stream recording on in seconds.

FPS = 30
RESOLUTIONS = ["320x240", "640x480", "1640x922"]
RES_INDEX = 1  # Resolution index to use, chosen from RESOLUTIONS.

LOG = True


logger = logging.getLogger("Camera")
logging.basicConfig(level=logging.INFO, format='%(name)s (%(levelname)s): %(message)s')
logger.disabled = not LOG


host = (HOST_IP, CAM_PORT)

pi_ip = "0.0.0.0"
pi_port = 5010


# Creating the UDP socket and binding it locally to the pi.
cam_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
cam_socket.bind((pi_ip, pi_port))


def send_img_bytes(img_bytes):
    """Send the pi camera current image view in byte format to the host.

    Args:
        img_bytes (bytes): The image from the pi camera in bytes.
    """

    img_num_bytes = len(img_bytes)

    # First sending the number of bytes of the image, to determine if
    # the image will be sent as a full frame or in chunks.
    img_num_bytes_packet = struct.pack("<L", img_num_bytes)

    try:
        cam_socket.sendto(img_num_bytes_packet, host)
    except:
        logger.warning("Failed to send image number of bytes.")


    # Send image in one chunk if image size is under the udp bytes limit.
    if img_num_bytes < UDP_BYTE_LIMIT:
        try:
            cam_socket.sendto(img_bytes, host)
        except:
            logger.warning("Failed to send full frame.")

    # Send in two chunks if image size exceeds the udp byte limit.
    else:
        half_img_num_bytes = img_num_bytes // 2

        try:
            cam_socket.sendto(img_bytes[:half_img_num_bytes], host)
            cam_socket.sendto(img_bytes[half_img_num_bytes:], host)
        except:
            logger.warning("Failed to send two frame halves.")


class CamHandler():
    """Used with the PiCamera library to get the image bytes, requires "write" function.
    """

    def write(self, img_bytes):
        """Required function for the PiCamera library to obtain image bytes.

        Args:
            img_bytes (bytes): The image from the pi camera in bytes.
        """

        send_img_bytes(img_bytes)


def start_cam_stream():
    """Start pi camera recording stream.
    """

    cam_handler = CamHandler()

    with picamera.PiCamera(resolution=RESOLUTIONS[RES_INDEX], framerate=FPS) as camera:
        logger.info("Getting ready...")
        time.sleep(2)

        camera.rotation = 180

        camera.start_recording(cam_handler, format="mjpeg")
        logger.info("Ready and started recording...")

        camera.wait_recording(TIME_TO_RECORD)
        camera.stop_recording()