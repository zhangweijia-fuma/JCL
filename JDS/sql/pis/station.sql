
-- ----------------------------
-- Table structure for station
-- ----------------------------
DROP TABLE IF EXISTS "pis"."station";
CREATE TABLE "pis"."station" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"name" varchar(50),
"name_en" varchar(100),
"code" varchar(50),
"type" varchar(50),
"ip_address" varchar(50),
"description" text,
"status" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."station" IS '车站表';
COMMENT ON COLUMN "pis"."station"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."station"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."station"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."station"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."station"."name" IS '车站中文名称';
COMMENT ON COLUMN "pis"."station"."name_en" IS '车站英文名称';
COMMENT ON COLUMN "pis"."station"."code" IS '车站编号';
COMMENT ON COLUMN "pis"."station"."type" IS '车站类型';
COMMENT ON COLUMN "pis"."station"."description" IS '车站说明';
COMMENT ON COLUMN "pis"."station"."ip_address" IS '车站服务器地址';
COMMENT ON COLUMN "pis"."station"."status" IS '车站服务器状态，0：在线，5：离线';
COMMENT ON COLUMN "pis"."station"."system_user_id" IS '系统用户ID';


-- ----------------------------
-- Records of station
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table station
-- ----------------------------
CREATE UNIQUE INDEX "station_name_cn_INDEX" ON "pis"."station" USING btree ("name", "system_user_id");
CREATE UNIQUE INDEX "station_name_en_INDEX" ON "pis"."station" USING btree ("name_en", "system_user_id");
CREATE UNIQUE INDEX "station_code_INDEX" ON "pis"."station" USING btree ("code", "system_user_id");

-- ----------------------------
-- Primary Key structure for table station
-- ----------------------------
ALTER TABLE "pis"."station" ADD PRIMARY KEY ("id");
