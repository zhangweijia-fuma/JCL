
-- ----------------------------
-- Table structure for device_component_list
-- ----------------------------
DROP TABLE IF EXISTS "public"."device_component_list";
CREATE TABLE "public"."device_component_list" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"device_type_id" int4,
"component_type_id" int4,
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "public"."device_component_list" IS '设备组件列表';
COMMENT ON COLUMN "public"."device_component_list"."create_time" IS '创建时间';
COMMENT ON COLUMN "public"."device_component_list"."update_time" IS '修改时间';
COMMENT ON COLUMN "public"."device_component_list"."create_id" IS '创建人';
COMMENT ON COLUMN "public"."device_component_list"."update_id" IS '修改人';
COMMENT ON COLUMN "public"."device_component_list"."device_type_id" IS '设备类型ID';
COMMENT ON COLUMN "public"."device_component_list"."component_type_id" IS '部件类型ID';
COMMENT ON COLUMN "public"."device_component_list"."system_user_id" IS '归属的系统用户ID';
COMMENT ON COLUMN "public"."device_component_list"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of device_component_list
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table device_component_list
-- ----------------------------
CREATE UNIQUE INDEX "device_component_list_type_INDEX" ON "public"."device_component_list" USING btree ("device_type_id", "component_type_id", "system_user_id");

-- ----------------------------
-- Primary Key structure for table device_component_list
-- ----------------------------
ALTER TABLE "public"."device_component_list" ADD PRIMARY KEY ("id");
