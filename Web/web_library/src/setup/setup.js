const fs = require("fs");
const md5 = require("md5");
const crypto = require("crypto");
const path = require("path");

function SETUP() {
  const oldPath = "flag.txt";

  const newPath =
    "new_" +
    process.versions.v8 +
    md5(btoa(crypto.randomBytes.toString("hex")).slice(0, 48));

  const dir = path.join(newPath);

  if (!fs.existsSync(dir)) {
    fs.mkdirSync(dir, { recursive: true });
  }
  if (fs.existsSync(oldPath)) {
    fs.rename(oldPath, path.join(newPath, "/flag.txt"), function (err) {
      if (err) {
        throw err;
      }
    });
  }
}

module.exports = SETUP;
