<template>
  <v-container>
    <v-select label="Mode" v-model="wifi.mode" :items="wifi_modes"></v-select>
    <v-text-field label="SSID" v-model="wifi.ssid"></v-text-field>
    <v-text-field
      label="Password"
      v-model="wifi.password"
      :type="wifi_password_visible ? 'text' : 'password'"
      :append-icon="wifiPasswordIcon()"
      @click:append="() => (wifi_password_visible = !wifi_password_visible)"
    ></v-text-field>
  </v-container>
</template>

<script>
import { mapActions } from "vuex";
import { mdiEye, mdiEyeOff } from "@mdi/js";

const wifi_modes = [
  { text: "Access Point", value: 1 },
  { text: "Station", value: 2 },
];

export default {
  data() {
    console.log(mdiEye);
    console.log(mdiEyeOff);
    return {
      wifi_modes,
      wifi: {
        mode: 1,
        ssid: "myssid",
        password: "mypassword",
      },
      wifi_password_visible: false,
      wifi_password_icon: mdiEye,
    };
  },
  methods: {
    ...mapActions(["setTitleAction"]),
    getConfig() {
      this.$ajax
        .get("/api/v1/config")
        .then((resp) => {
          this.wifi = resp.data.wifi;
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
    this.setTitleAction("WiFi");
    this.getConfig();
  },
};
</script>
