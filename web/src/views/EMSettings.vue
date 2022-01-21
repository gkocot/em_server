<template>
  <div>Energy Meter Settings</div>
</template>

<script>
import { mapActions } from "vuex";

export default {
  data() {
    return {};
  },
  methods: {
    ...mapActions(["setTitle"]),
  },
  mounted() {
    this.setTitle("Energy Meter");
  },
};
</script>
