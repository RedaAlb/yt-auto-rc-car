import sys

import camera


def run():
    """Main function to run on the raspberry pi.
    """
    camera.start_cam_stream()


if __name__ == "__main__":
    args = sys.argv[1:]

    run()

else:
    raise ImportError("Run this module directly, don't import it.")