
-- ----------------------------
-- Table structure for component_property_list
-- ----------------------------
DROP TABLE IF EXISTS "public"."component_property_list";
CREATE TABLE "public"."component_property_list" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"component_id" int4,
"property_id" int4,
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "public"."component_property_list" IS '设备部件属性关系表';
COMMENT ON COLUMN "public"."component_property_list"."create_time" IS '创建时间';
COMMENT ON COLUMN "public"."component_property_list"."update_time" IS '修改时间';
COMMENT ON COLUMN "public"."component_property_list"."create_id" IS '创建人';
COMMENT ON COLUMN "public"."component_property_list"."update_id" IS '修改人';
COMMENT ON COLUMN "public"."component_property_list"."component_id" IS '部件ID';
COMMENT ON COLUMN "public"."component_property_list"."property_id" IS '属性ID';
COMMENT ON COLUMN "public"."component_property_list"."system_user_id" IS '归属的系统用户ID';
COMMENT ON COLUMN "public"."component_property_list"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of component_property_list
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table component_property_list
-- ----------------------------
CREATE INDEX "component_property_list_id_INDEX" ON "public"."component_property_list" USING btree ("component_id", "property_id", "system_user_id");

-- ----------------------------
-- Primary Key structure for table component_property_list
-- ----------------------------
ALTER TABLE "public"."component_property_list" ADD PRIMARY KEY ("id");
