import fastifyView from "@fastify/view";
import fastify from "fastify";
import ejs from "ejs"
import { showData } from "./action/showData.js";
import { getData } from "./action/getData.js";
import { postData } from "./action/updateDB.js";

const app = fastify()

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
app.get('/getData', getData)
app.post('/postData', postData)

const start = async () => {
    try {
        await app.listen({port: 3000})
    } catch (error) {
        console.error(error)
        process.exit(1)
    }
}
start()