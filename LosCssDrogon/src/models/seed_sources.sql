drop table if exists seed_sources CASCADE;

CREATE TABLE seed_sources (
    id                      SERIAL          PRIMARY KEY,
    source_code             VARCHAR(64)     NOT NULL UNIQUE,            -- 种源编号（业务唯一）
    source_name             VARCHAR(128)    NOT NULL,                   -- 种源名称
    source_type             VARCHAR(32)     NOT NULL,                   -- 野生种源/栽培种源/组培种源
    variety_name            VARCHAR(128)    NOT NULL,                   -- 品种名称
    geographic_indication   VARCHAR(128),                               -- 地理标志
    introduction_date       DATE            NOT NULL,                   -- 引种日期
    quantity                NUMERIC(12, 2)  NOT NULL DEFAULT 0,         -- 数量
    unit                    VARCHAR(16)     NOT NULL,                   -- 公斤/株/克
    quality_grade           VARCHAR(16)     NOT NULL,                   -- 特级/一级/二级/三级
    quarantine_report       VARCHAR(256),                               -- 检疫报告（文件编号/URL）
    quality_report          TEXT,                                       -- 质检报告（详情）
    suitable_area           VARCHAR(256),                               -- 适宜种植区域
    storage_condition       VARCHAR(256),                               -- 存储条件
    supplier_name           VARCHAR(128),                               -- 供应商名称
    supplier_contact        VARCHAR(128),                               -- 供应商联系方式
    status                  VARCHAR(16)     NOT NULL DEFAULT '启用',    -- 启用/停用/待审核
    remark                  TEXT,
    created_at              TIMESTAMP       NOT NULL DEFAULT NOW(),
    updated_at              TIMESTAMP       NOT NULL DEFAULT NOW(),
    CONSTRAINT chk_seed_sources_source_type
        CHECK (source_type IN ('野生种源', '栽培种源', '组培种源')),
    CONSTRAINT chk_seed_sources_unit
        CHECK (unit IN ('公斤', '株', '克')),
    CONSTRAINT chk_seed_sources_quality_grade
        CHECK (quality_grade IN ('特级', '一级', '二级', '三级')),
    CONSTRAINT chk_seed_sources_status
        CHECK (status IN ('启用', '停用', '待审核')),
    CONSTRAINT chk_seed_sources_quantity_nonneg
        CHECK (quantity >= 0)
);

CREATE INDEX idx_seed_sources_status            ON seed_sources(status);
CREATE INDEX idx_seed_sources_source_type       ON seed_sources(source_type);
CREATE INDEX idx_seed_sources_variety_name      ON seed_sources(variety_name);
CREATE INDEX idx_seed_sources_supplier_name     ON seed_sources(supplier_name);
CREATE INDEX idx_seed_sources_introduction_date ON seed_sources(introduction_date);


CREATE OR REPLACE FUNCTION touch_updated_at()
RETURNS TRIGGER AS $$
BEGIN
NEW.updated_at = NOW();
RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_seed_sources_updated_at
BEFORE UPDATE ON seed_sources
FOR EACH ROW
EXECUTE FUNCTION touch_updated_at();


COMMENT ON TABLE  seed_sources IS                       '种源管理表';
COMMENT ON COLUMN seed_sources.id IS                    '主键ID';
COMMENT ON COLUMN seed_sources.source_code IS           '种源编号（业务唯一标识）';
COMMENT ON COLUMN seed_sources.source_name IS           '种源名称';
COMMENT ON COLUMN seed_sources.source_type IS           '种源类型：野生种源/栽培种源/组培种源';
COMMENT ON COLUMN seed_sources.variety_name IS          '品种名称';
COMMENT ON COLUMN seed_sources.geographic_indication IS '地理标志';
COMMENT ON COLUMN seed_sources.introduction_date IS     '引种日期';
COMMENT ON COLUMN seed_sources.quantity IS              '数量';
COMMENT ON COLUMN seed_sources.unit IS                  '单位：公斤/株/克';
COMMENT ON COLUMN seed_sources.quality_grade IS         '质量等级：特级/一级/二级/三级';
COMMENT ON COLUMN seed_sources.quarantine_report IS     '检疫报告（文件编号或URL）';
COMMENT ON COLUMN seed_sources.quality_report IS        '质检报告（详情）';
COMMENT ON COLUMN seed_sources.suitable_area IS         '适宜种植区域';
COMMENT ON COLUMN seed_sources.storage_condition IS     '存储条件';
COMMENT ON COLUMN seed_sources.supplier_name IS         '供应商名称';
COMMENT ON COLUMN seed_sources.supplier_contact IS      '供应商联系方式';
COMMENT ON COLUMN seed_sources.status IS                '状态：启用/停用/待审核';
COMMENT ON COLUMN seed_sources.remark IS                '备注';
COMMENT ON COLUMN seed_sources.created_at IS            '创建时间';
COMMENT ON COLUMN seed_sources.updated_at IS            '更新时间';