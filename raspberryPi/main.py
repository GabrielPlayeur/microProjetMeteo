import requests
import logging

logging.basicConfig(filename="logRaspberryPi.log", level=logging.INFO, format='%(asctime)s : %(levelname)s : %(message)s')

def getData():
    data = {"token": "test",
            "temperature": 13.0,
            "pression": 100,
            "pluie": "--",
            "vent": 56,
            "luminosite": 0.4,
            "humidite": 56
    }
    logging.info("Data received")
    return data

def formatReceivedData(rawData):
    data = rawData
    logging.info("Data formatted")
    return data

def postData(url, data):
    try:
        logging.info(f"Try to post data at {url}")
        res = requests.post(url=url, json=data)
        logging.info(f"{res.status_code} {res.text}")
    except Exception as err:
        logging.error(err)

URL = "http://localhost:3000/postData"
rawData = getData()
DATA = formatReceivedData(rawData)

postData(URL, DATA)