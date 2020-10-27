


-- ----------------------------
-- Table structure for user_group_menu
-- ----------------------------
DROP TABLE IF EXISTS "system"."user_group_menu";
CREATE TABLE "system"."user_group_menu" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"group_id" int4,
"menu_item_id" int4,
"system_user_id" int
)
WITH (OIDS=FALSE)

;
COMMENT ON TABLE "system"."user_group_menu" IS '用户组菜单表';
COMMENT ON COLUMN "system"."user_group_menu"."id" IS 'Id号';
COMMENT ON COLUMN "system"."user_group_menu"."create_time" IS '创建时间';
COMMENT ON COLUMN "system"."user_group_menu"."update_time" IS '修改时间';
COMMENT ON COLUMN "system"."user_group_menu"."create_id" IS '创建人';
COMMENT ON COLUMN "system"."user_group_menu"."update_id" IS '修改人';
COMMENT ON COLUMN "system"."user_group_menu"."group_id" IS '用户组ID';
COMMENT ON COLUMN "system"."user_group_menu"."menu_item_id" IS '菜单项ID';
COMMENT ON COLUMN "system"."user_group_menu"."system_user_id" IS '用户归属的用户类型ID';

-- ----------------------------
-- Records of user_group_menu
-- ----------------------------
-- 系统超级管理员菜单权限
INSERT INTO "system"."user_group_menu" (group_id, menu_item_id, system_user_id) VALUES (-1, -1, 1);
INSERT INTO "system"."user_group_menu" (group_id, menu_item_id, system_user_id) VALUES (-1, -2, 1);
INSERT INTO "system"."user_group_menu" (group_id, menu_item_id, system_user_id) VALUES (-1, -3, 1);
INSERT INTO "system"."user_group_menu" (group_id, menu_item_id, system_user_id) VALUES (-1, -4, 1);

update system.user_group_menu set group_id =(select ug.id from system.user_group ug where ug.name = 'supervisors'), menu_item_id = (select m.id from system.menu_item m where m.menu_code = 'dictManage') where menu_item_id = -1;
update system.user_group_menu set group_id =(select ug.id from system.user_group ug where ug.name = 'supervisors'), menu_item_id = (select m.id from system.menu_item m where m.menu_code = 'menuManage') where menu_item_id = -2;
update system.user_group_menu set group_id =(select ug.id from system.user_group ug where ug.name = 'supervisors'), menu_item_id = (select m.id from system.menu_item m where m.menu_code = 'systemUserManage') where menu_item_id = -3;
update system.user_group_menu set group_id =(select ug.id from system.user_group ug where ug.name = 'supervisors'), menu_item_id = (select m.id from system.menu_item m where m.menu_code = 'areaCodeManage') where menu_item_id = -4;


-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table user_group_member
-- ----------------------------
CREATE UNIQUE INDEX "ug_menu_index" ON "system"."user_group_menu" USING btree ("group_id", "menu_item_id");

-- ----------------------------
-- Primary Key structure for table user_group_menu
-- ----------------------------
ALTER TABLE "system"."user_group_menu" ADD PRIMARY KEY ("id");
