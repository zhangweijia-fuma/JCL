
-- ----------------------------
-- Table structure for property_list
-- ----------------------------
DROP TABLE IF EXISTS "public"."property_list";
CREATE TABLE "public"."property_list" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"code" varchar(128),
"name" varchar(128),
"name_en" varchar(128),
"value_type" varchar(128),
"value_s" text,
"value_i" int,
"value_f" float,
"description" varchar(128),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "public"."property_list" IS '属性类型表';
COMMENT ON COLUMN "public"."property_list"."create_time" IS '创建时间';
COMMENT ON COLUMN "public"."property_list"."update_time" IS '修改时间';
COMMENT ON COLUMN "public"."property_list"."create_id" IS '创建人';
COMMENT ON COLUMN "public"."property_list"."update_id" IS '修改人';
COMMENT ON COLUMN "public"."property_list"."code" IS '属性类型编号';
COMMENT ON COLUMN "public"."property_list"."name" IS '属性名称';
COMMENT ON COLUMN "public"."property_list"."name_en" IS '属性英文名称';
COMMENT ON COLUMN "public"."property_list"."value_type" IS '属性值类型';
COMMENT ON COLUMN "public"."property_list"."value_s" IS '字符串属性值';
COMMENT ON COLUMN "public"."property_list"."value_i" IS '整数属性值';
COMMENT ON COLUMN "public"."property_list"."value_f" IS '小数属性值';
COMMENT ON COLUMN "public"."property_list"."description" IS '属性描述';
COMMENT ON COLUMN "public"."property_list"."system_user_id" IS '归属的系统用户ID';
COMMENT ON COLUMN "public"."property_list"."system_user_id" IS '系统用户ID';
-- ----------------------------
-- Records of property_list
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table property_list
-- ----------------------------
CREATE UNIQUE INDEX "proerty_list_code_INDEX" ON "public"."property_list" USING btree ("code", "system_user_id");

-- ----------------------------
-- Primary Key structure for table property_list
-- ----------------------------
ALTER TABLE "public"."property_list" ADD PRIMARY KEY ("id");
