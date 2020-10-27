

-- ----------------------------
-- Table structure for menu_item
-- ----------------------------
DROP TABLE IF EXISTS "system"."menu_item";
CREATE TABLE "system"."menu_item" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"menu_code" varchar(50) COLLATE "default",
"menu_id" int4,
"path" varchar(100) COLLATE "default"
)
WITH (OIDS=FALSE)

;
COMMENT ON TABLE "system"."menu_item" IS '菜单项表';
COMMENT ON COLUMN "system"."menu_item"."id" IS 'ID';
COMMENT ON COLUMN "system"."menu_item"."create_time" IS '创建时间';
COMMENT ON COLUMN "system"."menu_item"."update_time" IS '修改时间';
COMMENT ON COLUMN "system"."menu_item"."create_id" IS '创建人';
COMMENT ON COLUMN "system"."menu_item"."update_id" IS '修改人';
COMMENT ON COLUMN "system"."menu_item"."menu_code" IS '菜单CODE';
COMMENT ON COLUMN "system"."menu_item"."menu_id" IS '菜单ID';
COMMENT ON COLUMN "system"."menu_item"."path" IS '路径';

-- ----------------------------
-- Records of menu_item
-- ----------------------------
INSERT INTO "system"."menu_item" (menu_code, path) VALUES ('accountManage',     '/accountManage');
INSERT INTO "system"."menu_item" (menu_code, path) VALUES ('groupManage',       '/groupManage');
INSERT INTO "system"."menu_item" (menu_code, path) VALUES ('operationLog',      '/operationLog');
INSERT INTO "system"."menu_item" (menu_code, path) VALUES ('dictManage',        '/dictManage');
INSERT INTO "system"."menu_item" (menu_code, path) VALUES ('menuManage',        '/menuManage');
INSERT INTO "system"."menu_item" (menu_code, path) VALUES ('systemUserManage',  '/systemUserManage');
INSERT INTO "system"."menu_item" (menu_code, path) VALUES ('areaCodeManage',    '/areaCodeManage');
INSERT INTO "system"."menu_item" (menu_code, path) VALUES ('firstPage',         '/toDo');
update system.menu_item set menu_id = (select m.id from system.menu m where m.code = menu_code); 

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table menu_item
-- ----------------------------
CREATE UNIQUE INDEX "menu_id_index" ON "system"."menu_item" USING btree ("menu_id");

-- ----------------------------
-- Primary Key structure for table menu_item
-- ----------------------------
ALTER TABLE "system"."menu_item" ADD PRIMARY KEY ("id");
