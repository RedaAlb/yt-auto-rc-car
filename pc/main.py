import sys

import cv2

import raspberry
import esp32


def run():
    """Main function to run on the pc.
    """

    # Using a thread for the ESP32 communication to not interfere with camera feed (this main thread).
    esp32.start_thread()
    
    while True:
        frame = raspberry.get_frame()  # Get frame from the raspberry pi using UDP connection.

        # Skip frame if unsuccesfully received and move straight on to the next frame.
        if frame is None:
            continue
        

        # Obtain the data received from the ESP32 using the thread queue.
        esp32_data_recv = esp32.get_queue_value()
        if esp32_data_recv is not None:
            print(esp32_data_recv)

        # Send data to the ESP32 using the thread queue.
        import random
        esp32_data_to_send = [1, random.randint(0, 255), 3]  # Sample data.
        esp32.set_queue_value(esp32_data_to_send)


        cv2.imshow("Frame", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    

    cv2.destroyAllWindows()
    esp32.set_queue_value(0)  # To terminate the ESP32 thread.


if __name__ == "__main__":
    args = sys.argv[1:]

    run()
else:
    raise ImportError("Run this module directly, don't import it.")