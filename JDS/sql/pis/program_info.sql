
-- ----------------------------
-- Table structure for program_info
-- ----------------------------
DROP TABLE IF EXISTS "pis"."program_info";
CREATE TABLE "pis"."program_info" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"code" varchar(50),
"name" varchar(50),
"list_id"  int,
"unit_id" int,
"program_type" varchar(50),
"duration" varchar(50),
"file_name" varchar(50),
"file_path" varchar(255),
"program_source" varchar(50),
"sort" int,
"uploader_id" int,
"uploader_name" varchar(50),
"upload_date" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."program_info" IS '节目信息表';
COMMENT ON COLUMN "pis"."program_info"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."program_info"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."program_info"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."program_info"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."program_info"."code" IS '节目编号';
COMMENT ON COLUMN "pis"."program_info"."name" IS '节目名称';
COMMENT ON COLUMN "pis"."program_info"."list_id" IS '节目单ID';
COMMENT ON COLUMN "pis"."program_info"."unit_id" IS '节目栏ID';
COMMENT ON COLUMN "pis"."program_info"."program_type" IS '节目类型';
COMMENT ON COLUMN "pis"."program_info"."duration" IS '节目时长';
COMMENT ON COLUMN "pis"."program_info"."file_name" IS '文件名称';
COMMENT ON COLUMN "pis"."program_info"."file_path" IS '文件路径';
COMMENT ON COLUMN "pis"."program_info"."program_source" IS '节目来源';
COMMENT ON COLUMN "pis"."program_info"."sort" IS '序号';
COMMENT ON COLUMN "pis"."program_info"."uploader_id" IS '上传者ID';
COMMENT ON COLUMN "pis"."program_info"."uploader_name" IS '上传者姓名';
COMMENT ON COLUMN "pis"."program_info"."upload_date" IS '上传日期，含时分秒';
COMMENT ON COLUMN "pis"."program_info"."system_user_id" IS '系统用户ID';


-- ----------------------------
-- Records of program_info
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table program_info
-- ----------------------------
CREATE UNIQUE INDEX "program_info_code_INDEX" ON "pis"."program_info" USING btree ("code", "system_user_id");

-- ----------------------------
-- Primary Key structure for table program_info
-- ----------------------------
ALTER TABLE "pis"."program_info" ADD PRIMARY KEY ("id");
