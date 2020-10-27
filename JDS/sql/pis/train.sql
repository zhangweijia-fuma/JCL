
-- ----------------------------
-- Table structure for train
-- ----------------------------
DROP TABLE IF EXISTS "pis"."train";
CREATE TABLE "pis"."train" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"name" varchar(50),
"name_en" varchar(100),
"code" varchar(50),
"description" text,
"line_id" int,
"sort" int,
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."train" IS '列车表';
COMMENT ON COLUMN "pis"."train"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."train"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."train"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."train"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."train"."name" IS '列车名称';
COMMENT ON COLUMN "pis"."train"."name_en" IS '列车英文名称';
COMMENT ON COLUMN "pis"."train"."code" IS '列车编号';
COMMENT ON COLUMN "pis"."train"."description" IS '列车说明';
COMMENT ON COLUMN "pis"."train"."line_id" IS '列车归属运营线路';
COMMENT ON COLUMN "pis"."train"."sort" IS '列车序号，从1开始，最大值999999';
COMMENT ON COLUMN "pis"."train"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of train
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table train
-- ----------------------------
CREATE UNIQUE INDEX "train_name_INDEX" ON "pis"."train" USING btree ("name", "system_user_id");

-- ----------------------------
-- Primary Key structure for table train
-- ----------------------------
ALTER TABLE "pis"."train" ADD PRIMARY KEY ("id");
