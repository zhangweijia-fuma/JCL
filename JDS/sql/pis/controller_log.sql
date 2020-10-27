
-- ----------------------------
-- Table structure for controller_log
-- ----------------------------
DROP TABLE IF EXISTS "pis"."controller_log";
CREATE TABLE "pis"."controller_log" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"controller" int,
"log_id" int,
"date" varchar(50),
"time" varchar(50),
"level" varchar(50),
"type" varchar(50),
"title" varchar(50),
"content" text,
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."controller_log" IS '控制器日志表';
COMMENT ON COLUMN "pis"."controller_log"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."controller_log"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."controller_log"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."controller_log"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."controller_log"."controller" IS '控制器ID';
COMMENT ON COLUMN "pis"."controller_log"."log_id" IS '日志ID';
COMMENT ON COLUMN "pis"."controller_log"."date" IS '日志日期';
COMMENT ON COLUMN "pis"."controller_log"."time" IS '日志时间';
COMMENT ON COLUMN "pis"."controller_log"."type" IS '日志类别';
COMMENT ON COLUMN "pis"."controller_log"."level" IS '日志级别';
COMMENT ON COLUMN "pis"."controller_log"."title" IS '日志标题';
COMMENT ON COLUMN "pis"."controller_log"."content" IS '日志内容';
COMMENT ON COLUMN "pis"."controller_log"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of controller_log
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table controller_log
-- ----------------------------
CREATE UNIQUE INDEX "controller_log_INDEX" ON "pis"."controller_log" USING btree ("time", "date", "controller", "system_user_id");

-- ----------------------------
-- Primary Key structure for table controller_log
-- ----------------------------
ALTER TABLE "pis"."controller_log" ADD PRIMARY KEY ("id");
