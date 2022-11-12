import logging


LOG_FORMAT = "%(name)s (%(levelname)s): %(message)s"


def create_logger(name):
    """Create a logger object using the logging module.

    Args:
        name (str): The name of the logger, which will be the prefix of the log.

    Returns:
        logging.Logger: The created logger object.
    """
    logger = logging.getLogger(name)
    logging.basicConfig(level=logging.INFO, format=LOG_FORMAT)

    return logger


def set_logger_display(logger, value):
    """Whether to display the logs or not.

    Args:
        logger (logging.Logger): The logger object to set value of.
        value (bool): Display the logs or not.
    """
    logger.disabled = not value