
-- ----------------------------
-- Table structure for stock
-- ----------------------------
DROP TABLE IF EXISTS "stock"."stock_minute_5_data";
CREATE TABLE "stock"."stock_minute_5_data" (
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

COMMENT ON TABLE "stock"."stock_minute_5_data" IS '股票5分钟线数据';
COMMENT ON COLUMN "stock"."stock_minute_5_data"."create_time" IS '创建时间';
COMMENT ON COLUMN "stock"."stock_minute_5_data"."update_time" IS '修改时间';
COMMENT ON COLUMN "stock"."stock_minute_5_data"."create_id" IS '创建人';
COMMENT ON COLUMN "stock"."stock_minute_5_data"."update_id" IS '修改人';
COMMENT ON COLUMN "stock"."stock_minute_5_data"."name" IS '股票名称';
COMMENT ON COLUMN "stock"."stock_minute_5_data"."code" IS '股票代码';
COMMENT ON COLUMN "stock"."stock_minute_5_data"."trade_time" IS '交易时间，HH:MM:SS';
COMMENT ON COLUMN "stock"."stock_minute_5_data"."open" IS '开盘价，单位为分';
COMMENT ON COLUMN "stock"."stock_minute_5_data"."close" IS '收盘价';
COMMENT ON COLUMN "stock"."stock_minute_5_data"."high" IS '最高价';
COMMENT ON COLUMN "stock"."stock_minute_5_data"."low" IS '最低价';
COMMENT ON COLUMN "stock"."stock_minute_5_data"."amount" IS '成交额';
COMMENT ON COLUMN "stock"."stock_minute_5_data"."vol" IS '成交量，单位：股';
COMMENT ON COLUMN "stock"."stock_minute_5_data"."last_close" IS '前一收盘价';
COMMENT ON COLUMN "stock"."stock_minute_5_data"."reserved" IS '保留';

-- ----------------------------
-- Records of stock_minute_5_data
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table stock
-- ----------------------------
CREATE INDEX "stock_minute_5_code_INDEX" ON "stock"."stock_minute_5_data" USING btree ("code");
CREATE INDEX "stock_minute_5_time_INDEX" ON "stock"."stock_minute_5_data" USING btree ("trade_time");

-- ----------------------------
-- Primary Key structure for table stock_minute_5_data
-- ----------------------------
ALTER TABLE "stock"."stock_minute_5_data" ADD PRIMARY KEY ("id");
