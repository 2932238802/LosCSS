# LosCSS 后端 API 文档

> 版本：v1.0  
> 最后更新：2026-05-05  
> 维护者：LosAngelous 
> 对应前端仓库：`LosCssVue`  
> 对应后端仓库：`LosCssDrogon`（Drogon + PostgreSQL）

---

## 一、基础约定

### 1.1 部署与地址

| 项目 | 值 |
| --- | --- |
| 前端开发地址 | `http://127.0.0.1:5173` |
| 后端开发地址 | `http://127.0.0.1:8080` |
| 前端 `axios.baseURL` | `/api` |
| Vite 代理规则 | `/api` → `http://127.0.0.1:8080`（不 rewrite） |
| 最终后端路由前缀 | `/api/*` |

> 后端 Drogon Controller 的 `PATH_ADD` 一律以 `/api/` 开头，与前端代理路径保持一致。

### 1.2 HTTP 方法约定（RESTful）

| 方法 | 语义 | 路径范式 |
| --- | --- | --- |
| `GET` | 列表 | `/api/{resource}` |
| `GET` | 详情 | `/api/{resource}/{id}` |
| `POST` | 新建 | `/api/{resource}` |
| `PUT` | 全量更新 | `/api/{resource}/{id}` |
| `DELETE` | 删除 | `/api/{resource}/{id}` |

> 当前阶段使用物理删除（HTTP DELETE）。后续若引入软删除，会在表中追加 `deleted_at` 字段，接口语义不变。

### 1.3 命名规约

| 层级 | 风格 | 示例 |
| --- | --- | --- |
| URL 路径 | **kebab-case** | `/api/seed-sources`、`/api/raw-material-ins` |
| 请求体 JSON / 响应体 JSON | **camelCase** | `sourceCode`、`batchNumber` |
| 数据库表 / 列 | **snake_case** | `seed_sources`、`source_code` |

> 字段转换由**后端 Controller 层**负责（camelCase ⇄ snake_case）。前端只看到 camelCase，数据库只存 snake_case。

### 1.4 统一响应格式

所有接口统一返回：

```json
{
  "code": 0,
  "message": "ok",
  "data": {}
}
```

| 字段 | 类型 | 说明 |
| --- | --- | --- |
| `code` | int | 业务状态码，`0` 表示成功，其它表示失败 |
| `message` | string | 可直接给用户看的提示 |
| `data` | any | 成功时的业务数据；失败时可为 `null` 或错误详情 |

#### 列表接口 `data` 结构

```json
{
  "items": [ /* 列表项 */ ],
  "total": 120,
  "page": 1,
  "pageSize": 20
}
```

> 前端当前通过 `normalizeListPayload` 兼容了三种格式（纯数组 / `{items}` / `{data}`），后端统一返回 `data.items` 形式即可。

#### 详情 / 新建 / 更新接口 `data` 结构

直接返回资源对象：

```json
{
  "id": 1,
  "sourceCode": "SS-001",
  "...": "..."
}
```

#### 删除接口 `data` 结构

```json
{ "id": 1 }
```

### 1.5 错误码规范

| code | 含义 | HTTP 状态 | 场景 |
| --- | --- | --- | --- |
| `0` | 成功 | 200 | 正常返回 |
| `400` | 请求参数错误 | 400 | 字段缺失、类型错误、枚举值非法 |
| `401` | 未登录 | 401 | 预留（当前阶段无鉴权） |
| `403` | 权限不足 | 403 | 预留 |
| `404` | 资源不存在 | 404 | `/{id}` 查无此记录 |
| `409` | 业务冲突 | 409 | 唯一键冲突（如 source_code 重复） |
| `422` | 数据校验失败 | 422 | 枚举值不在允许列表 |
| `500` | 服务器内部错误 | 500 | 未捕获异常 |
| `503` | 数据库不可用 | 503 | DB 连接失败 |

错误示例：

```json
{
  "code": 409,
  "message": "种源编号已存在：SS-001",
  "data": null
}
```

### 1.6 分页 / 搜索 / 排序（列表接口通用）

所有 `GET /api/{resource}` 列表接口支持以下 query 参数：

