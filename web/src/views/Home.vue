<template>
  <v-container>
    <!-- <v-layout text-xs-center wrap>
      <v-flex xs12 sm6 offset-sm3>
        <v-card>
          <v-img
            :src="require('../assets/emlogo.svg')"
            contain
            height="50"
          ></v-img>
          <v-card-title primary-title>
            <div class="ma-auto">
              <span class="grey--text">IDF version: {{ version }}</span>
              <br />
              <span class="grey--text">ESP cores: {{ cores }}</span>
            </div>
          </v-card-title>
        </v-card>
      </v-flex>
    </v-layout> -->
    
    <!-- LOGO -->
    <!-- <div style="display:flex">
      <div style="flex-grow:1"></div>
      <img src="../assets/emlogo.svg" style="display:block"/>
      <div style="flex-grow:1"></div>
    </div> -->
    
    <span >IDF version: {{ version }}</span><br />
    <span >ESP cores: {{ cores }}</span>
  </v-container>
</template>

<script>
import { mapActions } from "vuex";

export default {
  data() {
    return {
      version: null,
      cores: null,
    };
  },
  methods: {
    ...mapActions(["setTitle"]),
  },
  mounted() {
    this.setTitle("Home");
    // TBD Move to store
    this.$ajax
      .get("/api/v1/system_info")
      .then((data) => {
        this.version = data.data.version;
        this.cores = data.data.cores;
      })
      .catch((error) => {
        console.log(error);
      });
  },
};
</script>
