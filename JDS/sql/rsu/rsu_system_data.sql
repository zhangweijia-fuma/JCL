
-- RSU 业务系统数据，包括相关数据字典、菜单、菜单项及用户组



-- 创建菜单
insert into system.menu (code, name, sort, tier, parent_code) values ('RSUManage', 'RSU管理', '4', '0', NULL);
insert into system.menu (code, name, sort, tier, parent_code) values ('RSUProjectManage', 'RSU项目管理', '1', '1', 'RSUManage');
insert into system.menu (code, name, sort, tier, parent_code) values ('RSUDeviceManage', 'RSU设备管理', '2', '1', 'RSUManage');
insert into system.menu (code, name, sort, tier, parent_code) values ('RSUDeviceLocation', 'RSU设备地图', '3', '1', 'RSUManage');
insert into system.menu (code, name, sort, tier, parent_code) values ('RSUDeviceConfig', 'RSU设备设置', '4', '1', 'RSUManage');
update system.menu set parent_id = (select id from system.menu where code = 'RSUManage') where parent_code = 'RSUManage';

-- 创建菜单项
insert into system.menu_item (menu_code, path) values ('RSUProjectManage', '/RSUProjectManage');
insert into system.menu_item (menu_code, path) values ('RSUDeviceManage', '/RSUDeviceManage');
insert into system.menu_item (menu_code, path) values ('RSUDeviceLocation', '/RSUDeviceLocation');
insert into system.menu_item (menu_code, path) values ('RSUDeviceConfig', '/RSUDeviceConfig');
update system.menu_item set menu_id = (select m.id from system.menu m where m.code = menu_code);

-- RSU 用户由超级管理员创建并设置菜单权限
-- 相应用户组在创建系统用户的时候创建