| 参数 | 类型 | 默认 | 说明 |
| --- | --- | --- | --- |
| `page` | int | `1` | 页码，从 1 开始 |
| `pageSize` | int | `20` | 每页条数，最大 200 |
| `keyword` | string | - | 关键字模糊搜索（模块内指定若干字段做 ILIKE） |
| `sortBy` | string | `id` | 排序字段（camelCase，如 `createdAt`） |
| `sortOrder` | string | `desc` | `asc` / `desc` |
| `status` | string | - | 按状态过滤（模块有 status 字段时有效） |

> 未来扩展：`startDate` / `endDate` 区间过滤、多字段组合过滤等。

### 1.7 时间字段约定

| 类型 | 前端格式 | 后端存储 |
| --- | --- | --- |
| 日期（`type: 'date'`） | `"2026-05-05"` | `DATE` |
| 日期时间（`type: 'datetime-local'`） | `"2026-05-05T10:30"` | `TIMESTAMP` |
| 系统时间（`createdAt` / `updatedAt`） | ISO8601：`"2026-05-05T10:30:00.000Z"` | `TIMESTAMP DEFAULT NOW()` |

### 1.8 主键约定

- 全部使用 `SERIAL`（PostgreSQL 自增 `int`）主键，字段名 `id`。
- 前端创建时 `id` 字段传 `null` 或**不传**，后端自动生成并回写。
- 前端更新时 `id` 通过 URL 传递，请求体不强制携带 `id`。

---

## 二、模块总览

| # | 模块 Key | 中文名称 | URL 前缀 | 数据库表 |
| --- | --- | --- | --- | --- |
| 1 | `seedSources` | 种源管理 | `/api/seed-sources` | `seed_sources` |
| 2 | `seedlings` | 育苗管理 | `/api/seedlings` | `seedlings` |
| 3 | `plantingAreas` | 种植地块管理 | `/api/planting-areas` | `planting_areas` |
| 4 | `farmingOperations` | 农事操作管理 | `/api/farming-operations` | `farming_operations` |
| 5 | `harvests` | 采收管理 | `/api/harvests` | `harvests` |
| 6 | `rawMaterialIns` | 原料入库管理 | `/api/raw-material-ins` | `raw_material_ins` |
| 7 | `processingFlows` | 加工流程管理 | `/api/processing-flows` | `processing_flows` |
| 8 | `qualityChecks` | 质检管理 | `/api/quality-checks` | `quality_checks` |

业务链路（主要数据流向）：

```
种源(seed_sources)
    └── 育苗(seedlings)                   ── seedId
            └── 种植地块(planting_areas)
                    └── 农事操作(farming_operations)   ── areaId
                            └── 采收(harvests)          ── areaId, batchNumber
                                    └── 原料入库(raw_material_ins)   ── batchNumber
                                            └── 加工流程(processing_flows)
                                                    └── 质检(quality_checks)   ── batchNumber
```

> `batchNumber`（批次号）是串联全链路的关键业务字段，用于溯源查询。

---

## 三、模块 1：种源管理（Seed Sources）

### 3.1 资源定义

| URL 前缀 | `/api/seed-sources` |
| --- | --- |
| 数据库表 | `seed_sources` |
| 主键 | `id` (SERIAL) |
| 业务唯一键 | `source_code` |

### 3.2 字段表

| 前端字段 (camelCase) | DB 字段 (snake_case) | 类型 | 必填 | 约束 / 枚举 | 说明 |
| --- | --- | --- | --- | --- | --- |
| `id` | `id` | int | 自动 | PK, SERIAL | 主键 |
| `sourceCode` | `source_code` | string(64) | ✓ | UNIQUE | 种源编号 |
| `sourceName` | `source_name` | string(128) | ✓ | | 种源名称 |
| `sourceType` | `source_type` | string(32) | ✓ | `野生种源` / `栽培种源` / `组培种源` | 种源类型 |
| `varietyName` | `variety_name` | string(128) | ✓ | | 品种名称 |
| `geographicIndication` | `geographic_indication` | string(128) |  | | 地理标志 |
| `introductionDate` | `introduction_date` | date | ✓ | | 引种日期 |
| `quantity` | `quantity` | decimal(12,2) | ✓ | `>= 0` | 数量 |
| `unit` | `unit` | string(16) | ✓ | `公斤` / `株` / `克` | 计量单位 |
| `qualityGrade` | `quality_grade` | string(16) | ✓ | `特级` / `一级` / `二级` / `三级` | 质量等级 |
| `quarantineReport` | `quarantine_report` | string(256) |  | | 检疫报告（文件编号/URL） |
| `qualityReport` | `quality_report` | text |  | | 质检报告详情 |
| `suitableArea` | `suitable_area` | string(256) |  | | 适宜种植区域 |
| `storageCondition` | `storage_condition` | string(256) |  | | 存储条件 |
| `supplierName` | `supplier_name` | string(128) |  | | 供应商名称 |
| `supplierContact` | `supplier_contact` | string(128) |  | | 供应商联系方式 |
| `status` | `status` | string(16) | ✓ | `启用` / `停用` / `待审核` | 状态 |
| `remark` | `remark` | text |  | | 备注 |
| `createdAt` | `created_at` | timestamp | 自动 | | 创建时间 |
| `updatedAt` | `updated_at` | timestamp | 自动 | | 更新时间 |

