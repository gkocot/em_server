<template>
  <v-container>
    <Throbber :loaded="modbusSettingsLoaded">
      {{ modbusSettings }}
      <!-- <v-select
      label="Master"
      v-model="modbusSettings.master.id"
      :items="masterDeviceTypes()"
    ></v-select> -->

      <!-- <v-select
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

    <v-btn
      v-if="modbusSettingsDirty"
      x-large
      color="success"
      @click="applyModbusSettings()"
      >Apply</v-btn
    > -->
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
    console.log("Modbus created");
    this.setTitle("Modbus");
    await this.loadModbusConfig();
    this.modbusSettings = cloneDeep(this.modbusConfig);
    console.log(JSON.stringify(this.modbusSettings));
    this.modbusSettingsLoaded = true;
  },

  updated() {
    this.modbusSettingsDirty = !isEqual(this.modbusConfig, this.modbusSettings);
  },

  methods: {
    ...mapActions(["loadModbusConfig", "saveModbusConfig", "setTitle"]),

    async applyModbusSettings() {
      await this.saveModbusConfig(this.modbusSettings);
      this.modbusSettingsDirty = false;
    },

    // experimentButtonClick() {
    //   this.experimentValue = !this.experimentValue;
    // },
  },

  computed: {
    ...mapGetters(["modbusConfig"]),

    masterDeviceTypes() {
      return this.modbusSettings.deviceTypes.filter((t) => t.masterCapable);
    },
  },
};
</script>
