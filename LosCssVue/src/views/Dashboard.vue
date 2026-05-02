<template>
  <div class="dashboard-page">
    <el-row :gutter="16">
      <el-col v-for="item in menus" :key="item.key" :xs="24" :sm="12" :lg="8">
        <el-card shadow="hover" class="dashboard-card">
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
</template>

<script setup>
import { RouterLink } from "vue-router";
import { moduleMenus as menus } from "../stores/modules";
</script>
