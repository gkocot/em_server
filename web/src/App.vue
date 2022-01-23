<template>
  <v-app>
    <v-app-bar app clipped-left>
      <v-app-bar-nav-icon
        @click.stop="drawer.visible = !drawer.visible"
      ></v-app-bar-nav-icon>
      <v-toolbar-title>{{ title }}</v-toolbar-title>
      <div style="flex-grow:1"></div>
      <img src="./assets/emlogo.svg" style="height:100%;padding:10px 0px" />
    </v-app-bar>
    <v-navigation-drawer app clipped v-model="drawer.visible">
      <v-list dense>
        <v-list-item-group v-model="drawer.selectedItem" color="primary">
          <v-list-item
            v-for="(item, i) in drawer.items"
            :key="i"
            :to="item.route"
          >
            <v-list-item-icon>
              <v-icon v-text="item.icon"></v-icon>
            </v-list-item-icon>
            <v-list-item-content>
              <v-list-item-title v-text="item.text"></v-list-item-title>
            </v-list-item-content>
          </v-list-item>
        </v-list-item-group>
      </v-list>
    </v-navigation-drawer>

    <!-- Sizes your content based upon application components -->
    <v-main>
      <!-- Provides the application the proper gutter -->
      <v-container fluid>
        <router-view v-if="isConfigLoaded"></router-view>
        <div v-else class="text-center">
          <div class="pa-16">
            <v-progress-circular
              :size="150"
              color="red"
              indeterminate
            ></v-progress-circular>
          </div>
          <span class="grey--text">Loading configuration...</span>
        </div>
      </v-container>
    </v-main>
    <!-- <div style="display:flex;background-color:rgb(245,245,245)">
      <div style="flex-grow:1"></div>
      <img src="./assets/emlogo.svg" height="50" style="max-height: 100%"/>
      <div style="flex-grow:1"></div>
    </div> -->
  </v-app>
</template>

<script>
import {
  mdiHome,
  mdiWifiCog,
  mdiLan,
  mdiHomeLightningBoltOutline,
  mdiElectricSwitch,
  mdiSineWave,
} from "@mdi/js";
import { mapState, mapActions } from "vuex";

export default {
  name: "App",

  data() {
    return {
      isConfigLoaded: false,
      config: {},
      drawer: {
        visible: null,
        selectedItem: 0,
        items: [
          { text: "Home", icon: mdiHome, route: "/" },
          { text: "WiFi", icon: mdiWifiCog, route: "/wifi" },
          { text: "Modbus", icon: mdiLan, route: "/modbus" },
          {
            text: "Energy Meter",
            icon: mdiHomeLightningBoltOutline,
            route: "/em",
          },
          { text: "IO", icon: mdiElectricSwitch, route: "/io" },
          { text: "ADC", icon: mdiSineWave, route: "/adc" },
        ],
      },
    };
  },

  async mounted() {
    await this.loadConfig();
    this.isConfigLoaded = true;
  },

  methods: {
    ...mapActions(["loadConfig"]),
  },

  computed: {
    ...mapState(["title"]),
  },
};
</script>
