
-- ----------------------------
-- Table structure for controller
-- ----------------------------
DROP TABLE IF EXISTS "iot"."gps_module_trace";
CREATE TABLE "iot"."gps_module_trace" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"module_id" int,
"ip_addr" varchar(50),
"time" varchar(50),
"longitude" varchar(50),
"latitude" varchar(50),
"addr" text,
"cpu" varchar(50),
"memory" varchar(50),
"harddisk" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "iot"."gps_module_trace" IS '定位模块轨迹表';
COMMENT ON COLUMN "iot"."gps_module_trace"."create_time" IS '创建时间';
COMMENT ON COLUMN "iot"."gps_module_trace"."update_time" IS '修改时间';
COMMENT ON COLUMN "iot"."gps_module_trace"."create_id" IS '创建人';
COMMENT ON COLUMN "iot"."gps_module_trace"."update_id" IS '修改人';
COMMENT ON COLUMN "iot"."gps_module_trace"."module_id" IS '定位模块ID';
COMMENT ON COLUMN "iot"."gps_module_trace"."ip_addr" IS '模块IP地址';
COMMENT ON COLUMN "iot"."gps_module_trace"."time" IS '定位时间';
COMMENT ON COLUMN "iot"."gps_module_trace"."latitude" IS '纬度';
COMMENT ON COLUMN "iot"."gps_module_trace"."longitude" IS '经度';
COMMENT ON COLUMN "iot"."gps_module_trace"."addr" IS '地址';
COMMENT ON COLUMN "iot"."gps_module_trace"."cpu" IS 'CPU占有率';
COMMENT ON COLUMN "iot"."gps_module_trace"."memory" IS '内存占有率';
COMMENT ON COLUMN "iot"."gps_module_trace"."harddisk" IS '硬盘使用率';
COMMENT ON COLUMN "iot"."gps_module_trace"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of gps_module_trace
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table gps_module_trace
-- ----------------------------
CREATE UNIQUE INDEX "gps_module_trace_id_INDEX" ON "iot"."gps_module_trace" USING btree ("id", "system_user_id");
CREATE INDEX "gps_module_trace_module_id_INDEX" ON "iot"."gps_module_trace" USING btree ("module_id", "system_user_id");

-- ----------------------------
-- Primary Key structure for table gps_module_trace
-- ----------------------------
ALTER TABLE "iot"."gps_module_trace" ADD PRIMARY KEY ("id");
