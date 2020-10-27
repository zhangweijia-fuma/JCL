
-- ----------------------------
-- Table structure for device_type
-- ----------------------------
DROP TABLE IF EXISTS "public"."device_type";
CREATE TABLE "public"."device_type" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"code" varchar(128),
"name" varchar(128),
"name_en" varchar(128),
"abbr_en" varchar(50),
"description" varchar(128),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "public"."device_type" IS '设备部件类型表';
COMMENT ON COLUMN "public"."device_type"."create_time" IS '创建时间';
COMMENT ON COLUMN "public"."device_type"."update_time" IS '修改时间';
COMMENT ON COLUMN "public"."device_type"."create_id" IS '创建人';
COMMENT ON COLUMN "public"."device_type"."update_id" IS '修改人';
COMMENT ON COLUMN "public"."device_type"."code" IS '设备类型编号';
COMMENT ON COLUMN "public"."device_type"."name" IS '设备名称';
COMMENT ON COLUMN "public"."device_type"."name_en" IS '设备英文名称';
COMMENT ON COLUMN "public"."device_type"."abbr_en" IS '设备英文简称，长度为3字节';
COMMENT ON COLUMN "public"."device_type"."description" IS '设备描述';
COMMENT ON COLUMN "public"."device_type"."system_user_id" IS '归属的系统用户ID';

-- ----------------------------
-- Records of device_type
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table device_type
-- ----------------------------
CREATE UNIQUE INDEX "device_type_code_INDEX" ON "public"."device_type" USING btree ("name", "system_user_id");

-- ----------------------------
-- Primary Key structure for table device_type
-- ----------------------------
ALTER TABLE "public"."device_type" ADD PRIMARY KEY ("id");
