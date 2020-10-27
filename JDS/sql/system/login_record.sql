

-- ----------------------------
-- Table structure for login_record
-- ----------------------------
DROP TABLE IF EXISTS "system"."login_record";
CREATE TABLE "system"."login_record" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"user_id" int4,
"user_name" varchar(50),
"name"  varchar(50),
"ip_address" varchar(50),
"login_time" varchar(50),
"logout_time" varchar(50),
"result" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)

;
COMMENT ON TABLE "system"."login_record" IS '操作日志表';
COMMENT ON COLUMN "system"."login_record"."id" IS 'Id号';
COMMENT ON COLUMN "system"."login_record"."create_time" IS '创建时间';
COMMENT ON COLUMN "system"."login_record"."update_time" IS '修改时间';
COMMENT ON COLUMN "system"."login_record"."create_id" IS '创建人';
COMMENT ON COLUMN "system"."login_record"."update_id" IS '修改人';
COMMENT ON COLUMN "system"."login_record"."user_name" IS '登录使用的用户名';
COMMENT ON COLUMN "system"."login_record"."user_id" IS '登录使用的用户ID';
COMMENT ON COLUMN "system"."login_record"."name" IS '登录使用的用户名称';
COMMENT ON COLUMN "system"."login_record"."ip_address" IS '登录使用的IP地址';
COMMENT ON COLUMN "system"."login_record"."login_time" IS '登录时间';
COMMENT ON COLUMN "system"."login_record"."logout_time" IS '登出时间';
COMMENT ON COLUMN "system"."login_record"."result" IS '登录结果，成功或失败原因';
COMMENT ON COLUMN "system"."account"."system_user_id" IS '用户归属的用户类型ID';

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table code_value
-- ----------------------------
CREATE INDEX "login_time_index" ON "system"."login_record" USING btree ("login_time", "system_user_id");
CREATE INDEX "logout_time_index" ON "system"."login_record" USING btree ("logout_time", "system_user_id");
CREATE INDEX "login_userid_index" ON "system"."login_record" USING btree ("user_id", "system_user_id");

-- ----------------------------
-- Primary Key structure for table login_record
-- ----------------------------
ALTER TABLE "system"."login_record" ADD PRIMARY KEY ("id");
