
-- ----------------------------
-- Table structure for device_config
-- ----------------------------
DROP TABLE IF EXISTS "public"."device_config";
CREATE TABLE "public"."device_config" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"vendor_serial" int4,
"property_serial" int4,
"component_serial" int4,
"device_type_serial" int4,
"device_serial" int4,
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "public"."device_config" IS 'device 参数表';
COMMENT ON COLUMN "public"."device_config"."create_time" IS '创建时间';
COMMENT ON COLUMN "public"."device_config"."update_time" IS '修改时间';
COMMENT ON COLUMN "public"."device_config"."create_id" IS '创建人';
COMMENT ON COLUMN "public"."device_config"."update_id" IS '修改人';
COMMENT ON COLUMN "public"."device_config"."vendor_serial" IS '供应商序列号';
COMMENT ON COLUMN "public"."device_config"."property_serial" IS '属性序列号';
COMMENT ON COLUMN "public"."device_config"."component_serial" IS '部件序列号';
COMMENT ON COLUMN "public"."device_config"."device_type_serial" IS '设备类型序列号';
COMMENT ON COLUMN "public"."device_config"."device_serial" IS '设备序列号';
COMMENT ON COLUMN "public"."device_config"."system_user_id" IS '归属的系统用户ID';

-- ----------------------------
-- Records of device_config
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table device_config
-- ----------------------------
CREATE UNIQUE INDEX "device_config_INDEX" ON "public"."device_config" USING btree ("system_user_id");

-- ----------------------------
-- Primary Key structure for table device_config
-- ----------------------------
ALTER TABLE "public"."device_config" ADD PRIMARY KEY ("id");
