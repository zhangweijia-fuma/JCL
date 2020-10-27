
-- ----------------------------
-- Table structure for rsu_project
-- ----------------------------
DROP TABLE IF EXISTS "rsu"."rsu_project";
CREATE TABLE "rsu"."rsu_project" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"project_name" varchar(128),
"project_amount" float8,
"sign_date" varchar(50),
"sign_addr" varchar(50),
"parta_id" int4,
"parta_code" varchar(50),
"parta_man_name" varchar(50),
"parta_title" varchar(50),
"partb_id" int4,
"partb_code" varchar(50),
"partb_man_name" varchar(50),
"partb_title" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "rsu"."rsu_project" IS 'RSU项目表';
COMMENT ON COLUMN "rsu"."rsu_project"."create_time" IS '创建时间';
COMMENT ON COLUMN "rsu"."rsu_project"."update_time" IS '修改时间';
COMMENT ON COLUMN "rsu"."rsu_project"."create_id" IS '创建人';
COMMENT ON COLUMN "rsu"."rsu_project"."update_id" IS '修改人';
COMMENT ON COLUMN "rsu"."rsu_project"."project_name" IS '合同名称';
COMMENT ON COLUMN "rsu"."rsu_project"."project_amount" IS '合同金额';
COMMENT ON COLUMN "rsu"."rsu_project"."sign_date" IS '签署日期';
COMMENT ON COLUMN "rsu"."rsu_project"."sign_addr" IS '签署地点';

COMMENT ON COLUMN "rsu"."rsu_project"."parta_id" IS '甲方ID';
COMMENT ON COLUMN "rsu"."rsu_project"."parta_code" IS '甲方合同号';
COMMENT ON COLUMN "rsu"."rsu_project"."parta_man_name" IS '甲方代表人名称';
COMMENT ON COLUMN "rsu"."rsu_project"."parta_title" IS '甲方代表人职务';

COMMENT ON COLUMN "rsu"."rsu_project"."partb_id" IS '乙方ID';
COMMENT ON COLUMN "rsu"."rsu_project"."partb_code" IS '乙方合同号';
COMMENT ON COLUMN "rsu"."rsu_project"."partb_man_name" IS '乙方代表人名称';
COMMENT ON COLUMN "rsu"."rsu_project"."partb_title" IS '乙方代表人职务';

COMMENT ON COLUMN "rsu"."rsu_project"."system_user_id" IS '归属的系统用户ID';

-- ----------------------------
-- Records of rsu_project
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table rsu_project
-- ----------------------------
CREATE UNIQUE INDEX "rsu_project_code_INDEX" ON "rsu"."rsu_project" USING btree ("project_name", "system_user_id");
CREATE INDEX "rsu_project_parta_INDEX" ON "rsu"."rsu_project" USING btree ("parta_id", "system_user_id");
CREATE INDEX "rsu_project_partb_INDEX" ON "rsu"."rsu_project" USING btree ("partb_id", "system_user_id");

-- ----------------------------
-- Primary Key structure for table rsu_project
-- ----------------------------
ALTER TABLE "rsu"."rsu_project" ADD PRIMARY KEY ("id");
