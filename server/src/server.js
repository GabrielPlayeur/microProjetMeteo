import fastifyView from "@fastify/view"
import fastify from "fastify"
import ejs from "ejs"
import { showData } from "./action/showData.js"
import { postData } from "./action/updateDB.js"
import { getData } from "./action/getData.js"

const app = fastify({
  logger: {
    level: 'info',
    file: './log/info.log'
  }

})

app.register(fastifyView,{
    engine: {
        ejs
    }
})

app.addContentTypeParser(
    "application/json",
    { parseAs: "string" },
    function (req, body, done) {
      try {
        var newBody = {
          raw: body,
          parsed: JSON.parse(body),
        };
        done(null, newBody);
      } catch (error) {
        error.statusCode = 400;
        done(error, undefined);
      }
    }
  );

app.get('/', showData)
app.post('/postData', postData)
app.get('/getData', getData)

const start = async () => {
    try {
        await app.listen({port: 3000, host: '0.0.0.0'})
    } catch (error) {
        console.error(error)
        process.exit(1)
    }
}
start()
