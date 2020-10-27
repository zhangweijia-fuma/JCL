
-- ----------------------------
-- Table structure for controller
-- ----------------------------
DROP TABLE IF EXISTS "pis"."controller";
CREATE TABLE "pis"."controller" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"serial_no" varchar(50),
"code" varchar(50),
"name" varchar(50),
"device_type" varchar(50),
"install_type" varchar(50),
"install_id" int,
"ip_address" varchar(50),
"open_time" varchar(50),
"close_time" varchar(50),
"sort" int,
"line_id" int,
"status" varchar(50),
"status_update_time" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."controller" IS '控制器表';
COMMENT ON COLUMN "pis"."controller"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."controller"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."controller"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."controller"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."controller"."serial_no" IS '控制器出厂序列号';
COMMENT ON COLUMN "pis"."controller"."code" IS '控制器编号';
COMMENT ON COLUMN "pis"."controller"."name" IS '控制器名称';
COMMENT ON COLUMN "pis"."controller"."device_type" IS '控制器设备类型';
COMMENT ON COLUMN "pis"."controller"."install_type" IS '控制器安装类型，0：车辆控制器，1：站台上行控制器，2：站台下行控制器';
COMMENT ON COLUMN "pis"."controller"."install_id" IS '控制器安装地的ID号，如果是安装在站台，则ID为车站ID，安装在车辆则为车辆ID';
COMMENT ON COLUMN "pis"."controller"."ip_address" IS '控制器IP地址';
COMMENT ON COLUMN "pis"."controller"."open_time" IS '控制器开机时间';
COMMENT ON COLUMN "pis"."controller"."close_time" IS '控制器关机时间';
COMMENT ON COLUMN "pis"."controller"."sort" IS '控制器显示顺序';
COMMENT ON COLUMN "pis"."controller"."line_id" IS '控制器归属线路ID';
COMMENT ON COLUMN "pis"."controller"."status" IS '控制器状态';
COMMENT ON COLUMN "pis"."controller"."status_update_time" IS '控制器状态更新时间';

COMMENT ON COLUMN "pis"."controller"."system_user_id" IS '系统用户ID';


-- ----------------------------
-- Records of controller
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table controller
-- ----------------------------
CREATE UNIQUE INDEX "controller_code_INDEX" ON "pis"."controller" USING btree ("code", "system_user_id");

-- ----------------------------
-- Primary Key structure for table controller
-- ----------------------------
ALTER TABLE "pis"."controller" ADD PRIMARY KEY ("id");
