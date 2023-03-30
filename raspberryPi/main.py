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
            if data[0] == '{' and data[-1] == '}':
                return str(data).replace("'", '"')

def formatReceivedData(rawData: str) -> dict:
    logging.info("Start Format Data")
    data = loads(rawData)
    try:
        data['humidite'] = round(data['humidite'])
        data['pression'] = round(data['pression'],1)
        data['temperature'] = round(data['temperature'],1)
        data['luminosite'] = round(data['luminosite'])
        data['vent'] = round(data['vent'],1)
        logging.info("Data formatted")
    except Exception as err:
        logging.error(err)
    return data

def postData(url: str, data: dict) -> None:
    try:
        logging.info(f"Try to post data at {url}")
        res = post(url=url, json=data)
        logging.info(f"{res.status_code} {res.text}")
    except Exception as err:
        logging.error(err)

# Server URL
URL = "http://aissd.d011.fr:3000/postData"

# Arduino Connect Option
deviceName="/dev/ttyACM0"
baudRate=9600
timeout=1

def app(dev=False):
    logging.basicConfig(filename="logRaspberryPi.log", level=logging.INFO, format='%(asctime)s : %(levelname)s : %(message)s')

    try:
        ser = Serial(deviceName, baudRate, timeout=timeout)
    except Exception as err:
        logging.error(err)
        return

    rawData = getSerialData(ser)
    DATA = formatReceivedData(rawData)

<<<<<<< HEAD
    if dev:
        print(DATA)
        return

    postData(URL, DATA)
    return

if __name__ == "__main__":
    app(dev=False)
=======
    postData(URL, DATA)
>>>>>>> server
