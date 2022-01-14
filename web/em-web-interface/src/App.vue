<template>
  <v-app>
    <v-app-bar app clipped-left>
      <v-app-bar-nav-icon
        @click.stop="drawer.visible = !drawer.visible"
      ></v-app-bar-nav-icon>
      <v-toolbar-title>Energy Meter</v-toolbar-title>
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
        <!-- If using vue-router -->
        <router-view></router-view>
      </v-container>
    </v-main>
  </v-app>
</template>

<script>
import { mdiHome, mdiWifiCog, mdiHomeLightningBoltOutline, mdiElectricSwitch, mdiSineWave } from "@mdi/js";

export default {
  name: "App",

  data: () => ({
    drawer: {
      visible: false,
      selectedItem: 0,
      items: [
        { text: "Home", icon: mdiHome, route: "/" },
        { text: "WiFi", icon: mdiWifiCog, route: "/wifi" },
        {
          text: "Energy Meter",
          icon: mdiHomeLightningBoltOutline,
          route: "/em",
        },
        { text: "IO", icon: mdiElectricSwitch, route: "/io" },
        { text: "ADC", icon: mdiSineWave, route: "/adc" },
      ],
    },
  }),
};
</script>
