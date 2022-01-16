const bodyParser = require("body-parser");
const api = require("./api");

module.exports = (app) => {
  app.use(bodyParser.json());
  app.use("/api/v1", api);
};