### 3.3 接口清单

| 方法 | 路径 | 说明 |
| --- | --- | --- |
| `GET` | `/api/seed-sources` | 列表（支持分页/关键字/状态过滤） |
| `GET` | `/api/seed-sources/{id}` | 详情 |
| `POST` | `/api/seed-sources` | 新建 |
| `PUT` | `/api/seed-sources/{id}` | 更新 |
| `DELETE` | `/api/seed-sources/{id}` | 删除 |

### 3.4 请求 / 响应示例

#### 列表 `GET /api/seed-sources?page=1&pageSize=20&keyword=三七`

```json
{
  "code": 0,
  "message": "ok",
  "data": {
    "items": [
      {
        "id": 1,
        "sourceCode": "SS-001",
        "sourceName": "文山三七野生种源",
        "sourceType": "野生种源",
        "varietyName": "三七",
        "geographicIndication": "文山",
        "introductionDate": "2026-03-01",
        "quantity": 50.00,
        "unit": "公斤",
        "qualityGrade": "特级",
        "quarantineReport": "QR-2026-001",
        "qualityReport": "水分 10%，灰分 5%",
        "suitableArea": "云南文山海拔 1500-1800m",
        "storageCondition": "阴凉干燥",
        "supplierName": "文山农科所",
        "supplierContact": "13800000000",
        "status": "启用",
        "remark": "",
        "createdAt": "2026-05-01T08:00:00.000Z",
        "updatedAt": "2026-05-01T08:00:00.000Z"
      }
    ],
    "total": 1,
    "page": 1,
    "pageSize": 20
  }
}
```

#### 新建 `POST /api/seed-sources`

请求体：

```json
{
  "sourceCode": "SS-002",
  "sourceName": "云南三七栽培种源",
  "sourceType": "栽培种源",
  "varietyName": "三七",
  "geographicIndication": "文山",
  "introductionDate": "2026-05-05",
  "quantity": 120.5,
  "unit": "公斤",
  "qualityGrade": "一级",
  "quarantineReport": "QR-2026-002",
  "qualityReport": "",
  "suitableArea": "",
  "storageCondition": "阴凉干燥",
  "supplierName": "文山农科所",
  "supplierContact": "13800000000",
  "status": "启用",
  "remark": ""
}
```

响应：

```json
{
  "code": 0,
  "message": "ok",
  "data": {
    "id": 2,
    "sourceCode": "SS-002",
    "...": "..."
  }
}
```

#### 唯一键冲突错误

```json
{
  "code": 409,
  "message": "种源编号已存在：SS-002",
  "data": null
}
```

---

## 四、模块 2：育苗管理（Seedlings）

### 4.1 资源定义

| URL 前缀 | `/api/seedlings` |
| --- | --- |
| 数据库表 | `seedlings` |
| 主键 | `id` (SERIAL) |
| 外键 | `seed_id` → `seed_sources.id` |

### 4.2 字段表

| 前端字段 | DB 字段 | 类型 | 必填 | 约束 / 枚举 | 说明 |
| --- | --- | --- | --- | --- | --- |
| `id` | `id` | int | 自动 | PK | 主键 |
| `seedId` | `seed_id` | int | ✓ | FK → `seed_sources.id` | 关联种源 |
| `germinationRate` | `germination_rate` | decimal(5,2) |  | `0-100` | 发芽率（%） |
| `growthStatus` | `growth_status` | string(16) | ✓ | `准备中` / `育苗中` / `完成` | 生长状态 |
| `plantDate` | `plant_date` | date | ✓ | | 种植日期 |
| `startTime` | `start_time` | timestamp |  | | 育苗开始时间 |
| `endTime` | `end_time` | timestamp |  | | 育苗结束时间 |
| `method` | `method` | string(128) |  | | 育苗方法 |
| `environmentParams` | `environment_params` | text |  | | 环境参数 |
| `operator` | `operator` | string(64) |  | | 操作者 |
| `createdAt` | `created_at` | timestamp | 自动 | | |
| `updatedAt` | `updated_at` | timestamp | 自动 | | |

