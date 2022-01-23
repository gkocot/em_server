<template>
  <v-container>
    <v-select
      label="Mode"
      v-model="wifiSettings.mode"
      :items="wifiSettings.wifiModes"
    ></v-select>
    <v-text-field label="SSID" v-model="wifiSettings.ssid"></v-text-field>
    <v-text-field
      label="Password"
      v-model="wifiSettings.password"
      :type="wifiPasswordVisible ? 'text' : 'password'"
      :append-icon="getWiFiPasswordIcon()"
      @click:append="toggleWiFiPasswordVisible()"
    ></v-text-field>
    <v-btn
      v-if="wifiSettingsDirty"
      x-large
      color="success"
      @click="saveWiFiConfigAndRestart()"
      >Apply</v-btn
    >
  </v-container>
</template>

<script>
import { mapActions, mapGetters } from "vuex";
import { mdiEye, mdiEyeOff } from "@mdi/js";
import { cloneDeep, isEqual } from "lodash";
import axios from "axios";

export default {
  data() {
    return {
      wifiSettingsDirty: false,
      wifiPasswordVisible: false,
      wifiPasswordIcon: mdiEye,
      wifiSettings: {},
    };
  },

  created() {
    this.setTitle("WiFi");
    this.wifiSettings = cloneDeep(this.wifiConfig);
  },

  updated() {
    this.wifiSettingsDirty = !isEqual(this.wifiConfig, this.wifiSettings);
  },

  methods: {
    ...mapActions(["setTitle", "saveWiFiState", "saveConfig"]),

    async saveWiFiConfigAndRestart() {
      await this.saveWiFiState(this.wifiSettings);
      await this.saveConfig();
      this.wifiSettingsDirty = false;
      await axios.post("/api/v1/restart");
    },

    getWiFiPasswordIcon() {
      return this.wifiPasswordVisible ? mdiEyeOff : mdiEye;
    },

    toggleWiFiPasswordVisible() {
      this.wifiPasswordVisible = !this.wifiPasswordVisible;
    },
  },

  computed: {
    ...mapGetters(["wifiConfig"]),
  },
};
</script>
