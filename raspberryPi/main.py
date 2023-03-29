from requests import post
import logging
from serial import Serial
from json import loads

def getSerialData(ser: Serial) -> str:
    ser.reset_input_buffer()

    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').rstrip()
            logging.info("Data received")
            return str(data).replace("'", '"')

def formatReceivedData(rawData: str) -> dict:
    data = loads(rawData)
    logging.info("Data formatted")
    return data

def postData(url: str, data: dict) -> None:
    try:
        logging.info(f"Try to post data at {url}")
        res = post(url=url, json=data)
        logging.info(f"{res.status_code} {res.text}")
    except Exception as err:
        logging.error(err)

# Server URL
URL = "http://localhost:3000/postData"

# Arduino Connect Option
deviceName="/dev/ttyACM0"
baudRate=9600
timeout=1

if __name__ == "__main__":
    logging.basicConfig(filename="logRaspberryPi.log", level=logging.INFO, format='%(asctime)s : %(levelname)s : %(message)s')

    ser = Serial(deviceName, baudRate, timeout=timeout)

    rawData = getSerialData(ser)
    DATA = formatReceivedData(rawData)

    postData(URL, DATA)