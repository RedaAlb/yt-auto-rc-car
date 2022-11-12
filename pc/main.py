import sys

import cv2

import raspberry
import esp32

import ui
import datacollection


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


        # Collected data for model training if collection mode is turned on.
        if ui.current_mode == ui.DATA_COLLECTION_MODE:
            datacollection.add_data_sample(frame, esp32_data_recv)


        # Send data to the ESP32 using the thread queue.
        import random
        esp32_data_to_send = [1, random.randint(0, 255), 3]  # Sample data.
        esp32.set_queue_value(esp32_data_to_send)


        cv2.imshow("Frame", frame)

        key_pressed = cv2.waitKey(1)
        if key_pressed == ord("q"):
            break

        ui.handle_key_pressed(key_pressed)
    

    cv2.destroyAllWindows()
    esp32.set_queue_value(0)  # To terminate the ESP32 thread.


if __name__ == "__main__":
    args = sys.argv[1:]

    run()
else:
    raise ImportError("Run this module directly, don't import it.")