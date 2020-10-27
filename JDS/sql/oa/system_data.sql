
-- oa 业务系统数据，包括相关数据字典、菜单、菜单项及用户组


-- ----------------------------
-- Records of code_value
-- ----------------------------


-- 创建oa菜单
insert into system.menu (code, name, sort, tier, parent_code) values ('oaMenu',            '公司OA系统',       '0', '0', NULL);
insert into system.menu (code, name, sort, tier, parent_code) values ('oaBulletinBoard',   '公司公告牌',   '1', '1', 'oaMenu');
insert into system.menu (code, name, sort, tier, parent_code) values ('oaFile',            '公司文件发布', '2', '1', 'oaMenu');
insert into system.menu (code, name, sort, tier, parent_code) values ('oaMarketing',       '战略Marketing','3', '1', 'oaMenu');
insert into system.menu (code, name, sort, tier, parent_code) values ('oaSolutionDept',    '解决方案部',   '4', '1', 'oaMenu');
insert into system.menu (code, name, sort, tier, parent_code) values ('oaTechSupport',     '技术支持中心', '5', '1', 'oaMenu');
insert into system.menu (code, name, sort, tier, parent_code) values ('oaSales',           '市场部',       '6', '1', 'oaMenu');
insert into system.menu (code, name, sort, tier, parent_code) values ('oaFinance',         '财经管理部',   '7', '1', 'oaMenu');
insert into system.menu (code, name, sort, tier, parent_code) values ('oaAdminDept',       '行政管理部',   '8', '1', 'oaMenu');
insert into system.menu (code, name, sort, tier, parent_code) values ('oaForum',           '公司论坛',     '9', '1', 'oaMenu');
update system.menu set parent_id = b.id from (select m.id, m.code from system.menu m where m.parent_code is null) b where b.code = parent_code;

-- 创建菜单项
insert into system.menu_item (menu_code, path) values ('oaBulletinBoard',  '/oaBulletinBoard');
insert into system.menu_item (menu_code, path) values ('oaFile',           '/oaFile');
insert into system.menu_item (menu_code, path) values ('oaMarketing',      '/oaMarketing');
insert into system.menu_item (menu_code, path) values ('oaSolutionDept',   '/oaSolutionDept');
insert into system.menu_item (menu_code, path) values ('oaTechSupport',    '/oaTechSupport');
insert into system.menu_item (menu_code, path) values ('oaSales',          '/oaSales');
insert into system.menu_item (menu_code, path) values ('oaFinance',        '/oaFinance');
insert into system.menu_item (menu_code, path) values ('oaAdminDept',      '/oaAdminDept');
insert into system.menu_item (menu_code, path) values ('oaForum',          '/oaForum');
update system.menu_item set menu_id = (select m.id from system.menu m where m.code = menu_code); 

-- -- 创建oa用户组，用户组类型为1，表示该用户组不能被删除
-- INSERT INTO system.user_group (name, c_name, group_type, description) VALUES ('oaAdminGroup', 'oa管理员组', '1', '凯普顿管理员组');

-- 用户组权限，由管理员通过前台设置
