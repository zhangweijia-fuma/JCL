
-- ----------------------------
-- Table structure for rsu_list
-- ----------------------------
DROP TABLE IF EXISTS "rsu"."rsu_list";
CREATE TABLE "rsu"."rsu_list" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"device_id" int4,
"ip_address1" varchar(50),
"ip_address2" varchar(50),
"longtitude" float8,
"latitude" float8,
"project_id" int4,
"install_location" varchar(128),
"map_location" varchar(128),
"install_date" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "rsu"."rsu_list" IS 'RSU设备表';
COMMENT ON COLUMN "rsu"."rsu_list"."create_time" IS '创建时间';
COMMENT ON COLUMN "rsu"."rsu_list"."update_time" IS '修改时间';
COMMENT ON COLUMN "rsu"."rsu_list"."create_id" IS '创建人';
COMMENT ON COLUMN "rsu"."rsu_list"."update_id" IS '修改人';
COMMENT ON COLUMN "rsu"."rsu_list"."device_id" IS '设备ID';
COMMENT ON COLUMN "rsu"."rsu_list"."ip_address1" IS 'IP 地址1';
COMMENT ON COLUMN "rsu"."rsu_list"."ip_address2" IS 'IP 地址2';
COMMENT ON COLUMN "rsu"."rsu_list"."longtitude" IS '经度';
COMMENT ON COLUMN "rsu"."rsu_list"."latitude" IS '纬度';
COMMENT ON COLUMN "rsu"."rsu_list"."project_id" IS '项目ID';
COMMENT ON COLUMN "rsu"."rsu_list"."install_location" IS '安装路段';
COMMENT ON COLUMN "rsu"."rsu_list"."map_location" IS '地图获取的安装位置';
COMMENT ON COLUMN "rsu"."rsu_list"."install_date" IS '安装日期';
COMMENT ON COLUMN "rsu"."rsu_list"."system_user_id" IS '归属的系统用户ID';

-- ----------------------------
-- Records of rsu_list
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table rsu_list
-- ----------------------------
CREATE UNIQUE INDEX "rsu_list_code_INDEX" ON "rsu"."rsu_list" USING btree ("device_id", "system_user_id");

-- ----------------------------
-- Primary Key structure for table rsu_list
-- ----------------------------
ALTER TABLE "rsu"."rsu_list" ADD PRIMARY KEY ("id");
