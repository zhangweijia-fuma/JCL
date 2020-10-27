
-- ----------------------------
-- Table structure for oa_system_parameter
-- ----------------------------
DROP TABLE IF EXISTS "oa"."oa_system_parameter";
CREATE TABLE "oa"."oa_system_parameter" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"item" varchar(50),
"str_value" varchar(200),
"int_value" int4,
"year" int,
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "oa"."oa_system_parameter" IS '公告牌表';
COMMENT ON COLUMN "oa"."oa_system_parameter"."create_time" IS '创建时间';
COMMENT ON COLUMN "oa"."oa_system_parameter"."update_time" IS '修改时间';
COMMENT ON COLUMN "oa"."oa_system_parameter"."create_id" IS '创建人';
COMMENT ON COLUMN "oa"."oa_system_parameter"."update_id" IS '修改人';
COMMENT ON COLUMN "oa"."oa_system_parameter"."item" IS '参数名称';
COMMENT ON COLUMN "oa"."oa_system_parameter"."str_value" IS '参数字符值';
COMMENT ON COLUMN "oa"."oa_system_parameter"."int_value" IS '参数数字值';
COMMENT ON COLUMN "oa"."oa_system_parameter"."year" IS '参数有效年度';
COMMENT ON COLUMN "oa"."oa_system_parameter"."system_user_id" IS '系统用户ID';


-- ----------------------------
-- Records of oa_system_parameter
-- ----------------------------
-- INSERT INTO "oa"."oa_system_parameter" (item, str_value, int_value, year ) VALUES ('bulletinCode', '', '0', 2019);

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table oa_system_parameter
-- ----------------------------
CREATE INDEX "oa_system_parameter_item_INDEX" ON "oa"."oa_system_parameter" USING btree ("item", "system_user_id");
CREATE INDEX "oa_system_parameter_year_INDEX" ON "oa"."oa_system_parameter" USING btree ("year", "system_user_id");
CREATE INDEX "oa_system_parameter_systemuserid_INDEX" ON "oa"."oa_system_parameter" USING btree ("system_user_id", "system_user_id");

-- ----------------------------
-- Primary Key structure for table oa_system_parameter
-- ----------------------------
ALTER TABLE "oa"."oa_system_parameter" ADD PRIMARY KEY ("id");
