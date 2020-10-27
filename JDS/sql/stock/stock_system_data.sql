

-- 创建菜单
insert into system.menu (code, name, sort, tier, parent_code) values ('stockManage', '股票管理', '200', '0', NULL);
insert into system.menu (code, name, sort, tier, parent_code) values ('stockInfo', '股票信息', '1', '1', 'stockManage');
update system.menu set parent_id = b.id from (select m.id, m.code from system.menu m where m.parent_code is null) b where b.code = parent_code;


-- 创建菜单项
insert into system.menu_item (menu_code, path) values ('stockInfo', '/stockInfo');
update system.menu_item set menu_id = (select m.id from system.menu m where m.code = menu_code); 

-- 创建用户组，用户组类型为1，表示该用户组不能被删除，其他用户组由管理员在前端完成
-- INSERT INTO system.user_group (name, c_name, group_type, description) VALUES ('stockManage', '股票管理组', '1', '股票管理组');

-- 用户组权限，由管理员通过前端设置
