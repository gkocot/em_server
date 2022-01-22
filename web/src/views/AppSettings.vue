<template>
  <div>App Settings e.g. localization</div>
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
    this.setTitle("IO");
  },
};
</script>
