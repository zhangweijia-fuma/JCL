
-- ----------------------------
-- Table structure for controller_status
-- ----------------------------
DROP TABLE IF EXISTS "pis"."controller_status";
CREATE TABLE "pis"."controller_status" (
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

COMMENT ON TABLE "pis"."controller_status" IS '控制器状态表';
COMMENT ON COLUMN "pis"."controller_status"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."controller_status"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."controller_status"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."controller_status"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."controller_status"."controller" IS '控制器ID';
COMMENT ON COLUMN "pis"."controller_status"."ip_address" IS '控制器IP地址';
COMMENT ON COLUMN "pis"."controller_status"."date" IS '状态日期';
COMMENT ON COLUMN "pis"."controller_status"."time" IS '状态时间';
COMMENT ON COLUMN "pis"."controller_status"."cpu" IS 'CPU 占有率';
COMMENT ON COLUMN "pis"."controller_status"."memory" IS '内存占有率';
COMMENT ON COLUMN "pis"."controller_status"."harddisk" IS '硬盘占有率';
COMMENT ON COLUMN "pis"."controller_status"."status" IS '控制器状态，0：正常，1：未连接';
COMMENT ON COLUMN "pis"."controller_status"."system_user_id" IS '系统用户ID';




-- ----------------------------
-- Records of controller_status
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table controller_status
-- ----------------------------
CREATE UNIQUE INDEX "controller_status_INDEX" ON "pis"."controller_status" USING btree ("time", "date", "controller", "system_user_id");

-- ----------------------------
-- Primary Key structure for table controller_status
-- ----------------------------
ALTER TABLE "pis"."controller_status" ADD PRIMARY KEY ("id");
