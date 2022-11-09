# Setup
- Run `pip install -r .\requirements.txt` to install all required packages.


# Entry point
- `main.py`, run this script using `python main.py`


# Modules
- `raspberry.py`, to receive the camera feed from the raspberry pi using a UDP connection.
    - Key changes to make:
    - `HOST_IP`, local pc ip address.
- `esp32.py`, to send and receive data between the pc and the ESP32 using a UDP connection.
    - Key changes to make:
    - `HOST_IP`, local pc ip address.