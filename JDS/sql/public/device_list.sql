
-- ----------------------------
-- Table structure for device_list
-- ----------------------------
DROP TABLE IF EXISTS "public"."device_list";
CREATE TABLE "public"."device_list" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"device_name" varchar(128),
"device_name_en" varchar(128),
"device_code" varchar(128),
"device_sn" varchar(50),
"device_desc" text,
"device_type_id" int4,
"vendor_id" int4,
"produce_date" varchar(128),
"produce_location" varchar(128),
"produce_batch" varchar(128),
"qr_code" varchar(128),
"start_used_date" varchar(50),
"end_used_date" varchar(50),
"device_status" varchar(50),
"status_update_time" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "public"."device_list" IS '设备类型表';
COMMENT ON COLUMN "public"."device_list"."create_time" IS '创建时间';
COMMENT ON COLUMN "public"."device_list"."update_time" IS '修改时间';
COMMENT ON COLUMN "public"."device_list"."create_id" IS '创建人';
COMMENT ON COLUMN "public"."device_list"."update_id" IS '修改人';
COMMENT ON COLUMN "public"."device_list"."device_name" IS '设备名称';
COMMENT ON COLUMN "public"."device_list"."device_name_en" IS '设备英文名称';
COMMENT ON COLUMN "public"."device_list"."device_code" IS '设备编号';
COMMENT ON COLUMN "public"."device_list"."device_sn" IS '设备编号';
COMMENT ON COLUMN "public"."device_list"."device_desc" IS '设备描述';
COMMENT ON COLUMN "public"."device_list"."device_type_id" IS '设备类型ID';
COMMENT ON COLUMN "public"."device_list"."vendor_id" IS '供应商ID';
COMMENT ON COLUMN "public"."device_list"."produce_date" IS '生产日期';
COMMENT ON COLUMN "public"."device_list"."produce_location" IS '生产地点';
COMMENT ON COLUMN "public"."device_list"."produce_batch" IS '生产批次';
COMMENT ON COLUMN "public"."device_list"."qr_code" IS '二维码';
COMMENT ON COLUMN "public"."device_list"."start_used_date" IS '设备启用日期';
COMMENT ON COLUMN "public"."device_list"."end_used_date" IS '设备停用日期';
COMMENT ON COLUMN "public"."device_list"."device_status" IS '设备状态';
COMMENT ON COLUMN "public"."device_list"."status_update_time" IS '设备状态更新时间';
COMMENT ON COLUMN "public"."device_list"."system_user_id" IS '归属的系统用户ID';

-- ----------------------------
-- Records of device_list
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table device_list
-- ----------------------------
CREATE UNIQUE INDEX "device_list_code_INDEX" ON "public"."device_list" USING btree ("device_code", "system_user_id");

-- ----------------------------
-- Primary Key structure for table device_list
-- ----------------------------
ALTER TABLE "public"."device_list" ADD PRIMARY KEY ("id");
