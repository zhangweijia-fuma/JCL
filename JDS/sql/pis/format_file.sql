
-- ----------------------------
-- Table structure for format_file
-- ----------------------------
DROP TABLE IF EXISTS "pis"."format_file";
CREATE TABLE "pis"."format_file" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"code" varchar(50),
"name" varchar(50),
"uploader_id" int,
"uploader_name" varchar(50),
"upload_date" varchar(50),
"file_name" varchar(50),
"file_path" varchar(255),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."format_file" IS '版式文件表';
COMMENT ON COLUMN "pis"."format_file"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."format_file"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."format_file"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."format_file"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."format_file"."code" IS '节目单编号';
COMMENT ON COLUMN "pis"."format_file"."name" IS '节目单名称';
COMMENT ON COLUMN "pis"."format_file"."uploader_id" IS '上传人ID';
COMMENT ON COLUMN "pis"."format_file"."uploader_name" IS '上传人姓名';
COMMENT ON COLUMN "pis"."format_file"."upload_date" IS '上传日期，含时分秒';
COMMENT ON COLUMN "pis"."format_file"."file_name" IS '文件名';
COMMENT ON COLUMN "pis"."format_file"."file_path" IS '文件路径';
COMMENT ON COLUMN "pis"."format_file"."system_user_id" IS '系统用户ID';


-- ----------------------------
-- Records of format_file
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table format_file
-- ----------------------------
CREATE UNIQUE INDEX "program_list_name_INDEX" ON "pis"."format_file" USING btree ("name", "system_user_id");

-- ----------------------------
-- Primary Key structure for table format_file
-- ----------------------------
ALTER TABLE "pis"."format_file" ADD PRIMARY KEY ("id");
