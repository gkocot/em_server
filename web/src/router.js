import Vue from "vue";
import Router from "vue-router";
import Home from "./views/Home.vue";
import WiFi from "./views/WiFi.vue";
import EMSettings from "./views/EMSettings.vue";
import ADC from "./views/ADC.vue";
import IO from "./views/IO.vue";

Vue.use(Router);

export default new Router({
  mode: "history",
  base: process.env.BASE_URL,
  routes: [
    {
      path: "/",
      redirect: "/home",
    },
    {
      path: "/home",
      name: "home",
      component: Home,
    },
    {
      path: "/wifi",
      name: "wifi",
      component: WiFi,
    },
    {
      path: "/em",
      name: "em",
      component: EMSettings,
    },
    {
      path: "/adc",
      name: "adc",
      component: ADC,
    },
    {
      path: "/io",
      name: "io",
      component: IO,
    },
    {
      path: "*",
      redirect: "/home",
    },
  ],
});
