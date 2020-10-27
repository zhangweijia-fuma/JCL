
-- ----------------------------
-- Table structure for stock
-- ----------------------------
DROP TABLE IF EXISTS "stock"."favority_list";
CREATE TABLE "stock"."favority_list" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"name" varchar(50),
"code" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "stock"."favority_list" IS '自选股表';
COMMENT ON COLUMN "stock"."favority_list"."create_time" IS '创建时间';
COMMENT ON COLUMN "stock"."favority_list"."update_time" IS '修改时间';
COMMENT ON COLUMN "stock"."favority_list"."create_id" IS '创建人';
COMMENT ON COLUMN "stock"."favority_list"."update_id" IS '修改人';
COMMENT ON COLUMN "stock"."favority_list"."name" IS '股票名称';
COMMENT ON COLUMN "stock"."favority_list"."code" IS '股票代码';
COMMENT ON COLUMN "stock"."favority_list"."system_user_id" IS '归属的系统用户ID';

-- ----------------------------
-- Records of favority_list
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table favority_list
-- ----------------------------
CREATE UNIQUE INDEX "stock_favority_code_INDEX" ON "stock"."favority_list" USING btree ("code", "system_user_id");

-- ----------------------------
-- Primary Key structure for table favority_list
-- ----------------------------
ALTER TABLE "stock"."favority_list" ADD PRIMARY KEY ("id");
