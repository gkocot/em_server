<template>
  <div>ADC</div>
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
    this.setTitle("ADC");
  },
};
</script>
