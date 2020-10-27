
-- ----------------------------
-- Table structure for controller_status_history
-- ----------------------------
DROP TABLE IF EXISTS "pis"."controller_status_history";
CREATE TABLE "pis"."controller_status_history" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"controller" int,
"ip_address" varchar(50),
"date" varchar(50),
"time" varchar(50),
"cpu" int,
"memory" int,
"harddisk" int,
"status" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."controller_status_history" IS '控制器状态历史记录表';
COMMENT ON COLUMN "pis"."controller_status_history"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."controller_status_history"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."controller_status_history"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."controller_status_history"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."controller_status_history"."controller" IS '控制器ID';
COMMENT ON COLUMN "pis"."controller_status_history"."ip_address" IS '控制器IP地址';
COMMENT ON COLUMN "pis"."controller_status_history"."date" IS '状态日期';
COMMENT ON COLUMN "pis"."controller_status_history"."time" IS '状态时间';
COMMENT ON COLUMN "pis"."controller_status_history"."cpu" IS 'CPU 占有率';
COMMENT ON COLUMN "pis"."controller_status_history"."memory" IS '内存占有率';
COMMENT ON COLUMN "pis"."controller_status_history"."harddisk" IS '硬盘占有率';
COMMENT ON COLUMN "pis"."controller_status_history"."status" IS '控制器状态，0：正常，1：未连接';
COMMENT ON COLUMN "pis"."controller_status_history"."system_user_id" IS '系统用户ID';




-- ----------------------------
-- Records of controller_status_history
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table controller_status_history
-- ----------------------------
CREATE UNIQUE INDEX "controller_status_history_INDEX" ON "pis"."controller_status_history" USING btree ("time", "date", "controller", "system_user_id");

-- ----------------------------
-- Primary Key structure for table controller_status_history
-- ----------------------------
ALTER TABLE "pis"."controller_status_history" ADD PRIMARY KEY ("id");
