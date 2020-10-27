
-- ----------------------------
-- Table structure for vendor_list
-- ----------------------------
DROP TABLE IF EXISTS "public"."vendor_list";
CREATE TABLE "public"."vendor_list" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int,
"update_id" int,
"vendor_id" int,
"vendor_code" varchar(50),
"vendor_type" varchar(50),
"certifaction_date" varchar(50),
"certifaction_status" varchar(50),
"certifaction_desc" varchar(128),
"start_date" varchar(128),
"end_date" varchar(128),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "public"."vendor_list" IS '供应商列表';
COMMENT ON COLUMN "public"."vendor_list"."create_time" IS '创建时间';
COMMENT ON COLUMN "public"."vendor_list"."update_time" IS '修改时间';
COMMENT ON COLUMN "public"."vendor_list"."create_id" IS '创建人';
COMMENT ON COLUMN "public"."vendor_list"."update_id" IS '修改人';
COMMENT ON COLUMN "public"."vendor_list"."vendor_id" IS '供应商ID';
COMMENT ON COLUMN "public"."vendor_list"."vendor_code" IS '供应商编号';
COMMENT ON COLUMN "public"."vendor_list"."vendor_type" IS '供应商类型';
COMMENT ON COLUMN "public"."vendor_list"."certifaction_date" IS '供应商认证日期';
COMMENT ON COLUMN "public"."vendor_list"."certifaction_status" IS '供应商认证状态';
COMMENT ON COLUMN "public"."vendor_list"."certifaction_desc" IS '认证描述';
COMMENT ON COLUMN "public"."vendor_list"."start_date" IS '合作开始日期';
COMMENT ON COLUMN "public"."vendor_list"."end_date" IS '合作结束日期';
COMMENT ON COLUMN "public"."vendor_list"."system_user_id" IS '归属的系统用户ID';

-- ----------------------------
-- Records of vendor_list
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table vendor_list
-- ----------------------------
CREATE UNIQUE INDEX "vendor_list_code_INDEX" ON "public"."vendor_list" USING btree ("vendor_code", "system_user_id");
CREATE INDEX "vendor_list_id_INDEX" ON "public"."vendor_list" USING btree ("vendor_id", "system_user_id");

-- ----------------------------
-- Primary Key structure for table vendor_list
-- ----------------------------
ALTER TABLE "public"."vendor_list" ADD PRIMARY KEY ("id");
