

-- ----------------------------
-- Table structure for user_group
-- ----------------------------
DROP TABLE IF EXISTS "system"."user_group";
CREATE TABLE "system"."user_group" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"name" varchar(50) ,
"c_name" varchar(50),
"group_type" varchar(50),
"description" varchar(100),
"system_user_id" int,
"rec_status" varchar(50)
)
WITH (OIDS=FALSE)

;
COMMENT ON TABLE "system"."user_group" IS '用户组表';
COMMENT ON COLUMN "system"."user_group"."create_time" IS '创建时间';
COMMENT ON COLUMN "system"."user_group"."update_time" IS '修改时间';
COMMENT ON COLUMN "system"."user_group"."create_id" IS '创建人';
COMMENT ON COLUMN "system"."user_group"."update_id" IS '修改人';
COMMENT ON COLUMN "system"."user_group"."name" IS '用户组英文名称，必须唯一';
COMMENT ON COLUMN "system"."user_group"."c_name" IS '用户组中文名称';
COMMENT ON COLUMN "system"."user_group"."group_type" IS '用户组类型，0：系统用户组，不显示，1：不可删除，2：可删除';
COMMENT ON COLUMN "system"."user_group"."description" IS '用户组说明';
COMMENT ON COLUMN "system"."user_group"."system_user_id" IS '用户组归属的系统用户类型ID';
COMMENT ON COLUMN "system"."user_group"."system_user_id" IS '用户归属的用户类型ID';
COMMENT ON COLUMN "system"."user_group"."rec_status" IS '记录状态';

-- ----------------------------
-- Records of user_group
-- ----------------------------
-- INSERT INTO "system"."user_group" (name, c_name, group_type, description) VALUES ('administrators', '系统管理员组', '1',  '系统管理员组');
INSERT INTO "system"."user_group" (name, c_name, group_type, description, system_user_id, rec_status) VALUES ('supervisors', '超级管理员组', '0',  '超级管理员组', 1, '2');


-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table user_group
-- ----------------------------
ALTER TABLE "system"."user_group" ADD PRIMARY KEY ("id");
