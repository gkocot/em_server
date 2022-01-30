<template>
  <Throbber :loaded="modbusSettingsLoaded">
    <v-expansion-panels multiple accordion>
      <v-expansion-panel
        v-for="device in modbusSettings.devices"
        :key="device.id"
      >
        <v-expansion-panel-header>
          Device {{ device.id }} {{ device.master ? "Master" : "Slave" }}
        </v-expansion-panel-header>
        <v-expansion-panel-content>
          <v-select
            label="Type"
            v-model="device.type"
            :items="getAllowedDeviceTypes(device)"
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
          <v-select
            label="Parrity"
            v-model="device.parrity"
            :items="modbusSettings.parrityOptions"
          ></v-select>
          <v-text-field
            label="Address"
            type="number"
            min="0"
            max="247"
            v-model.number="device.address"
          ></v-text-field>
          <v-checkbox
            class="masterSlaveCheckbox"
            v-model="device.master"
            :disabled="device.master"
            label="Master"
            @change="setOtherDevicesToSlaveMode(device)"
          ></v-checkbox>
        </v-expansion-panel-content>
      </v-expansion-panel>
    </v-expansion-panels>
    <!-- TBD No inline styles -->
    <div style="padding-top: 10px; float: right">
      <v-btn
        v-if="modbusSettingsDirty"
        x-large
        color="success"
        @click="applyModbusSettings()"
        >Apply</v-btn
      >
    </div>
  </Throbber>
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
    // console.log(JSON.stringify(this.modbusSettings.devices)); // TBD remove debug code
  },

  methods: {
    ...mapActions(["loadModbusConfig", "saveModbusConfig", "setTitle"]),

    async applyModbusSettings() {
      await this.saveModbusConfig(this.modbusSettings);
      this.modbusSettingsDirty = false;
    },

    getAllowedDeviceTypes: function (device) {
      const allowedDeviceTypes = [];
      for (const element of this.modbusSettings.deviceTypes) {
        if (element.masterCapable || !device.master) {
          allowedDeviceTypes.push({ value: element.id, text: element.name });
        }
      }

      if (
        allowedDeviceTypes.filter((e) => e.value === device.type).length === 0
      ) {
        device.type = allowedDeviceTypes[0].value;
      }

      return allowedDeviceTypes;
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

    setOtherDevicesToSlaveMode(device) {
      for (const element of this.modbusSettings.devices) {
        if (element.id !== device.id) {
          element.master = false;
        }
      }
    },
  },

  computed: {
    ...mapGetters(["modbusConfig"]),
  },
};
</script>

<style scoped>
/* TBD color variables common for whole project */
.theme--light.v-input--is-disabled.masterSlaveCheckbox {
  color: rgb(25, 118, 210);
}
</style>
