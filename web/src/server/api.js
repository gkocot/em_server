const express = require("express");
const router = express.Router();
const os = require("os");

// middleware that is specific to this router
// router.use(function timeLog(req, res, next) {
//   console.log("Time: ", Date.now());
//   next();
// });
router.get("/config", function (req, res) {
  res.json({
    wifi: { mode: 1, ssid: "myssidd-api", password: "mypasswordd-api" },
  });
});

router.post("/restart", function (req, res) {
  res.send("Restarting in 10s ...");
});

router.get("/system_info", function (req, res) {
  res.json({
    version: `${os.platform()} ${os.release()}`,
    cores: os.cpus().length,
  });
});

module.exports = router;
