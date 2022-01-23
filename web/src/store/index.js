import Vue from "vue";
import Vuex from "vuex";
import axios from "axios";
import { cloneDeep } from "lodash";

Vue.use(Vuex);

export default new Vuex.Store({
  strict: process.env.NODE_ENV !== "production",

  state: {
    title: "Home",
    config: {
      wifi: {
        mode: 2,
        ssid: "myssid",
        password: "mypassword",
      },
    },
  },

  mutations: {
    mutateConfig(state, config) {
      state.config = cloneDeep(config);
    },
    mutateWifiConfig(state, wifi) {
      state.config.wifi = cloneDeep(wifi);
    },
    mutateModbusConfig(state, modbus) {
      state.config.modbus = cloneDeep(modbus);
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

    async saveConfig({ state }) {
      try {
        await axios.post("/api/v1/config", state.config);
      } catch (error) {
        console.log(error);
      }
    },

    async saveModbus({ state }) {
      try {
        await axios.post("/api/v1/modbus", state.config.modbus);
      } catch (error) {
        console.log(error);
      }
    },

    async saveWiFiState({ commit }, wifi) {
      commit("mutateWifiConfig", wifi);
    },

    async saveModbusState({ commit }, modbus) {
      commit("mutateModbusConfig", modbus);
    },

    setTitle({ commit }, title) {
      commit("mutateTitle", title);
    },
  },

  getters: {
    wifiConfig: (state) => {
      return state.config.wifi;
    },
    modbusConfig: (state) => {
      return state.config.modbus;
    },
  },
});
