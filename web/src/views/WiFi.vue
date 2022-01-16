<template>
  <v-container>
    <v-select label="Mode" v-model="wifi_mode" :items="wifi_modes"></v-select>
    <v-text-field label="SSID" v-model="wifi_ssid"></v-text-field>
    <v-text-field
      label="Password"
      v-model="wifi_password"
      :type="wifi_password_visible ? 'text' : 'password'"
      :append-icon="wifiPasswordIcon()"
      @click:append="() => (wifi_password_visible = !wifi_password_visible)"
    ></v-text-field>
  </v-container>
</template>

<script>
import { mdiEye, mdiEyeOff } from "@mdi/js";

export default {
  data() {
    console.log(mdiEye);
    console.log(mdiEyeOff);
    return {
      wifi_modes: ["Access Point", "Station"],
      wifi_mode: "Access Point",
      wifi_ssid: "myssid",
      wifi_password: "mypassword",
      wifi_password_visible: false,
      wifi_password_icon: mdiEye,
    };
  },
  methods: {
    getConfig() {
      this.$ajax
        .get("/api/v1/config")
        .then((data) => {
          console.log(data);
        })
        .catch((error) => {
          console.log(error);
        });
    },
    wifiPasswordIcon() {
      return this.wifi_password_visible ? mdiEyeOff : mdiEye;
    },
  },
  mounted() {
    this.getConfig();
  },
};
</script>