### 4.3 接口清单

| 方法 | 路径 | 说明 |
| --- | --- | --- |
| `GET` | `/api/seedlings` | 列表 |
| `GET` | `/api/seedlings/{id}` | 详情 |
| `POST` | `/api/seedlings` | 新建 |
| `PUT` | `/api/seedlings/{id}` | 更新 |
| `DELETE` | `/api/seedlings/{id}` | 删除 |

### 4.4 示例

#### 新建 `POST /api/seedlings`

```json
{
  "seedId": 1,
  "germinationRate": 92.5,
  "growthStatus": "育苗中",
  "plantDate": "2026-05-01",
  "startTime": "2026-05-01T08:00",
  "endTime": null,
  "method": "温床育苗",
  "environmentParams": "温度 22-25℃，湿度 70%",
  "operator": "张三"
}
```

---

## 五、模块 3：种植地块管理（Planting Areas）

### 5.1 资源定义

| URL 前缀 | `/api/planting-areas` |
| --- | --- |
| 数据库表 | `planting_areas` |

### 5.2 字段表

| 前端字段 | DB 字段 | 类型 | 必填 | 枚举 | 说明 |
| --- | --- | --- | --- | --- | --- |
| `id` | `id` | int | 自动 | PK | |
| `areaName` | `area_name` | string(128) | ✓ | | 地块名称 |
| `soilType` | `soil_type` | string(16) | ✓ | `沙土` / `壤土` / `粘土` | 土壤类型 |
| `size` | `size` | decimal(10,2) | ✓ | `>= 0` | 面积（亩） |
| `location` | `location` | string(256) | ✓ | | 地理位置 |
| `status` | `status` | string(16) | ✓ | `空闲` / `种植中` / `休耕` | 地块状态 |
| `description` | `description` | text |  | | 描述 |
| `createdAt` | `created_at` | timestamp | 自动 | | |
| `updatedAt` | `updated_at` | timestamp | 自动 | | |

### 5.3 接口清单

| 方法 | 路径 | 说明 |
| --- | --- | --- |
| `GET` | `/api/planting-areas` | 列表 |
| `GET` | `/api/planting-areas/{id}` | 详情 |
| `POST` | `/api/planting-areas` | 新建 |
| `PUT` | `/api/planting-areas/{id}` | 更新 |
| `DELETE` | `/api/planting-areas/{id}` | 删除 |

### 5.4 示例

#### 新建 `POST /api/planting-areas`

```json
{
  "areaName": "A-01 号地块",
  "soilType": "壤土",
  "size": 15.5,
  "location": "文山州 A 区",
  "status": "空闲",
  "description": "东北向坡地，排水良好"
}
```

---

## 六、模块 4：农事操作管理（Farming Operations）

### 6.1 资源定义

| URL 前缀 | `/api/farming-operations` |
| --- | --- |
| 数据库表 | `farming_operations` |
| 外键 | `area_id` → `planting_areas.id` |

### 6.2 字段表

| 前端字段 | DB 字段 | 类型 | 必填 | 枚举 | 说明 |
| --- | --- | --- | --- | --- | --- |
| `id` | `id` | int | 自动 | PK | |
| `areaId` | `area_id` | int | ✓ | FK → `planting_areas.id` | 关联地块 |
| `operationType` | `operation_type` | string(16) | ✓ | `施肥` / `灌溉` / `除草` / `病虫害防治` | 操作类型 |
| `operationDate` | `operation_date` | date | ✓ | | 操作日期 |
| `operatorId` | `operator_id` | int | ✓ | | 操作人员 ID |
| `materialUsed` | `material_used` | text |  | | 使用农资 |
| `status` | `status` | string(16) | ✓ | `计划中` / `进行中` / `已完成` / `已取消` | 操作状态 |
| `resultDescription` | `result_description` | text |  | | 操作结果 |
| `reviewStatus` | `review_status` | string(16) | ✓ | `待审核` / `已通过` / `需修正` | 审核状态 |
| `createdAt` | `created_at` | timestamp | 自动 | | |
| `updatedAt` | `updated_at` | timestamp | 自动 | | |

