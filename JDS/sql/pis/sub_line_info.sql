
-- ----------------------------
-- Table structure for sub_line_info
-- ----------------------------
DROP TABLE IF EXISTS "pis"."sub_line_info";
CREATE TABLE "pis"."sub_line_info" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"line_id" int4,
"sub_line_name" varchar(50),
"sub_line_name_en" varchar(50),
"up_start_station" int4,
"up_end_station" int4,
"down_start_station" int4,
"down_end_station" int4,
"up_start_time" varchar(50),
"up_end_time" varchar(50),
"down_start_time" varchar(50),
"down_end_time" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."sub_line_info" IS '运营线路表';
COMMENT ON COLUMN "pis"."sub_line_info"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."sub_line_info"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."sub_line_info"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."sub_line_info"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."sub_line_info"."line_id" IS '运营线路ID';
COMMENT ON COLUMN "pis"."sub_line_info"."sub_line_name" IS '支线中文名称';
COMMENT ON COLUMN "pis"."sub_line_info"."sub_line_name_en" IS '支线英文名称';
COMMENT ON COLUMN "pis"."sub_line_info"."up_start_station" IS '上行起点站ID';
COMMENT ON COLUMN "pis"."sub_line_info"."up_end_station" IS '上行终点站ID';
COMMENT ON COLUMN "pis"."sub_line_info"."down_start_station" IS '下行起点站ID';
COMMENT ON COLUMN "pis"."sub_line_info"."down_end_station" IS '下行终点站ID';
COMMENT ON COLUMN "pis"."sub_line_info"."up_start_time" IS '上行发车时间';
COMMENT ON COLUMN "pis"."sub_line_info"."up_end_time" IS '上行收车时间';
COMMENT ON COLUMN "pis"."sub_line_info"."down_start_time" IS '下行发车时间';
COMMENT ON COLUMN "pis"."sub_line_info"."down_end_time" IS '下行收车时间';
COMMENT ON COLUMN "pis"."sub_line_info"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of sub_line_info
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table sub_line_info
-- ----------------------------
CREATE INDEX "sub_line_info_code_INDEX" ON "pis"."sub_line_info" USING btree ("line_id", "system_user_id");

-- ----------------------------
-- Primary Key structure for table sub_line_info
-- ----------------------------
ALTER TABLE "pis"."sub_line_info" ADD PRIMARY KEY ("id");
