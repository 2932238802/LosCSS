import { createRouter, createWebHashHistory } from 'vue-router';
import Dashboard from '../views/Dashboard.vue';
import ModulePage from '../views/ModulePage.vue';

const routes = [
  { path: '/', name: 'Dashboard', component: Dashboard, meta: { title: '首页' } },
  { path: '/seed-sources', name: 'SeedSources', component: ModulePage, props: { moduleKey: 'seedSources' }, meta: { title: '种源管理' } },
  { path: '/seedlings', name: 'Seedlings', component: ModulePage, props: { moduleKey: 'seedlings' }, meta: { title: '育苗管理' } },
  { path: '/planting-areas', name: 'PlantingAreas', component: ModulePage, props: { moduleKey: 'plantingAreas' }, meta: { title: '种植地块管理' } },
  { path: '/farming-operations', name: 'FarmingOperations', component: ModulePage, props: { moduleKey: 'farmingOperations' }, meta: { title: '农事操作管理' } },
  { path: '/harvests', name: 'Harvests', component: ModulePage, props: { moduleKey: 'harvests' }, meta: { title: '采收管理' } },
  { path: '/raw-material-ins', name: 'RawMaterialIns', component: ModulePage, props: { moduleKey: 'rawMaterialIns' }, meta: { title: '原料入库管理' } },
  { path: '/processing-flows', name: 'ProcessingFlows', component: ModulePage, props: { moduleKey: 'processingFlows' }, meta: { title: '加工流程管理' } },
  { path: '/quality-checks', name: 'QualityChecks', component: ModulePage, props: { moduleKey: 'qualityChecks' }, meta: { title: '质检管理' } }
];

export default createRouter({
  history: createWebHashHistory(),
  routes
});