### 6.3 接口清单

| 方法 | 路径 | 说明 |
| --- | --- | --- |
| `GET` | `/api/farming-operations` | 列表 |
| `GET` | `/api/farming-operations/{id}` | 详情 |
| `POST` | `/api/farming-operations` | 新建 |
| `PUT` | `/api/farming-operations/{id}` | 更新 |
| `DELETE` | `/api/farming-operations/{id}` | 删除 |

### 6.4 示例

#### 新建 `POST /api/farming-operations`

```json
{
  "areaId": 1,
  "operationType": "施肥",
  "operationDate": "2026-05-05",
  "operatorId": 1001,
  "materialUsed": "有机肥 200kg / 亩",
  "status": "计划中",
  "resultDescription": "",
  "reviewStatus": "待审核"
}
```

---

## 七、模块 5：采收管理（Harvests）

### 7.1 资源定义

| URL 前缀 | `/api/harvests` |
| --- | --- |
| 数据库表 | `harvests` |
| 外键 | `area_id` → `planting_areas.id` |
| 业务唯一键 | `batch_number` |

### 7.2 字段表

| 前端字段 | DB 字段 | 类型 | 必填 | 枚举 | 说明 |
| --- | --- | --- | --- | --- | --- |
| `id` | `id` | int | 自动 | PK | |
| `areaId` | `area_id` | int | ✓ | FK → `planting_areas.id` | 种植地块 |
| `harvestDate` | `harvest_date` | date | ✓ | | 采收日期 |
| `batchNumber` | `batch_number` | string(64) | ✓ | UNIQUE | 批次号（溯源主键） |
| `outputQuantity` | `output_quantity` | decimal(12,2) | ✓ | `>= 0` | 采收数量（kg） |
| `qualityLevel` | `quality_level` | string(16) | ✓ | `优` / `良` / `合格` | 采收质量 |
| `responsiblePerson` | `responsible_person` | string(64) | ✓ | | 负责人 |
| `harvestMethod` | `harvest_method` | string(16) | ✓ | `人工采收` / `机械采收` | 采收方式 |
| `storageLocation` | `storage_location` | string(128) | ✓ | | 存储仓库 |
| `inspectionStatus` | `inspection_status` | string(16) | ✓ | `待检` / `已检合格` / `已检不合格` | 质检状态 |
| `remarks` | `remarks` | text |  | | 备注 |
| `createdAt` | `created_at` | timestamp | 自动 | | |
| `updatedAt` | `updated_at` | timestamp | 自动 | | |

### 7.3 接口清单

| 方法 | 路径 | 说明 |
| --- | --- | --- |
| `GET` | `/api/harvests` | 列表 |
| `GET` | `/api/harvests/{id}` | 详情 |
| `POST` | `/api/harvests` | 新建 |
| `PUT` | `/api/harvests/{id}` | 更新 |
| `DELETE` | `/api/harvests/{id}` | 删除 |

### 7.4 示例

#### 新建 `POST /api/harvests`

```json
{
  "areaId": 1,
  "harvestDate": "2026-10-15",
  "batchNumber": "BATCH-2026-001",
  "outputQuantity": 1250.5,
  "qualityLevel": "优",
  "responsiblePerson": "李四",
  "harvestMethod": "人工采收",
  "storageLocation": "A 仓",
  "inspectionStatus": "待检",
  "remarks": ""
}
```

---

## 八、模块 6：原料入库管理（Raw Material Ins）

### 8.1 资源定义

| URL 前缀 | `/api/raw-material-ins` |
| --- | --- |
| 数据库表 | `raw_material_ins` |
| 关联 | `batch_number` ↔ `harvests.batch_number` |

### 8.2 字段表

