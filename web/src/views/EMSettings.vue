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
    ...mapActions(["setTitleAction"]),
  },
  mounted() {
    this.setTitleAction("Energy Meter");
  },
};
</script>
