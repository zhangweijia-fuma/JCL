
-- ----------------------------
-- Table structure for operation_line
-- ----------------------------
DROP TABLE IF EXISTS "pis"."operation_line";
CREATE TABLE "pis"."operation_line" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"name" varchar(50),
"name_en" varchar(50),
"code" varchar(50),
"memo" text ,
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."operation_line" IS '运营线路表';
COMMENT ON COLUMN "pis"."operation_line"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."operation_line"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."operation_line"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."operation_line"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."operation_line"."name" IS '线路名称';
COMMENT ON COLUMN "pis"."operation_line"."name_en" IS '英文名称';
COMMENT ON COLUMN "pis"."operation_line"."code" IS '线路编号';
COMMENT ON COLUMN "pis"."operation_line"."memo" IS '备注';
COMMENT ON COLUMN "pis"."operation_line"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of operation_line
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table operation_line
-- ----------------------------
CREATE UNIQUE INDEX "operation_line_code_INDEX" ON "pis"."operation_line" USING btree ("code", "system_user_id");

-- ----------------------------
-- Primary Key structure for table operation_line
-- ----------------------------
ALTER TABLE "pis"."operation_line" ADD PRIMARY KEY ("id");
