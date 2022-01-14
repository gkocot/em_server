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
          <v-list-item v-for="(item, i) in drawer.items" :key="i">
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
        Container
        <!-- If using vue-router -->
        <router-view></router-view>
      </v-container>
    </v-main>
  </v-app>
</template>

<script>
export default {
  name: "App",

  data: () => ({
    drawer: {
      visible: false,
      selectedItem: 0,
      items: [
        { text: "Home", icon: "mdi-home" },
        { text: "WiFi", icon: "mdi-wifi-cog" },
        { text: "Energy Meter", icon: "mdi-home-lightning-bolt-outline" },
        { text: "IO", icon: "mdi-electric-switch" },
        { text: "ADC", icon: "mdi-sine-wave" },
      ],
    },
  }),
};
</script>
