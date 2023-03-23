import {db} from "../database.js"

export const getData = (req, res) => {
    const data = db.prepare('SELECT * FROM datameteo').all()
    console.log(data)
    return "data receive"
}