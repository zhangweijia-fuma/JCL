
-- ----------------------------
-- Table structure for program_unit
-- ----------------------------
DROP TABLE IF EXISTS "pis"."program_unit";
CREATE TABLE "pis"."program_unit" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"code" varchar(50),
"name" varchar(50),
"start_time" varchar(50),
"end_time" varchar(50),
"list_id"  int,
"sort" int,
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."program_unit" IS '节目栏表';
COMMENT ON COLUMN "pis"."program_unit"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."program_unit"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."program_unit"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."program_unit"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."program_unit"."code" IS '节目单编号';
COMMENT ON COLUMN "pis"."program_unit"."name" IS '节目单名称';
COMMENT ON COLUMN "pis"."program_unit"."start_time" IS '节目栏开始时间';
COMMENT ON COLUMN "pis"."program_unit"."end_time" IS '节目栏结束时间';
COMMENT ON COLUMN "pis"."program_unit"."list_id" IS '节目单ID';
COMMENT ON COLUMN "pis"."program_unit"."sort" IS '节目栏在节目单中的排序序号';
COMMENT ON COLUMN "pis"."program_unit"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of program_unit
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table program_unit
-- ----------------------------
CREATE UNIQUE INDEX "program_unit_code_INDEX" ON "pis"."program_unit" USING btree ("code", "system_user_id");

-- ----------------------------
-- Primary Key structure for table program_unit
-- ----------------------------
ALTER TABLE "pis"."program_unit" ADD PRIMARY KEY ("id");
