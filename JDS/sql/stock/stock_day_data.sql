
-- ----------------------------
-- Table structure for stock
-- ----------------------------
DROP TABLE IF EXISTS "stock"."stock_day_data";
CREATE TABLE "stock"."stock_day_data" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"name" varchar(50),
"code" varchar(50),
"trade_time" varchar(50),
"open" int,
"close" int,
"high" int,
"low" int,
"amount" int,
"vol" int,
"last_close" int,
"reserved" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "stock"."stock_day_data" IS '股票日线数据';
COMMENT ON COLUMN "stock"."stock_day_data"."create_time" IS '创建时间';
COMMENT ON COLUMN "stock"."stock_day_data"."update_time" IS '修改时间';
COMMENT ON COLUMN "stock"."stock_day_data"."create_id" IS '创建人';
COMMENT ON COLUMN "stock"."stock_day_data"."update_id" IS '修改人';
COMMENT ON COLUMN "stock"."stock_day_data"."name" IS '股票名称';
COMMENT ON COLUMN "stock"."stock_day_data"."code" IS '股票代码';
COMMENT ON COLUMN "stock"."stock_day_data"."trade_time" IS '交易日期';
COMMENT ON COLUMN "stock"."stock_day_data"."open" IS '开盘价，单位为分';
COMMENT ON COLUMN "stock"."stock_day_data"."close" IS '收盘价';
COMMENT ON COLUMN "stock"."stock_day_data"."high" IS '最高价';
COMMENT ON COLUMN "stock"."stock_day_data"."low" IS '最低价';
COMMENT ON COLUMN "stock"."stock_day_data"."amount" IS '成交额';
COMMENT ON COLUMN "stock"."stock_day_data"."vol" IS '成交量，单位：手';
COMMENT ON COLUMN "stock"."stock_day_data"."last_close" IS '昨日收盘价';

-- ----------------------------
-- Records of stock_day_data
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table stock
-- ----------------------------
CREATE INDEX "stock_day_code_INDEX" ON "stock"."stock_day_data" USING btree ("code");
CREATE INDEX "stock_day_time_INDEX" ON "stock"."stock_day_data" USING btree ("trade_time");

-- ----------------------------
-- Primary Key structure for table stock_day_data
-- ----------------------------
ALTER TABLE "stock"."stock_day_data" ADD PRIMARY KEY ("id");
