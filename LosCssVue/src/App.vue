<template>
  <div class="app-shell">
    <aside class="sidebar">
      <div class="brand-block">
        <h1>LosCSS</h1>
        <p>系统管理员后台</p>
      </div>

      <nav class="menu-list">
        <RouterLink
          v-for="item in menus"
          :key="item.key"
          :to="item.path"
          class="menu-item"
          active-class="is-active"
        >
          <span class="menu-title">{{ item.title }}</span>
          <small class="menu-desc">{{ item.description }}</small>
        </RouterLink>
      </nav>
    </aside>

    <main class="main-content">
      <section class="page-body">
        <RouterView />
      </section>
    </main>
  </div>
</template>

<script setup>
import { computed } from 'vue';
import { useRoute, RouterLink, RouterView } from 'vue-router';
import { moduleMenus, modules } from './stores/modules';

const route = useRoute();
const menus = moduleMenus;

const currentTitle = computed(() => route.meta?.title || 'LosCSS 管理后台');
const currentDescription = computed(() => {
  const match = Object.values(modules).find((item) => item.title === route.meta?.title);
  return match?.description || '集中管理农业业务全链路模块';
});
</script>
