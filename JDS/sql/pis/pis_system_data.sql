
-- PIS 业务系统数据，包括相关数据字典、菜单、菜单项及用户组


-- ----------------------------
-- Records of code_value
-- ----------------------------
CREATE OR REPLACE FUNCTION hex2_to_dec(in_hex TEXT) RETURNS int
IMMUTABLE STRICT LANGUAGE sql AS
$body$
  SELECT CAST(CAST(('x' || CAST($1 AS text)) AS bit(16)) AS INT);
$body$;

-- PIS实体类别
delete from system.code_type where code='ENTITY_TYPE';
insert into system.code_type ( code, name, description, status ) values( 'ENTITY_TYPE', 'PIS实体类别', 'PIS中实体类别', '1');

delete from system.code_value where type_code='ENTITY_TYPE';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('0', '车头控制器', '车头控制器', 'ENTITY_TYPE', '0', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', '车尾控制器', '车尾控制器', 'ENTITY_TYPE', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', '上行控制器', '上行控制器', 'ENTITY_TYPE', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('3', '下行控制器', '下行控制器', 'ENTITY_TYPE', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('4', '站厅控制器', '站厅控制器', 'ENTITY_TYPE', '4', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('5', '其他控制器', '其他控制器', 'ENTITY_TYPE', '5', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('6', '车站服务器', '车站服务器', 'ENTITY_TYPE', '6', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('7', '流媒体服务器', '流媒体服务器', 'ENTITY_TYPE', '7', '1'); 
insert into system.code_value ( code, name, description, type_code, sort, status ) values('8', '综合监控系统', '综合监控系统', 'ENTITY_TYPE', '8', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('9', 'ATS', 'ATS', 'ENTITY_TYPE', '9', '1'); 
insert into system.code_value ( code, name, description, type_code, sort, status ) values('10', '中心服务器', '中心服务器', 'ENTITY_TYPE', '10', '1');

update system.code_value set type_id = ( select id from system.code_type where code='ENTITY_TYPE' ) where type_code = 'ENTITY_TYPE';

-- PIS车站类别
delete from system.code_type where code='STATION_TYPE';
insert into system.code_type ( code, name, description, status ) values( 'STATION_TYPE', 'PIS车站类别', 'PIS车站类别', '1');

delete from system.code_value where type_code='STATION_TYPE';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('N', '运营车站', '运营车站', 'STATION_TYPE', '0', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('S', '非运营车站', '非运营车站', 'STATION_TYPE', '1', '1');
update system.code_value set type_id = ( select id from system.code_type where code='STATION_TYPE' ) where type_code = 'STATION_TYPE';

-- PIS车辆类别
delete from system.code_type where code='VEHICLE_TYPE';
insert into system.code_type ( code, name, description, status ) values( 'VEHICLE_TYPE', 'PIS车站类别', 'PIS车站类别', '1');

delete from system.code_value where type_code='VEHICLE_TYPE';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('DT', '带司机室的拖车', '带司机室的拖车', 'VEHICLE_TYPE', '0', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('TP', '带受电弓的拖车', '带受电弓的拖车', 'VEHICLE_TYPE', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('M', '动车', '动车', 'VEHICLE_TYPE', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('T', '拖车', '拖车', 'VEHICLE_TYPE', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('DM', '带司机室的动车', '带司机室的动车', 'VEHICLE_TYPE', '4', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('MP', '带受电弓的动车', '带受电弓的动车', 'VEHICLE_TYPE', '5', '1');
update system.code_value set type_id = ( select id from system.code_type where code='VEHICLE_TYPE' ) where type_code = 'VEHICLE_TYPE';


-- PIS消息类别

delete from system.code_type where code='MESSAGE_TYPE';
insert into system.code_type ( code, name, description, status ) values( 'MESSAGE_TYPE', 'PIS消息类别', 'PIS消息类别', '1');

delete from system.code_value where type_code='MESSAGE_TYPE';
insert into system.code_value ( code, description, name, type_code, sort, status ) values('0', 'HEARTBEAT', '心跳', 'MESSAGE_TYPE', '0', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('1', 'HEARTBEAT_ACK', '心跳响应', 'MESSAGE_TYPE', '1', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('2', 'SEND_PROGRAM_LIST', '发送节目单', 'MESSAGE_TYPE', '2', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('3', 'SEND_PROGRAM_LIST_ACK', '发送节目单响应', 'MESSAGE_TYPE', '3', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('4', 'SEND_NEWS', '发送新闻信息', 'MESSAGE_TYPE', '4', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('5', 'SEND_NEWS_ACK', '发送新闻响应', 'MESSAGE_TYPE', '5', '1');
-- insert into system.code_value ( code, description, name, type_code, sort, status ) values('6', 'CLEAR_NEWS', '清除新闻信息', 'MESSAGE_TYPE', '6', '1');
-- insert into system.code_value ( code, description, name, type_code, sort, status ) values('7', 'CLEAR_NEWS_ACK', '清除新闻响应', 'MESSAGE_TYPE', '7', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('8', 'SEND_EMERGENCE', '发送紧急消息', 'MESSAGE_TYPE', '8', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('9', 'SEND_EMERGENCE_ACK', '发送紧急消息响应', 'MESSAGE_TYPE', '9', '1');
-- insert into system.code_value ( code, description, name, type_code, sort, status ) values('10', 'CLEAR_EMERGENCE', '清除紧急', 'MESSAGE_TYPE', '10', '1');
-- insert into system.code_value ( code, description, name, type_code, sort, status ) values('11', 'CLEAR_EMERGENCE_ACK', '清除紧急响应', 'MESSAGE_TYPE', '11', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('12', 'SEND_LINE_INFO', '线路信息', 'MESSAGE_TYPE', '12', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('13', 'SEND_LINE_INFO_ACK', '线路信息响应', 'MESSAGE_TYPE', '13', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('14', 'SEND_STATION_TIME', '站点到达信息', 'MESSAGE_TYPE', '14', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('15', 'SEND_STATION_ACK', '站点到达信息响应', 'MESSAGE_TYPE', '15', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('16', 'SEND_FORMAT_FILE', '发送版式文件', 'MESSAGE_TYPE', '16', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('17', 'SEND_FORMAT_FILE_ACK', '发送版式文件响应', 'MESSAGE_TYPE', '17', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('18', 'SYSTEM_REBOOT', '系统重启', 'MESSAGE_TYPE', '18', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('19', 'SYSTEM_REBOOT_ACK', '系统重启响应', 'MESSAGE_TYPE', '19', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('20', 'RUN_STATUS', '控制器状态', 'MESSAGE_TYPE', '20', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('21', 'RUN_STATUS_ACK', '控制器状态响应', 'MESSAGE_TYPE', '21', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('22', 'RUN_LOG', '控制器日志', 'MESSAGE_TYPE', '22', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('23', 'RUN_LOG_ACK', '控制器日志响应', 'MESSAGE_TYPE', '23', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('24', 'SEND_UPDATE', '控制器版本更新', 'MESSAGE_TYPE', '24', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('25', 'SEND_UPDATE_ACK', '控制器版本更新响应', 'MESSAGE_TYPE', '25', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('26', 'INITIAL_REQUEST', '控制器初始化', 'MESSAGE_TYPE', '26', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('27', 'INITIAL_REQUEST_ACK', '控制器初始化响应', 'MESSAGE_TYPE', '27', '1');

insert into system.code_value ( code, description, name, type_code, sort, status ) values('28', 'SEND_WORK_TIME', '更新休眠时间', 'MESSAGE_TYPE', '28', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('29', 'SEND_WORK_TIME_ACK', '更新休眠时间响应', 'MESSAGE_TYPE', '28', '1');

insert into system.code_value ( code, description, name, type_code, sort, status ) values('30', 'VIEW_CONTROLLER_VIDEO', '查看控制器实时视频', 'MESSAGE_TYPE', '30', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('31', 'VIEW_CONTROLLER_VIDEO_ACK', '查看控制器实时视频响应', 'MESSAGE_TYPE', '31', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('32', 'SEND_SAFE_VIDEO', '发送安全垫片', 'MESSAGE_TYPE', '32', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('33', 'SEND_SAFE_VIDEO_ACK', '发送安全垫片响应', 'MESSAGE_TYPE', '33', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('34', 'SEND_DOWNLOAD_COMPLETE', '下载完成通知消息', 'MESSAGE_TYPE', '34', '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values('35', 'SEND_DOWNLOAD_COMPLETE_ACK', '下载完成通知响应消息', 'MESSAGE_TYPE', '35', '1');

insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('0100')), 'MODBUS_DATA_ACK', '回应包', 'MESSAGE_TYPE', 								(select hex2_to_dec('0100')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('0101')), 'MODBUS_READ_HALL_CONTROLLER_STATUS', '读取站厅控制器站台', 'MESSAGE_TYPE', (select hex2_to_dec('0101')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('0102')), 'MODBUS_READ_UP_CONTROLLER_STATUS', '读取上行控制器站台', 'MESSAGE_TYPE', 	(select hex2_to_dec('0102')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('0103')), 'MODBUS_READ_DOWN_CONTROLLER_STATUS', '读取下行控制器站台', 'MESSAGE_TYPE', (select hex2_to_dec('0103')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('010B')), 'MODBUS_SET_HALL_CONTROLLER', '控制站厅控制器站台', 'MESSAGE_TYPE', 		(select hex2_to_dec('010B')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('010C')), 'MODBUS_SET_UP_CONTROLLER', '控制上行控制器站台', 'MESSAGE_TYPE', 			(select hex2_to_dec('010C')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('010D')), 'MODBUS_SET_DOWN_CONTROLLER', '控制下行控制器站台', 'MESSAGE_TYPE', 		(select hex2_to_dec('010D')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('0115')), 'MODBUS_SELECT_ALL_CONTROLLER', '选择所有控制器', 'MESSAGE_TYPE', 			(select hex2_to_dec('0115')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('0116')), 'MODBUS_SET_MSG_MODE', '设置消息模式', 'MESSAGE_TYPE', 						(select hex2_to_dec('0116')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('0133')), 'MODBUS_SET_MSG_CONTENT', '设置消息内容', 'MESSAGE_TYPE', 					(select hex2_to_dec('0133')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('01C9')), 'MODBUS_DOWN_LINE1_DEST', '下行第一辆车目的地', 'MESSAGE_TYPE', 			(select hex2_to_dec('01C9')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('01CD')), 'MODBUS_DOWN_LINE1_TIME_HH', '下行第一辆车时间，小时', 'MESSAGE_TYPE', 		(select hex2_to_dec('01CD')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('01CC')), 'MODBUS_DOWN_LINE1_TIME_MM', '下行第一辆车时间，分钟', 'MESSAGE_TYPE', 		(select hex2_to_dec('01CC')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('01CF')), 'MODBUS_DOWN_LINE2_DEST', '下行第二辆车目的地', 'MESSAGE_TYPE', 			(select hex2_to_dec('01CF')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('01C3')), 'MODBUS_DOWN_LINE2_TIME_HH', '下行第二辆车时间，小时', 'MESSAGE_TYPE', 		(select hex2_to_dec('01C3')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('01C2')), 'MODBUS_DOWN_LINE2_TIME_MM', '下行第二辆车时间，分钟', 'MESSAGE_TYPE', 		(select hex2_to_dec('01C2')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('01C8')), 'MODBUS_UP_LINE1_DEST', '上行第一辆车目的地', 'MESSAGE_TYPE', 				(select hex2_to_dec('01C8')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('01CB')), 'MODBUS_UP_LINE1_TIME_HH', '上行第一辆车时间，小时', 'MESSAGE_TYPE', 		(select hex2_to_dec('01CB')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('01CA')), 'MODBUS_UP_LINE1_TIME_MM', '上行第一辆车时间，分钟', 'MESSAGE_TYPE', 		(select hex2_to_dec('01CA')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('01CE')), 'MODBUS_UP_LINE2_DEST', '上行第二辆车目的地', 'MESSAGE_TYPE', 				(select hex2_to_dec('01CE')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('01D1')), 'MODBUS_UP_LINE2_TIME_HH', '上行第二辆车时间，小时', 'MESSAGE_TYPE', 		(select hex2_to_dec('01D1')), '1');
insert into system.code_value ( code, description, name, type_code, sort, status ) values((select hex2_to_dec('01D0')), 'MODBUS_UP_LINE2_TIME_MM', '上行第二辆车时间，分钟', 'MESSAGE_TYPE', 		(select hex2_to_dec('01D0')), '1');

update system.code_value set type_id = ( select id from system.code_type where code='MESSAGE_TYPE' ) where type_code = 'MESSAGE_TYPE';

-- 错误颜色

delete from system.code_type where code='ERROR_COLOR';
insert into system.code_type ( code, name, description, status ) values( 'ERROR_COLOR', '错误级别颜色', '错误级别颜色', '1');

delete from system.code_value where type_code='ERROR_COLOR';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('0', 'green', '正常颜色', 'ERROR_COLOR', '0', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', 'balck', '普通事件', 'ERROR_COLOR', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', 'blue', '普通故障', 'ERROR_COLOR', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('3', 'orange', '严重故障', 'ERROR_COLOR', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('4', 'red', '系统故障', 'ERROR_COLOR', '4', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('5', 'gray', '未连接', 'ERROR_COLOR', '5', '1');
update system.code_value set type_id = ( select id from system.code_type where code='ERROR_COLOR' ) where type_code = 'ERROR_COLOR';

-- 控制器状态

delete from system.code_type where code='CONTROLLER_STATUS';
insert into system.code_type ( code, name, description, status ) values( 'CONTROLLER_STATUS', '控制器状态', '控制器状态', '1');

delete from system.code_value where type_code='CONTROLLER_STATUS';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('0', '正常', '正常', 'CONTROLLER_STATUS', '0', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('5', '未连接', '未连接', 'CONTROLLER_STATUS', '5', '1');
update system.code_value set type_id = ( select id from system.code_type where code='CONTROLLER_STATUS' ) where type_code = 'CONTROLLER_STATUS';

-- 控制器日志级别

delete from system.code_type where code='CTRL_LOG_LEVEL';
insert into system.code_type ( code, name, description, status ) values( 'CTRL_LOG_LEVEL', '控制器日志级别', '控制器日志级别', '1');

delete from system.code_value where type_code='CTRL_LOG_LEVEL';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', '普通事件', '普通事件', 'CTRL_LOG_LEVEL', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', '普通故障', '普通故障', 'CTRL_LOG_LEVEL', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('3', '严重故障', '严重故障', 'CTRL_LOG_LEVEL', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('4', '系统故障', '系统错误', 'CTRL_LOG_LEVEL', '4', '1');
update system.code_value set type_id = ( select id from system.code_type where code='CTRL_LOG_LEVEL' ) where type_code = 'CTRL_LOG_LEVEL';

-- 中心服务器发给控制器的消息类别

delete from system.code_type where code='MSG_TYPE_CONTROLLER';
insert into system.code_type ( code, name, description, status ) values( 'MSG_TYPE_CONTROLLER', '控制器消息类别', '中心服务器发给控制器的消息类别', '1');

delete from system.code_value where type_code='MSG_TYPE_CONTROLLER';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', '节目单', '节目单', 'MSG_TYPE_CONTROLLER', '0', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('4', '新闻', '新闻', 'MSG_TYPE_CONTROLLER', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('8', '紧急消息', '紧急消息', 'MSG_TYPE_CONTROLLER', '8', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('16', '版式文件', '版式文件', 'MSG_TYPE_CONTROLLER', '16', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('18', '重启', '重启', 'MSG_TYPE_CONTROLLER', '18', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('24', '软件更新', '软件更新', 'MSG_TYPE_CONTROLLER', '24', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('28', '更新休眠时间', '更新休眠时间', 'MSG_TYPE_CONTROLLER', '28', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('32', '发送安全垫片', '发送安全垫片', 'MSG_TYPE_CONTROLLER', '32', '1');

update system.code_value set type_id = ( select id from system.code_type where code='MSG_TYPE_CONTROLLER' ) where type_code = 'MSG_TYPE_CONTROLLER';

-- 消息发送状态

delete from system.code_type where code='PROGRAM_SEND_STATUS';
insert into system.code_type ( code, name, description, status ) values( 'PROGRAM_SEND_STATUS', '中心服务器消息发送状态', '中心服务器发给控制器的消息状态', '1');

delete from system.code_value where type_code='PROGRAM_SEND_STATUS';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', '未下发', '未下发', 'PROGRAM_SEND_STATUS', '0', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', '已下发', '已下发', 'PROGRAM_SEND_STATUS', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('3', '接收成功', '接收成功', 'PROGRAM_SEND_STATUS', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('4', '部分成功', '部分成功', 'PROGRAM_SEND_STATUS', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('5', '完全失败', '完全失败', 'PROGRAM_SEND_STATUS', '4', '1');
update system.code_value set type_id = ( select id from system.code_type where code='PROGRAM_SEND_STATUS' ) where type_code = 'PROGRAM_SEND_STATUS';


-- 控制器消息接收状态

delete from system.code_type where code='CTRL_RECEIVE_STATUS';
insert into system.code_type ( code, name, description, status ) values( 'CTRL_RECEIVE_STATUS', '控制器消息接收状态', '控制器消息接收状态', '1');

delete from system.code_value where type_code='CTRL_RECEIVE_STATUS';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('0', '失败', '失败 ', 'CTRL_RECEIVE_STATUS', '0', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', '成功', '成功', 'CTRL_RECEIVE_STATUS', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', '待接收', '待接收', 'CTRL_RECEIVE_STATUS', '2', '1');
-- 针对节目单节目下载及安全垫片下载，下载完成后控制器需要回复下载完成消息
insert into system.code_value ( code, name, description, type_code, sort, status ) values('3', '正在下载', '正在下载', 'CTRL_RECEIVE_STATUS', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('4', '下载完成', '下载完成', 'CTRL_RECEIVE_STATUS', '4', '1');
update system.code_value set type_id = ( select id from system.code_type where code='CTRL_RECEIVE_STATUS' ) where type_code = 'CTRL_RECEIVE_STATUS';

-- 节目单申请状态

delete from system.code_type where code='PROGRAM_APPLY_STATUS';
insert into system.code_type ( code, name, description, status ) values( 'PROGRAM_APPLY_STATUS', '节目单申请状态', '节目单申请状态', '1');

delete from system.code_value where type_code='PROGRAM_APPLY_STATUS';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', '未提交', '未提交', 'PROGRAM_APPLY_STATUS', '0', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', '待审核', '待审核', 'PROGRAM_APPLY_STATUS', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('3', '已通过', '已通过', 'PROGRAM_APPLY_STATUS', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('4', '驳回', '驳回', 'PROGRAM_APPLY_STATUS', '3', '1');
update system.code_value set type_id = ( select id from system.code_type where code='PROGRAM_APPLY_STATUS' ) where type_code = 'PROGRAM_APPLY_STATUS';

-- 控制器日志类别

delete from system.code_type where code='CTRL_LOG_TYPE';
insert into system.code_type ( code, name, description, status ) values( 'CTRL_LOG_TYPE', '控制器日志类别', '控制器日志类别', '1');

delete from system.code_value where type_code='CTRL_LOG_TYPE';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', '硬件', '系统硬件', 'CTRL_LOG_TYPE', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', '系统', '系统', 'CTRL_LOG_TYPE', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('3', '网络', '网络', 'CTRL_LOG_TYPE', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('4', '应用软件', '应用软件', 'CTRL_LOG_TYPE', '4', '1');
update system.code_value set type_id = ( select id from system.code_type where code='CTRL_LOG_TYPE' ) where type_code = 'CTRL_LOG_TYPE';

-- 控制器设备类型

delete from system.code_type where code='CONTROLLER_DEVICE_TYPE';
insert into system.code_type ( code, name, description, status ) values( 'CONTROLLER_DEVICE_TYPE', '控制器设备类型', '控制器设备类型', '1');

delete from system.code_value where type_code='CONTROLLER_DEVICE_TYPE';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', '常规控制器', '常规控制器', 'CONTROLLER_DEVICE_TYPE', '1', '1');
update system.code_value set type_id = ( select id from system.code_type where code='CONTROLLER_DEVICE_TYPE' ) where type_code = 'CONTROLLER_DEVICE_TYPE';

-- 控制器安装类型

delete from system.code_type where code='CONTROLLER_INSTALL_TYPE';
insert into system.code_type ( code, name, description, status ) values( 'CONTROLLER_INSTALL_TYPE', '控制器安装类型', '控制器安装类型', '1');

delete from system.code_value where type_code='CONTROLLER_INSTALL_TYPE';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('0', '车头控制器', '车头控制器', 'CONTROLLER_INSTALL_TYPE', '0', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', '车尾控制器', '车尾控制器', 'CONTROLLER_INSTALL_TYPE', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', '上行控制器', '下行控制器', 'CONTROLLER_INSTALL_TYPE', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('3', '下行控制器', '下行控制器', 'CONTROLLER_INSTALL_TYPE', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('4', '站厅控制器', '站厅控制器', 'CONTROLLER_INSTALL_TYPE', '4', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('5', '其他控制器', '其他控制器', 'CONTROLLER_INSTALL_TYPE', '5', '1');
update system.code_value set type_id = ( select id from system.code_type where code='CONTROLLER_INSTALL_TYPE' ) where type_code = 'CONTROLLER_INSTALL_TYPE';


-- 创建PIS菜单
insert into system.menu (code, name, sort, tier, parent_code) values ('serviceManage',    '业务维护',     '1', '0', NULL);
insert into system.menu (code, name, sort, tier, parent_code) values ('lineManage',       '线路维护',     '1', '1', 'serviceManage');
insert into system.menu (code, name, sort, tier, parent_code) values ('ctrlLogQuery',     '日志查询',     '2', '1', 'serviceManage');
insert into system.menu (code, name, sort, tier, parent_code) values ('runningStatus',    '运行状态',     '3', '1', 'serviceManage');
insert into system.menu (code, name, sort, tier, parent_code) values ('videoSurv',        '视频监播',     '4', '1', 'serviceManage');
insert into system.menu (code, name, sort, tier, parent_code) values ('playList',         '节目单',       '2', '0', NULL);
insert into system.menu (code, name, sort, tier, parent_code) values ('playListManage',   '节目单管理',   '1', '1', 'playList');
insert into system.menu (code, name, sort, tier, parent_code) values ('programFileManage','节目管理',     '2', '1', 'playList');
insert into system.menu (code, name, sort, tier, parent_code) values ('formatFileManage', '版式文件管理', '3', '1', 'playList');
insert into system.menu (code, name, sort, tier, parent_code) values ('messageManage',    '消息管理',     '3', '0', NULL);
insert into system.menu (code, name, sort, tier, parent_code) values ('sendMessage',      '发送信息',     '1', '1', 'messageManage');
insert into system.menu (code, name, sort, tier, parent_code) values ('MessageQuery',     '消息查询',     '2', '1', 'messageManage');
update system.menu set parent_id = b.id from (select m.id, m.code from system.menu m where m.parent_code is null) b where b.code = parent_code;

-- 创建菜单项
insert into system.menu_item (menu_code, path) values ('lineManage', '/lineManage');
insert into system.menu_item (menu_code, path) values ('ctrlLogQuery', '/ctrl_log_query');
insert into system.menu_item (menu_code, path) values ('runningStatus', '/runningStatus');
insert into system.menu_item (menu_code, path) values ('playListManage', '/programManage');
insert into system.menu_item (menu_code, path) values ('programFileManage', '/programFileManage');
insert into system.menu_item (menu_code, path) values ('formatFileManage', '/formatFileManage');
insert into system.menu_item (menu_code, path) values ('videoSurv', '/videoSurv');
insert into system.menu_item (menu_code, path) values ('sendMessage', '/sendMessageToController');
insert into system.menu_item (menu_code, path) values ('MessageQuery', '/messageQuery');
update system.menu_item set menu_id = (select m.id from system.menu m where m.code = menu_code); 

-- -- 创建PIS用户组，用户组类型为1，表示该用户组不能被删除
-- INSERT INTO system.user_group (name, c_name, group_type, description) VALUES ('listSendGroup', '节目单发送组', '1', '节目单发送组');
-- INSERT INTO system.user_group (name, c_name, group_type, description) VALUES ('listCheckGroup', '节目单审批组', '1', '节目单审批组');

-- 用户组权限，由管理员通过前台设置
