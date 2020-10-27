
-- ----------------------------
-- Table structure for component_list
-- ----------------------------
DROP TABLE IF EXISTS "public"."component_list";
CREATE TABLE "public"."component_list" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"code" varchar(128),
"name" varchar(128),
"name_en" varchar(128),
"description" varchar(128),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "public"."component_list" IS '设备部件类型表';
COMMENT ON COLUMN "public"."component_list"."create_time" IS '创建时间';
COMMENT ON COLUMN "public"."component_list"."update_time" IS '修改时间';
COMMENT ON COLUMN "public"."component_list"."create_id" IS '创建人';
COMMENT ON COLUMN "public"."component_list"."update_id" IS '修改人';
COMMENT ON COLUMN "public"."component_list"."code" IS '部件类型编号';
COMMENT ON COLUMN "public"."component_list"."name" IS '部件名称';
COMMENT ON COLUMN "public"."component_list"."name_en" IS '部件英文名称';
COMMENT ON COLUMN "public"."component_list"."description" IS '部件描述';
COMMENT ON COLUMN "public"."component_list"."system_user_id" IS '归属的系统用户ID';

-- ----------------------------
-- Records of component_list
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table component_list
-- ----------------------------
CREATE UNIQUE INDEX "component_list_code_INDEX" ON "public"."component_list" USING btree ("code", "system_user_id");

-- ----------------------------
-- Primary Key structure for table component_list
-- ----------------------------
ALTER TABLE "public"."component_list" ADD PRIMARY KEY ("id");
