import Vue from "vue";
import Vuex from "vuex";
import axios from "axios";

Vue.use(Vuex);

export default new Vuex.Store({
  strict: process.env.NODE_ENV !== "production",

  state: {
    title: "Home",
    config: {
      wifi: {
        mode: 1,
        ssid: "myssid",
        password: "mypassword",
      },
    },
  },

  mutations: {
    mutateConfig(state, config) {
      state.config = config;
    },
    mutateTitle(state, title) {
      state.title = title;
    },
  },

  actions: {
    async loadConfig({ commit }) {
      try {
        const resp = await axios.get("/api/v1/config");
        commit("mutateConfig", resp.data);
      } catch (error) {
        console.log(error);
      }
    },
    setTitle({ commit }, title) {
      commit("mutateTitle", title);
    },
  },

  getters: {
    wifiConfig: (state) => {
      return state.config.wifi;
    },
  },
});
