const express = require("express");
const router = express.Router();
const os = require("os");

const sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

let config = require("../../../conf/emconfig.json");

// middleware that is specific to this router
// router.use(function timeLog(req, res, next) {
//   console.log("Time: ", Date.now());
//   next();
// });
router.get("/config", async function (req, res) {
  console.log("start GET /config");
  await sleep(10000);
  console.log("finish GET /config");
  res.json(config);
});

router.post("/config", function (req, res) {
  console.log("POST /config");
  console.log(JSON.stringify(req.body));
  config = req.body;
  res.sendStatus(204);
});

router.post("/restart", function (req, res) {
  console.log("POST /restart");
  res.sendStatus(205);
});

router.get("/system_info", function (req, res) {
  res.json({
    version: `${os.platform()} ${os.release()}`,
    cores: os.cpus().length,
  });
});

module.exports = router;
