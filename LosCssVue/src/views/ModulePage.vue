<template>
  <div class="module-page">
    <el-card shadow="never" class="toolbar-card">
      <div class="toolbar-row">
        <div class="toolbar-title-block">
          <el-button
            :icon="HomeFilled"
            size="small"
            plain
            class="back-home-btn"
            @click="goHome"
          >
            返回首页
          </el-button>
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
        >
          <template #default="scope">
            {{ formatForeignCell(scope.row, column) }}
          </template>
        </el-table-column>
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
                  ['text', 'number', 'decimal', 'date', 'datetime-local'].includes(
                    field.type,
                  )
                "
                v-model="formModel[field.field]"
                :type="mapInputType(field.type)"
                :min="field.min"
                :max="field.max"
                :step="field.step"
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
              <el-select
                v-else-if="field.type === 'foreign'"
                v-model="formModel[field.field]"
                filterable
                clearable
                placeholder="请选择"
                style="width: 100%"
              >
                <el-option
                  v-for="opt in foreignOptionsMap[field.ref] || []"
                  :key="opt.id"
                  :value="opt.id"
                  :label="opt[field.refLabel] || `#${opt.id}`"
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
import { useRouter } from "vue-router";
import { HomeFilled } from "@element-plus/icons-vue";
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

const router = useRouter();
function goHome() {
  router.push("/");
}

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

// 外键候选项缓存：{ seedSources: [{id, sourceName, ...}], plantingAreas: [...] }
// 当字段元数据有 type='foreign' + ref 时，会从这里取下拉选项
const foreignOptionsMap = reactive({});

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
  // 拦截器已经剥到 body.data：标准列表结构是 { items, total, page, pageSize }
  if (Array.isArray(payload?.items)) return payload.items;
  // 兼容退化情况：直接是数组
  if (Array.isArray(payload)) return payload;
  // 兼容旧格式：{ data: [...] }
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
    const reason = error?.message || "未知错误";
    errorMessage.value = `${config.value.title}列表加载失败：${reason}`;
    list.value = [];
  } finally {
    loading.value = false;
  }
}

/**
 * 拉取所有 foreign 字段引用模块的列表，缓存到 foreignOptionsMap
 * 例如：seedlings 模块有 seedId 引用 seedSources，就会把 seedSources 的列表拉下来
 */
async function loadForeignOptions() {
  const fields = config.value.fields || [];
  const refKeys = new Set(
    fields.filter((f) => f.type === "foreign" && f.ref).map((f) => f.ref),
  );

  for (const refKey of refKeys) {
    const refApi = apiMap[refKey];
    if (!refApi) continue;
    try {
      // 拉一页较多的数据用作下拉，足够覆盖大多数业务量
      const resp = await refApi.list({ page: 1, pageSize: 200 });
      foreignOptionsMap[refKey] = normalizeListPayload(resp);
    } catch (error) {
      console.error(`加载外键模块 ${refKey} 列表失败：`, error);
      foreignOptionsMap[refKey] = [];
    }
  }
}

/**
 * 把外键 id 渲染成对应的"业务名称"，用于列表表格显示
 * 业务约定：用户看到的是名称，不暴露内部 id
 */
function formatForeignCell(row, column) {
  const fieldDef = (config.value.fields || []).find(
    (f) => f.field === column.field,
  );
  if (!fieldDef || fieldDef.type !== "foreign") {
    return row[column.field];
  }
  const id = row[column.field];
  if (id === null || id === undefined || id === "") return "";
  const list = foreignOptionsMap[fieldDef.ref] || [];
  const matched = list.find((item) => item.id === id);
  if (!matched) return `# ${id}（已删除）`;
  return matched[fieldDef.refLabel] ?? `# ${id}`;
}

function openCreateDialog() {
  resetForm();
  isEditing.value = false;
  dialogVisible.value = true;
  // 打开新增对话框时，刷新外键候选列表，保证下拉拿到最新数据
  loadForeignOptions();
}

function openEditDialog(row) {
  resetForm();
  Object.assign(formModel, JSON.parse(JSON.stringify(row)));
  isEditing.value = true;
  dialogVisible.value = true;
  loadForeignOptions();
}

function buildSubmitPayload() {
  // 深拷贝表单值，并清理空值 + 类型修正 + 范围校验
  // 业务约定：'' / null / undefined 表示"用户没填"，从请求体里剔除，
  // 让后端走 NULL 或默认值；而 0、false 是合法输入要保留。
  const raw = JSON.parse(JSON.stringify(formModel));

  // type='number' 的字段对应数据库 INT 列，C++ 端是 int32_t，必须发数字
  // type='decimal' 的字段对应数据库 DECIMAL 列，Drogon 把它映射成 std::string，必须发字符串
  // （Element Plus el-input 不论 type 是什么，v-model 默认都绑成字符串）
  const fieldMap = new Map();
  for (const f of config.value.fields || []) {
    fieldMap.set(f.field, f);
  }

  const payload = {};
  for (const [k, v] of Object.entries(raw)) {
    if (v === "" || v === null || v === undefined) continue;

    const field = fieldMap.get(k);
    const type = field?.type;

    if (type === "number" || type === "decimal" || type === "foreign") {
      const n = Number(v);
      if (Number.isNaN(n)) continue;

      // 范围校验（foreign 也复用这套）
      if (field?.min !== undefined && n < field.min) {
        throw new Error(`「${field.label}」不能小于 ${field.min}`);
      }
      if (field?.max !== undefined && n > field.max) {
        throw new Error(`「${field.label}」不能大于 ${field.max}`);
      }

      // INT / 外键 id 字段必须是整数
      if ((type === "number" || type === "foreign") && !Number.isInteger(n)) {
        throw new Error(`「${field.label}」必须是整数`);
      }

      payload[k] = type === "decimal" ? String(v) : n;
      continue;
    }

    payload[k] = v;
  }
  return payload;
}

async function submitForm() {
  saving.value = true;
  try {
    const payload = buildSubmitPayload();
    const idField = config.value.idField || "id";
    const id = payload[idField];

    // 临时调试：打印实际提交的 payload（可在浏览器 console 查看）
    console.log("[submitForm] module:", props.moduleKey, "payload:", JSON.parse(JSON.stringify(payload)));

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
    const msg = error?.message || "保存失败";
    ElMessage.error(msg);
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
    const msg = error?.message || "删除失败";
    ElMessage.error(msg);
  }
}

function mapInputType(type) {
  if (type === "datetime-local") return "datetime-local";
  if (type === "number") return "number";
  if (type === "decimal") return "number";
  if (type === "date") return "date";
  return "text";
}

watch(
  () => props.moduleKey,
  () => {
    keyword.value = "";
    resetForm();
    loadList();
    loadForeignOptions();
  },
  { immediate: true },
);
</script>

<!-- 【修改开始】 -->
<style scoped>
.toolbar-title-block {
  display: flex;
  flex-direction: column;
  gap: 6px;
}

.back-home-btn {
  align-self: flex-start;
  margin-bottom: 4px;
}

.toolbar-title-block h3 {
  margin: 0;
  font-size: 20px;
  font-weight: 700;
  color: #0f172a;
}

.toolbar-title-block p {
  margin: 0;
  color: #6b7280;
  font-size: 13px;
}
</style>
<!-- 【修改结束】 -->
