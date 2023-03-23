import {db} from "../database.js"

export const showData = (req, res) => {
    const data = db.prepare('SELECT * FROM datameteo').all()
    console.log(data)
    return "C'est bien ici la meteo ?"
}