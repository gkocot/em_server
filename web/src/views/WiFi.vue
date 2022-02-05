<template>
  <Throbber :loaded="wifiSettingsLoaded">
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
      :rules="wifiPasswordRules"
    ></v-text-field>
    <div style="padding-top: 10px; float: right">
      <v-btn
        v-if="wifiSettingsDirty && wifiSettingsValid"
        x-large
        color="success"
        @click="applyWiFiSettings()"
        >Apply</v-btn
      >
    </div>
  </Throbber>
</template>

<script>
import Throbber from "../components/Throbber.vue";
import { mapActions, mapGetters } from "vuex";
import { mdiEye, mdiEyeOff } from "@mdi/js";
import { cloneDeep, isEqual } from "lodash";
import axios from "axios";

export default {
  components: {
    Throbber,
  },

  data() {
    return {
      wifiSettingsLoaded: false,
      wifiSettingsValid: true,
      wifiSettingsDirty: false,
      wifiPasswordVisible: false,
      wifiPasswordIcon: mdiEye,
      wifiPasswordRules: [
        (wifiPassword) => {
          this.wifiSettingsValid = wifiPassword.length >= 8;
          return this.wifiSettingsValid || "min. 8 characters";
        },
      ],
      wifiSettings: {},
    };
  },

  async created() {
    this.setTitle("WiFi");
    await this.loadWiFiConfig();
    this.wifiSettings = cloneDeep(this.wifiConfig);
    this.wifiSettingsLoaded = true;
  },

  updated() {
    this.wifiSettingsDirty = !isEqual(this.wifiConfig, this.wifiSettings);
  },

  methods: {
    ...mapActions(["loadWiFiConfig", "saveWiFiConfig", "setTitle"]),

    async applyWiFiSettings() {
      await this.saveWiFiConfig(this.wifiSettings);
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
