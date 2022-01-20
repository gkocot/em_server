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
    ...mapActions(["setTitleAction"]),
  },
  mounted() {
    this.setTitleAction("ADC");
  },
};
</script>
