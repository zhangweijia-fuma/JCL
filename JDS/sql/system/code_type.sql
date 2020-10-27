

-- ----------------------------
-- Table structure for code_type
-- ----------------------------
DROP TABLE IF EXISTS "system"."code_type";
CREATE TABLE "system"."code_type" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"code" varchar(50) COLLATE "default",
"name" varchar(50) COLLATE "default",
"name_en" varchar(50) COLLATE "default",
"description" varchar(100) COLLATE "default",
"status" varchar(50) COLLATE "default" DEFAULT 1
)
WITH (OIDS=FALSE)

;
COMMENT ON TABLE "system"."code_type" IS '数据字典代码类型表';
COMMENT ON COLUMN "system"."code_type"."id" IS 'Id号';
COMMENT ON COLUMN "system"."code_type"."create_time" IS '创建时间';
COMMENT ON COLUMN "system"."code_type"."update_time" IS '修改时间';
COMMENT ON COLUMN "system"."code_type"."create_id" IS '创建人';
COMMENT ON COLUMN "system"."code_type"."update_id" IS '修改人';
COMMENT ON COLUMN "system"."code_type"."code" IS '代码';
COMMENT ON COLUMN "system"."code_type"."name" IS '名字';
COMMENT ON COLUMN "system"."code_type"."name_en" IS '英文名字';
COMMENT ON COLUMN "system"."code_type"."description" IS '描述.';
COMMENT ON COLUMN "system"."code_type"."status" IS '状态，0为停用，1为启用';

-- ----------------------------
-- Records of code_type
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table code_type
-- ----------------------------
ALTER TABLE "system"."code_type" ADD PRIMARY KEY ("id");
