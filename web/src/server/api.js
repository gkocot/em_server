const express = require("express");
const router = express.Router();
const os = require("os");
const fs = require("fs").promises;
const configFilePath = "../../../conf/emconfig.json";
const wifiConfigFilePath = "../../../conf/wifi.json";

const sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

let config = require(configFilePath);
let wifiConfig = require(wifiConfigFilePath);

// middleware that is specific to this router
// router.use(function timeLog(req, res, next) {
//   console.log("Time: ", Date.now());
//   next();
// });

router.get("/config", async function (req, res) {
  console.log("GET /config");
  await sleep(1000);
  res.json(config);
});

router.get("/wifi", async function (req, res) {
  console.log("GET /wifi");
  await sleep(1000);
  res.json(wifiConfig);
});

router.post("/config", async function (req, res) {
  console.log("POST /config");
  console.log(JSON.stringify(req.body));
  config = req.body; // TBD: cloneDeep?
  await fs.writeFile(configFilePath, config);
  res.sendStatus(204);
});

router.post("/wifi", async function (req, res) {
  console.log("POST /wifi");
  console.log(JSON.stringify(req.body));
  wifiConfig = req.body; // TBD: cloneDeep?
  await fs.writeFile(wifiConfigFilePath, wifiConfig);
  res.sendStatus(204);
});

router.post("/modbus", async function (req, res) {
  console.log("POST /modbus");
  console.log(JSON.stringify(req.body));
  config.modbus = req.body; // TBD: cloneDeep?
  await flushConfig();
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
