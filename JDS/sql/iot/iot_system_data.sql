
-- IOT 业务系统数据，包括相关数据字典、菜单、菜单项及用户组


-- 定位模块状态

delete from system.code_type where code='GPS_MODULE_STATUS';
insert into system.code_type ( code, name, description, status ) values( 'GPS_MODULE_STATUS', '定位模块状态', '定位模块状态', '1');

delete from system.code_value where type_code='GPS_MODULE_STATUS';

insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', '普通故障', '普通故障', 'GPS_MODULE_LOG_LEVEL', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('3', '严重故障', '严重故障', 'GPS_MODULE_LOG_LEVEL', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('4', '系统故障', '系统错误', 'GPS_MODULE_LOG_LEVEL', '4', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('5', '未连接', '未连接', 'GPS_MODULE_STATUS', '5', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('6', '正常', '正常', 'GPS_MODULE_STATUS', '6', '1');
update system.code_value set type_id = ( select id from system.code_type where code='GPS_MODULE_STATUS' ) where type_code = 'GPS_MODULE_STATUS';

-- 定位模块日志级别

delete from system.code_type where code='GPS_MODULE_LOG_LEVEL';
insert into system.code_type ( code, name, description, status ) values( 'GPS_MODULE_LOG_LEVEL', '定位模块日志级别', '定位模块日志级别', '1');

delete from system.code_value where type_code='GPS_MODULE_LOG_LEVEL';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', '普通事件', '普通事件', 'GPS_MODULE_LOG_LEVEL', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', '普通故障', '普通故障', 'GPS_MODULE_LOG_LEVEL', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('3', '严重故障', '严重故障', 'GPS_MODULE_LOG_LEVEL', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('4', '系统故障', '系统错误', 'GPS_MODULE_LOG_LEVEL', '4', '1');
update system.code_value set type_id = ( select id from system.code_type where code='GPS_MODULE_LOG_LEVEL' ) where type_code = 'GPS_MODULE_LOG_LEVEL';

-- 定位模块错误显示颜色

delete from system.code_type where code='GPS_MODULE_ERROR_COLOR';
insert into system.code_type ( code, name, description, status ) values( 'GPS_MODULE_ERROR_COLOR', '定位模块错误显示颜色', '定位模块错误显示颜色', '1');

delete from system.code_value where type_code='GPS_MODULE_ERROR_COLOR';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', 'balck', '普通事件', 'GPS_MODULE_ERROR_COLOR', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', 'blue', '普通故障', 'GPS_MODULE_ERROR_COLOR', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('3', 'orange', '严重故障', 'GPS_MODULE_ERROR_COLOR', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('4', 'red', '系统故障', 'GPS_MODULE_ERROR_COLOR', '4', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('5', 'gray', '未连接', 'GPS_MODULE_ERROR_COLOR', '5', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('6', 'green', '正常颜色', 'GPS_MODULE_ERROR_COLOR', '6', '1');
update system.code_value set type_id = ( select id from system.code_type where code='GPS_MODULE_ERROR_COLOR' ) where type_code = 'GPS_MODULE_ERROR_COLOR';


-- 定位模块设备类型

delete from system.code_type where code='GPS_MODULE_DEVICE_TYPE';
insert into system.code_type ( code, name, description, status ) values( 'GPS_MODULE_DEVICE_TYPE', '定位模块设备类型', '定位模块设备类型', '1');

delete from system.code_value where type_code='GPS_MODULE_DEVICE_TYPE';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', '常规模块', '常规模块', 'GPS_MODULE_DEVICE_TYPE', '1', '1');
update system.code_value set type_id = ( select id from system.code_type where code='GPS_MODULE_DEVICE_TYPE' ) where type_code = 'GPS_MODULE_DEVICE_TYPE';

-- 定位模块安装类型

delete from system.code_type where code='GPS_MODULE_INSTALL_TYPE';
insert into system.code_type ( code, name, description, status ) values( 'GPS_MODULE_INSTALL_TYPE', '定位模块安装类型', '定位模块安装类型', '1');

delete from system.code_value where type_code='GPS_MODULE_INSTALL_TYPE';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', '汽车', '汽车', 'GPS_MODULE_INSTALL_TYPE', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', '机床', '机床', 'GPS_MODULE_INSTALL_TYPE', '2', '1');
update system.code_value set type_id = ( select id from system.code_type where code='GPS_MODULE_INSTALL_TYPE' ) where type_code = 'GPS_MODULE_INSTALL_TYPE';


-- 创建IOT菜单
insert into system.menu (code, name, sort, tier, parent_code) values ('gpsManage', '定位模块管理', '100', '0', NULL);
insert into system.menu (code, name, sort, tier, parent_code) values ('gpsModuleManage', '模块管理', '1', '1', 'gpsManage');
insert into system.menu (code, name, sort, tier, parent_code) values ('gpsModuleMap', '位置地图', '2', '1', 'gpsManage');
insert into system.menu (code, name, sort, tier, parent_code) values ('gpsModuleTrace', '模块轨迹', '3', '1', 'gpsManage');
update system.menu set parent_id = b.id from (select m.id, m.code from system.menu m where m.parent_code is null) b where b.code = parent_code;


-- 创建菜单项
insert into system.menu_item (menu_code, path) values ('gpsModuleManage', '/gpsModuleManage');
insert into system.menu_item (menu_code, path) values ('gpsModuleMap', '/gpsModuleMap');
insert into system.menu_item (menu_code, path) values ('gpsModuleTrace', '/gpsModuleTrace');
update system.menu_item set menu_id = (select m.id from system.menu m where m.code = menu_code); 

-- -- 创建用户组，用户组类型为1，表示该用户组不能被删除，其他用户组由管理员在前端完成
-- INSERT INTO system.user_group (name, c_name, group_type, description) VALUES ('iotManage', '物联网管理组', '1', '物联网管理组');

-- 用户组权限，由管理员通过前端设置
