const express = require("express");
const router = express.Router();
const os = require("os");

let config = {
  wifi: { mode: 1, ssid: "myssidd-server", password: "mypasswordd-server" },
};

// middleware that is specific to this router
// router.use(function timeLog(req, res, next) {
//   console.log("Time: ", Date.now());
//   next();
// });
router.get("/config", function (req, res) {
  console.log("GET /config");
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
