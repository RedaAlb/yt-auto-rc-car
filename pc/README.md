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
    - `ESP32_IP`, local ESP32 ip address, obtain this from ESP32 serial monitor on boot.
- `datacollection.py`, to collect training data for model training.
- `ui.py`, handles all user interface input.
- `log.py`, to create loggers using the logging module.


# Directories

- `/data`, contains the data collected during the data collection stage.

For more info, refer to the corresponding readme file in each directory.