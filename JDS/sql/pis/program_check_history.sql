
-- ----------------------------
-- Table structure for program_check_history
-- ----------------------------
DROP TABLE IF EXISTS "pis"."program_check_history";
CREATE TABLE "pis"."program_check_history" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"list_id" int,
"checker_id" int,
"checker_name" varchar(50),
"check_date" varchar(50),
"check_opinion" varchar(50),
"check_result" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."program_check_history" IS '节目单审核记录表';
COMMENT ON COLUMN "pis"."program_check_history"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."program_check_history"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."program_check_history"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."program_check_history"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."program_check_history"."list_id" IS '节目单ID';
COMMENT ON COLUMN "pis"."program_check_history"."checker_id" IS '审核人名称';
COMMENT ON COLUMN "pis"."program_check_history"."checker_name" IS '审核人名称';
COMMENT ON COLUMN "pis"."program_check_history"."check_date" IS '审核日期';
COMMENT ON COLUMN "pis"."program_check_history"."check_opinion" IS '审核意见';
COMMENT ON COLUMN "pis"."program_check_history"."check_result" IS '审核结果';
COMMENT ON COLUMN "pis"."program_check_history"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of program_check_history
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table program_check_history
-- ----------------------------
CREATE UNIQUE INDEX "program_check_history_id_INDEX" ON "pis"."program_check_history" USING btree ("list_id", "system_user_id");

-- ----------------------------
-- Primary Key structure for table program_check_history
-- ----------------------------
ALTER TABLE "pis"."program_check_history" ADD PRIMARY KEY ("id");
