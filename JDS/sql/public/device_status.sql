
-- ----------------------------
-- Table structure for device_status
-- ----------------------------
DROP TABLE IF EXISTS "public"."device_status";
CREATE TABLE "public"."device_status" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"device_id" int,
"ip_address" varchar(50),
"date" varchar(50),
"time" varchar(50),
"cpu" int,
"memory" int,
"harddisk" int,
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "public"."device_status" IS '控制器状态表';
COMMENT ON COLUMN "public"."device_status"."create_time" IS '创建时间';
COMMENT ON COLUMN "public"."device_status"."update_time" IS '修改时间';
COMMENT ON COLUMN "public"."device_status"."create_id" IS '创建人';
COMMENT ON COLUMN "public"."device_status"."update_id" IS '修改人';
COMMENT ON COLUMN "public"."device_status"."device_id" IS '控制器ID';
COMMENT ON COLUMN "public"."device_status"."ip_address" IS '控制器IP地址';
COMMENT ON COLUMN "public"."device_status"."date" IS '状态日期';
COMMENT ON COLUMN "public"."device_status"."time" IS '状态时间';
COMMENT ON COLUMN "public"."device_status"."cpu" IS 'CPU 占有率';
COMMENT ON COLUMN "public"."device_status"."memory" IS '内存占有率';
COMMENT ON COLUMN "public"."device_status"."harddisk" IS '硬盘占有率';
COMMENT ON COLUMN "public"."device_status"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of device_status
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table device_status
-- ----------------------------
CREATE UNIQUE INDEX "device_status_INDEX" ON "public"."device_status" USING btree ("time", "date", "device_id", "system_user_id");

-- ----------------------------
-- Primary Key structure for table device_status
-- ----------------------------
ALTER TABLE "public"."device_status" ADD PRIMARY KEY ("id");
