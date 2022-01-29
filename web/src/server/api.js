const express = require("express");
const router = express.Router();
const os = require("os");
const fs = require("fs").promises;
const path = require("path");
const wifiConfigFilePath = "../../../conf/wifi.json";
const modbusConfigFilePath = "../../../conf/modbus.json";

const sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

let wifiConfig = require(wifiConfigFilePath);
let modbusConfig = require(modbusConfigFilePath);

// middleware that is specific to this router
// router.use(function timeLog(req, res, next) {
//   console.log("Time: ", Date.now());
//   next();
// });

router.get("/wifi", async function (req, res) {
  console.log("GET /wifi");
  await sleep(1000);
  res.json(wifiConfig);
});

router.get("/modbus", async function (req, res) {
  console.log("GET /modbus");
  await sleep(1000);
  res.json(modbusConfig);
});

router.post("/wifi", async function (req, res) {
  console.log("POST /wifi");
  console.log(JSON.stringify(req.body));
  wifiConfig = req.body;
  await fs.writeFile(
    path.join(__dirname, wifiConfigFilePath),
    JSON.stringify(wifiConfig)
  );
  res.sendStatus(204);
});

router.post("/modbus", async function (req, res) {
  console.log("POST /modbus");
  console.log(JSON.stringify(req.body));
  modbusConfig = req.body;
  await fs.writeFile(
    path.join(__dirname, modbusConfigFilePath),
    JSON.stringify(modbusConfig)
  );
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
