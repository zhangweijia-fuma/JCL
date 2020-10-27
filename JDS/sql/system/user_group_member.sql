
-- ----------------------------
-- Table structure for user_group_member
-- ----------------------------
DROP TABLE IF EXISTS "system"."user_group_member";
CREATE TABLE "system"."user_group_member" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"group_id" int4,
"user_id" int4,
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "system"."user_group_member" IS '用户组成员表';
COMMENT ON COLUMN "system"."user_group_member"."id" IS 'Id号';
COMMENT ON COLUMN "system"."user_group_member"."create_time" IS '创建时间';
COMMENT ON COLUMN "system"."user_group_member"."update_time" IS '修改时间';
COMMENT ON COLUMN "system"."user_group_member"."create_id" IS '创建人';
COMMENT ON COLUMN "system"."user_group_member"."update_id" IS '修改人';
COMMENT ON COLUMN "system"."user_group_member"."group_id" IS '用户组ID';
COMMENT ON COLUMN "system"."user_group_member"."user_id" IS '用户组成员ID';
COMMENT ON COLUMN "system"."user_group_member"."system_user_id" IS '用户归属的用户类型ID';

-- ----------------------------
-- Records of user_group_member
-- ----------------------------
INSERT INTO "system"."user_group_member" (group_id, user_id, system_user_id) VALUES (-1, -1, 1);
update system.user_group_member set group_id = (select ug.id from system.user_group ug where ug.name = 'supervisors'), user_id = (select ua.id from system.account ua where ua.user_name = 'supervisor') where group_id = -1;

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------


-- ----------------------------
-- Indexes structure for table user_group_member
-- ----------------------------
CREATE UNIQUE INDEX "ug_member_index" ON "system"."user_group_member" USING btree ("group_id", "user_id");

-- ----------------------------
-- Primary Key structure for table user_group_member
-- ----------------------------
ALTER TABLE "system"."user_group_member" ADD PRIMARY KEY ("id");
