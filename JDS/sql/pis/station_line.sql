
-- ----------------------------
-- Table structure for station_line
-- ----------------------------
DROP TABLE IF EXISTS "pis"."station_line";
CREATE TABLE "pis"."station_line" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"station_id" int,
"line_id" int,
"sort" int,
"up_start_time" varchar(50),
"up_end_time" varchar(50),
"down_start_time" varchar(50),
"down_end_time" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."station_line" IS '线路车站表';
COMMENT ON COLUMN "pis"."station_line"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."station_line"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."station_line"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."station_line"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."station_line"."station_id" IS '车站ID';
COMMENT ON COLUMN "pis"."station_line"."line_id" IS '线路ID';
COMMENT ON COLUMN "pis"."station_line"."sort" IS '车站序号';
COMMENT ON COLUMN "pis"."station_line"."up_start_time" IS '上行发车时间';
COMMENT ON COLUMN "pis"."station_line"."up_end_time" IS '上行收车时间';
COMMENT ON COLUMN "pis"."station_line"."down_start_time" IS '下行发车时间';
COMMENT ON COLUMN "pis"."station_line"."down_end_time" IS '下行收车时间';
COMMENT ON COLUMN "pis"."station_line"."system_user_id" IS '系统用户ID';


-- ----------------------------
-- Records of station_line
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table station_line
-- ----------------------------
CREATE UNIQUE INDEX "station_line_INDEX" ON "pis"."station_line" USING btree ("station_id", "line_id", "system_user_id");

-- ----------------------------
-- Primary Key structure for table station_line
-- ----------------------------
ALTER TABLE "pis"."station_line" ADD PRIMARY KEY ("id");
