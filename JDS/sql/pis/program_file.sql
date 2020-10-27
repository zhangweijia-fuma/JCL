
-- ----------------------------
-- Table structure for program_file
-- ----------------------------
DROP TABLE IF EXISTS "pis"."program_file";
CREATE TABLE "pis"."program_file" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"file_name" varchar(255),
"file_path" varchar(255),
"uploader_id" int,
"uploader_name" varchar(50),
"upload_date" varchar(50),
"video_type" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."program_file" IS '控制器表';
COMMENT ON COLUMN "pis"."program_file"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."program_file"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."program_file"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."program_file"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."program_file"."file_name" IS '文件名称';
COMMENT ON COLUMN "pis"."program_file"."file_path" IS '文件路径';
COMMENT ON COLUMN "pis"."program_file"."uploader_id" IS '上传者ID';
COMMENT ON COLUMN "pis"."program_file"."uploader_name" IS '上传者姓名';
COMMENT ON COLUMN "pis"."program_file"."upload_date" IS '上传日期，含时分秒';
COMMENT ON COLUMN "pis"."program_file"."video_type" IS '视频类型，0：安全垫片，1：普通节目视频';
COMMENT ON COLUMN "pis"."program_file"."system_user_id" IS '系统用户ID';


-- ----------------------------
-- Records of program_file
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table program_file
-- ----------------------------
CREATE UNIQUE INDEX "program_filename_INDEX" ON "pis"."program_file" USING btree ("file_name", "video_type", "system_user_id");

-- ----------------------------
-- Primary Key structure for table program_file
-- ----------------------------
ALTER TABLE "pis"."program_file" ADD PRIMARY KEY ("id");
