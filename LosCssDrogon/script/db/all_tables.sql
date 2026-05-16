-- ============================================================
-- LosCSS 数据库建表脚本（除 seed_sources 外的 7 张业务表）
-- 注意：seed_sources 已存在，本脚本不重建
-- ============================================================

-- 模块 2：育苗管理
CREATE TABLE IF NOT EXISTS seedlings (
  id                  SERIAL PRIMARY KEY,
  seed_id             INT NOT NULL REFERENCES seed_sources(id),
  germination_rate    DECIMAL(5, 2),
  growth_status       VARCHAR(16) NOT NULL,
  plant_date          DATE NOT NULL,
  start_time          TIMESTAMP,
  end_time            TIMESTAMP,
  method              VARCHAR(128),
  environment_params  TEXT,
  operator            VARCHAR(64),
  created_at          TIMESTAMP NOT NULL DEFAULT NOW(),
  updated_at          TIMESTAMP NOT NULL DEFAULT NOW()
);

-- 模块 3：种植地块管理
CREATE TABLE IF NOT EXISTS planting_areas (
  id            SERIAL PRIMARY KEY,
  area_name     VARCHAR(128) NOT NULL,
  soil_type     VARCHAR(16) NOT NULL,
  size          DECIMAL(10, 2) NOT NULL,
  location      VARCHAR(256) NOT NULL,
  status        VARCHAR(16) NOT NULL,
  description   TEXT,
  created_at    TIMESTAMP NOT NULL DEFAULT NOW(),
  updated_at    TIMESTAMP NOT NULL DEFAULT NOW()
);

-- 模块 4：农事操作管理
CREATE TABLE IF NOT EXISTS farming_operations (
  id                   SERIAL PRIMARY KEY,
  area_id              INT NOT NULL REFERENCES planting_areas(id),
  operation_type       VARCHAR(16) NOT NULL,
  operation_date       DATE NOT NULL,
  operator_id          INT NOT NULL,
  material_used        TEXT,
  status               VARCHAR(16) NOT NULL,
  result_description   TEXT,
  review_status        VARCHAR(16) NOT NULL,
  created_at           TIMESTAMP NOT NULL DEFAULT NOW(),
  updated_at           TIMESTAMP NOT NULL DEFAULT NOW()
);

-- 模块 5：采收管理
CREATE TABLE IF NOT EXISTS harvests (
  id                  SERIAL PRIMARY KEY,
  area_id             INT NOT NULL REFERENCES planting_areas(id),
  harvest_date        DATE NOT NULL,
  batch_number        VARCHAR(64) NOT NULL UNIQUE,
  output_quantity     DECIMAL(12, 2) NOT NULL,
  quality_level       VARCHAR(16) NOT NULL,
  responsible_person  VARCHAR(64) NOT NULL,
  harvest_method      VARCHAR(16) NOT NULL,
  storage_location    VARCHAR(128) NOT NULL,
  inspection_status   VARCHAR(16) NOT NULL,
  remarks             TEXT,
  created_at          TIMESTAMP NOT NULL DEFAULT NOW(),
  updated_at          TIMESTAMP NOT NULL DEFAULT NOW()
);

-- 模块 6：原料入库管理
CREATE TABLE IF NOT EXISTS raw_material_ins (
  id                SERIAL PRIMARY KEY,
  material_type     VARCHAR(16) NOT NULL,
  batch_number      VARCHAR(64) NOT NULL,
  supplier_name     VARCHAR(128) NOT NULL,
  in_date           DATE NOT NULL,
  quantity          DECIMAL(12, 2) NOT NULL,
  unit              VARCHAR(16) NOT NULL,
  quality_level     VARCHAR(16) NOT NULL,
  storage_location  VARCHAR(128) NOT NULL,
  inspection_no     VARCHAR(64),
  status            VARCHAR(16) NOT NULL,
  operator          VARCHAR(64) NOT NULL,
  created_at        TIMESTAMP NOT NULL DEFAULT NOW(),
  updated_at        TIMESTAMP NOT NULL DEFAULT NOW()
);

-- 模块 7：加工流程管理
CREATE TABLE IF NOT EXISTS processing_flows (
  id                SERIAL PRIMARY KEY,
  flow_name         VARCHAR(128) NOT NULL,
  flow_type         VARCHAR(16) NOT NULL,
  device_id         INT NOT NULL,
  operator          VARCHAR(64) NOT NULL,
  start_time        TIMESTAMP,
  end_time          TIMESTAMP,
  percent_complete  DECIMAL(5, 2) NOT NULL DEFAULT 0,
  standard_hours    DECIMAL(8, 2) NOT NULL,
  actual_hours      DECIMAL(8, 2),
  status            VARCHAR(16) NOT NULL,
  created_at        TIMESTAMP NOT NULL DEFAULT NOW(),
  updated_at        TIMESTAMP NOT NULL DEFAULT NOW()
);

-- 模块 8：质检管理
CREATE TABLE IF NOT EXISTS quality_checks (
  id              SERIAL PRIMARY KEY,
  product_id      VARCHAR(64) NOT NULL,
  check_date      DATE NOT NULL,
  result          VARCHAR(16) NOT NULL,
  issue           TEXT,
  inspector       VARCHAR(64) NOT NULL,
  method          VARCHAR(16) NOT NULL,
  batch_number    VARCHAR(64) NOT NULL,
  production_date DATE NOT NULL,
  report_data     TEXT,
  check_status    VARCHAR(16) NOT NULL,
  created_at      TIMESTAMP NOT NULL DEFAULT NOW(),
  updated_at      TIMESTAMP NOT NULL DEFAULT NOW()
);
