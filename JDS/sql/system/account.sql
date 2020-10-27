
-- ----------------------------
-- Table structure for account
-- ----------------------------
DROP TABLE IF EXISTS "system"."account";
CREATE TABLE "system"."account" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"user_name" varchar(50),
"pass" varchar(50),
"user_type" varchar(50),
"status" varchar(50),
"name" varchar(50),
"sex" varchar(50),
"birthday" date,
"country" varchar(50),
"company" varchar(50),
"dept_name" varchar(50),
"description" text,
"mobile_phone1" varchar(50),
"mobile_phone2" varchar(50),
"mailbox" varchar(50),
"id_no" varchar(50),
"address1" varchar(50),
"address2" varchar(50),
"photo_path" varchar(100),
"default_language" varchar(50),
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "system"."account" IS '用户账户表';
COMMENT ON COLUMN "system"."account"."create_time" IS '创建时间';
COMMENT ON COLUMN "system"."account"."update_time" IS '修改时间';
COMMENT ON COLUMN "system"."account"."create_id" IS '创建人';
COMMENT ON COLUMN "system"."account"."update_id" IS '修改人';
COMMENT ON COLUMN "system"."account"."user_name" IS '用户名';
COMMENT ON COLUMN "system"."account"."pass" IS '密码';
COMMENT ON COLUMN "system"."account"."user_type" IS '用户类型，本单位用户，第三方用户';
COMMENT ON COLUMN "system"."account"."status" IS '账户状态';
COMMENT ON COLUMN "system"."account"."name" IS '姓名';
COMMENT ON COLUMN "system"."account"."sex" IS '性别';
COMMENT ON COLUMN "system"."account"."birthday" IS '出生日期';
COMMENT ON COLUMN "system"."account"."country" IS '国籍';
COMMENT ON COLUMN "system"."account"."company" IS '用户单位名称';
COMMENT ON COLUMN "system"."account"."dept_name" IS '用户部门名称';
COMMENT ON COLUMN "system"."account"."description" IS '用户个人描述';
COMMENT ON COLUMN "system"."account"."mobile_phone1" IS '用户移动电话1';
COMMENT ON COLUMN "system"."account"."mobile_phone2" IS '用户移动电话2';
COMMENT ON COLUMN "system"."account"."mailbox" IS '电子邮件';
COMMENT ON COLUMN "system"."account"."id_no" IS '用户身份证号码';
COMMENT ON COLUMN "system"."account"."address1" IS '用户地址1';
COMMENT ON COLUMN "system"."account"."address2" IS '用户地址2';
COMMENT ON COLUMN "system"."account"."photo_path" IS '用户头像地址';
COMMENT ON COLUMN "system"."account"."default_language" IS '用户使用的缺省语言类型';
COMMENT ON COLUMN "system"."account"."system_user_id" IS '用户归属的用户类型ID';


-- ----------------------------
-- Records of account
-- ----------------------------
-- 默认密码：james2019
INSERT INTO "system"."account" (user_name, pass, user_type, status, name, sex, photo_path ) VALUES ('supervisor', 'ffb7af62aa14bec385f7e66ab16e5090', '0', '2', '超级管理员', '1', './resource/images/default.png');

update system.account set system_user_id = (select su.id from system.system_user_list su where su.user_name_en = 'SYSTEM');

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table account
-- ----------------------------
CREATE UNIQUE INDEX "ACCOUNT_USERNAME_INDEX" ON "system"."account" USING btree ("user_name", "system_user_id");

-- ----------------------------
-- Primary Key structure for table account
-- ----------------------------
ALTER TABLE "system"."account" ADD PRIMARY KEY ("id");
