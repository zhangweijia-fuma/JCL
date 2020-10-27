
-- ----------------------------
-- Table structure for vehicle
-- ----------------------------
DROP TABLE IF EXISTS "pis"."vehicle";
CREATE TABLE "pis"."vehicle" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"serial_no" varchar(50),
"name" varchar(50),
"code" varchar(50),
"type" varchar(50),
"default_count" int,
"train_id" int,
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."vehicle" IS '车辆表';
COMMENT ON COLUMN "pis"."vehicle"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."vehicle"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."vehicle"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."vehicle"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."vehicle"."serial_no" IS '车辆出厂序列号';
COMMENT ON COLUMN "pis"."vehicle"."name" IS '车辆名称';
COMMENT ON COLUMN "pis"."vehicle"."code" IS '车辆编号';
COMMENT ON COLUMN "pis"."vehicle"."type" IS '车辆类型';
COMMENT ON COLUMN "pis"."vehicle"."default_count" IS '车辆控制器数量';
COMMENT ON COLUMN "pis"."vehicle"."train_id" IS '车辆归属的列车编组号';
COMMENT ON COLUMN "pis"."vehicle"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of vehicle
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table vehicle
-- ----------------------------
CREATE UNIQUE INDEX "vehicle_code_INDEX" ON "pis"."vehicle" USING btree ("code", "system_user_id");

-- ----------------------------
-- Primary Key structure for table vehicle
-- ----------------------------
ALTER TABLE "pis"."vehicle" ADD PRIMARY KEY ("id");
