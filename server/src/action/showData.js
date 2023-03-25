import {db} from "../database.js"
import jsdom  from 'jsdom'
import https from "node:https"
import util from 'util'

const URL = "https://www.meteociel.fr/previsions/15488/carquefou.htm"
const INDEX_TABLE = 6

const getHtmlPromise = util.promisify(getHtml)
export const showData = async (req, res) => {
    try {
        const forecast = await getWeatherForecast(res)
        const data = db.prepare('SELECT * FROM datameteo').all()
        return res.view("./template/index.ejs", {
                forecast:forecast.toString(),
                data:JSON.stringify(data).toString()
            })
    } catch (error) {
        console.error(error)
        res.status(500).send('Erreur lors de la récupération des données')
    }
  }

 async function getWeatherForecast(res) {
    try {
        // Récupérer les données de la page HTML
        const body = await getHtmlPromise()
        let tableKeys = ['Jour','Heure','Temp.','Temp.ressen.','dir.','moy.','raf.','Pluiesur 3h','Humidité','Pression','Temps']
        let table = getTable(body, tableKeys)
        let affichage = JSON.stringify(table)

        return affichage
    } catch (error) {
        console.error(error)
        res.status(500).send('Erreur lors de la récupération des données')
    }
}

function getTable(htmlTable, tableKeys) {
    const dom = new jsdom.JSDOM(htmlTable)
    const allTableRows = dom.window.document.querySelectorAll("table")
    let tableRows = allTableRows[INDEX_TABLE].querySelectorAll("table tr")

    let table = []
    let currentDay = ""

    for (let i = 2; i < tableRows.length; i++) {
        const curRow = tableRows[i].querySelectorAll('td')
        if (curRow.length==0) {
            continue
        }

        let rowValues = {}

        let j = 0
        let keysCurseur = 0
        if (curRow.length==tableKeys.length){ // Si on change de jour on modifie pour les lignes suivant
            currentDay = curRow[0].textContent.replace(/\s+/g, '')
            j++
        }
        rowValues[tableKeys[keysCurseur]] = currentDay // Ajoute de la date
        keysCurseur++

        for (j; j < curRow.length; j++) {
            rowValues[tableKeys[keysCurseur]]=curRow[j].textContent.replace(/\s+/g, '')
            keysCurseur++
        }
        table.push(rowValues)
    }
    return table
}

function getHtml (callback){
    const options = {
        headers: {
            'Accept-Charset': 'utf-8'
        }
    }
    const req = https.request(URL, options,(res) => {
        let data = ''
        res.on('data', (chunk) => {
            data = data + chunk.toString()
        })

        res.on('end', () => {
            const body = data
            callback(null, body)
        })
    })

    req.on('error', (e) => {
        callback(e)
    })

    req.end()
}