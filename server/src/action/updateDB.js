import {db} from "../database.js"

const BODYKEYS = ['token', 'temperature', 'pression', 'pluie', 'vent', 'luminosite', 'humidite']

export const postData = (req, res) => {
    const data = req.body["parsed"]

    if (checkRequestBody(data)==false){
        return "Il manque des donnees dans le body !"
    }

    const stationID = getStationID(data["token"])
    if (stationID==undefined) {
        return "Vous n'avez pas les droits !"
    }

    saveNewData(stationID['id'], data)

    return "data saved"
}

const checkRequestBody = (body) => {
    for (let index = 0; index < BODYKEYS.length; index++) {
        const element = BODYKEYS[index];
        if (body[element]==undefined || body[element].toString().length==0) {
            return false
        }
    }
    return true 
}

const getStationID = (token) => {
    const data = db.prepare('SELECT id FROM stationmeteo WHERE token = ?').get(token)
    return data
}

const saveNewData = (id, body) => {
    db.prepare('INSERT INTO datameteo (station_id, date, temperature, pression, pluie, vent, luminosite, humidite) VALUES (?, ?, ?, ?, ?, ?, ?, ?)')
        .run(
            id,
            getCurrentDate(),
            body["temperature"],
            body["pression"],
            body["pluie"],
            body["vent"],
            body["luminosite"],
            body["humidite"],
        )
}

const getCurrentDate = () => {
    const d = new Date()
    const date = d.toISOString().split('T')[0];
    const time = d.toTimeString().split(' ')[0];
    return `${date} ${time}`
}