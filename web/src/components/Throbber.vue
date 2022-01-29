<template>
  <div v-if="loaded">
    <slot></slot>
  </div>
  <div v-else class="text-center">
    <v-container>
      <div class="pa-16">
        <v-progress-circular
          :size="150"
          color="red"
          indeterminate
        ></v-progress-circular>
      </div>
      <span class="grey--text">Loading configuration...</span>
    </v-container>
  </div>
</template>

<script>
export default {
  props: {
    loaded: {
      type: Boolean,
      required: true,
    },
  },

  data() {
    return {};
  },
};
</script>
