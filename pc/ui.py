import log
import datacollection


# Used to set the mode of the ui/program.
DATA_COLLECTION_MODE = 1  # Used when collecting data for model training.
MODES_MSGS = ["No mode", "Data collection"]  # Log messages for each mode.

LOG = True


logger = log.create_logger("UI")
log.set_logger_display(logger, LOG)


current_mode = 0  # See modes above.


def handle_key_pressed(key_pressed):
    """Handle any key presses made using the pc keyboard.

    Args:
        key_pressed (int): The pressed key on the pc keyboard.
    """
    if key_pressed == ord("p"):
        set_mode(DATA_COLLECTION_MODE)
    elif key_pressed == ord("s"):
        datacollection.save_collected_data()
        set_mode(0)


def set_mode(mode):
    """Set the mode of the ui/program.

    Args:
        mode (int): The new mode to set.
    """
    global current_mode

    if current_mode == mode:
        current_mode = 0
    else:
        current_mode = mode

    logger.info(f"Current mode: {current_mode} {MODES_MSGS[current_mode]}")