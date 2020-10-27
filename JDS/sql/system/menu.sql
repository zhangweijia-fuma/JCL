
-- ----------------------------
-- Table structure for menu
-- ----------------------------
DROP TABLE IF EXISTS "system"."menu";
CREATE TABLE "system"."menu" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int,
"update_id" int,
"code" varchar(50),
"name" varchar(50),
"sort" int,
"tier" int,
"parent_id" int,
"parent_code" varchar(50)
)
WITH (OIDS=FALSE)

;
COMMENT ON TABLE "system"."menu" IS '菜单表';
COMMENT ON COLUMN "system"."menu"."id" IS 'Id号';
COMMENT ON COLUMN "system"."menu"."create_time" IS '创建时间';
COMMENT ON COLUMN "system"."menu"."update_time" IS '修改时间';
COMMENT ON COLUMN "system"."menu"."create_id" IS '创建人';
COMMENT ON COLUMN "system"."menu"."update_id" IS '修改人';
COMMENT ON COLUMN "system"."menu"."code" IS '菜单CODE';
COMMENT ON COLUMN "system"."menu"."name" IS '菜单名字';
COMMENT ON COLUMN "system"."menu"."sort" IS '排序，从0开始';
COMMENT ON COLUMN "system"."menu"."tier" IS '层级，顶层菜单是0，依次类推';
COMMENT ON COLUMN "system"."menu"."parent_id" IS '父菜单ID';
COMMENT ON COLUMN "system"."menu"."parent_code" IS '父菜单CODE';

-- ----------------------------
-- Records of menu
-- ----------------------------
INSERT INTO "system"."menu" (code, name, sort, tier, parent_code) VALUES ('systemConfig',    '系统配置',     1000, 0, null);
INSERT INTO "system"."menu" (code, name, sort, tier, parent_code) VALUES ('systemManage',    '系统管理',     1000, 0, null);
INSERT INTO "system"."menu" (code, name, sort, tier, parent_code) VALUES ('accountManage',   '账户管理',     1,    1, 'systemManage');
INSERT INTO "system"."menu" (code, name, sort, tier, parent_code) VALUES ('groupManage',     '用户组管理',   2,    1, 'systemManage');
INSERT INTO "system"."menu" (code, name, sort, tier, parent_code) VALUES ('operationLog',    '操作日志',     4,    1, 'systemManage');

INSERT INTO "system"."menu" (code, name, sort, tier, parent_code) VALUES ('dictManage',      '数据字典',     1,    1, 'systemConfig');
INSERT INTO "system"."menu" (code, name, sort, tier, parent_code) VALUES ('menuManage',      '菜单管理',     2,    1, 'systemConfig');
INSERT INTO "system"."menu" (code, name, sort, tier, parent_code) VALUES ('systemUserManage','系统用户管理', 3,    1, 'systemConfig');
INSERT INTO "system"."menu" (code, name, sort, tier, parent_code) VALUES ('areaCodeManage',  '区域代码管理', 4,    1, 'systemConfig');
INSERT INTO "system"."menu" (code, name, sort, tier, parent_code) VALUES ('firstPage',       '首页',         0,    0, null);
update system.menu set parent_id = b.id from (select m.id, m.code from system.menu m where m.parent_code is null) b where b.code = parent_code;


-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Primary Key structure for table menu
-- ----------------------------
ALTER TABLE "system"."menu" ADD PRIMARY KEY ("id");
