const configureAPI = require("./src/server/configure");

module.exports = {
  devServer: {
    before: configureAPI,
  },
  transpileDependencies: ["vuetify"],
  productionSourceMap: true,
  configureWebpack: {
    devtool: "source-map",
  },
};
