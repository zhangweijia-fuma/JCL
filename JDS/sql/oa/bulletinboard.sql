
-- ----------------------------
-- Table structure for bulletinboard
-- ----------------------------
DROP TABLE IF EXISTS "oa"."bulletinboard";
CREATE TABLE "oa"."bulletinboard" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"code" varchar(50),
"title" varchar(200),
"publisher_id" int4,
"publisher_dept" varchar(128),
"publish_date" varchar(50),
"content" text,
"file_path" varchar(128),
"apply_date" varchar(50),
"checker_id" int4,
"check_date" varchar(50),
"check_opinion" varchar(128),
"status" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "oa"."bulletinboard" IS '公告牌表';
COMMENT ON COLUMN "oa"."bulletinboard"."create_time" IS '创建时间';
COMMENT ON COLUMN "oa"."bulletinboard"."update_time" IS '修改时间';
COMMENT ON COLUMN "oa"."bulletinboard"."create_id" IS '创建人';
COMMENT ON COLUMN "oa"."bulletinboard"."update_id" IS '修改人';
COMMENT ON COLUMN "oa"."bulletinboard"."code" IS '公告编号';
COMMENT ON COLUMN "oa"."bulletinboard"."title" IS '公告标题';
COMMENT ON COLUMN "oa"."bulletinboard"."publisher_id" IS '发布人ID';
COMMENT ON COLUMN "oa"."bulletinboard"."publisher_dept" IS '发布人部门';
COMMENT ON COLUMN "oa"."bulletinboard"."publish_date" IS '发布日期';
COMMENT ON COLUMN "oa"."bulletinboard"."content" IS '内容';
COMMENT ON COLUMN "oa"."bulletinboard"."file_path" IS '文件存放路径';
COMMENT ON COLUMN "oa"."bulletinboard"."apply_date" IS '申请提交日期';
COMMENT ON COLUMN "oa"."bulletinboard"."checker_id" IS '审核人ID';
COMMENT ON COLUMN "oa"."bulletinboard"."check_date" IS '审核日期';
COMMENT ON COLUMN "oa"."bulletinboard"."check_opinion" IS '审核意见';
COMMENT ON COLUMN "oa"."bulletinboard"."status" IS '发布状态';
COMMENT ON COLUMN "oa"."bulletinboard"."system_user_id" IS '系统用户ID';


-- ----------------------------
-- Records of bulletinboard
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table bulletinboard
-- ----------------------------
CREATE UNIQUE INDEX "bulletinboard_code_INDEX" ON "oa"."bulletinboard" USING btree ("code", "system_user_id");
CREATE  INDEX "bulletinboard_date_INDEX" ON "oa"."bulletinboard" USING btree ("publish_date", "system_user_id");

-- ----------------------------
-- Primary Key structure for table bulletinboard
-- ----------------------------
ALTER TABLE "oa"."bulletinboard" ADD PRIMARY KEY ("id");
