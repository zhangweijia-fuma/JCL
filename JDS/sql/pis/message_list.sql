
-- ----------------------------
-- Table structure for message_list
-- ----------------------------
DROP TABLE IF EXISTS "pis"."message_list";
CREATE TABLE "pis"."message_list" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"message_type" varchar(50),
"operation_id1" int4,
"operation_id2" int4,
"content" text,
"sender_id" int,
"sender_name" varchar(50),
"send_date" varchar(50),
"send_time" varchar(50),
"send_status" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."message_list" IS '消息发送记录表';
COMMENT ON COLUMN "pis"."message_list"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."message_list"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."message_list"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."message_list"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."message_list"."message_type" IS '消息类型';
COMMENT ON COLUMN "pis"."message_list"."operation_id1" IS '操作项ID1';
COMMENT ON COLUMN "pis"."message_list"."operation_id2" IS '操作项ID2';
COMMENT ON COLUMN "pis"."message_list"."content" IS '内容';
COMMENT ON COLUMN "pis"."message_list"."sender_name" IS '发生在姓名';
COMMENT ON COLUMN "pis"."message_list"."send_date" IS '发送日期';
COMMENT ON COLUMN "pis"."message_list"."send_time" IS '发送日期';
COMMENT ON COLUMN "pis"."message_list"."sender_id" IS '发送者ID';
COMMENT ON COLUMN "pis"."message_list"."send_status" IS '发送状态';
COMMENT ON COLUMN "pis"."message_list"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of message_list
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table message_list
-- ----------------------------
CREATE INDEX "message_list_id1_INDEX" ON "pis"."message_list" USING btree ("operation_id1", "system_user_id");
CREATE INDEX "message_list_id2_INDEX" ON "pis"."message_list" USING btree ("operation_id2", "system_user_id");

-- ----------------------------
-- Primary Key structure for table message_list
-- ----------------------------
ALTER TABLE "pis"."message_list" ADD PRIMARY KEY ("id");
