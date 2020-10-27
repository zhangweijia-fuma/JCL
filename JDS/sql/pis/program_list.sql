
-- ----------------------------
-- Table structure for program_list
-- ----------------------------
DROP TABLE IF EXISTS "pis"."program_list";
CREATE TABLE "pis"."program_list" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"code" varchar(50),
"name" varchar(50),
"valid_date" varchar(50),
"apply_id" int,
"apply_name" varchar(50),
"apply_date" varchar(50),
"apply_status" varchar(50),
"checker_id" int,
"checker_name" varchar(50),
"check_date" varchar(50),
"video_type" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."program_list" IS '节目单表';
COMMENT ON COLUMN "pis"."program_list"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."program_list"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."program_list"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."program_list"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."program_list"."code" IS '节目单编号';
COMMENT ON COLUMN "pis"."program_list"."name" IS '节目单名称';
COMMENT ON COLUMN "pis"."program_list"."valid_date" IS '节目单生效日期';
COMMENT ON COLUMN "pis"."program_list"."apply_id" IS '申请人ID';
COMMENT ON COLUMN "pis"."program_list"."apply_name" IS '申请人姓名';
COMMENT ON COLUMN "pis"."program_list"."apply_date" IS '申请日期，含时分秒';
COMMENT ON COLUMN "pis"."program_list"."apply_status" IS '审批状态，0：未提交，1：通过，2：驳回，数据字典编码：PROGRAM_APPLY_STATUS';

COMMENT ON COLUMN "pis"."program_list"."checker_id" IS '审批人ID';
COMMENT ON COLUMN "pis"."program_list"."checker_name" IS '审批人姓名';
COMMENT ON COLUMN "pis"."program_list"."check_date" IS '审批日期，含时分秒';
COMMENT ON COLUMN "pis"."program_list"."video_type" IS '节目类型，0：录播，1：直播';
COMMENT ON COLUMN "pis"."program_list"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of program_list
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table program_list
-- ----------------------------
CREATE UNIQUE INDEX "program_list_code_INDEX" ON "pis"."program_list" USING btree ("code", "system_user_id");

-- ----------------------------
-- Primary Key structure for table program_list
-- ----------------------------
ALTER TABLE "pis"."program_list" ADD PRIMARY KEY ("id");
