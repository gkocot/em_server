import Vue from "vue";
import Vuex from "vuex";

Vue.use(Vuex);

export default new Vuex.Store({
  state: {
    title: "Home",
  },
  mutations: {
    setTitle(state, title) {
      state.title = title;
    },
  },
  actions: {
    setTitleAction({ commit }, title) {
      commit("setTitle", title);
    },
  },
  modules: {},
});
