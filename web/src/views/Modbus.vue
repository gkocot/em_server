<template>
  <v-container>
    <v-select
      label="Speed"
      v-model="modbusSettings.speed"
      :items="modbusSettings.speeds"
    ></v-select>

    <v-row>
      <v-col>Device Name</v-col>
      <v-col>Address</v-col>
    </v-row>
    <v-row dense v-for="device in modbusSettings.devices" :key="device.address">
      <v-col><v-text-field v-model="device.name"></v-text-field></v-col>
      <v-col><v-text-field v-model="device.address"></v-text-field></v-col>
    </v-row>

    <!-- <v-text-field label="SSID" v-model="wifiSettings.ssid"></v-text-field>
    <v-text-field
      label="Password"
      v-model="wifiSettings.password"
      :type="wifiPasswordVisible ? 'text' : 'password'"
      :append-icon="getWiFiPasswordIcon()"
      @click:append="toggleWiFiPasswordVisible()"
    ></v-text-field> -->
    <v-btn
      v-if="modbusSettingsDirty"
      x-large
      color="success"
      @click="applyModbusConfig()"
      >Apply</v-btn
    >
  </v-container>
</template>

<script>
import { mapActions, mapGetters } from "vuex";
// import { mdiEye, mdiEyeOff } from "@mdi/js";
import { cloneDeep, isEqual } from "lodash";
// import axios from "axios";

export default {
  data() {
    return {
      modbusSettingsDirty: false,
      // wifiPasswordVisible: false,
      // wifiPasswordIcon: mdiEye,
      modbusSettings: {},
    };
  },

  created() {
    this.setTitle("Modbus");
    this.modbusSettings = cloneDeep(this.modbusConfig);
  },

  updated() {
    this.modbusSettingsDirty = !isEqual(this.modbusConfig, this.modbusSettings);
  },

  methods: {
    // ...mapActions(["setTitle", "saveWiFiState", "saveConfig"]),
    ...mapActions(["setTitle"]),

    // async saveWiFiConfigAndRestart() {
    //   await this.saveWiFiState(this.wifiSettings);
    //   await this.saveConfig();
    //   this.wifiResetRequired = false;
    //   await axios.post("/api/v1/restart");
    // },

    // getWiFiPasswordIcon() {
    //   return this.wifiPasswordVisible ? mdiEyeOff : mdiEye;
    // },

    // toggleWiFiPasswordVisible() {
    //   this.wifiPasswordVisible = !this.wifiPasswordVisible;
    // },
  },

  computed: {
    ...mapGetters(["modbusConfig"]),
  },
};
</script>