| 前端字段 | DB 字段 | 类型 | 必填 | 枚举 | 说明 |
| --- | --- | --- | --- | --- | --- |
| `id` | `id` | int | 自动 | PK | |
| `materialType` | `material_type` | string(16) | ✓ | `鲜品` / `干品` / `切片` | 原料类型 |
| `batchNumber` | `batch_number` | string(64) | ✓ | | 入库批次号 |
| `supplierName` | `supplier_name` | string(128) | ✓ | | 供应商名称 |
| `inDate` | `in_date` | date | ✓ | | 入库日期 |
| `quantity` | `quantity` | decimal(12,2) | ✓ | `>= 0` | 入库数量 |
| `unit` | `unit` | string(16) | ✓ | | 计量单位 |
| `qualityLevel` | `quality_level` | string(16) | ✓ | `特级` / `一级` / `二级` | 质量等级 |
| `storageLocation` | `storage_location` | string(128) | ✓ | | 仓库位置 |
| `inspectionNo` | `inspection_no` | string(64) |  | | 检验报告编号 |
| `status` | `status` | string(16) | ✓ | `待检` / `合格` / `待处理` | 入库状态 |
| `operator` | `operator` | string(64) | ✓ | | 操作人员 |
| `createdAt` | `created_at` | timestamp | 自动 | | |
| `updatedAt` | `updated_at` | timestamp | 自动 | | |

### 8.3 接口清单

| 方法 | 路径 | 说明 |
| --- | --- | --- |
| `GET` | `/api/raw-material-ins` | 列表 |
| `GET` | `/api/raw-material-ins/{id}` | 详情 |
| `POST` | `/api/raw-material-ins` | 新建 |
| `PUT` | `/api/raw-material-ins/{id}` | 更新 |
| `DELETE` | `/api/raw-material-ins/{id}` | 删除 |

### 8.4 示例

#### 新建 `POST /api/raw-material-ins`

```json
{
  "materialType": "鲜品",
  "batchNumber": "BATCH-2026-001",
  "supplierName": "文山种植基地",
  "inDate": "2026-10-16",
  "quantity": 1200.0,
  "unit": "公斤",
  "qualityLevel": "一级",
  "storageLocation": "原料 A 库",
  "inspectionNo": "INS-2026-0001",
  "status": "待检",
  "operator": "王五"
}
```

---

## 九、模块 7：加工流程管理（Processing Flows）

### 9.1 资源定义

| URL 前缀 | `/api/processing-flows` |
| --- | --- |
| 数据库表 | `processing_flows` |

### 9.2 字段表

| 前端字段 | DB 字段 | 类型 | 必填 | 枚举 | 说明 |
| --- | --- | --- | --- | --- | --- |
| `id` | `id` | int | 自动 | PK | |
| `flowName` | `flow_name` | string(128) | ✓ | | 流程名称 |
| `flowType` | `flow_type` | string(16) | ✓ | `清洗` / `蒸制` / `干燥` / `切片` / `包装` | 流程类型 |
| `deviceId` | `device_id` | int | ✓ | | 加工设备 ID |
| `operator` | `operator` | string(64) | ✓ | | 负责人 |
| `startTime` | `start_time` | timestamp |  | | 开始时间 |
| `endTime` | `end_time` | timestamp |  | | 结束时间 |
| `percentComplete` | `percent_complete` | decimal(5,2) | ✓ | `0-100` | 当前进度（%） |
| `standardHours` | `standard_hours` | decimal(8,2) | ✓ | `>= 0` | 标准工时 |
| `actualHours` | `actual_hours` | decimal(8,2) |  | `>= 0` | 实际工时 |
| `status` | `status` | string(16) | ✓ | `未开始` / `进行中` / `已暂停` / `已完成` | 流程状态 |
| `createdAt` | `created_at` | timestamp | 自动 | | |
| `updatedAt` | `updated_at` | timestamp | 自动 | | |

### 9.3 接口清单

| 方法 | 路径 | 说明 |
| --- | --- | --- |
| `GET` | `/api/processing-flows` | 列表 |
| `GET` | `/api/processing-flows/{id}` | 详情 |
| `POST` | `/api/processing-flows` | 新建 |
| `PUT` | `/api/processing-flows/{id}` | 更新 |
| `DELETE` | `/api/processing-flows/{id}` | 删除 |

### 9.4 示例

#### 新建 `POST /api/processing-flows`

```json
{
  "flowName": "三七清洗工序",
  "flowType": "清洗",
  "deviceId": 101,
  "operator": "赵六",
  "startTime": "2026-10-20T08:00",
  "endTime": null,
  "percentComplete": 0,
  "standardHours": 4.0,
  "actualHours": null,
  "status": "未开始"
}
```

