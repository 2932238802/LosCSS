<template>
  <div class="module-page">
    <el-card shadow="never" class="toolbar-card">
      <div class="toolbar-row">
        <div>
          <h3>{{ config.title }}</h3>
          <p>{{ config.description }}</p>
        </div>
        <div class="toolbar-actions">
          <el-input
            v-model="keyword"
            clearable
            placeholder="输入关键字筛选当前列表"
            style="width: 260px"
          />
          <el-button type="primary" @click="openCreateDialog">新增</el-button>
          <el-button @click="loadList">刷新</el-button>
        </div>
      </div>
    </el-card>

    <el-card shadow="never">
      <el-alert
        v-if="errorMessage"
        :title="errorMessage"
        type="error"
        show-icon
        :closable="false"
        class="mb-16"
      />

      <el-table
        :data="filteredList"
        border
        stripe
        v-loading="loading"
        style="width: 100%"
      >
        <el-table-column
          v-for="column in config.columns"
          :key="column.field"
          :prop="column.field"
          :label="column.label"
          min-width="140"
        />
        <el-table-column label="操作" width="180" fixed="right">
          <template #default="scope">
            <el-button size="small" @click="openEditDialog(scope.row)"
              >编辑</el-button
            >
            <el-button
              size="small"
              type="danger"
              plain
              @click="removeItem(scope.row)"
              >删除</el-button
            >
          </template>
        </el-table-column>
      </el-table>
    </el-card>

    <el-dialog v-model="dialogVisible" :title="dialogTitle" width="760px">
      <el-form label-width="120px">
        <el-row :gutter="16">
          <el-col v-for="field in config.fields" :key="field.field" :span="12">
            <el-form-item :label="field.label" :required="field.required">
              <el-input
                v-if="
                  !field.type ||
                  ['text', 'number', 'date', 'datetime-local'].includes(
                    field.type,
                  )
                "
                v-model="formModel[field.field]"
                :type="mapInputType(field.type)"
              />
              <el-input
                v-else-if="field.type === 'textarea'"
                v-model="formModel[field.field]"
                type="textarea"
                :rows="3"
              />
              <el-select
                v-else-if="field.type === 'select'"
                v-model="formModel[field.field]"
                style="width: 100%"
              >
                <el-option
                  v-for="option in field.options || []"
                  :key="option"
                  :label="option"
                  :value="option"
                />
              </el-select>
              <el-input v-else v-model="formModel[field.field]" />
            </el-form-item>
          </el-col>
        </el-row>
      </el-form>

      <template #footer>
        <el-button @click="dialogVisible = false">取消</el-button>
        <el-button type="primary" :loading="saving" @click="submitForm"
          >保存</el-button
        >
      </template>
    </el-dialog>
  </div>
</template>

<script setup>
import { computed, reactive, ref, watch } from "vue";
import { ElMessage, ElMessageBox } from "element-plus";
import { modules } from "../stores/modules";
import {
  seedSourceApi,
  seedlingApi,
  plantingAreaApi,
  farmingOperationApi,
  harvestApi,
  rawMaterialInApi,
  processingFlowApi,
  qualityCheckApi,
} from "../api/modules";

const props = defineProps({
  moduleKey: {
    type: String,
    required: true,
  },
});

const apiMap = {
  seedSources: seedSourceApi,
  seedlings: seedlingApi,
  plantingAreas: plantingAreaApi,
  farmingOperations: farmingOperationApi,
  harvests: harvestApi,
  rawMaterialIns: rawMaterialInApi,
  processingFlows: processingFlowApi,
  qualityChecks: qualityCheckApi,
};

const loading = ref(false);
const saving = ref(false);
const dialogVisible = ref(false);
const isEditing = ref(false);
const keyword = ref("");
const errorMessage = ref("");
const list = ref([]);
const formModel = reactive({});

const config = computed(() => modules[props.moduleKey]);
const currentApi = computed(() => apiMap[props.moduleKey]);
const dialogTitle = computed(
  () => `${isEditing.value ? "编辑" : "新增"}${config.value.title}`,
);

const filteredList = computed(() => {
  const term = keyword.value.trim().toLowerCase();
  if (!term) return list.value;

  return list.value.filter((item) =>
    Object.values(item || {}).some((value) =>
      String(value ?? "")
        .toLowerCase()
        .includes(term),
    ),
  );
});

function resetForm() {
  Object.keys(formModel).forEach((key) => delete formModel[key]);
  Object.assign(
    formModel,
    JSON.parse(JSON.stringify(config.value.defaultForm)),
  );
}

function normalizeListPayload(payload) {
  if (Array.isArray(payload)) return payload;
  if (Array.isArray(payload?.items)) return payload.items;
  if (Array.isArray(payload?.data)) return payload.data;
  return [];
}

async function loadList() {
  loading.value = true;
  errorMessage.value = "";

  try {
    const response = await currentApi.value.list();
    list.value = normalizeListPayload(response);
  } catch (error) {
    console.error(error);
    errorMessage.value = `${config.value.title}列表加载失败，请检查后端接口或代理配置。`;
    list.value = [];
  } finally {
    loading.value = false;
  }
}

function openCreateDialog() {
  resetForm();
  isEditing.value = false;
  dialogVisible.value = true;
}

function openEditDialog(row) {
  resetForm();
  Object.assign(formModel, JSON.parse(JSON.stringify(row)));
  isEditing.value = true;
  dialogVisible.value = true;
}

function buildSubmitPayload() {
  return JSON.parse(JSON.stringify(formModel));
}

async function submitForm() {
  saving.value = true;
  try {
    const payload = buildSubmitPayload();
    const idField = config.value.idField || "id";
    const id = payload[idField];

    if (isEditing.value && id !== null && id !== undefined && id !== "") {
      await currentApi.value.update(id, payload);
      ElMessage.success("更新成功");
    } else {
      delete payload[idField];
      await currentApi.value.create(payload);
      ElMessage.success("新增成功");
    }

    dialogVisible.value = false;
    await loadList();
  } catch (error) {
    console.error(error);
    ElMessage.error("保存失败，请检查接口参数是否与后端一致");
  } finally {
    saving.value = false;
  }
}

async function removeItem(row) {
  const idField = config.value.idField || "id";
  const id = row?.[idField];
  if (id === null || id === undefined || id === "") {
    ElMessage.warning("当前记录缺少主键，无法删除");
    return;
  }

  try {
    await ElMessageBox.confirm(
      `确认删除当前${config.value.title}记录吗？`,
      "删除确认",
      {
        type: "warning",
      },
    );
    await currentApi.value.remove(id);
    ElMessage.success("删除成功");
    await loadList();
  } catch (error) {
    if (error === "cancel" || error === "close") return;
    console.error(error);
    ElMessage.error("删除失败，请检查接口是否支持该删除路径");
  }
}

function mapInputType(type) {
  if (type === "datetime-local") return "datetime-local";
  if (type === "number") return "number";
  if (type === "date") return "date";
  return "text";
}

watch(
  () => props.moduleKey,
  () => {
    keyword.value = "";
    resetForm();
    loadList();
  },
  { immediate: true },
);
</script>
