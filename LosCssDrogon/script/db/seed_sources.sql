TRUNCATE TABLE seed_sources RESTART IDENTITY CASCADE;

INSERT INTO seed_sources (source_code, source_name, source_type, variety_name,
    geographic_indication, introduction_date, quantity, unit, quality_grade,
    quarantine_report, quality_report, suitable_area, storage_condition,
    supplier_name, supplier_contact, status, remark)
VALUES
('SS-001', '文山三七野生种源', '野生种源', '三七', '文山', '2026-03-01', 50.00,
 '公斤', '特级', 'QR-2026-001', '水分10%，灰分5%',
 '云南文山海拔1500-1800m', '阴凉干燥',
 '文山农科所', '13800000000', '启用', ''),

('SS-002', '云南三七栽培种源', '栽培种源', '三七', '文山', '2026-05-05', 120.50,
 '公斤', '一级', 'QR-2026-002', '',
 '云南文山', '阴凉干燥',
 '文山农科所', '13800000000', '启用', '批量繁育用'),

('SS-003', '苗族三七组培种源', '组培种源', '三七', '广西', '2026-04-20', 80.00,
 '株', '二级', 'QR-2026-003', '',
 '广西百色', '温室存储',
 '广西植物所', '13900000000', '启用', '实验性引种'),

('SS-004', '当归野生种源', '野生种源', '当归', '甘肃岷县', '2026-02-15', 30.00,
 '公斤', '特级', 'QR-2026-004', '挥发油含量高',
 '甘肃岷县海拔2500m以上', '冷库存储',
 '岷县药材公司', '13700000000', '启用', ''),

('SS-005', '黄芪栽培种源', '栽培种源', '黄芪', '内蒙古', '2026-03-20', 200.00,
 '公斤', '一级', 'QR-2026-005', '',
 '内蒙古赤峰', '通风干燥',
 '赤峰中药公司', '13600000000', '停用', '本季暂不引种');

SELECT COUNT(*) AS inserted_count FROM seed_sources;