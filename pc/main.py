import sys

import cv2
import raspberry


def run():
    """Main function to run on the pc.
    """
    while True:
        frame = raspberry.get_frame()

        # Skip frame if unsuccesfully received and move straight on to the next frame.
        if frame is None:
            continue

        cv2.imshow("Frame", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break


if __name__ == "__main__":
    args = sys.argv[1:]

    run()
else:
    raise ImportError("Run this module directly, don't import it.")