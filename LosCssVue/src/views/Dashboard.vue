<template>
  <div class="dashboard-page" :style="{ backgroundImage: `url(${backgroundUrl})` }">
    <div class="dashboard-overlay">
      <div class="dashboard-hero">
        <h1 class="hero-title">LosCSS 中药材溯源管理平台</h1>
        <p class="hero-subtitle">从种源到质检，全产业链可追溯 · 数字化农业管理</p>
      </div>

      <el-row :gutter="16" class="dashboard-grid">
        <el-col v-for="item in menus" :key="item.key" :xs="24" :sm="12" :lg="8">
          <el-card shadow="hover" class="dashboard-card glass-card">
            <template #header>
              <div class="card-header">
                <span>{{ item.title }}</span>
                <el-tag type="success">业务模块</el-tag>
              </div>
            </template>
            <p>{{ item.description }}</p>
            <RouterLink :to="item.path">
              <el-button type="primary" plain>进入模块</el-button>
            </RouterLink>
          </el-card>
        </el-col>
      </el-row>
    </div>
  </div>
</template>

<script setup>
import { RouterLink } from "vue-router";
import { moduleMenus as menus } from "../stores/modules";
import backgroundUrl from "../assets/background.jpg";
</script>

<style scoped>
.dashboard-page {
  position: relative;
  min-height: calc(100vh - 48px);
  margin: -24px;
  padding: 0;
  background-size: cover;
  background-position: center;
  background-repeat: no-repeat;
  background-attachment: fixed;
}

.dashboard-overlay {
  min-height: calc(100vh - 48px);
  padding: 48px 32px 32px;
  background: linear-gradient(
    180deg,
    rgba(255, 255, 255, 0.55) 0%,
    rgba(255, 255, 255, 0.75) 100%
  );
  backdrop-filter: saturate(1.1);
}

.dashboard-hero {
  margin-bottom: 32px;
  padding: 24px 28px;
  border-radius: 16px;
  background: rgba(255, 255, 255, 0.55);
  backdrop-filter: blur(16px) saturate(1.2);
  -webkit-backdrop-filter: blur(16px) saturate(1.2);
  border: 1px solid rgba(255, 255, 255, 0.6);
  box-shadow: 0 6px 24px rgba(15, 23, 42, 0.08);
}

.hero-title {
  margin: 0 0 8px;
  font-size: 30px;
  font-weight: 700;
  color: #0f172a;
  letter-spacing: 0.5px;
}

.hero-subtitle {
  margin: 0;
  font-size: 15px;
  color: #475569;
}

.dashboard-grid {
  margin: 0;
}

.glass-card {
  background: rgba(255, 255, 255, 0.55) !important;
  backdrop-filter: blur(14px) saturate(1.2);
  -webkit-backdrop-filter: blur(14px) saturate(1.2);
  border: 1px solid rgba(255, 255, 255, 0.6) !important;
  box-shadow: 0 4px 20px rgba(15, 23, 42, 0.06) !important;
  transition: transform 0.2s ease, box-shadow 0.2s ease;
}

.glass-card:hover {
  transform: translateY(-2px);
  box-shadow: 0 10px 28px rgba(15, 23, 42, 0.12) !important;
}

.glass-card :deep(.el-card__header) {
  background: rgba(255, 255, 255, 0.35);
  border-bottom: 1px solid rgba(255, 255, 255, 0.5);
}

.glass-card :deep(.el-card__body) {
  background: transparent;
}
</style>
<!-- 【修改结束】 -->