---

## 十、模块 8：质检管理（Quality Checks）

### 10.1 资源定义

| URL 前缀 | `/api/quality-checks` |
| --- | --- |
| 数据库表 | `quality_checks` |
| 关联 | `batch_number` ↔ `harvests.batch_number` / `raw_material_ins.batch_number` |

### 10.2 字段表

| 前端字段 | DB 字段 | 类型 | 必填 | 枚举 | 说明 |
| --- | --- | --- | --- | --- | --- |
| `id` | `id` | int | 自动 | PK | |
| `productId` | `product_id` | string(64) | ✓ | | 产品编号 |
| `checkDate` | `check_date` | date | ✓ | | 检验日期 |
| `result` | `result` | string(16) | ✓ | `合格` / `不合格` / `待复核` | 检验结果 |
| `issue` | `issue` | text |  | | 质量问题 |
| `inspector` | `inspector` | string(64) | ✓ | | 检验人员 |
| `method` | `method` | string(16) | ✓ | `感官检验` / `理化检验` / `微生物检验` | 检验方法 |
| `batchNumber` | `batch_number` | string(64) | ✓ | | 批次号 |
| `productionDate` | `production_date` | date | ✓ | | 生产日期 |
| `reportData` | `report_data` | text |  | | 检验报告 |
| `checkStatus` | `check_status` | string(16) | ✓ | `待检验` / `检验中` / `已完成` | 检验状态 |
| `createdAt` | `created_at` | timestamp | 自动 | | |
| `updatedAt` | `updated_at` | timestamp | 自动 | | |

### 10.3 接口清单

| 方法 | 路径 | 说明 |
| --- | --- | --- |
| `GET` | `/api/quality-checks` | 列表 |
| `GET` | `/api/quality-checks/{id}` | 详情 |
| `POST` | `/api/quality-checks` | 新建 |
| `PUT` | `/api/quality-checks/{id}` | 更新 |
| `DELETE` | `/api/quality-checks/{id}` | 删除 |

### 10.4 示例

#### 新建 `POST /api/quality-checks`

```json
{
  "productId": "PRD-2026-001",
  "checkDate": "2026-10-25",
  "result": "合格",
  "issue": "",
  "inspector": "孙七",
  "method": "理化检验",
  "batchNumber": "BATCH-2026-001",
  "productionDate": "2026-10-20",
  "reportData": "水分 9.8%，皂苷含量 5.2%，符合药典标准",
  "checkStatus": "已完成"
}
```

---

## 十一、通用附录

### 11.1 健康检查接口（非业务）

| 方法 | 路径 | 说明 |
| --- | --- | --- |
| `GET` | `/api/health` | 返回 `{ code: 0, message: "ok", data: { status: "UP", db: "UP", timestamp: "..." } }` |

### 11.2 跨域（CORS）

开发阶段由 Vite 代理处理，后端**默认不开 CORS**。  
上 Docker / 生产环境时，后端通过 `CorsFilter` 允许：

```
Access-Control-Allow-Origin: *              (或具体前端域名)
Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS
Access-Control-Allow-Headers: Content-Type, Authorization
Access-Control-Max-Age: 86400
```

### 11.3 字段转换规则（Controller 层）

| 方向 | 规则 |
| --- | --- |
| 请求进入 | JSON 请求体的 camelCase key → DB 的 snake_case 列 |
| 响应返回 | DB 的 snake_case 列 → JSON 响应体的 camelCase key |

例：`sourceCode` ⇄ `source_code`、`batchNumber` ⇄ `batch_number`、`createdAt` ⇄ `created_at`。

> 实现建议：封装一个 `utils/Naming.h` 提供 `toCamel` / `toSnake` 工具函数；或在 Drogon Model 生成后，写一个 `json_helper` 做键转换。

### 11.4 鉴权（预留）

当前阶段：**无鉴权**，所有接口开放。  
后续计划：

- 新增 `POST /api/auth/login` → 返回 JWT
- 新增 `POST /api/auth/logout`
- 新增 `GET /api/auth/me`
- 所有业务接口加 `LoginFilter`，校验请求头 `Authorization: Bearer <token>`

### 11.5 变更日志

| 日期 | 版本 | 说明 |
| --- | --- | --- |
| 2026-05-05 | v1.0 | 首版文档，覆盖 8 个业务模块的标准 CRUD |
