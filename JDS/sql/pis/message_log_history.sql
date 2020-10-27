
-- ----------------------------
-- Table structure for message_log_history
-- ----------------------------
DROP TABLE IF EXISTS "pis"."message_log_history";
CREATE TABLE "pis"."message_log_history" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"msg_list_id" int,
"operation_id1" int4,
"operation_id2" int4,

"sender_id" int,
"sender_type" varchar(50),
"sender_name" varchar(50),
"sender_code" varchar(50),
"sender_addr" varchar(50),

"receiver_id" int,
"receiver_type" varchar(50),
"receiver_name" varchar(50),
"receiver_code" varchar(50),
"receiver_addr" varchar(50),

"message_type" varchar(50),
"message_sn" int,
"send_date"  varchar(50),
"send_time" varchar(50),
"receive_date" varchar(50),
"receive_time" varchar(50),

"content_type"  varchar(50),
"bin_content" text,
"text_content" text,
"send_status" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "pis"."message_log_history" IS '消息发送历史记录详表';
COMMENT ON COLUMN "pis"."message_log_history"."create_time" IS '创建时间';
COMMENT ON COLUMN "pis"."message_log_history"."update_time" IS '修改时间';
COMMENT ON COLUMN "pis"."message_log_history"."create_id" IS '创建人';
COMMENT ON COLUMN "pis"."message_log_history"."update_id" IS '修改人';

COMMENT ON COLUMN "pis"."message_log_history"."msg_list_id" IS '发送消息的记录ID';
COMMENT ON COLUMN "pis"."message_list"."operation_id1" IS '操作项ID1';
COMMENT ON COLUMN "pis"."message_list"."operation_id2" IS '操作项ID2';

COMMENT ON COLUMN "pis"."message_log_history"."sender_id" IS '发送方ID';
COMMENT ON COLUMN "pis"."message_log_history"."sender_type" IS '发送方类别';
COMMENT ON COLUMN "pis"."message_log_history"."sender_name" IS '发送方名称';
COMMENT ON COLUMN "pis"."message_log_history"."sender_code" IS '发送方编码';
COMMENT ON COLUMN "pis"."message_log_history"."sender_addr" IS '发送方地址';

COMMENT ON COLUMN "pis"."message_log_history"."receiver_id" IS '接收方ID';
COMMENT ON COLUMN "pis"."message_log_history"."receiver_type" IS '接收方类别';
COMMENT ON COLUMN "pis"."message_log_history"."receiver_name" IS '接收方名称';
COMMENT ON COLUMN "pis"."message_log_history"."receiver_code" IS '接收方编码';
COMMENT ON COLUMN "pis"."message_log_history"."receiver_addr" IS '接收方地址';

COMMENT ON COLUMN "pis"."message_log_history"."message_type" IS '消息类别';
COMMENT ON COLUMN "pis"."message_log_history"."message_sn" IS '消息序号，初始值为1，重发消息后以前的记录该值为0';
COMMENT ON COLUMN "pis"."message_log_history"."send_date" IS '发送日期';
COMMENT ON COLUMN "pis"."message_log_history"."send_time" IS '发送时间';
COMMENT ON COLUMN "pis"."message_log_history"."receive_date" IS '接收日期';
COMMENT ON COLUMN "pis"."message_log_history"."receive_time" IS '接收时间';

COMMENT ON COLUMN "pis"."message_log_history"."content_type" IS '内容类别';
COMMENT ON COLUMN "pis"."message_log_history"."bin_content" IS '原始内容';
COMMENT ON COLUMN "pis"."message_log_history"."text_content" IS '编码后的类别';
COMMENT ON COLUMN "pis"."message_log_history"."send_status" IS '发送状态';
COMMENT ON COLUMN "pis"."message_log_history"."system_user_id" IS '系统用户ID';

-- ----------------------------
-- Records of message_log_history
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table program_list
-- ----------------------------
CREATE INDEX "message_log_history_type_INDEX" ON "pis"."message_log_history" USING btree ("message_type", "system_user_id");
CREATE INDEX "message_log_history_sn_INDEX" ON "pis"."message_log_history" USING btree ("message_sn", "system_user_id");
CREATE INDEX "message_log_history_sid_INDEX" ON "pis"."message_log_history" USING btree ("sender_id", "system_user_id");
CREATE INDEX "message_log_history_rid_INDEX" ON "pis"."message_log_history" USING btree ("receiver_id", "system_user_id");
CREATE INDEX "message_log_history_msg_list_id" ON "pis"."message_log_history" USING btree ("msg_list_id", "system_user_id");

-- ----------------------------
-- Primary Key structure for table message_log_history
-- ----------------------------
ALTER TABLE "pis"."message_log_history" ADD PRIMARY KEY ("id");
