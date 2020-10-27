-- ----------------------------
-- Table structure for file
-- ----------------------------
DROP TABLE IF EXISTS "public"."file";
CREATE TABLE "public"."file" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"file_name" varchar(100) COLLATE "default",
"path" varchar(255) COLLATE "default",
"file_size" int8,
"file_type" varchar(32) COLLATE "default",
"store_path" varchar(255) COLLATE "default",
"file_service_type" varchar(50) COLLATE "default",
"file_service_id" int4,
"system_user_id" int
)
WITH (OIDS=FALSE);

COMMENT ON TABLE "public"."file" IS '文件表';
COMMENT ON COLUMN "public"."file"."id" IS 'Id号';
COMMENT ON COLUMN "public"."file"."create_time" IS '创建时间';
COMMENT ON COLUMN "public"."file"."update_time" IS '修改时间';
COMMENT ON COLUMN "public"."file"."create_id" IS '创建人';
COMMENT ON COLUMN "public"."file"."update_id" IS '修改人';
COMMENT ON COLUMN "public"."file"."file_name" IS '文件名';
COMMENT ON COLUMN "public"."file"."path" IS '文件路径';
COMMENT ON COLUMN "public"."file"."system_user_id" IS '归属的系统用户ID';

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table file
-- ----------------------------
ALTER TABLE "public"."file" ADD PRIMARY KEY ("id");
