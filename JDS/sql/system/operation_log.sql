

-- ----------------------------
-- Table structure for operation_log
-- ----------------------------
DROP TABLE IF EXISTS "system"."operation_log";
CREATE TABLE "system"."operation_log" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"operator_id" int4,
"menu_code" varchar(50) COLLATE "default",
"operation_context" text,
"operation_id" int4,
"operation_no" varchar(50) COLLATE "default",
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "system"."operation_log" IS '操作日志表';
COMMENT ON COLUMN "system"."operation_log"."id" IS 'Id号';
COMMENT ON COLUMN "system"."operation_log"."create_time" IS '创建时间';
COMMENT ON COLUMN "system"."operation_log"."update_time" IS '修改时间';
COMMENT ON COLUMN "system"."operation_log"."create_id" IS '创建人';
COMMENT ON COLUMN "system"."operation_log"."update_id" IS '修改人';
COMMENT ON COLUMN "system"."operation_log"."operator_id" IS '操作者id';
COMMENT ON COLUMN "system"."operation_log"."menu_code" IS '操作项';
COMMENT ON COLUMN "system"."operation_log"."operation_context" IS '操作内容';
COMMENT ON COLUMN "system"."operation_log"."operation_id" IS '操作记录id';
COMMENT ON COLUMN "system"."operation_log"."operation_no" IS '操作単号';
COMMENT ON COLUMN "system"."operation_log"."system_user_id" IS '日志归属的系统用户类型ID';


-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table operation_log
-- ----------------------------
CREATE UNIQUE INDEX "operation_log_index" ON "system"."operation_log" USING btree ("system_user_id");

-- ----------------------------
-- Primary Key structure for table operation_log
-- ----------------------------
ALTER TABLE "system"."operation_log" ADD PRIMARY KEY ("id");
