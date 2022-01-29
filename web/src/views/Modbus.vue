<template>
  <v-container>
    <Throbber :loaded="modbusSettingsLoaded">
      <div v-for="device in modbusSettings.devices" :key="device.id">
        <v-card>
          <!-- TBD No inline styles! -->
          <div style="padding: 10px">
            <div>Device {{ device.id }}</div>
            <v-select
              label="Type"
              v-model="device.type"
              :items="getDeviceTypes"
            ></v-select>
            <v-select
              label="Speed"
              v-model="device.speed"
              :items="getAllowedDeviceSpeeds(device)"
            ></v-select>
            <v-select
              label="Stop Bits"
              v-model="device.stopBits"
              :items="getAllowedDeviceStopBits(device)"
            ></v-select>
          </div>
        </v-card>
      </div>
    </Throbber>
  </v-container>
</template>

<script>
import Throbber from "../components/Throbber.vue";
import { mapActions, mapGetters } from "vuex";
import { cloneDeep, isEqual } from "lodash";

export default {
  components: {
    Throbber,
  },

  data() {
    return {
      //   experimentValue: false,
      modbusSettingsLoaded: false,
      modbusSettingsDirty: false,
      modbusSettings: {},
    };
  },

  async created() {
    this.setTitle("Modbus");
    await this.loadModbusConfig();
    this.modbusSettings = cloneDeep(this.modbusConfig);
    // console.log(JSON.stringify(this.modbusSettings)); // TBD remove debug code
    this.modbusSettingsLoaded = true;
  },

  updated() {
    this.modbusSettingsDirty = !isEqual(this.modbusConfig, this.modbusSettings);
    console.log(JSON.stringify(this.modbusSettings.devices));
  },

  methods: {
    ...mapActions(["loadModbusConfig", "saveModbusConfig", "setTitle"]),

    async applyModbusSettings() {
      await this.saveModbusConfig(this.modbusSettings);
      this.modbusSettingsDirty = false;
    },

    getAllowedDeviceSpeeds: function (device) {
      const allowedSpeeds = this.modbusSettings.deviceTypes[device.type].speeds;
      if (!allowedSpeeds.includes(device.speed)) {
        device.speed = allowedSpeeds[0];
      }
      return allowedSpeeds;
    },

    getAllowedDeviceStopBits: function (device) {
      const allowedStopBits =
        this.modbusSettings.deviceTypes[device.type].stopBits;
      if (!allowedStopBits.includes(device.stopBits)) {
        device.stopBits = allowedStopBits[0];
      }
      return allowedStopBits;
    },

    // experimentButtonClick() {
    //   this.experimentValue = !this.experimentValue;
    // },
  },

  computed: {
    ...mapGetters(["modbusConfig"]),

    getDeviceTypes: function () {
      return this.modbusSettings.deviceTypes.map((t) => {
        return { value: t.id, text: t.name };
      });
    },
  },
};
</script>
