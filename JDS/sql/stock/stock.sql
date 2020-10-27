
-- ----------------------------
-- Table structure for stock
-- ----------------------------
DROP TABLE IF EXISTS "stock"."stock";
CREATE TABLE "stock"."stock" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"name" varchar(50),
"code" varchar(50),
"full_name" varchar(500),
"name_en" varchar(500),
"regist_addr" varchar(500),
"list_date" varchar(500),
"capitalization" int8,
"circulatingshares" int8
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "stock"."stock" IS '股票基本信息表';
COMMENT ON COLUMN "stock"."stock"."create_time" IS '创建时间';
COMMENT ON COLUMN "stock"."stock"."update_time" IS '修改时间';
COMMENT ON COLUMN "stock"."stock"."create_id" IS '创建人';
COMMENT ON COLUMN "stock"."stock"."update_id" IS '修改人';
COMMENT ON COLUMN "stock"."stock"."name" IS '股票名称';
COMMENT ON COLUMN "stock"."stock"."code" IS '股票代码';
COMMENT ON COLUMN "stock"."stock"."full_name" IS '公司全称';
COMMENT ON COLUMN "stock"."stock"."name_en" IS '英文名称';
COMMENT ON COLUMN "stock"."stock"."regist_addr" IS '注册地址';
COMMENT ON COLUMN "stock"."stock"."list_date" IS '上市日期';
COMMENT ON COLUMN "stock"."stock"."capitalization" IS '总股本';
COMMENT ON COLUMN "stock"."stock"."circulatingshares" IS '流通股本';

-- ----------------------------
-- Records of stock
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table stock
-- ----------------------------
CREATE UNIQUE INDEX "stock_code_INDEX" ON "stock"."stock" USING btree ("code");

-- ----------------------------
-- Primary Key structure for table stock
-- ----------------------------
ALTER TABLE "stock"."stock" ADD PRIMARY KEY ("id");
