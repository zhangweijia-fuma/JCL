
-- ----------------------------
-- Table structure for controller_log_history
-- ----------------------------
DROP TABLE IF EXISTS "pis"."controller_log_history";
CREATE TABLE "pis"."controller_log_history" (
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

COMMENT ON TABLE "pis"."controller_log_history" IS '控制器日志历史记录表';
COMMENT ON COLUMN "pis"."controller_log_history"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."controller_log_history"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."controller_log_history"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."controller_log_history"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."controller_log_history"."controller" IS '控制器ID';
COMMENT ON COLUMN "pis"."controller_log_history"."log_id" IS '日志ID';
COMMENT ON COLUMN "pis"."controller_log_history"."date" IS '日志日期';
COMMENT ON COLUMN "pis"."controller_log_history"."time" IS '日志时间';
COMMENT ON COLUMN "pis"."controller_log_history"."type" IS '日志类别';
COMMENT ON COLUMN "pis"."controller_log_history"."level" IS '日志级别';
COMMENT ON COLUMN "pis"."controller_log_history"."title" IS '日志标题';
COMMENT ON COLUMN "pis"."controller_log_history"."content" IS '日志内容';
COMMENT ON COLUMN "pis"."controller_log_history"."system_user_id" IS '系统用户ID';



-- ----------------------------
-- Records of controller_log_history
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table controller_log_history
-- ----------------------------
CREATE UNIQUE INDEX "controller_log_history_INDEX" ON "pis"."controller_log_history" USING btree ("time", "date", "controller", "system_user_id");

-- ----------------------------
-- Primary Key structure for table controller_log_history
-- ----------------------------
ALTER TABLE "pis"."controller_log_history" ADD PRIMARY KEY ("id");
