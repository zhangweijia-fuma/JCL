
-- ----------------------------
-- Table structure for system_user_list
-- ----------------------------
DROP TABLE IF EXISTS "system"."system_user_list";
CREATE TABLE "system"."system_user_list" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"user_name" varchar(50),
"user_name_en" varchar(50),
"abbr_name" varchar(50),
"abbr_name_en" varchar(50),
"app_name" varchar(50),
"app_name_en" varchar(50),
"admin_account_id" int,
"admin_group_id" int,
"logo_vertical" varchar(50),
"logo_horizontal" varchar(50),
"user_desc" text,
"user_service" varchar(50),
"status" varchar(50)
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "system"."system_user_list" IS '系统用户表';
COMMENT ON COLUMN "system"."system_user_list"."create_time" IS '创建时间';
COMMENT ON COLUMN "system"."system_user_list"."update_time" IS '修改时间';
COMMENT ON COLUMN "system"."system_user_list"."create_id" IS '创建人';
COMMENT ON COLUMN "system"."system_user_list"."update_id" IS '修改人';
COMMENT ON COLUMN "system"."system_user_list"."user_name" IS '系统用户中文名';
COMMENT ON COLUMN "system"."system_user_list"."user_name_en" IS '系统用户英文名';
COMMENT ON COLUMN "system"."system_user_list"."abbr_name" IS '系统用户缩写名称';
COMMENT ON COLUMN "system"."system_user_list"."abbr_name_en" IS '系统用户缩写英文名称';
COMMENT ON COLUMN "system"."system_user_list"."app_name" IS '系统应用名称';
COMMENT ON COLUMN "system"."system_user_list"."app_name_en" IS '系统应用英文名称';
COMMENT ON COLUMN "system"."system_user_list"."admin_account_id" IS '系统用户管理人员账户ID';
COMMENT ON COLUMN "system"."system_user_list"."admin_group_id" IS '系统用户用户组ID';
COMMENT ON COLUMN "system"."system_user_list"."logo_vertical" IS '竖向Logo文件名';
COMMENT ON COLUMN "system"."system_user_list"."logo_horizontal" IS '横向Logo文件名';
COMMENT ON COLUMN "system"."system_user_list"."user_desc" IS '用户描述';
COMMENT ON COLUMN "system"."system_user_list"."user_service" IS '业务名称，用来和前端业务类型匹配';
COMMENT ON COLUMN "system"."system_user_list"."status" IS '用户账户状态';
-- ----------------------------
-- Records of system_user_list
-- ----------------------------

INSERT INTO "system"."system_user_list" (user_name, user_name_en, logo_vertical, logo_horizontal, user_desc, admin_account_id, admin_group_id, status) VALUES ('系统默认用户', 'SYSTEM', './resource/images/defaultVerticalLogo.png', './resource/images/defaultHorizontalLogo.png', '系统默认用户', 1, 1, 2);

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table system_user_list
-- ----------------------------
CREATE UNIQUE INDEX "system_user_list_admin_id_index" ON "system"."system_user_list" USING btree ("admin_account_id");
COMMENT ON INDEX "system"."system_user_list_admin_id_index" IS '管理员账户索引，账号ID唯一';
CREATE UNIQUE INDEX "system_user_list_group_id_index" ON "system"."system_user_list" USING btree ("admin_group_id");
CREATE UNIQUE INDEX "system_user_list_name_en_index" ON "system"."system_user_list" USING btree ("user_name_en");
CREATE UNIQUE INDEX "system_user_list_name_index" ON "system"."system_user_list" USING btree ("user_name");

-- ----------------------------
-- Primary Key structure for table system_user_list
-- ----------------------------
ALTER TABLE "system"."system_user_list" ADD PRIMARY KEY ("id");
