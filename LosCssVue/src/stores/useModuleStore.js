import { defineStore } from 'pinia';
import { computed, ref } from 'vue';
import { modules } from './modules';

export const useModuleStore = defineStore('module-store', () => {
  const activeModuleKey = ref('seedSources');
  const keyword = ref('');

  const activeModule = computed(() => modules[activeModuleKey.value]);

  function setActiveModule(key) {
    if (modules[key]) {
      activeModuleKey.value = key;
      keyword.value = '';
    }
  }

  function setKeyword(value) {
    keyword.value = value;
  }

  return {
    modules,
    activeModuleKey,
    activeModule,
    keyword,
    setActiveModule,
    setKeyword
  };
});
