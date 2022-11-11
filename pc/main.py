import sys

import cv2

import raspberry
import esp32


def run():
    """Main function to run on the pc.
    """
    while True:
        frame = raspberry.get_frame()

        # Skip frame if unsuccesfully received and move straight on to the next frame.
        if frame is None:
            continue

        esp32_recv_data = esp32.recv_esp32_data()
        if esp32_recv_data is not None:
            print("ESP32 received data:", esp32_recv_data)


        import random
        esp32_data_to_send = [2, 2, 2]  # Sample data.
        esp32_data_to_send[1] = random.randint(0, 255)
        esp32.send_data_esp32(esp32_data_to_send)


        cv2.imshow("Frame", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break


if __name__ == "__main__":
    args = sys.argv[1:]

    run()
else:
    raise ImportError("Run this module directly, don't import it.")