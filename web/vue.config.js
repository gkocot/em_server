const configureAPI = require("./src/server/configure");

module.exports = {
  devServer: {
    before: configureAPI,
  },
  transpileDependencies: ["vuetify"],
  productionSourceMap: false,
  configureWebpack: {
    devtool: "source-map",
  },
};
