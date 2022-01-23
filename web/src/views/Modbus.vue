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
import { cloneDeep, isEqual } from "lodash";

export default {
  data() {
    return {
      modbusSettingsDirty: false,
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
    ...mapActions(["setTitle", "saveModbusState", "saveModbus"]),
    ...mapActions(["setTitle"]),

    async applyModbusConfig() {
      await this.saveModbusState(this.modbusSettings);
      await this.saveModbus();
      this.modbusSettingsDirty = false;
    },
  },

  computed: {
    ...mapGetters(["modbusConfig"]),
  },
};
</script>
