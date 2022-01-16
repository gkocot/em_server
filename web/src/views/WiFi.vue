<template>
  <div>
    <div>WiFi</div>
    <button @click="getConfig()">Get Config</button>
  </div>
</template>

<script>
export default {
  data() {
    return {};
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
  },
  mounted() {
    this.getConfig();
  },
};
</script>
