
-- ----------------------------
-- Table structure for gps_module
-- ----------------------------
DROP TABLE IF EXISTS "iot"."gps_module";
CREATE TABLE "iot"."gps_module" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"name" varchar(50),
"code" varchar(50),
"serial_no" varchar(50),
"device_type" varchar(50),
"vendor" varchar(50),
"ip_addr" varchar(50),
"mac_addr" varchar(50),
"install_date" varchar(50),
"install_batch" varchar(50),
"install_type" varchar(50),
"install_id" int,
"status" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "iot"."gps_module" IS '定位模块表';
COMMENT ON COLUMN "iot"."gps_module"."create_time" IS '创建时间';
COMMENT ON COLUMN "iot"."gps_module"."update_time" IS '修改时间';
COMMENT ON COLUMN "iot"."gps_module"."create_id" IS '创建人';
COMMENT ON COLUMN "iot"."gps_module"."update_id" IS '修改人';
COMMENT ON COLUMN "iot"."gps_module"."name" IS '模块名称';
COMMENT ON COLUMN "iot"."gps_module"."code" IS '模块编号';
COMMENT ON COLUMN "iot"."gps_module"."serial_no" IS '控制器出厂序列号';
COMMENT ON COLUMN "iot"."gps_module"."device_type" IS '控制器设备类型';
COMMENT ON COLUMN "iot"."gps_module"."vendor" IS '生产厂家名称';
COMMENT ON COLUMN "iot"."gps_module"."ip_addr" IS '模块IP地址';
COMMENT ON COLUMN "iot"."gps_module"."mac_addr" IS '模块MAC地址';
COMMENT ON COLUMN "iot"."gps_module"."install_date" IS '安装日期';
COMMENT ON COLUMN "iot"."gps_module"."install_batch" IS '安装批次';
COMMENT ON COLUMN "iot"."gps_module"."install_type" IS '安装类型';
COMMENT ON COLUMN "iot"."gps_module"."install_id" IS '安装地点的ID';
COMMENT ON COLUMN "iot"."gps_module"."status" IS '定位模块状态';
COMMENT ON COLUMN "iot"."gps_module"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of gps_module
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table gps_module
-- ----------------------------
CREATE UNIQUE INDEX "gps_module_code_INDEX" ON "iot"."gps_module" USING btree ("code", "system_user_id");

-- ----------------------------
-- Primary Key structure for table gps_module
-- ----------------------------
ALTER TABLE "iot"."gps_module" ADD PRIMARY KEY ("id");
