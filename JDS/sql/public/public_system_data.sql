-- 公共部分数据字典参数

-- 组织类别
delete from system.code_type where code='ORGANIZATION_TYPE';
insert into system.code_type ( code, name, description, status ) values( 'ORGANIZATION_TYPE', '组织类别', '组织类别', '1');

delete from system.code_value where type_code='ORGANIZATION_TYPE';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', '国有企业', '国有企业', 'ORGANIZATION_TYPE', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', '集体企业', '集体企业', 'ORGANIZATION_TYPE', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('3', '民营企业', '民营企业', 'ORGANIZATION_TYPE', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('4', '个体企业', '个体企业', 'ORGANIZATION_TYPE', '4', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('5', '美资合资', '美资合资', 'ORGANIZATION_TYPE', '5', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('6', '欧洲合资', '欧洲合资', 'ORGANIZATION_TYPE', '6', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('7', '港澳台合资', '港澳台合资', 'ORGANIZATION_TYPE', '7', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('8', '其他合资', '其他合资', 'ORGANIZATION_TYPE', '8', '1'); 
insert into system.code_value ( code, name, description, type_code, sort, status ) values('9', '美资独资', '美资独资', 'ORGANIZATION_TYPE', '9', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('10', '欧洲独资', '欧洲独资', 'ORGANIZATION_TYPE', '10', '1'); 
insert into system.code_value ( code, name, description, type_code, sort, status ) values('11', '港澳台独资', '港澳台独资', 'ORGANIZATION_TYPE', '11', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('12', '其他独资', '其他独资', 'ORGANIZATION_TYPE', '12', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('13', '政府机构', '政府机构', 'ORGANIZATION_TYPE', '13', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('14', '社会团体', '社会团体', 'ORGANIZATION_TYPE', '14', '1');

update system.code_value set type_id = ( select id from system.code_type where code='ORGANIZATION_TYPE' ) where type_code = 'ORGANIZATION_TYPE';

-- 身份证件类型
delete from system.code_type where code='IDENTITY_TYPE';
insert into system.code_type ( code, name, description, status ) values( 'IDENTITY_TYPE', '身份证件类型', '身份证件类型', '1');

delete from system.code_value where type_code='IDENTITY_TYPE';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', '居民身份证', 'ID', 'IDENTITY_TYPE', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', '临时居民身份证', 'Temp ID', 'IDENTITY_TYPE', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('3', '军官证', 'Military ID', 'IDENTITY_TYPE', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('4', '士兵证', 'Soldier ID', 'IDENTITY_TYPE', '4', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('5', '军校学员证', 'Military Cadet card', 'IDENTITY_TYPE', '5', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('6', '警官证', 'Police ID', 'IDENTITY_TYPE', '6', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('7', '护照', 'Passport', 'IDENTITY_TYPE', '7', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('8', '港澳通行证', '港澳通行证', 'IDENTITY_TYPE', '8', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('9', '港澳同乡回乡证', 'Taiwan cell card', 'IDENTITY_TYPE', '9', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('10', '港澳居民来往内地通行证', '港澳居民来往内地通行证', 'IDENTITY_TYPE', '10', '1'); 
insert into system.code_value ( code, name, description, type_code, sort, status ) values('11', '台湾居民来往大陆通行证', '台湾居民来往大陆通行证', 'IDENTITY_TYPE', '11', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('12', '大陆居民往来台湾通行证', '大陆居民往来台湾通行证', 'IDENTITY_TYPE', '12', '1'); 
insert into system.code_value ( code, name, description, type_code, sort, status ) values('13', '外国人居留证', '外国人居留证', 'IDENTITY_TYPE', '13', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('14', '外国人出入境证', '外国人出入境证', 'IDENTITY_TYPE', '14', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('15', '外交官证', '外交官证', 'IDENTITY_TYPE', '15', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('16', '刑满释放证明', '刑满释放证明', 'IDENTITY_TYPE', '16', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('17', '过期居民身份证', '过期居民身份证', 'IDENTITY_TYPE', '17', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('18', '居民户口簿', '居民户口簿', 'IDENTITY_TYPE', '18', '1');

update system.code_value set type_id = ( select id from system.code_type where code='IDENTITY_TYPE' ) where type_code = 'IDENTITY_TYPE';



-- 供应商类型
delete from system.code_type where code='VENDOR_TYPE';
insert into system.code_type ( code, name, description, status ) values( 'VENDOR_TYPE', '供应商类型', 'Vendor Type', '1');

delete from system.code_value where type_code='VENDOR_TYPE';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('SP', '配件供应商', 'Spare Part Supplier', 'VENDOR_TYPE', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('DP', '设备供应商', 'Device Supplier', 'VENDOR_TYPE', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('MS', '维修服务供应商', 'Maintanance Service Supplier', 'VENDOR_TYPE', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('SM', '办公用品供应商', 'Office Material Supplier', 'VENDOR_TYPE', '4', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('RO', '研发外包供应商', 'R&D Supplier', 'VENDOR_TYPE', '5', '1');
update system.code_value set type_id = ( select id from system.code_type where code='VENDOR_TYPE' ) where type_code = 'VENDOR_TYPE';

-- 供应商合作状态
delete from system.code_type where code='VENDOR_COOPERATION_STATUS';
insert into system.code_type ( code, name, description, status ) values( 'VENDOR_COOPERATION_STATUS', '供应商合作状态', 'Vendor Cooperation Status', '1');

delete from system.code_value where type_code='VENDOR_COOPERATION_STATUS';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('CI', '合作启动', 'Cooperation Initial', 'VENDOR_COOPERATION_STATUS', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('CA', '审批申请', 'Cooperation Apply', 'VENDOR_COOPERATION_STATUS', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('CO', '合作进行', 'Cooperation Ongoing', 'VENDOR_COOPERATION_STATUS', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('CP', '暂停合作', 'Cooperation Pause', 'VENDOR_COOPERATION_STATUS', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('CE', '合作结束', 'Cooperation End', 'VENDOR_COOPERATION_STATUS', '4', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('CN', '无合作', 'Cooperation None', 'VENDOR_COOPERATION_STATUS', '5', '1');
update system.code_value set type_id = ( select id from system.code_type where code='VENDOR_COOPERATION_STATUS' ) where type_code = 'VENDOR_COOPERATION_STATUS';


-- 属性值类型
-- 每增减一个类型值类型，就需要在属性类型表中增加一个字段
delete from system.code_type where code='PROPERTY_VALUE_TYPE';
insert into system.code_type ( code, name, description, status ) values( 'PROPERTY_VALUE_TYPE', '属性值类型', 'Property Value Type', '1');

delete from system.code_value where type_code='PROPERTY_VALUE_TYPE';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('STR', '字符串', 'Normal String', 'PROPERTY_VALUE_TYPE', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('INT', '整数', 'Integer Number', 'PROPERTY_VALUE_TYPE', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('FLT', '小数', 'Floating Number', 'PROPERTY_VALUE_TYPE', '3', '1');
update system.code_value set type_id = ( select id from system.code_type where code='PROPERTY_VALUE_TYPE' ) where type_code = 'PROPERTY_VALUE_TYPE';

-- -- 创建设备维护管理菜单
-- insert into system.menu (code, name, sort, tier, parent_code) values ('DeviceManageConfig', '设备配置管理', '600', '0', NULL);
-- update system.menu set parent_id = (select id from system.menu where code = 'DeviceManageConfig') where parent_code = 'DeviceManageConfig';

-- -- 创建菜单项
-- update system.menu_item set menu_id = (select m.id from system.menu m where m.code = menu_code);


-- 创建公共部分菜单
insert into system.menu (code, name, sort, tier, parent_code) values ('SystemBaseConfig', '系统配置', '999', '0', NULL);

INSERT INTO system.menu (code, name, sort, tier, parent_code) VALUES ('OrganizationManage',     '组织团体管理', '1', '1', 'SystemBaseConfig');
insert into system.menu (code, name, sort, tier, parent_code) values ('vendorManage',           '供应商管理',   '2', '1', 'SystemBaseConfig');
insert into system.menu (code, name, sort, tier, parent_code) values ('DeviceManage',           '设备管理',     '3', '1', 'SystemBaseConfig');
insert into system.menu (code, name, sort, tier, parent_code) values ('ComponentManage',        '组件管理',     '4', '1', 'SystemBaseConfig');
insert into system.menu (code, name, sort, tier, parent_code) values ('deviceTypeManage',       '设备类型管理', '5', '1', 'SystemBaseConfig');
insert into system.menu (code, name, sort, tier, parent_code) values ('propertyTypeManage',     '属性类型管理', '6', '1', 'SystemBaseConfig');
insert into system.menu (code, name, sort, tier, parent_code) values ('componentTypeManage',    '组件类型管理', '7', '1', 'SystemBaseConfig');
INSERT INTO system.menu (code, name, sort, tier, parent_code) VALUES ('OrganizationStatistic',  '统计分析',     '8', '1', 'SystemBaseConfig');
update system.menu set parent_id = (select id from system.menu where code = 'SystemBaseConfig') where parent_code = 'SystemBaseConfig';

-- 创建菜单项
insert into system.menu_item (menu_code, path) values ('OrganizationManage', '/OrganizationManage');
insert into system.menu_item (menu_code, path) values ('OrganizationStatistic', '/OrganizationStatistic');

insert into system.menu_item (menu_code, path) values ('vendorManage', '/vendorManage');
insert into system.menu_item (menu_code, path) values ('DeviceManage', '/DeviceManage');
insert into system.menu_item (menu_code, path) values ('ComponentManage', '/ComponentManage');

insert into system.menu_item (menu_code, path) values ('propertyTypeManage', '/propertyTypeManage');
insert into system.menu_item (menu_code, path) values ('componentTypeManage', '/componentTypeManage');
insert into system.menu_item (menu_code, path) values ('deviceTypeManage', '/deviceTypeManage');
update system.menu_item set menu_id = (select m.id from system.menu m where m.code = menu_code);
