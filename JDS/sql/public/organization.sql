-- ----------------------------
-- Table structure for organization
-- ----------------------------
DROP TABLE IF EXISTS "public"."organization";
CREATE TABLE "public"."organization" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"name" varchar(100),
"name_en" varchar(255),
"abbr" varchar(50),
"abbr_en" varchar(50),
"organization_type" varchar(50),
"registe_date" varchar(50),
"registe_addr" varchar(255),
"current_addr" varchar(255),
"business_scope" text,
"social_credit_code" varchar(50),
"legal_representative" varchar(50),
"legal_id_type" varchar(50),
"legal_id_no" varchar(50),
"contactor_name" varchar(50),
"contactor_mobile" varchar(50),
"logo_file" varchar(50),
"description" text,
"system_user_id" int
)
WITH (OIDS=FALSE);

COMMENT ON TABLE "public"."organization" IS '组织基本信息表';
COMMENT ON COLUMN "public"."organization"."id" IS 'Id';
COMMENT ON COLUMN "public"."organization"."create_time" IS '创建时间';
COMMENT ON COLUMN "public"."organization"."update_time" IS '修改时间';
COMMENT ON COLUMN "public"."organization"."create_id" IS '创建人';
COMMENT ON COLUMN "public"."organization"."update_id" IS '修改人';
COMMENT ON COLUMN "public"."organization"."name" IS '组织名称';
COMMENT ON COLUMN "public"."organization"."name_en" IS '组织英文名称';
COMMENT ON COLUMN "public"."organization"."abbr" IS '组织名称缩写';
COMMENT ON COLUMN "public"."organization"."abbr_en" IS '组织英文名称缩写';
COMMENT ON COLUMN "public"."organization"."organization_type" IS '组织类别';
COMMENT ON COLUMN "public"."organization"."registe_date" IS '注册时间';
COMMENT ON COLUMN "public"."organization"."registe_addr" IS '注册地址';
COMMENT ON COLUMN "public"."organization"."business_scope" IS '经营范围';
COMMENT ON COLUMN "public"."organization"."social_credit_code" IS '组织社会信用代码';
COMMENT ON COLUMN "public"."organization"."legal_representative" IS '法人代表名称';
COMMENT ON COLUMN "public"."organization"."legal_id_type" IS '法人代表身份证件类型';
COMMENT ON COLUMN "public"."organization"."legal_id_no" IS '法人证件号码';
COMMENT ON COLUMN "public"."organization"."contactor_name" IS '联系人名称';
COMMENT ON COLUMN "public"."organization"."contactor_mobile" IS '联系人电话';
COMMENT ON COLUMN "public"."organization"."logo_file" IS '组织Logo文件名称';
COMMENT ON COLUMN "public"."organization"."description" IS '简介说明';
COMMENT ON COLUMN "public"."organization"."system_user_id" IS '归属的系统用户ID';

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table organization
-- ----------------------------
CREATE UNIQUE INDEX "organization_name" ON "public"."organization" USING btree ("name", "system_user_id");
CREATE UNIQUE INDEX "organization_name_en" ON "public"."organization" USING btree ("name_en", "system_user_id");
CREATE INDEX "organization_abbr" ON "public"."organization" USING btree ("abbr", "system_user_id");

-- ----------------------------
-- Primary Key structure for table organization
-- ----------------------------
ALTER TABLE "public"."organization" ADD PRIMARY KEY ("id");
