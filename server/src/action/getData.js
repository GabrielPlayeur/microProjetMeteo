import {db} from "../database.js"

export const getData = (req, res) => {
    const data = db.prepare('SELECT name,date,temperature,pression,pluie,vent,luminosite,humidite FROM datameteo INNER JOIN stationmeteo on stationmeteo.id = datameteo.station_id').all()
    req.log.info('Send all database weather info')
    return data
}