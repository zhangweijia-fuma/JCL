
-- ----------------------------
-- Table structure for service_menu
-- ----------------------------
DROP TABLE IF EXISTS "system"."system_user_menu";
CREATE TABLE "system"."system_user_menu" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"menu_id" int4,
"system_user_id" int
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "system"."system_user_menu" IS '业务类型菜单表';
COMMENT ON COLUMN "system"."system_user_menu"."create_time" IS '创建时间';
COMMENT ON COLUMN "system"."system_user_menu"."update_time" IS '修改时间';
COMMENT ON COLUMN "system"."system_user_menu"."create_id" IS '创建人';
COMMENT ON COLUMN "system"."system_user_menu"."update_id" IS '修改人';
COMMENT ON COLUMN "system"."system_user_menu"."menu_id" IS '菜单ID';
COMMENT ON COLUMN "system"."system_user_menu"."system_user_id" IS '用户归属的用户类型ID';

-- ----------------------------
-- Records of system_user_menu
-- ----------------------------


-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table system_user_menu
-- ----------------------------
CREATE UNIQUE INDEX "system_user_menu_index" ON "system"."system_user_menu" USING btree ("system_user_id", "menu_id");

-- ----------------------------
-- Primary Key structure for table system_user_menu
-- ----------------------------
ALTER TABLE "system"."system_user_menu" ADD PRIMARY KEY ("id");
