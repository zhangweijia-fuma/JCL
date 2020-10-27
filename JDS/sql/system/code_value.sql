
-- ----------------------------
-- Table structure for code_value
-- ----------------------------
DROP TABLE IF EXISTS "system"."code_value";
CREATE TABLE "system"."code_value" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"type_id" int4,
"code" varchar(50) COLLATE "default",
"name" varchar(50) COLLATE "default",
"name_en" varchar(50) COLLATE "default",
"description" varchar(100) COLLATE "default",
"type_code" varchar(50) COLLATE "default",
"sort" int4,
"status" varchar(50) COLLATE "default" DEFAULT 1
)
WITH (OIDS=FALSE)

;
COMMENT ON TABLE "system"."code_value" IS '数据字典代码值表';
COMMENT ON COLUMN "system"."code_value"."id" IS 'Id号';
COMMENT ON COLUMN "system"."code_value"."create_time" IS '创建时间';
COMMENT ON COLUMN "system"."code_value"."update_time" IS '修改时间';
COMMENT ON COLUMN "system"."code_value"."create_id" IS '创建人';
COMMENT ON COLUMN "system"."code_value"."update_id" IS '修改人';
COMMENT ON COLUMN "system"."code_value"."type_id" IS '类型id';
COMMENT ON COLUMN "system"."code_value"."code" IS '编码';
COMMENT ON COLUMN "system"."code_value"."name" IS '名字';
COMMENT ON COLUMN "system"."code_value"."name_en" IS '英文名字';
COMMENT ON COLUMN "system"."code_value"."description" IS '描述';
COMMENT ON COLUMN "system"."code_value"."type_code" IS '类型编码';
COMMENT ON COLUMN "system"."code_value"."sort" IS '顺序，从0开始';
COMMENT ON COLUMN "system"."code_value"."status" IS '状态';

-- ----------------------------
-- Records of code_value
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table code_value
-- ----------------------------
CREATE INDEX "type_value_type_code_idx" ON "system"."code_value" USING btree ("type_code");

-- ----------------------------
-- Primary Key structure for table code_value
-- ----------------------------
ALTER TABLE "system"."code_value" ADD PRIMARY KEY ("id");
