
-- ----------------------------
-- Table structure for vendor_check_history
-- ----------------------------
DROP TABLE IF EXISTS "public"."vendor_check_history";
CREATE TABLE "public"."vendor_check_history" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"vendor_id" int,
"checker_id" int,
"checker_name" varchar(50),
"check_date" varchar(50),
"check_opinion" varchar(50),
"check_result" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "public"."vendor_check_history" IS '供应商审核记录表';
COMMENT ON COLUMN "public"."vendor_check_history"."create_time" IS '创建时间';
COMMENT ON COLUMN "public"."vendor_check_history"."update_time" IS '修改时间';
COMMENT ON COLUMN "public"."vendor_check_history"."create_id" IS '创建人';
COMMENT ON COLUMN "public"."vendor_check_history"."update_id" IS '修改人';
COMMENT ON COLUMN "public"."vendor_check_history"."vendor_id" IS '供应商ID';
COMMENT ON COLUMN "public"."vendor_check_history"."checker_id" IS '审核人名称';
COMMENT ON COLUMN "public"."vendor_check_history"."checker_name" IS '审核人名称';
COMMENT ON COLUMN "public"."vendor_check_history"."check_date" IS '审核日期';
COMMENT ON COLUMN "public"."vendor_check_history"."check_opinion" IS '审核意见';
COMMENT ON COLUMN "public"."vendor_check_history"."check_result" IS '审核结果';
COMMENT ON COLUMN "public"."vendor_check_history"."system_user_id" IS '归属的系统用户ID';

-- ----------------------------
-- Records of vendor_check_history
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table vendor_check_history
-- ----------------------------
CREATE INDEX "vendor_check_history_id_INDEX" ON "public"."vendor_check_history" USING btree ("vendor_id", "system_user_id");

-- ----------------------------
-- Primary Key structure for table vendor_check_history
-- ----------------------------
ALTER TABLE "public"."vendor_check_history" ADD PRIMARY KEY ("id");
