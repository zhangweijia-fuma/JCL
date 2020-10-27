
-- ----------------------------
-- Table structure for message_list_history
-- ----------------------------
DROP TABLE IF EXISTS "pis"."message_list_history";
CREATE TABLE "pis"."message_list_history" (
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

COMMENT ON TABLE "pis"."message_list_history" IS '消息发送历史记录表';
COMMENT ON COLUMN "pis"."message_list_history"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."message_list_history"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."message_list_history"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."message_list_history"."update_id" IS '修改人';
COMMENT ON COLUMN "pis"."message_list_history"."message_type" IS '消息类型';
COMMENT ON COLUMN "pis"."message_list_history"."operation_id1" IS '操作项ID1';
COMMENT ON COLUMN "pis"."message_list_history"."operation_id2" IS '操作项ID2';
COMMENT ON COLUMN "pis"."message_list_history"."content" IS '内容';
COMMENT ON COLUMN "pis"."message_list_history"."sender_name" IS '发生在姓名';
COMMENT ON COLUMN "pis"."message_list_history"."send_date" IS '发送日期';
COMMENT ON COLUMN "pis"."message_list_history"."send_time" IS '发送日期';
COMMENT ON COLUMN "pis"."message_list_history"."sender_id" IS '发送者ID';
COMMENT ON COLUMN "pis"."message_list_history"."send_status" IS '发送状态';
COMMENT ON COLUMN "pis"."message_list_history"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of message_list_history
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table message_list_history
-- ----------------------------
CREATE INDEX "message_list_history_id1_INDEX" ON "pis"."message_list_history" USING btree ("operation_id1", "system_user_id");
CREATE INDEX "message_list_history_id2_INDEX" ON "pis"."message_list_history" USING btree ("operation_id2", "system_user_id");

-- ----------------------------
-- Primary Key structure for table message_list_history
-- ----------------------------
ALTER TABLE "pis"."message_list_history" ADD PRIMARY KEY ("id");
