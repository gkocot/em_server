import Vue from "vue";
import Vuex from "vuex";
import axios from "axios";
import { cloneDeep } from "lodash";

// const sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

Vue.use(Vuex);

export default new Vuex.Store({
  strict: process.env.NODE_ENV !== "production",

  state: {
    title: "Home",
    wifi: {
      loaded: false,
      mode: 2,
      ssid: "myssid",
      password: "mypassword",
    },
    modbus: {
      loaded: false,
    },
  },

  mutations: {
    mutateConfig(state, config) {
      state.config = cloneDeep(config);
    },
    mutateWiFiConfig(state, wifi) {
      state.wifi = cloneDeep(wifi);
      state.wifi.loaded = true;
    },
    mutateModbusConfig(state, modbus) {
      state.modbus = cloneDeep(modbus);
      state.modbus.loaded = true;
    },
    mutateTitle(state, title) {
      state.title = title;
    },
  },

  actions: {
    async loadWiFiConfig({ commit, state }) {
      if (!state.wifi.loaded) {
        try {
          const { data: wifi } = await axios.get("/api/v1/wifi");
          commit("mutateWiFiConfig", wifi);
        } catch (error) {
          console.log(error);
        }
      }
    },

    async saveWiFiConfig({ commit }, wifi) {
      try {
        const wifiConfig = cloneDeep(wifi);
        delete wifiConfig.loaded;
        await axios.post("/api/v1/wifi", wifiConfig);
        commit("mutateWiFiConfig", wifiConfig);
      } catch (error) {
        console.log(error);
      }
    },

    async loadModbusConfig({ commit, state }) {
      if (!state.modbus.loaded) {
        try {
          const { data: modbus } = await axios.get("/api/v1/modbus");
          commit("mutateModbusConfig", modbus);
        } catch (error) {
          console.log(error);
        }
      }
    },

    async saveModbusConfig({ commit }, modbus) {
      try {
        const modbusConfig = cloneDeep(modbus);
        delete modbusConfig.loaded;
        await axios.post("/api/v1/modbus", modbusConfig);
        commit("mutateModbusConfig", modbusConfig);
      } catch (error) {
        console.log(error);
      }
    },

    async saveModbusState({ commit }, modbus) {
      commit("mutateModbusConfig", modbus);
    },

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

    setTitle({ commit }, title) {
      commit("mutateTitle", title);
    },
  },

  getters: {
    wifiConfig: (state) => {
      return state.wifi;
    },
    modbusConfig: (state) => {
      return state.modbus;
    },
  },
});
