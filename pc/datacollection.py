import os
import numpy as np

import log


LOG = True


logger = log.create_logger("Data collection")
log.set_logger_display(logger, LOG)


# Used to store the training data when collecting data.
frames = []  # Will hold all the car frames collected during data collection.
labels = []  # The labels for each frame, which is the ESP32 data (contains joystick data).


def add_data_sample(frame, label):
    """Add data sample pair.

    Args:
        frame (numpy.ndarray): The frame coming from the raspberry pi camera.
        label (list): The data coming from the ESP32 which contains the joysticks data.
    """
    if frame is None or label is None:
        return

    frames.append(frame)
    labels.append(label)
    logger.info(f"Added sample {len(labels)}, label: {label}")


def save_collected_data():
    """Save the collected data to .npy files in numpy array format.
    """
    if frames == [] or labels == []:
        logger.warning("No data to save.")
        return

    frames_arr = np.array(frames)
    labels_arr = np.array(labels)

    save_dir = ""
    if os.path.exists("pc"):
        save_dir = "pc/data"
    else:
        save_dir = "data"
    
    if not os.path.exists(save_dir):
        os.makedirs(save_dir)

    with open(f"{save_dir}/frames.npy", "wb") as file:
        np.save(file, frames_arr)

    with open(f"{save_dir}/labels.npy", "wb") as file:
        np.save(file, labels_arr)

    logger.info(f"Saved collected data, frames: {frames_arr.shape}, labels: {labels_arr.shape}")