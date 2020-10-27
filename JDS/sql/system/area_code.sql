
-- ----------------------------
-- Table structure for area_code
-- ----------------------------
DROP TABLE IF EXISTS "system"."area_code";
CREATE TABLE "system"."area_code" (
"id" serial NOT NULL,
"create_time" timestamp(6),
"update_time" timestamp(6),
"create_id" int4,
"update_id" int4,
"area_type" varchar(50),
"area_code" varchar(50),
"area_name" varchar(50),
"area_name_en" varchar(50),
"parent_area_id" int,
"flag" varchar(128)
)
WITH (OIDS=FALSE)
;

COMMENT ON TABLE "system"."area_code" IS '区域代码表';
COMMENT ON COLUMN "system"."area_code"."create_time" IS '创建时间';
COMMENT ON COLUMN "system"."area_code"."update_time" IS '修改时间';
COMMENT ON COLUMN "system"."area_code"."create_id" IS '创建人';
COMMENT ON COLUMN "system"."area_code"."update_id" IS '修改人';
COMMENT ON COLUMN "system"."area_code"."area_type" IS '区域类型';
COMMENT ON COLUMN "system"."area_code"."area_code" IS '区域编号';
COMMENT ON COLUMN "system"."area_code"."area_name" IS '区域名称';
COMMENT ON COLUMN "system"."area_code"."area_name_en" IS '区域英文名称';
COMMENT ON COLUMN "system"."area_code"."parent_area_id" IS '上级区域ID';
COMMENT ON COLUMN "system"."area_code"."flag" IS '国旗或区旗';

-- ----------------------------
-- Records of area_code
-- ----------------------------

-- ----------------------------
-- Alter Sequences Owned By 
-- ----------------------------

-- ----------------------------
-- Indexes structure for table area_code
-- ----------------------------
CREATE UNIQUE INDEX "area_code_code_INDEX" ON "system"."area_code" USING btree ("area_code", "parent_area_id");
CREATE INDEX "area_code_name_INDEX" ON "system"."area_code" USING btree ("area_name");
CREATE INDEX "area_code_name_en_INDEX" ON "system"."area_code" USING btree ("area_name_en");

-- ----------------------------
-- Primary Key structure for table area_code
-- ----------------------------
ALTER TABLE "system"."area_code" ADD PRIMARY KEY ("id");

-- 区域类型
delete from system.code_type where code='AREA_TYPE';
insert into system.code_type ( code, name, description, status ) values( 'AREA_TYPE', '区域类型', 'Area Type', '1');

delete from system.code_value where type_code='AREA_TYPE';
insert into system.code_value (code, name, description, type_code, sort, status ) values('S', '国家', 'Country', 'AREA_TYPE', '1', '1');
insert into system.code_value (code, name, description, type_code, sort, status ) values('P', '省份', 'Province', 'AREA_TYPE', '2', '1');
insert into system.code_value (code, name, description, type_code, sort, status ) values('C', '地市', 'City', 'AREA_TYPE', '3', '1');
insert into system.code_value (code, name, description, type_code, sort, status ) values('D', '区县', 'District or County', 'AREA_TYPE', '4', '1');
insert into system.code_value (code, name, description, type_code, sort, status ) values('T', '乡镇', 'Town', 'AREA_TYPE', '5', '1');
update system.code_value set type_id = ( select id from system.code_type where code='AREA_TYPE' ) where type_code = 'AREA_TYPE';

insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'W', 'EA', '地球', 'Ether');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'AD', '安道尔共和国', 'Andorra');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'AE', '阿拉伯联合酋长国', 'United Arab Emirates');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'AF', '阿富汗', 'Afghanistan');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'AG', '安提瓜和巴布达', 'Antigua and Barbuda');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'AI', '安圭拉岛', 'Anguilla');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'AL', '阿尔巴尼亚', 'Albania');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'AM', '亚美尼亚', 'Armenia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'AO', '安哥拉', 'Angola');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'AR', '阿根廷', 'Argentina');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'AT', '奥地利', 'Austria');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'AU', '澳大利亚', 'Australia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'AZ', '阿塞拜疆', 'Azerbaijan');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BB', '巴巴多斯', 'Barbados');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BD', '孟加拉国', 'Bangladesh');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BE', '比利时', 'Belgium');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BF', '布基纳法索', 'Burkina-faso');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BG', '保加利亚', 'Bulgaria');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BH', '巴林', 'Bahrain');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BI', '布隆迪', 'Burundi');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BJ', '贝宁', 'Benin');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BL', '巴勒斯坦', 'Palestine');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BM', '百慕大群岛', 'Bermuda Is.');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BN', '文莱', 'Brunei');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BO', '玻利维亚', 'Bolivia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BR', '巴西', 'Brazil');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BS', '巴哈马', 'Bahamas');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BW', '博茨瓦纳', 'Botswana');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BY', '白俄罗斯', 'Belarus');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'BZ', '伯利兹', 'Belize');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'CA', '加拿大', 'Canada');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'CF', '中非共和国', 'Central African Republic');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'CG', '刚果', 'Congo');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'CH', '瑞士', 'Switzerland');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'CK', '库克群岛', 'Cook Is.');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'CL', '智利', 'Chile');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'CM', '喀麦隆', 'Cameroon');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'CN', '中国', 'China');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'CO', '哥伦比亚', 'Colombia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'CR', '哥斯达黎加', 'Costa Rica');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'CS', '捷克', 'Czech');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'CU', '古巴', 'Cuba');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'CY', '塞浦路斯', 'Cyprus');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'CZ', '捷克', 'Czech Republic');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'DE', '德国', 'Germany');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'DJ', '吉布提', 'Djibouti');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'DK', '丹麦', 'Denmark');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'DO', '多米尼加共和国', 'Dominica Rep.');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'DZ', '阿尔及利亚', 'Algeria');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'EC', '厄瓜多尔', 'Ecuador');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'EE', '爱沙尼亚', 'Estonia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'EG', '埃及', 'Egypt');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'ES', '西班牙', 'Spain');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'ET', '埃塞俄比亚', 'Ethiopia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'FI', '芬兰', 'Finland');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'FJ', '斐济', 'Fiji');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'FR', '法国', 'France');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'GA', '加蓬', 'Gabon');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'GB', '英国', 'United Kiongdom');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'GD', '格林纳达', 'Grenada');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'GE', '格鲁吉亚', 'Georgia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'GF', '法属圭亚那', 'French Guiana');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'GH', '加纳', 'Ghana');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'GI', '直布罗陀', 'Gibraltar');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'GM', '冈比亚', 'Gambia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'GN', '几内亚', 'Guinea');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'GR', '希腊', 'Greece');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'GT', '危地马拉', 'Guatemala');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'GU', '关岛', 'Guam');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'GY', '圭亚那', 'Guyana');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'HK', '香港特别行政区', 'Hongkong');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'HN', '洪都拉斯', 'Honduras');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'HT', '海地', 'Haiti');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'HU', '匈牙利', 'Hungary');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'ID', '印度尼西亚', 'Indonesia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'IE', '爱尔兰', 'Ireland');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'IL', '以色列', 'Israel');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'IN', '印度', 'India');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'IQ', '伊拉克', 'Iraq');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'IR', '伊朗', 'Iran');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'IS', '冰岛', 'Iceland');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'IT', '意大利', 'Italy');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'JM', '牙买加', 'Jamaica');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'JO', '约旦', 'Jordan');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'JP', '日本', 'Japan');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'KE', '肯尼亚', 'Kenya');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'KG', '吉尔吉斯坦', 'Kyrgyzstan');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'KH', '柬埔寨', 'Kampuchea (Cambodia )');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'KP', '朝鲜', 'North Korea');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'KR', '韩国', 'Korea');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'KT', '科特迪瓦共和国', 'Republic of Ivory Coast');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'KW', '科威特', 'Kuwait');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'KZ', '哈萨克斯坦', 'Kazakstan');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'LA', '老挝', 'Laos');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'LB', '黎巴嫩', 'Lebanon');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'LC', '圣卢西亚', 'St.Lucia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'LI', '列支敦士登', 'Liechtenstein');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'LK', '斯里兰卡', 'Sri Lanka');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'LR', '利比里亚', 'Liberia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'LS', '莱索托', 'Lesotho');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'LT', '立陶宛', 'Lithuania');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'LU', '卢森堡', 'Luxembourg');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'LV', '拉脱维亚', 'Latvia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'LY', '利比亚', 'Libya');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'MA', '摩洛哥', 'Morocco');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'MC', '摩纳哥', 'Monaco');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'MD', '摩尔多瓦', 'Republic of Moldova');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'MG', '马达加斯加', 'Madagascar');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'ML', '马里', 'Mali');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'MM', '缅甸', 'Burma');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'MN', '蒙古', 'Mongolia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'MO', '澳门', 'Macao');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'MS', '蒙特塞拉特岛', 'Montserrat Is');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'MT', '马耳他', 'Malta');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'MU', '毛里求斯', 'Mauritius');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'MV', '马尔代夫', 'Maldives');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'MW', '马拉维', 'Malawi');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'MX', '墨西哥', 'Mexico');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'MY', '马来西亚', 'Malaysia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'MZ', '莫桑比克', 'Mozambique');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'NA', '纳米比亚', 'Namibia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'NE', '尼日尔', 'Niger');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'NG', '尼日利亚', 'Nigeria');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'NI', '尼加拉瓜', 'Nicaragua');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'NL', '荷兰', 'Netherlands');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'NO', '挪威', 'Norway');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'NP', '尼泊尔', 'Nepal');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'NR', '瑙鲁', 'Nauru');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'NZ', '新西兰', 'New Zealand');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'OM', '阿曼', 'Oman');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'PA', '巴拿马', 'Panama');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'PE', '秘鲁', 'Peru');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'PF', '法属玻利尼西亚', 'French Polynesia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'PG', '巴布亚新几内亚', 'Papua New Cuinea');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'PH', '菲律宾', 'Philippines');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'PK', '巴基斯坦', 'Pakistan');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'PL', '波兰', 'Poland');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'PR', '波多黎各', 'Puerto Rico');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'PT', '葡萄牙', 'Portugal');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'PY', '巴拉圭', 'Paraguay');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'QA', '卡塔尔', 'Qatar');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'RO', '罗马尼亚', 'Romania');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'RU', '俄罗斯', 'Russia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SA', '沙特阿拉伯', 'Saudi Arabia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SB', '所罗门群岛', 'Solomon Is');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SC', '塞舌尔', 'Seychelles');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SD', '苏丹', 'Sudan');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SE', '瑞典', 'Sweden');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SG', '新加坡', 'Singapore');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SI', '斯洛文尼亚', 'Slovenia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SK', '斯洛伐克', 'Slovakia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SL', '塞拉利昂', 'Sierra Leone');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SM', '圣马力诺', 'San Marino');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SN', '塞内加尔', 'Senegal');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SO', '索马里', 'Somali');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SR', '苏里南', 'Suriname');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'ST', '圣多美和普林西比', 'Sao Tome and Principe');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SV', '萨尔瓦多', 'EI Salvador');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SY', '叙利亚', 'Syria');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'SZ', '斯威士兰', 'Swaziland');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'TD', '乍得', 'Chad');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'TG', '多哥', 'Togo');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'TH', '泰国', 'Thailand');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'TJ', '塔吉克斯坦', 'Tajikstan');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'TM', '土库曼斯坦', 'Turkmenistan');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'TN', '突尼斯', 'Tunisia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'TO', '汤加', 'Tonga');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'TR', '土耳其', 'Turkey');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'TT', '特立尼达和多巴哥', 'Trinidad and Tobago');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'TW', '台湾省', 'Taiwan');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'TZ', '坦桑尼亚', 'Tanzania');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'UA', '乌克兰', 'Ukraine');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'UG', '乌干达', 'Uganda');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'US', '美国', 'United States of America');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'UY', '乌拉圭', 'Uruguay');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'UZ', '乌兹别克斯坦', 'Uzbekistan');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'VC', '圣文森特岛', 'Saint Vincent');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'VE', '委内瑞拉', 'Venezuela');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'VN', '越南', 'Vietnam');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'YE', '也门', 'Yemen');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'YU', '南斯拉夫', 'Yugoslavia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'ZA', '南非', 'South Africa');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'ZM', '赞比亚', 'Zambia');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'ZR', '扎伊尔', 'Zaire');
insert into system.area_code (area_type, area_code, area_name, area_name_en) values( 'S', 'ZW', '津巴布韦', 'Zimbabwe');
update system.area_code set parent_area_id = 0, flag = area_name_en ||'.png';

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'AHS', '安徽', 'Anhui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'BJS', '北京', 'Beijing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'FJS', '福建', 'Fujian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'GSS', '甘肃', 'Gansu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'GDS', '广东', 'Guangdong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'GXS', '广西', 'Guangxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'GZS', '贵州', 'Guizhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'HNS', '海南', 'Hainan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'HES', '河北', 'Hebei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'HAS', '河南', 'Henan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'HLJ', '黑龙江', 'Heilongjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'HBS', '湖北', 'Hubei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'HN#', '湖南', 'Henan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'JLS', '吉林', 'Jilin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'JSS', '江苏', 'Jiangsu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'JXS', '江西', 'Jiangxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'LNS', '辽宁', 'Liaoning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'NMG', '内蒙古', 'Neimenggu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'NXQ', '宁夏', 'Ningxia', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'QHS', '青海', 'Qinghai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'SDS', '山东', 'Shandong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'SXS', '山西', 'Shanxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'SX#', '陕西', 'Shaanxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'SHS', '上海', 'Shanghai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'SCS', '四川', 'Sichuang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'TJS', '天津', 'Tianjing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'XZQ', '西藏', 'Tibet', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'XJQ', '新疆', 'Xinjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'YNS', '云南', 'Yunnan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'ZJS', '浙江', 'Zhejiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'CQS', '重庆', 'Chongqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'MOS', '澳门', 'Macao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'HKS', '香港', 'Hongkong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'P', 'TWS', '台湾', 'Taiwan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_code = 'CN') where area_type = 'P';

-- -----------------------------------------------------------------------------------------------------------------------------------------------
-- 安徽省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HFS', '合肥市', 'Hefei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WHS', '芜湖市', 'Wuhu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BBS', '蚌埠市', 'Bengbu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HNS', '淮南市', 'Huainan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'MAS', '马鞍山市', 'Maanshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HBS', '淮北市', 'Huaibei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TLS', '铜陵市', 'Tongling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'AQS', '安庆市', 'Anqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HSS', '黄山市', 'Huangshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TZS', '滁州市', 'Chuzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'FYS', '阜阳市', 'Fuyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SZS', '宿州市', 'Suzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LAS', '六安市', 'Liuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BZS', '亳州市', 'Bozhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CZS', '池州市', 'Chizhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XCS', '宣城市', 'Xuancheng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '安徽') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SSQ', '蜀山区', 'Shushan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHQ', '瑶海区', 'Yaohai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYQ', '庐阳区', 'Luyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BHQ', '包河区', 'Baohe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CFX', '长丰县', 'Changfeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FDX', '肥东县', 'Feidong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FXX', '肥西县', 'Feixi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LJX', '庐江县', 'Lujiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CHS', '巢湖市', 'Chaohushi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '合肥市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JHQ', '镜湖区', 'Jinghu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJQ', '弋江区', 'Yijiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JJQ', '鸠江区', 'Jiujiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SSQ', '三山区', 'Sanshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WHX', '芜湖县', 'Wuhu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NLX', '南陵县', 'Nanling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FCX', '繁昌县', 'Fanchang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WWX', '无为县', 'Wuwei', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '芜湖市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BSQ', '蚌山区', 'Bengshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZH', '龙子湖区', 'Longzihu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHQ', '禹会区', 'Yuhui District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSQ', '淮上区', 'Huaishang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYX', '怀远县', 'Huaiyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GZX', '固镇县', 'Guzhen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WHX', '五河县', 'Wuhe', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '蚌埠市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DTX', '大通区', 'Datong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TJA', '田家庵区', 'Tianjianan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XJJ', '谢家集区', 'Xiejiaji District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BGS', '八公山区', 'Bagongshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PJQ', '潘集区', 'Panji District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FTX', '凤台县', 'Fengtai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXX', '寿县', 'Shouxian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '淮南市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSQ', '雨山区', 'Yushan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSQ', '花山区', 'Huashan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BWQ', '博望区', 'Bowang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DTX', '当涂县', 'Dangtu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSX', '含山县', 'Hanshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HXX', '和县', 'Hexian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '马鞍山市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSQ', '相山区', 'Xiangshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DJQ', '杜集区', 'Duji District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSQ', '烈山区', 'Lieshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXX', '濉溪县', 'Shuixi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '淮北市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TGQ', '铜官区', 'Tongguan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JQQ', '郊区', 'Outskirts', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YAQ', '义安区', 'Yian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYX', '枞阳县', 'Zongyang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '铜陵市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJQ', '迎江区', 'Yingjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DGQ', '大观区', 'Daguan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXQ', '宜秀区', 'Yixiu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TCS', '桐城市', 'Tongcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SSX', '宿松县', 'Susong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'THX', '太湖县', 'Taihu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HNX', '怀宁县', 'Huaining', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '岳西县', 'Yuexi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WJX', '望江县', 'Wangjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QCS', '潜山市', 'Qianshan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '安庆市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TXQ', '屯溪区', 'Tuxi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSQ', '黄山区', 'Huangshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZQ', '徽州区', 'Huizhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XNX', '休宁县', 'Xiuning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXX', '歙县', 'Shexian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QMX', '祁门县', 'Qimen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '黟县', 'Yixian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '黄山市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYQ', '琅琊区', 'Langya District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NQQ', '南谯区', 'Nanqiao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TCS', '天长市', 'Tianchang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MGS', '明光市', 'Mingguang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QJX', '全椒县', 'Quanjiao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LAX', '来安县', 'Laian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DYX', '定远县', 'DingYuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FYX', '凤阳县', 'Fengyang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '滁州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZQ', '颍州区', 'Yingzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YDQ', '颍东区', 'Yingdong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YQQ', '颍泉区', 'Yingquan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JSS', '界首市', 'Jieshou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LQX', '临泉县', 'Linquan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSX', '颍上县', 'Yingshang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FNX', '阜南县', 'Funan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'THX', '太和县', 'Taihe', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '阜阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YQQ', '埇桥区', 'Yongqiao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXX', '萧县', 'Xiaoxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXX', '泗县', 'Sixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DSX', '砀山县', 'Dangshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LBX', '灵璧县', 'Lingbi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '宿州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JAQ', '金安区', 'Jinan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YAQ', '裕安区', 'Yuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJQ', '叶集区', 'Yeji District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSX', '霍山县', 'Huoshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HQX', '霍邱县', 'Huoqiu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCX', '舒城县', 'Shucheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZX', '金寨县', 'Jinzhai', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '六安市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QCQ', '谯城区', 'Qiaocheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '利辛县', 'Lixin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GYX', '涡阳县', 'Guoyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MCX', '蒙城县', 'Mengcheng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '亳州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GCQ', '贵池区', 'Guichi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DZX', '东至县', 'Dongzhi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'STX', '石台县', 'Shitai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QYX', '青阳县', 'Qingyang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '池州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XZQ', '宣州区', 'Xuanzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NGS', '宁国市', 'Ningguo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GDX', '广德县', 'Guangde', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '郎溪县', 'Langxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '泾县', 'Jingxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JDX', '旌德县', 'Jingde', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JX#', '绩溪县', 'Jixi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '宣城市') where parent_area_id = -1;

-- ------------------------------------------------------------------------------------------------------------------------------------------------------------
-- 北京市
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DCQ', '东城区', 'Dongcheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XCQ', '西城区', 'Xicheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CYQ', '朝阳区', 'Caoyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'FTQ', '丰台区', 'Fengtai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SJS', '石景山区', 'Shijingshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HDQ', '海淀区', 'Haidian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'MTG', '门头沟区', 'Mentougou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'FSQ', '房山区', 'Fangshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TZQ', '通州区', 'Tongzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SYQ', '顺义区', 'Shunyi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CPQ', '昌平区', 'Changping District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DXQ', '大兴区', 'Daxing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HRQ', '怀柔区', 'Huairou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'MYQ', '密云区', 'Miyun District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'PGQ', '平谷区', 'Pinggu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YQQ', '延庆区', 'Yanqing District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '北京') where parent_area_id = -1;

-- ------------------------------------------------------------------------------------------------------------------------------------------------------------
-- 福建省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'FZS', '福州市', 'Fuzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XMS', '厦门市', 'Xiamen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'PTS', '莆田市', 'Putian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SMS', '三明市', 'Sanming', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QZS', '泉州市', 'Quanzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZZS', '漳州市', 'Zhangzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NPS', '南平市', 'Nanping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LYS', '龙岩市', 'Longyan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NDS', '宁德市', 'Ningde', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'PTQ', '平潭综合实验区', 'Pingtan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '福建') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XZQ', '鼓楼区', 'Gulong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NGQ', '台江区', 'Taijiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GDQ', '仓山区', 'Cangshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXQ', '马尾区', 'Mawei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JAQ', '晋安区', 'Jinan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JDQ', '长乐区', 'Changle District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXQ', '福清市', 'Fuqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NGX', '闽侯县', 'Minhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GDX', '连江县', 'Lianjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '罗源县', 'Luoyan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MQX', '闽清县', 'Minqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JDX', '永泰县', 'Yongtai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '平潭县', 'Pingtan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '福州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SMQ', '思明区', 'Siming District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCQ', '海沧区', 'Haicang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLQ', '湖里区', 'huli District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JMQ', '集美区', 'Jimei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TAQ', '同安区', 'Tongan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XAQ', '翔安区', 'Xiangan District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '厦门市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CXQ', '城厢区', 'Chengxiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJQ', '涵江区', 'Hanjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCQ', '荔城区', 'Licheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYQ', '秀屿区', 'Xiuyu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYC', '仙游县', 'Xianyou', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '莆田市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLQ', '梅列区', 'Meilie District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYQ', '三元区', 'Sanyuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YAS', '永安市', 'Yongan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MXX', '明溪县', 'Mingxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QLX', '清流县', 'Qingliu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NHX', '宁化县', 'Ninghua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DTX', '大田县', 'Datian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '尤溪县', 'Youxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXX', '沙县', 'Shaxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JLX', '将乐县', 'Jiangle', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TNX', '泰宁县', 'Taining', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JNX', '建宁县', 'Jianning', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '三明市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCQ', '鲤城区', 'Licheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FZQ', '丰泽区', 'Fengze District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LJQ', '洛江区', 'Luojiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QGQ', '泉港区', 'Quangang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SSS', '石狮市', 'Shishi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JJS', '晋江市', 'Jinjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NAS', '南安市', 'Nanan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HAS', '惠安县', 'Huian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AXS', '安溪县', 'Anxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCS', '永春县', 'Yongchun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DHS', '德化县', 'Dehua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JMS', '金门县', 'Jinmen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QMS', '清濛开发区', 'Qingmeng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '泉州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCQ', '芗城区', 'Xiangcheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LWQ', '龙文区', 'Longwen District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHS', '龙海市', 'Longhai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '云霄县', 'Yunxiao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZPX', '漳浦县', 'Zhangpu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZAX', '诏安县', 'Zhaoan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CTX', '长泰县', 'Changtai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DSX', '东山县', 'Dongshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NJX', '南靖县', 'Nanjing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PHX', '平和县', 'Pinghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HAX', '华安县', 'Huaan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '漳州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYQ', '建阳区', 'Jianyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SWQ', '邵武市', 'Shaowu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WYS', '武夷山市', 'Wuyishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JOS', '建瓯市', 'Jianou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCX', '顺昌县', 'Shunchang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PCX', '蒲城县', 'Pucheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GZX', '光泽县', 'Guangze', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXX', '松溪县', 'Songxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZHX', '政和县', 'Zhenghe', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '南平市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XLQ', '新罗区', 'Xinluo District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YDQ', '永定区', 'Yongding District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZPS', '漳平市', 'Zhangping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CDX', '长汀县', 'Changding', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHX', '上杭县', 'Shanghang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WPX', '武平县', 'Wuping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '连城县', 'Lianchneg', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '龙岩市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JCQ', '蕉城区', 'Jiaocheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WAS', '福安市', 'Fuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FDS', '福鼎市', 'Fuding', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XPX', '霞浦县', 'Xiapu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GTX', '古田县', 'Gutian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PNX', '屏南县', 'Pingnan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SNX', '寿宁县', 'Shouning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZNX', '周宁县', 'Zhouning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TRX', '拓荣县', 'Tuorong', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '宁德市') where parent_area_id = -1;

-- -------------------------------------------------------------------------------------------------------------
-- 甘肃省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LZS', '兰州市', 'Lanzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JYG', '嘉峪关市', 'jiayuguan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JQS', '酒泉市', 'Jiuquan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZYS', '张掖市', 'Zhangye', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JCS', '金昌市', 'Jincang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WWS', '武威市', 'Wuwei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BYS', '白银市', 'Baiying', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DXS', '定西市', 'Dingxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TSS', '天水市', 'Tianshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'PLS', '平凉市', 'Pingliang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QYS', '庆阳市', 'Qingyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LNS', '陇南市', 'Longnan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LXZ', '临夏州', 'linxia', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'GNZ', '甘南州', 'Gannan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '甘肃') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JCQ', '城关区', 'Chengguan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QLH', '七里河区', 'Qilihe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XGQ', '西固区', 'Xigu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ANQ', '安宁区', 'Anning District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HGQ', '红古区', 'Honggu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YDX', '永登县', 'Yongdeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GLX', '皋兰县', 'Gaolan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZX', '榆中县', 'Yuzong', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '兰州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JCQ', '金川区', 'Jinchuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCX', '永昌县', 'Yongchang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '金昌市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BYQ', '白银区', 'Baiyin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PCQ', '平川区', 'Pingchuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYX', '靖远县', 'Jinyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HNX', '会宁县', 'Huining', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JTX', '景泰县', 'Jintai', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '白银市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TZQ', '泰州区', 'Taizhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MJQ', '麦积区', 'Maiji District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QSX', '清水县', 'Qingshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QAX', '秦安县', 'Qinan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GGX', '甘谷县', 'Gangu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WSX', '武山县', 'Wushan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZJC', '张家川县', 'Zhangjiachuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '天水市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZQ', '凉州区', 'Liangzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MQX', '民勤县', 'Minqin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GLX', '古浪县', 'Gulang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TZX', '天祝县', 'Tianzhu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '武威市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GZQ', '甘州区', 'Ganzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SNX', '肃南县', 'Shunan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLX', '民乐县', 'Minle', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZX', '临泽县', 'Linze', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GTX', '高台县', 'Gaotai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SDX', '山丹县', 'Shandan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '张掖市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KTQ', '崆峒区', 'Kongtong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JCX', '泾川县', 'Jinchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LTX', '灵台县', 'Lintai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CXX', '崇信县', 'Chongxin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HTX', '华亭县', 'Huating', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZLX', '庄浪县', 'Zhuanglang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JNX', '静宁县', 'Shandan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '平凉市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SZQ', '肃州区', 'Suzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JTX', '金塔县', 'Jinta', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GZX', '瓜州县', 'Guazhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SBX', '肃北县', 'Subei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ASK', '阿塞克县', 'Asaike', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YMS', '玉门市', 'Yumen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DHS', '敦煌市', 'Dunhuang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '酒泉市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XFQ', '西峰区', 'Xifeng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QCX', '庆城县', 'Qingcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HXX', '环县', 'Huanxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCX', '华池县', 'Huachi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSX', '合水县', 'heshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZNX', '正宁县', 'Zhengning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NXX', '宁县', 'Ningxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYX', '镇原县', 'Zhenyuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '庆阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ADQ', '安定区', 'Anding District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TWX', '通渭县', 'Tongwei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '陇西县', 'Longxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WYX', '渭源县', 'Weiyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LTX', '临洮县', 'Lintao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZXX', '漳县', 'Zhangxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MXX', '岷县', 'Minxian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '定西市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WDQ', '武都区', 'Wudu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CXX', '成县', 'Chengxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WXX', '文县', 'Wenxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DCX', '宕昌县', 'Dangcang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KXX', '康县', 'Kangxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHX', '西和县', 'Xihe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '礼县', 'Lixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HXX', '徽县', 'Huixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LDX', '两当县', 'Liangdangxian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '陇南市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXS', '临夏市', 'Linxia District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '临夏县', 'Linxia', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KLX', '康乐县', 'Kangle', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJX', '永靖县', 'Yongjing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GHX', '广河县', 'Guanghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZX', '和政县', 'Hezheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DXX', '东乡县', 'Dongxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JSS', '积石山县', 'Jishishan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '临夏州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZS', '合作市', 'Hezuo District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LTX', '临潭县', 'Lintan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZNX', '卓尼县', 'Zhuini', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZQX', '舟曲县', 'Zhouqu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DBX', '迭部县', 'diebu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MQX', '玛曲县', 'Maqu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LQX', '碌曲县', 'Luqu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHX', '夏河县', 'Xiahe', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '甘南州') where parent_area_id = -1;

-- -------------------------------------------------------------------------------------------------------------
-- 广东省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'GZS', '广州市', 'Guangzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SZS', '深圳市', 'Shenzhen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZHS', '珠海市', 'Zhuhai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'STS', '汕头市', 'Shantou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'FSS', '佛山市', 'Foshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SGS', '韶关市', 'Shaoguan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZJS', '湛江市', 'Zhanjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZQS', '肇庆市', 'Zhaoqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JMS', '江门市', 'Jiangmen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'MMS', '茂名市', 'Maoming', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HZS', '惠州市', 'Huizhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'MZS', '梅州市', 'Meizhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SWS', '汕尾市', 'Shanwei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HYS', '河源市', 'heyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YJS', '阳江市', 'Yanjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QYS', '清远市', 'Qingyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DGS', '东莞市', 'Dongguan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZSS', '中山市', 'Zhongshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CZS', '潮州市', 'Chaozhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JYS', '揭阳市', 'Jieyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YFS', '云浮市', 'Yunfu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '广东') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXQ', '越秀区', 'Yuexiu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LWQ', '荔湾区', 'Liwan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZQ', '海珠区', 'Haizhu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'THQ', '天河区', 'Tianhe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BYQ', '白云区', 'Baiyun District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HPQ', '黄浦区', 'Huangpu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HDQ', '花都区', 'Huadu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PYQ', '番禺区', 'Panyu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NSQ', '南沙区', 'nansha District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZCQ', '增城区', 'Zengcheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CHQ', '从化区', 'Conghua District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '广州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FTQ', '福田区', 'Futian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHQ', '罗湖区', 'Luohu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NSQ', '南山区', 'Nanshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YTQ', '盐田区', 'Yantian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BAQ', '宝安区', 'Baoan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LGQ', '龙岗区', 'Longgang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LH#', '龙华区', 'Longhua District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PSQ', '坪山区', 'Pingshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GMQ', '光明区', 'GuangmingDistrict', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '深圳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XZQ', '香洲区', 'Xiangzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DMQ', '斗门区', 'Doumen District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JWQ', '金湾区', 'Jinwan District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '珠海市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JPQ', '金平区', 'Jinping District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHQ', '龙湖区', 'Longhu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJQ', '濠江区', 'haojiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CNQ', '潮南区', 'Chaonan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYQ', '潮阳区', 'Chaoyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CHQ', '澄海区', 'Chenghai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NAQ', '南澳县', 'Nanao', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '汕头市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CCQ', '禅城区', 'ChanCheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GMQ', '高明区', 'Gaoming District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SSQ', '山水区', 'Shanshui District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NHQ', '南海区', 'Nanhai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SDQ', '顺德区', 'Shunde District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '佛山市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZJQ', '浈江区', 'Zhenjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WJQ', '武江区', 'Wujiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QJQ', '曲江区', 'Qujiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCS', '乐昌市', 'Lecang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NXS', '南雄市', 'Nanxiong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXX', '始兴县', 'Shixing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RHX', '仁化县', 'Renhua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WYX', '翁源县', 'Wengyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XFX', '新丰县', 'Xinfeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RYX', '乳源县', 'Ruyuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '韶关市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CKQ', '赤坎区', 'Chikan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSQ', '霞山区', 'Xiashan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PTQ', '坡头区', 'Potou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MZQ', '麻章区', 'Mazhang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WCS', '吴川市', 'Wuchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZS', '雷州市', 'Leizhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LJS', '廉江市', 'Lianjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXX', '遂溪县', 'Suixi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XWX', '徐闻县', 'Xuwen', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '湛江市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DZQ', '端州区', 'Duanzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DHQ', '鼎湖区', 'Dinghu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GYQ', '高要区', 'Gaoyao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHS', '四会市', 'Sihui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GNX', '广宁县', 'Guangning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJX', '怀集县', 'Huaiji', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FKX', '封开县', 'fengkai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DQX', '德庆县', 'Deqing', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '肇庆市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZJQ', '江海区', 'Haijiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WJQ', '蓬江区', 'Pengjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QJQ', '新会区', 'Xinhui District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCS', '台山市', 'Taishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NXS', '鹤山市', 'Heshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXS', '开平市', 'Kaiping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RHS', '恩平市', 'Enping', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '江门市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MNQ', '茂南区', 'Maonan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DBQ', '电白区', 'Dianbai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GZS', '高州市', 'Gaozhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZS', '化州市', 'Huazhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYS', '信宜市', 'Xinyi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '茂名市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCQ', '惠城区', 'Huicheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYQ', '惠阳区', 'Huiyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BLX', '博罗县', 'Boluo District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LMX', '龙门县', 'Longmen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HDX', '惠东县', 'Huidong', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '惠州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MJQ', '梅江区', 'Meijiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MXQ', '梅县区', 'Meixian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XNQ', '兴宁市', 'Xingning District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DBX', '大埔县', 'Dabu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FSX', '丰顺县', 'fengshun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WHX', '五华县', 'Wuhua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PYX', '平远县', 'Pingyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JLX', '蕉岭县', 'Jiaoling', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '梅州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CQQ', '城区', 'Chengqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LFS', '陆丰市', 'Lufeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HFX', '海丰县', 'Haifeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHX', '陆河县', 'Luhe', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '汕尾市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCQ', '源城区', 'Yuancheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '龙川县', 'Longchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LPX', '连平县', 'Lianping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DYX', '东源县', 'Dongyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HPX', '和平县', 'Heping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZJX', '紫金县', 'Zijin', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '河源市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JCQ', '江城区', 'Jiangcheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YDQ', '阳东区', 'Yangdong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCS', '阳春市', 'Yangchun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '阳西县', 'Yangxi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '阳江市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QCQ', '清城区', 'Qingcheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXQ', '清新区', 'Qingxin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YDS', '英德市', 'yingde', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZS', '连州市', 'Lianzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FGS', '佛冈县', 'Fogang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSS', '阳山县', 'Yangshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSS', '连山县', 'Lianshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LNS', '连南县', 'Liannan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '清远市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DCS', '东城街道', 'Dongcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NCS', '南城街道', 'Nancheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WJS', '万江街道', 'Wanjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GCS', '莞城街道', 'Guancheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SJZ', '石碣镇', 'Shijie', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SLZ', '石龙镇', 'Shilong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CSZ', '茶山镇', 'Chahshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PSZ', '石排镇', 'Shipai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QSZ', '企石镇', 'Qishi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLZ', '横沥镇', 'hengli', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DKZ', '东坑镇', 'Dongkang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CPZ', '常平镇', 'Changping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LBZ', '寮步镇', 'Liaobu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZMT', '樟木头镇', 'Zhangmutou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DLZ', '大朗镇', 'Dalang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJZ', '黄江镇', 'Huangjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXZ', '清溪镇', 'Qingxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TXZ', '塘厦镇', 'tangxia', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FGZ', '凤岗镇', 'Fenggang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DLS', '大岭山镇', 'Dalingshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CAZ', '长安镇', 'Changan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HMZ', '虎门镇', 'Fumen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJ#', '厚街镇', 'Houjie', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'STZ', '沙田镇', 'Shatian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DJZ', '道滘镇', 'Daojiao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HM#', '洪梅镇', 'Hongmei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MYZ', '麻涌镇', 'Machong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WNX', '望牛墩镇', 'Wanniudun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZTZ', '中堂镇', 'Zhongtang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GBZ', '高埗镇', 'Gaobu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '东莞市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SQQ', '石岐区', 'Shiqiqu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DQQ', '东区', 'Dongqu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XQQ', '西区', 'Xiqu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NQQ', '南区', 'Nanqu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WGS', '五桂山', 'Wuguishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XLZ', '小榄镇', 'Xiaolan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HPZ', '黄圃镇', 'Huangpu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MZZ', '民众镇', 'Minzhong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DFZ', '东凤镇', 'Dongfeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DSZ', '东升镇', 'Dongsheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GZZ', '古镇镇', 'Guzhen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXZ', '沙溪镇', 'Shaxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TZZ', '坦洲镇', 'Tanzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GKZ', '港口镇', 'Gangkou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SJZ', '三角镇', 'Sanjiao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLZ', '横栏镇', 'henglan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NTZ', '南头镇', 'Nantou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FSZ', '阜沙镇', 'Fusha', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NLZ', '南朗镇', 'Nanlang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SX#', '三乡镇', 'Sanxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BFZ', '板芙镇', 'Banfu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DYZ', '大涌镇', 'Dayong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SWZ', '神湾镇', 'Shenwan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '中山市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XQQ', '湘桥区', 'Xiangqiao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CAQ', '潮安区', 'Chaoan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RPX', '饶平县', 'Raoping', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '潮州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RCQ', '榕城区', 'Rongcheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JDQ', '揭东区', 'Jieodng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '揭西县', 'Jiexi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLX', '惠来县', 'Huilai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PNS', '普宁市', 'Puning', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '揭阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCQ', '云城区', 'Yuncheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXX', '新兴县', 'Xinxing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YNX', '郁南县', 'Yunan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YAX', '云安县', 'Yunan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LDS', '罗定市', 'Luoding', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '云浮市') where parent_area_id = -1;

-- --------------------------------------------------------------------------------------------------------------------------------------------------------
-- 广西
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NNS', '南宁市', 'Nanning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LZS', '柳州市', 'Liuzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'GLS', '桂林市', 'Guilin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WZS', '梧州市', 'Wuzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BHS', '北海市', 'Beihai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'FCG', '防城港市', 'Fangchenggang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QZS', '钦州市', 'Qinzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'GGS', '贵港市', 'Guigang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YLS', '玉林市', 'Yulin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BSS', '百色市', 'Baise', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HZS', '贺州市', 'Hezhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HCS', '河池市', 'Hechi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LBS', '来宾市', 'Laibin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CZS', '崇左市', 'Congzuo', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '广西') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXQ', '青秀区', 'Qingxiu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XNQ', '兴宁区', 'Xingning District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXT', '西乡塘区', 'Xixiangtang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LQQ', '良庆区', 'Liangqing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JNQ', '江南区', 'Jiangnan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YNQ', '邕宁区', 'Yongning District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WMQ', '武鸣区', 'Wuming District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LAX', '隆安县', 'Longan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MSX', '马山县', 'Mahsan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SLX', '上林县', 'Shanglin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BYX', '宾阳县', 'Binyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HXX', '横县', 'Hengxian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '南宁市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FTQ', '城中区', 'Futian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHQ', '鱼峰区', 'Luohu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NSQ', '柳北区', 'Nanshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YTQ', '柳南区', 'Yantian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BAQ', '柳江区', 'Baoan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '柳城县', 'Liucheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZX', '鹿寨县', 'Luzai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RAX', '融安县', 'Rongan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RSX', '融水县', 'Rongshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SJX', '三江县', 'Sanshui', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '柳州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSQ', '象山区', 'Xiangshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XFQ', '秀峰区', 'Xiufeng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DCQ', '叠彩区', 'Diecai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXQ', '七星区', 'Qixing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSQ', '雁山区', 'Yanshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LGQ', '临桂区', 'Lingui District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSX', '阳朔县', 'Yangshuo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '灵川县', 'Lingchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QZX', '全州县', 'Quanzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LPX', '乐平县', 'leping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XAX', '兴安县', 'Xingan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GYX', '灌阳县', 'Guanyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LP#', '荔浦县', 'Lipu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYX', '资源县', 'Ziyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YFX', '永福县', 'Yongfu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '龙胜县', 'Longshen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GCX', '恭城县', 'Gongcheng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '桂林市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WXQ', '万秀区', 'Wanxiu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYQ', '龙圩区', 'Longxu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CZQ', '长洲区', 'Changzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CXS', '岑溪市', 'Cenxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CWX', '苍梧县', 'Cangwu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TXX', '藤县', 'Tengxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NSX', '蒙山县', 'Mengshan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '梧州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCQ', '海城区', 'HaiCheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHQ', '银海区', 'Yinhai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TSG', '铁山港区', 'Tieshangang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HPX', '合浦县', 'Hepu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '北海市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GKQ', '港口区', 'Gangkou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FCQ', '防城区', 'Fangcheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DXS', '东兴市', 'Dongxing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SSX', '上思县', 'Shangxi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '防城港市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QNQ', '钦南区', 'Qinnan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QBQ', '钦北区', 'Qinbei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '灵山县', 'Lingshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PBX', '浦北县', 'Pubei', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '钦州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GBQ', '港北区', 'Gangbei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GNQ', '港南区', 'Gangnan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QTQ', '覃塘区', 'Qintang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GPS', '桂平市', 'Guiping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PNX', '平南县', 'Pingnan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '贵港市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZQ', '玉州区', 'Yuzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FMQ', '福绵区', 'Fumian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BLS', '北流市', 'Beiliu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RXX', '容县', 'Rongxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '陆川县', 'Luchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BBX', '博白县', 'Bobai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYX', '兴业县', 'Xingye', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '玉林市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJQ', '右江区', 'Youjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXS', '靖西市', 'Jingxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYX', '凌云县', 'Lingyun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PGX', '平果县', 'Pingguo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XLX', '西林县', 'Xilin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LY#', '乐业县', 'Leye', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DBX', '德保县', 'Debao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TLX', '田林县', 'Tuanlin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TYX', '田阳县', 'Tianyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TDX', '田东县', 'Tiandong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NPX', '那坡县', 'Napo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LLX', '隆林县', 'Longlin', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '百色市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BBQ', '八步区', 'Babu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PGQ', '平桂区', 'Pinggui District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZNX', '钟山县', 'Zhongshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZPX', '昭平县', 'Zhaoping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FCX', '富川县', 'Fuchuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '贺州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JCJ', '金城江区', 'Jinchengjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZS', '宜州市', 'Yizhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TEX', '天峨县', 'Tiane', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FSX', '凤山县', 'Fengshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NDX', '南丹县', 'nandan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DLX', '东兰县', 'Donglan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DAX', '都安县', 'Duan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '罗城县', 'Luocheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BMX', '巴马县', 'Bama', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJX', '环江县', 'Huanjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DHX', '大化县', 'Dahua', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '河池市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LBQ', '来宾区', 'Laibin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCS', '合川市', 'Hechuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XZX', '象州县', 'Xiangzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WXX', '武宣县', 'Wuxuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCX', '忻城县', 'Xincheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '金秀县', 'Jinxiu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '来宾市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZQ', '江州区', 'Jiangzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PXS', '凭祥市', 'Pingxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NMX', '宁明县', 'Ningming', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FSX', '扶绥县', 'Fusui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZX', '龙州县', 'Longzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DXX', '大新县', 'Daxin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TDX', '天等县', 'Tiandeng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '崇左市') where parent_area_id = -1;


-- -----------------------------------------------------------------------------------------------------------------------------------------------
-- 贵州省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'GYS', '贵阳市', 'Guiyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LPS', '六盘水市', 'Liupanshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZYS', '遵义市', 'Zunyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ASS', '安顺市', 'Anshun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BJS', '毕节市', 'Bijie', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TRS', '铜仁市', 'Tongren', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QXN', '黔西南州', 'Qianxinan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QDN', '黔东南州', 'Qiandongnan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QNZ', '黔南州', 'Qiannan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '贵州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NMQ', '南明区', 'Nanming District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YYQ', '云岩区', 'Yunyan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HXQ', '花溪区', 'Huaxi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WDQ', '乌当区', 'Wudang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BYQ', '白云区', 'Baiyun District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GSH', '观山湖区', 'Guanshanhu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KYX', '开阳县', 'Kaiyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XFX', '息烽县', 'Xifeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XWX', '修文县', 'Xiuwen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QZX', '清镇市', 'Qingzhen', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '贵阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZSQ', '钟山区', 'Zhongshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZQ', '六枝特区', 'Liuzhi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCX', '水城县', 'Shuicheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PXX', '盘县', 'Panxian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '六盘水市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HHG', '红花岗区', 'Honghuagang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCQ', '汇川区', 'Huichuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BZQ', '播州区', 'Bozhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TZX', '桐梓县', 'Tongzi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYX', '绥阳县', 'Shuiyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZAX', '正安县', 'Zhengan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DZX', '道真县', 'Daozhen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WCX', '务川县', 'Wuchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FGX', '凤冈县', 'Fenggang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MTX', '湄潭县', 'Meitan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YQX', '余庆县', 'Yuqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSX', '习水县', 'Xishui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CSS', '赤水市', 'Chishui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RHS', '仁怀市', 'Renhuai', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '遵义市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXQ', '西秀区', 'Xixiu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PBQ', '平坝区', 'Pingba District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PDX', '普定县', 'Puding', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZNX', '镇宁县', 'Zhenning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GLX', '关岭县', 'Guanling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYX', '紫云县', 'Ziyun', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '安顺市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXG', '七星关区', 'Qixingguan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DFX', '大方县', 'Dafang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXX', '黔西县', 'Qianxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JSX', '金沙县', 'Jinsha', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZX', '织金县', 'Zhijin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NYX', '纳雍县', 'Nayong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WNX', '威宁县', 'Weining', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZX', '赫章县', 'Hezhang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '毕节市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BJQ', '碧江区', 'Bijiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WSQ', '万山区', 'Wanshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JKX', '江口县', 'Jiangkou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YPX', '玉屏县', 'Yuping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SQX', '石阡县', 'Shiqian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SNX', '思南县', 'Sinan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJX', '印江县', 'Yinjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DJX', '德江县', 'Dejiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHX', '沿河县', 'Yanhe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'STX', '松桃县', 'Songtao', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '铜仁市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYS', '兴义市', 'Xingyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XRX', '兴仁县', 'Xingren', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PAX', '普安县', 'Puan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QLX', '晴隆县', 'Qinglong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZFX', '贞丰县', 'Zhenfeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WMX', '望谟县', 'Wanmo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CHX', '册亨县', 'CeHeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ALX', '安龙县', 'Anlong', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '黔西南州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KLS', '凯里市', 'Kaili', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HPX', '黄平县', 'Huangping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SDX', '施秉县', 'Shibing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHX', '三穗县', 'Sansui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYX', '镇远县', 'Zhenyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CGX', '岑巩县', 'Cengong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TZX', '天柱县', 'Tianzhu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JPX', '锦屏县', 'Jinping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JHX', '剑河县', 'Jianhe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TJX', '台江县', 'Taijiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PLX', '黎平县', 'Liping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RJX', '榕江县', 'Rongjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CJX', '从江县', 'Congjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '雷山县', 'Leishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MJX', '麻江县', 'Majiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DDX', '丹寨县', 'Danzhai', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '黔东南州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DYS', '都匀市', 'Duyun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FQS', '福泉市', 'Fuquan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LBX', '荔波县', 'Libo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GDX', '贵定县', 'Guiding', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WAX', '瓮安县', 'Wengan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DSX', '独山县', 'Dushan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PTX', '平塘县', 'Pingtang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LDX', '罗甸县', 'Luodian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CSX', '长顺县', 'Changshun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LLX', '龙里县', 'Longli', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSX', '惠水县', 'Huishui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SDX', '三都县', 'Sandu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '黔南州') where parent_area_id = -1;

-- -----------------------------------------------------------------------------------------------------------------------------------------------
-- 海南省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HKS', '海口市', 'Haikou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SYS', '三亚市', 'Sanya', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SSS', '三沙市', 'Sansha', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DZS', '儋州市', 'Danzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WZS', '五指山市', 'Wuzhishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QHS', '琼海市', 'Qionghai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WCS', '文昌市', 'Wenchang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WNS', '万宁市', 'Wanning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DFS', '东方市', 'Dongfang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DAX', '定安县', 'Dingan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TCX', '屯昌县', 'Tunchang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CMX', '澄迈县', 'Chengmai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LGX', '临高县', 'Lingao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BSX', '白沙县', 'Baisha', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CJX', '昌江县', 'Changjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LDX', '乐东县', 'Ledong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '陵水县', 'Lingshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BTX', '保亭县', 'Baoting', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QZX', '琼中县', 'Qiongzhong', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '海南') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYQ', '秀英区', 'Xiuying District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHQ', '龙华区', 'Longhua District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QSQ', '琼山区', 'Qiongshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLQ', '美兰区', 'Meilan District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '海口市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HTQ', '海棠区', 'Haitang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYQ', '吉阳区', 'Jiyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TYQ', '天涯区', 'Tianya District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZQ', '崖州区', 'Yazhou District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '三亚市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXD', '永兴岛', 'Yongxing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DDD', '东岛', 'Dongdao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYD', '金银岛', 'Jinyin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHD', '珊瑚岛', 'Shanhu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CHD', '琛航岛', 'Chenhang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZJD', '中建岛', 'Zhongjian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZSD', '赵述岛', 'Zhaoshu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GQD', '甘泉岛', 'Ganquan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BDD', '北岛', 'Beidao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZDD', '中岛', 'Zhongdao', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '三沙市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NDZ', '那大镇', 'Nada', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NFZ', '南丰镇', 'Nanfeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXZ', '雅星镇', 'Yaxing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HQZ', '和庆镇', 'Heqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DCZ', '大成镇', 'Dacheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XZZ', '新州镇', 'Xinzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GCZ', '光村镇', 'Guangcun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DC#', '东成镇', 'Dongcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'EMZ', '峨蔓镇', 'Eman', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYZ', '兰洋镇', 'Lanyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WWZ', '王五镇', 'Wangwu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HPZ', '排浦镇', 'Paipu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HTZ', '海头镇', 'Haitou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MTZ', '木棠镇', 'Mutang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BMJ', '白马井镇', 'Baimajing', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '儋州市') where parent_area_id = -1;

-- -----------------------------------------------------------------------------------------------------------------------------------------------------
-- 河北省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SJZ', '石家庄市', 'Shijiazhuang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TSS', '唐山市', 'Tangshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QHD', '秦皇岛市', 'Qinhuangdao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HDS', '邯郸市', 'Handan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XTS', '邢台市', 'Xingtai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BDS', '保定市', 'Baoding', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZJK', '张家口市', 'Zhangjiakou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CDS', '承德市', 'Chengde', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CZS', '沧州市', 'Cangzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LFS', '廊坊市', 'Langfang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SHS', '衡水市', 'Hengshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XAQ', '雄安新区', 'Xiongan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '河北') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CAQ', '长安区', 'Changan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXQ', '桥西区', 'Qiaoxi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHQ', '新华区', 'Xinhua District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXK', '井陉矿区', 'Jingxingkuang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHQ', '裕华区', 'Yuhua District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GCQ', '藁城区', 'Gaocheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LQQ', '鹿泉区', 'Luquan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCQ', '栾城区', 'Luancheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XJS', '辛集市', 'Xinji', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZS', '晋州市', 'Jinzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XLS', '新乐市', 'Xinle', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PSX', '平山县', 'Pingshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '井陉县', 'Jingxing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZDX', '正定县', 'Zhengding', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XTX', '行唐县', 'Xingtang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '灵寿县', 'Lingshou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GYX', '高邑县', 'Gaoyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZXX', '赵县', 'Zhaoxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZHX', '赞皇县', 'Zanhuang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SZX', '深泽县', 'Shenze', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WJX', '无极县', 'Wuji', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSX', '元氏县', 'Yuanshi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '石家庄市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LBQ', '路北区', 'Lubei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LNQ', '路南区', 'Lunan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GZQ', '古治区', 'Guzhi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KPQ', '开平区', 'Kaiping District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FNQ', '丰南区', 'Fengnan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FRQ', '丰润区', 'fengrun District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CFD', '曹妃甸区', 'Caofeidian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZHS', '遵化市', 'Zunhua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QAS', '迁安市', 'Qianan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXX', '迁西县', 'Qianxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LNX', '滦南县', 'Luannan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YTX', '玉田县', 'Yutian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LTX', '乐亭县', 'Laoting', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '滦县', 'Luanxian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '唐山市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HGQ', '海港区', 'Haigang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHG', '山海关区', 'Shanhaiguan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BDH', '北戴河区', 'Beidaihe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FNQ', '抚宁区', 'Funing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CLX', '昌黎县', 'Changli', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LLX', '卢龙县', 'Lulong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QLX', '青龙县', 'Qinglog', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '秦皇岛市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CTQ', '丛台区', 'Congtai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FXQ', '复兴区', 'Fuxing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSQ', '邯山区', 'Hanshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YNQ', '永年区', 'Yongnian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FX#', '肥乡区', 'Feixiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FFK', '峰峰矿区', 'fengfengkuang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WAS', '武安市', 'Wuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QZX', '曲周县', 'Quzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GTX', '馆陶县', 'Guantao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WXX', '魏县', 'Weixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CAX', '成安县', 'Chengan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DMX', '大名县', 'Daming', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXX', '涉县', 'Shexian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZX', '鸡泽县', 'Jize', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXX', '邱县', 'Qiuxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GPX', '广平县', 'Guangping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZX', '临漳县', 'Linzhang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CXX', '磁县', 'Cixian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '邯郸市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QDQ', '桥东区', 'Qiaodong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXQ', '桥西区', 'Qiaoxi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NGS', '南宫市', 'Nangong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHS', '沙河市', 'Shahe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XTX', '邢台县', 'Xingtai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BXX', '柏乡县', 'Baixiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RXX', '任县', 'Renxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QHX', '清河县', 'Qinghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NJX', '宁晋县', 'Ningjin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WXX', '威县', 'Weixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYX', '隆尧县', 'Longyao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '临城县', 'Lincheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GZX', '广宗县', 'Guangzong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '临西县', 'Linxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NQX', '内丘县', 'Neiqiu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PXX', '平乡县', 'Pingxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JLX', '巨鹿县', 'Julu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHX', '新河县', 'Xinhe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NHX', '南和县', 'Nanhe', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '邢台市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXQ', '竞秀区', 'Jinxiu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCQ', '莲池区', 'Lianci District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QYQ', '清苑区', 'Qingyuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSQ', '徐水区', 'Xushui District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MCQ', '满城区', 'Mancheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZZS', '涿州市', 'Zhuozhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DZS', '定州市', 'Dingzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AGS', '安国市', 'Anguo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GBD', '高碑店市', 'Gaobeidian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '涞水县', 'Laishui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FPX', '阜平县', 'Fuping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DXX', '定兴县', 'Dingxing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TXX', '唐县', 'Tangxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GYX', '高阳县', 'Gaoyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYX', '涞源县', 'Laiyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WDX', '望都县', 'Wandu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '易县', 'Yixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QYX', '曲阳县', 'Quyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '蠡县', 'Lixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SPX', '顺平县', 'Shunping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BYX', '博野县', 'Boye', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '保定市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXQ', '桥西区', 'Qiaoxi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QDQ', '桥东区', 'Qiaodong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHQ', '宣化区', 'Xuanhua District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHY', '下花园区', 'Xiahuayuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CLQ', '崇礼区', 'Chongli District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WQQ', '万全区', 'Wanquan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KBX', '康保县', 'Kangbao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZBX', '张北县', 'Zhangbei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YYX', '阳原县', 'Yangyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CCX', '赤城县', 'Chicheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GYX', '沽源县', 'Guyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HAX', '怀安县', 'huaian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLX', '怀来县', 'Huailai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYX', '尚义县', 'Shangyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '蔚县', 'Yuxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZLX', '涿鹿县', 'Zhuolu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '张家口市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SQQ', '双桥区', 'Shuangqiao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SLQ', '双滦区', 'Shuangluan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSY', '鹰手营子矿区', 'Yinshouyingzikuang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PQS', '平泉市', 'Pingquan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CDX', '承德县', 'Chengde', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XLX', '兴隆县', 'Xinglong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHX', '隆化县', 'Longhua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LPX', '滦平县', 'Luanping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FNX', '丰宁县', 'fengning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WCX', '围场县', 'Weichang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KCX', '宽城县', 'Kuancheng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '承德市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHQ', '运河区', 'Yunhe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHQ', '新华区', 'Xinhua District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BTQ', '泊头区', 'Botou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RQQ', '任丘市', 'Renqiu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYS', '黄骅市', 'Huangye', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJS', '河间市', 'hejian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CXX', '沧县', 'Cangxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXX', '青县', 'Qingxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXX', '献县', 'Xianxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DGX', '东光县', 'Dongguang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HXX', '海兴县', 'Haixing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSX', '盐山县', 'Yanshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SNX', '肃宁县', 'Suning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NPX', '南皮县', 'nanpi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WQX', '吴桥县', 'Wuqiao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MCX', '孟村县', 'Mengcun', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '沧州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GYQ', '广阳区', 'Guangyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ACQ', '安次区', 'Anci District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BZQ', '霸州市', 'Bazhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHS', '三河市', 'Sanhe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GAX', '固安县', 'Guan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YQX', '永清县', 'Yongqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHX', '香河县', 'Xianghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DCX', '大城县', 'Dacheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WAX', '文安县', 'Wenan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DC#', '大厂县', 'Dachang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '廊坊市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TCQ', '桃城区', 'Taocheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZQ', '冀州区', 'Jizhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SZQ', '深州区', 'Shenzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RYX', '饶阳县', 'Raoyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZQX', '枣强县', 'Zaoqiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GCX', '故城县', 'Gucheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FCX', '阜城县', 'Fucheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'APX', '安平县', 'Anping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WYX', '武邑县', 'Wuyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '景县', 'Jingxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WQX', '武强县', 'Wuqiang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '衡水市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '安新县', 'Anxin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RCX', '容城县', 'Rongcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXX', '雄县', 'Xiongxian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '雄安新区') where parent_area_id = -1;

-- -----------------------------------------------------------------------------------------------------------------------------------------------------
-- 河南省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZZS', '郑州市', 'Zhengzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'KFS', '开封市', 'Kaifeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LYS', '洛阳市', 'Luoyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'PDS', '平顶山市', 'Pingdingshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'AYS', '安阳市', 'Anyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HBS', '鹤壁市', 'Hebi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XXS', '新乡市', 'Xinxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JZS', '焦作市', 'Jiaozuo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'PYS', '濮阳市', 'Puyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XCS', '许昌市', 'Xuchang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LHS', '漯河市', 'Luohe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SMX', '三门峡市', 'Sanmenxia', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NYS', '南阳市', 'Nanyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SQS', '商丘市', 'Shangqiu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XYS', '信阳市', 'Xinyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZKS', '周口市', 'Zhoukou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZMD', '驻马店市', 'Zhumadian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JYS', '济源市', 'Jiyuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '河南') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYQ', '中原区', 'Zhonguuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'EQQ', '二七区', 'Erqi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GCQ', '管城区', 'Guancheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JSQ', '金水区', 'Jinshui District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SJQ', '上街区', 'Shangjie District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJQ', '惠济区', 'Huiji District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZMX', '中牟县', 'Zhongmu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GYS', '巩义市', 'Gongyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYS', '荥阳市', 'Xingyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XMS', '新密市', 'Xinmi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XZS', '新郑市', 'Xinzheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DFS', '登封市', 'Dengfeng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '郑州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LTQ', '龙亭区', 'Longting District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHQ', '顺河区', 'Shunhe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YWT', '禹王台区', 'Yuwangtai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XFQ', '祥符区', 'Xiangfu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXX', '杞县', 'Qixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TXX', '通许县', 'Tongxu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WSX', '尉氏县', 'Weishi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LKX', '兰考县', 'Lankao', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '开封市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCQ', '老城区', 'Laocheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XGQ', '西工区', 'Xigong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CHQ', '瀍河区', 'Chanhe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXQ', '涧西区', 'Jianxi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JLQ', '吉利区', 'Jili District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LLQ', '洛龙区', 'Luolong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSS', '偃师市', 'Yanshi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MJX', '孟津县', 'Mengjin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XAX', '新安县', 'Xinan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '栾川县', 'Luanchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXX', '嵩县', 'Songxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RYX', '汝阳县', 'Ruyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YYX', '宜阳县', 'Yiyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LNX', '洛宁县', 'Luoning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCX', '伊川县', 'Yichuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '洛阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHQ', '新华区', 'Xinhua District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WDQ', '卫东区', 'Weidong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SLQ', '石龙区', 'Shilong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZHQ', '湛河区', 'Zhanhe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WGS', '舞钢市', 'Wugang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZS', '汝州市', 'Ruzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BFX', '宝丰县', 'Baofeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '叶县', 'Yexian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '鲁山县', 'Lushan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '郏县', 'Jiaxian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '平顶山市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WFQ', '文峰区', 'Wenfeng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BGQ', '北关区', 'Beiguan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YDQ', '殷都区', 'Yindu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LAQ', '龙安区', 'Longan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZS', '林州市', 'Linzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AYX', '安阳县', 'Anyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HXX', '滑县', 'Huaixan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NHX', '内黄县', 'Neihuang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TYX', '汤阴县', 'Tangyin', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '安阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSQ', '鹤山区', 'Heshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCQ', '山城区', 'Shancheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QBQ', '淇滨区', 'Qibin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXX', '浚县', 'Xunxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXX', '淇县', 'Qixian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '鹤壁市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HQQ', '红旗区', 'Hongqi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WBQ', '卫滨区', 'Weibin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FQQ', '凤泉区', 'Fengquan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MYQ', '牧野区', 'Muye District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WHS', '卫辉市', 'Weihui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXX', '新乡县', 'Xinxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJX', '获嘉县', 'Huojia', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YYX', '原阳县', 'Yanyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJX', '延津县', 'Yanjin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FQX', '封丘县', 'Fengqiu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYX', '长垣县', 'Changyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HXS', '辉县市', 'Huixian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '新乡市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JFQ', '解放区', 'Jiefang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZZQ', '中站区', 'Zhongzhan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MCQ', '马村区', 'Macun District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYQ', '山阳区', 'Shanyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QYS', '沁阳市', 'Qinyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MZS', '孟州市', 'Mengzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XWX', '修武县', 'Xiuwu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BAX', '博爱县', 'Boai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WZX', '武陟县', 'Wuzhi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WXX', '温县', 'Wenxian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '焦作市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLQ', '华龙区', 'Hualong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QFX', '清丰县', 'Qingfeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NLX', '南乐县', 'Nanle', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FX',  '范县',   'Fanxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TQX', '台前县', 'Taiqian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PYX', '濮阳县', 'Puyang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '濮阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WDQ', '魏都区', 'Weidu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JAQ', '建安区', 'Jianan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZS', '禹州市', 'Yuzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CGS', '长葛市', 'Changge', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLX', '鄢陵县', 'Yanling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XC#', '襄城县', 'Xiangcheng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '许昌市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHQ', '源汇区', 'Yuanhui District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCQ', '郾城区', 'Yancheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZLQ', '召陵区', 'Zhouling District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WYX', '舞阳县', 'Wuyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYX', '临颍县', 'Linying', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '漯河市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HBQ', '湖滨区', 'Hubin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YMS', '义马市', 'Yima', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LBS', '灵宝市', 'Lingbao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MCX', '渑池县', 'Mianchi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXX', '陕县', 'Xiaxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '卢氏县', 'Lushi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '三门峡市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WCQ', '宛城区', 'Wancheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WLQ', '卧龙区', 'Wolong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DZS', '邓州市', 'Dengzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NZX', '南召县', 'Nanzhao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FCX', '方城县', 'Fangcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXX', '西峡县', 'Xixia', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZPX', '镇平县', 'Zhenping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NXX', '内乡县', 'Neixiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZCX', '浙川县', 'Zhechuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SQX', '社旗县', 'Sheqi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'THX', '唐河县', 'Tanghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYX', '新野县', 'Xinye', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TBX', '桐柏县', 'Tongbai', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '南阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYQ', '梁园区', 'Liangyuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYQ', '睢阳区', 'Suiyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCS', '永城市', 'Yongcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MQX', '民权县', 'Minquan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXX', '睢县', 'Suixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NLX', '宁陵县', 'Ningling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TCX', '拓城县', 'Tuocheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCX', '虞城县', 'Yucheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYX', '夏邑县', 'Xiayi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '商丘市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHQ', '浉河区', 'Shihe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PQQ', '平桥区', 'Pingqiao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '罗山县', 'Luoshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GSX', '光山县', 'Guangshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXX', '新县', 'Xinxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCX', '商城县', 'Shangcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GS#', '固始县', 'Gushi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCX', '潢川县', 'Huangchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HBX', '淮滨县', 'Huaibin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XX#', '息县', 'Xixian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '信阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CHQ', '川汇区', 'Chuanhui District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCS', '项城市', 'Xiangcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FGX', '扶沟县', 'Fugou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHX', '西华县', 'Xihua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SSX', '商水县', 'Shangshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SQX', '沈丘县', 'Shenqiu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DCX', '郸城县', 'Dancheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYX', '淮阳县', 'Huaiyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TKX', '太康县', 'Taikang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYX', '鹿邑县', 'Luyi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '周口市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCQ', '驿城区', 'Yicheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XPX', '西平县', 'Xiping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCX', '上蔡县', 'Shangcai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PYX', '平舆县', 'Pingyu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYX', '正阳县', 'Zhengyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QSX', '确山县', 'Queshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QYX', '沁阳县', 'Qinyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RNX', '汝南县', 'Ruyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SPX', '遂平县', 'Shuiping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCX', '新蔡县', 'Xincai', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '驻马店市') where parent_area_id = -1;

-- -----------------------------------------------------------------------------------------------------------------------------------------------------
-- 黑龙江省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HEB', '哈尔滨市', 'Haerbin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QQH', '齐齐哈尔市', 'Qiqihaer', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'MDJ', '牡丹江市', 'Mudanjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JMS', '佳木斯市', 'Jiamusi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DQS', '大庆市', 'Daqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YCS', '伊春市', 'Yicun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JXS', '鸡西市', 'Jixi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HGS', '鹤岗市', 'Hegang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SYS', '双鸭山市', 'Shuangyashan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QTH', '七台河市', 'Qitaihe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SHS', '绥化市', 'Suihua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HHS', '黑河市', 'Heihe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DXA', '大兴安岭地区', 'Daxinganling', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '黑龙江') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DLQ', '道里区', 'Daoli District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NGQ', '南岗区', 'Nangang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DWQ', '道外区', 'Daowai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PFQ', '平房区', 'Pingfang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SBQ', '松北区', 'Songbei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XFQ', '香坊区', 'Xiangfang istrict', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLQ', '呼兰区', 'Hulan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ACQ', '阿城区', 'Acheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCQ', '双城区', 'Shuangcheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SZS', '尚志市', 'Shangzhi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WCS', '五常市', 'Wuchang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLX', '依兰县', 'Yilan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FZX', '方正县', 'Fangzheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BXX', '宾县', 'Binxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BYX', '巴彦县', 'Bayan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLX', '木兰县', 'Mulan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'THX', '通河县', 'Tonghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSX', '延寿县', 'Tanshou', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '哈尔滨市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSQ', '龙沙区', 'Longsha District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JHQ', '建华区', 'Jianhua District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TFQ', '铁锋区', 'Tiefeng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AAX', '昂昂溪区', 'Angangxi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FLE', '富拉尔基区', 'Fulaerji District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NZS', '碾子山区', 'Nianzishan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLS', '梅里斯区', 'Meilisi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NHS', '讷河市', 'Nehe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LJX', '龙江县', 'Longjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YAX', '依安县', 'Yian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TLX', '泰来县', 'Tailai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GLX', '甘南县', 'Gannan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FYX', '富裕县', 'Fuyu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KSX', '克山县', 'Keshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KDX', '克东县', 'Kedong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BQX', '拜泉县', 'Baiquan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '齐齐哈尔市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DAQ', '东安区', 'Dongan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YMQ', '阳明区', 'Yangming District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AMQ', '爱民区', 'Aimin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XAQ', '西安区', 'Xian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SFH', '绥芬河市', 'Suifenhe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLS', '海林市', 'Hailin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ANS', '安宁市', 'Anning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLS', '穆棱市', 'Muling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DNS', '东宁市', 'Dongning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '林口县', 'Linkou', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '牡丹江市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYQ', '向阳区', 'Xiangyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QJQ', '前进区', 'Qianjin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DFQ', '东风区', 'Dongfeng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JQQ', '郊区', 'Qiaoqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TJS', '同江市', 'Tongjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FJS', '富锦市', 'Fuyjin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HNX', '桦南县', 'Huannan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCX', '桦川县', 'Huachuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TYX', '汤原县', 'Tangyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FYS', '抚远市', 'Fuyuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '佳木斯市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SET', '萨尔图区', 'Saertu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LFQ', '龙凤区', 'Longfeng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RHL', '让胡路区', 'Ranghulu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HGQ', '红岗区', 'Honggang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DTQ', '大同区', 'Datong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZZX', '肇州县', 'Zhaozhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYX', '肇源县', 'Zhouyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LDX', '林甸县', 'Lindian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DEB', '杜尔伯特县', 'Duerbote', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '大庆市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCQ', '伊春区', 'Yichun District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NCQ', '南岔区', 'Nancha District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHQ', '友好区', 'Youhao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XLQ', '西林区', 'Xilin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CLQ', '翠峦区', 'Cuiluan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XQQ', '新青区', 'Xinqing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MXQ', '美溪区', 'Meixi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JST', '金山屯区', 'Jinshantun District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WYQ', '五营区', 'Wuying District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WMH', '乌马河区', 'Wumahe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TWH', '汤旺河区', 'Tangwanghe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DLQ', '带岭区', 'Dailing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WYL', '乌伊岭区', 'Wuyiling District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HXQ', '红星区', 'Hongxing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SGL', '上甘岭区', 'Shangganling District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TLS', '铁力市', 'Tieli', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYX', '嘉荫县', 'Jiayin', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '伊春市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JGQ', '鸡冠区', 'Jiguan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSQ', '恒山区', 'Hengshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DDQ', '滴道区', 'Didao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSQ', '梨树区', 'Lishu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CZH', '城子河区', 'Chengzihe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MSQ', '麻山区', 'Mashan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLS', '虎林市', 'Hulin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MSS', '密山市', 'Mishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JDX', '鸡东县', 'Jidong', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '鸡西市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYQ', '向阳区', 'Xiangyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GNQ', '工农区', 'Gongnong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NSQ', '南山区', 'Nanshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XAQ', '兴安区', 'Xingan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DSQ', '东山区', 'Dongshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSQ', '兴山区', 'Xingshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LBX', '萝北县', 'Luobei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SBX', '绥滨县', 'Suibin', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '鹤岗市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JSQ', '尖山区', 'Jianshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LDQ', '岭东区', 'Lingdong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SFT', '四方台区', 'Sifangtai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '集贤县', 'Jixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YYX', '友谊县', 'Youyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BQX', '宝清县', 'Baoqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RHX', '饶河县', 'Raohe', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '双鸭山市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXQ', '新兴区', 'Xinxing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TSQ', '桃山区', 'Taoshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QZH', '茄子河区', 'Qiezihe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BLX', '勃利县', 'Boli', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '七台河市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BLQ', '北林区', 'Beilin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ADS', '安达市', 'Anda', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZDS', '肇东市', 'Zhaodong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLS', '海伦市', 'Hailun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WKX', '望奎县', 'Wangkui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '兰西县', 'Lanxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QGX', '青冈县', 'Qinggang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QAX', '庆安县', 'Qingan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MSX', '明水县', 'Mingshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SLX', '绥棱县', 'Suiling', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '绥化市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AHQ', '爱辉区', 'Aihui District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BAS', '北安市', 'Beian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WDL', '五大连池市', 'Wudalianchi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NJX', '嫩江县', 'Nenjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XKX', '逊克县', 'Xunke', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SWX', '孙吴县', 'Sunwu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '黑河市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JGD', '加格达奇区', 'Jiagedaqi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SLQ', '松岭区', 'Songling District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XLQ', '新林区', 'Xinlin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZQ', '呼中区', 'Huzhong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MHS', '漠河市', 'Mohe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HMX', '呼玛县', 'Huma', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'THX', '塔河县', 'Tahe', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '大兴安岭地区') where parent_area_id = -1;

-- -----------------------------------------------------------------------------------------------------------------------------------------------------
-- 湖北省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WHS', '武汉市', 'Wuhan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HSS', '黄石市', 'Huangshi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SYS', '十堰市', 'Shiyan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YCS', '宜昌市', 'Yichang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XYS', '襄阳市', 'Xiangyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'EZS', '鄂州市', 'Ezhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SZS', '随州市', 'Suizhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XGS', '孝感市', 'Xiaogan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XNS', '咸宁市', 'Xianning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HGS', '黄冈市', 'Huanggang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JMS', '荆门市', 'Jingmen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JZS', '荆州市', 'Jingzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XN#', '仙桃市', 'Xianning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XNZ', '恩施州', 'Xianning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QJS', '潜江市', 'Qianjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JM#', '天门市', 'Jingmen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SNJ', '神农架地区', 'Jingzhou', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '湖北') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JAQ', '江岸区', 'Jiangan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JHQ', '江汉区', 'Jianghan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QKQ', '硚口区', 'Qiaokou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYQ', '汉阳区', 'Hanyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WCQ', '武昌区', 'Wuchang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QSQ', '青山区', 'Qinshan istrict', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSQ', '洪山区', 'Hongshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DXH', '东西湖区', 'Dongxihu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HNQ', '汉南区', 'Hannan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CDQ', '蔡甸区', 'Caidian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXQ', '江夏区', 'Jiangxia District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HPQ', '黄陂区', 'Huangpu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XZQ', '新洲区', 'Xinzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DHQ', '东湖区', 'Donghu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZKQ', '沌口区', 'Zhuankou District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '武汉市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSG', '黄石港区', 'HuangShigang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSS', '西塞山区', 'Xisaishan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XLQ', '下陆区', 'Xialu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TSQ', '铁山区', 'Tieshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '阳新县', 'Yangxin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DYX', '大冶县', 'Daye', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '黄石市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MJQ', '茅箭区', 'Maojian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZWQ', '张湾区', 'Zhangwan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YYQ', '郧阳区', 'Yunyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '郧西县', 'Yunxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZSX', '竹山县', 'Zhushan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZXX', '竹溪县', 'Zhuxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FXX', '房县', 'Fangxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DJK', '丹江口市', 'Danjiangkou', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '十堰市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XLQ', '西陵区', 'Xiling District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WJG', '伍家岗区', 'Wujiagang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DJQ', '点军区', 'Dianjun District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XTQ', '猇亭区', 'Xiaoting District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLQ', '夷陵区', 'Yiling District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YAX', '远安县', 'Yuanan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSX', '兴山县', 'Xingshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZGX', '秭归县', 'Zigui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYX', '长阳县', 'Changyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WFX', '五峰县', 'Wufeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YDS', '宜都市', 'Yidu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DYS', '当阳市', 'Dangyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZJS', '枝江市', 'Zhijiang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '宜昌市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYQ', '襄城区', 'Xiangyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FCQ', '樊城区', 'Fancheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XZQ', '襄州区', 'Xiangzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NZX', '南漳县', 'Nanzhang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GCX', '谷城县', 'Gucheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BKX', '保康县', 'Baokang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHK', '老河口市', 'Laohekou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYS', '枣阳市', 'Zaoyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCS', '宜城市', 'Yicheng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '襄阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZH', '梁子湖区', 'Liangzihu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HRQ', '华容区', 'Huarong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ECQ', '鄂城区', 'Echeng District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '鄂州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZDQ', '曾都区', 'Zengdu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXX', '随县', 'Suixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GSS', '广水市', 'Guangshui', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '随州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XNQ', '孝南区', 'Xiaonan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCX', '孝昌县', 'Xiaochang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DWX', '大悟县', 'Dawu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YMX', '云梦县', 'Yunmeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCS', '应城市', 'Yingcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCS', '汉川市', 'Hanchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ALS', '安陆市', 'Anlu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '孝感市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XAQ', '咸安区', 'Xianan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYX', '嘉鱼县', 'JIayu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYX', '崇阳县', 'Chongyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TCX', '通城县', 'Tongcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TSX', '通山县', 'Tongshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CBS', '赤壁市', 'Chibi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '咸宁市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZQ', '黄州区', 'Huangzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TFX', '团风县', 'Tuanfeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HAX', '红安县', 'Hongan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LTX', '罗田县', 'Luotian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSX', '英山县', 'Yingshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HMX', '黄梅县', 'Huangmei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSX', '浠水县', 'Xishui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QCX', '蕲春县', 'Qichun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WXS', '武穴市', 'Wuxue', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MCS', '麻城市', 'Macheng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '黄冈市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DBQ', '东宝区', 'Dongbao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DDQ', '掇刀区', 'Duodao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYX', '沙洋县', 'Shayang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZXS', '钟祥市', 'Zhongxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JSS', '京山市', 'Jingshan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '荆门市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BLQ', '沙市区', 'Beilin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ADQ', '荆州区', 'Anda', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZDX', '江陵县', 'Zhaodong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLX', '监利县', 'Hailun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WKX', '公安县', 'Wangkui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXS', '松滋市', 'Lanxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QGS', '石首市', 'Qinggang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QAS', '洪湖市', 'Qingan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '荆州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ESS', '恩施市', 'Enshi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCS', '利川市', 'Lichuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JSX', '建始县', 'Jianshi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BDX', '巴东县', 'Badong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XEX', '宣恩县', 'Xuanen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XFX', '咸丰县', 'Xianfeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LFX', '来凤县', 'Laifeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HFX', '鹤峰县', 'Hefeng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '恩施州') where parent_area_id = -1;

---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 湖南省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CSS', '长沙市', 'Changsha', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZZS', '株洲市', 'Zhuzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XTS', '湘潭市', 'Xiangtan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HYS', '衡阳市', 'Hengyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SYS', '邵阳市', 'Shaoyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YYS', '岳阳市', 'Yueyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CDS', '常德市', 'Changde', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZJJ', '张家界市', 'Zhangjiajie', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YY#', '益阳市', 'Yiyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CZS', '郴州市', 'Chenzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YZS', '永州市', 'Yongzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HHS', '怀化市', 'Huaihua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LDS', '娄底市', 'Loudi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XXZ', '湘西州', 'Xiangxi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '湖南') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YMQ', '岳麓区', 'Yuelu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FRQ', '芙蓉区', 'Furong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TXQ', '天心区', 'Tianxin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KFQ', '开福区', 'Kaifu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHQ', '雨花区', 'Yuehua District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WCQ', '望城区', 'Wangcheng istrict', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CSX', '长沙县', 'Changsha', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYX', '浏阳县', 'Liuyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NXS', '宁乡市', 'Ningxiang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '长沙市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TYQ', '天元区', 'Tianyuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HTQ', '荷塘区', 'Hetang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSQ', '芦淞区', 'Lusong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SFQ', '石峰区', 'Shifeng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LLS', '醴陵市', 'Liling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZZX', '株洲县', 'Zhuzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLX', '炎陵县', 'Yanling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CLX', '茶陵县', 'Chaling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '攸县', 'Xiuxian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '株洲市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YTQ', '岳塘区', 'Yuetang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHQ', '雨湖区', 'Yuhu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXS', '湘乡市', 'Xiangxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SSS', '韶山市', 'Shaoshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XTX', '湘潭县', 'Xiangtan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '湘潭市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YFQ', '雁峰区', 'Yanfeng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZHQ', '珠晖区', 'Zhuhui District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SGQ', '石鼓区', 'Shigu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZXQ', '蒸湘区', 'Zhengxiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NYQ', '南岳区', 'Nanyue District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYS', '耒阳市', 'Leiyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CNS', '常宁市', 'Changning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYX', '衡阳县', 'Hengyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HDX', '衡东县', 'Hengdong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSX', '横山县', 'Hengshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HNX', '衡南县', 'Hengnan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QDX', '祁东县', 'Qidong', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '衡阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SQQ', '双清区', 'Shuangqing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DXQ', '大祥区', 'Daxiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BTQ', '北塔区', 'Beita District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WGS', '武冈市', 'Wugang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SDX', '邵东县', 'Zhaodong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DKX', '洞口县', 'Dongkou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSX', '新邵县', 'Xinshao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SNX', '绥宁县', 'Suining', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XNX', '新宁县', 'Xinning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYX', '邵阳县', 'Shaoyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHX', '隆回县', 'Longhui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CBX', '城步县', 'Chengbu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '邵阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YYL', '岳阳楼区', 'Yueyanglou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JSQ', '君山区', 'Junshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXQ', '云溪区', 'Yunxi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXS', '临湘市', 'Linxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLS', '汨罗市', 'Miluo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YYX', '岳阳县', 'Yueyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYX', '湘阴县', 'Xiangyin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PJX', '平江县', 'Pingjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HRX', '华容县', 'Huarong', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '岳阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WLQ', '武陵区', 'Wuling District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DCQ', '鼎城区', 'Dingcheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JJS', '津市市', 'Jinshi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '澧县', 'Lixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LLX', '临澧县', 'Linli', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TYX', '桃源县', 'Taoyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSX', '汉寿县', 'Hanshou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AXX', '安乡县', 'Anxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SMX', '石门县', 'Shimeng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '常德市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YDQ', '永定区', 'Yongding District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WLY', '武陵源区', 'Wulingyuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CLX', '慈利县', 'Cili', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SZX', '桑植县', 'Sangzhi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '张家界市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSQ', '赫山区', 'Heshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYQ', '资阳区', 'Ziyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJX', '沅江市', 'Yuanjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TJX', '桃江县', 'Taojiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NXX', '南县', 'Nanxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AHX', '安化县', 'Anhua', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '益阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BFQ', '北湖区', 'Beihu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXQ', '苏仙区', 'Suxian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZXS', '资兴市', 'Zixing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZX', '宜章县', 'Yizhang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RCX', '汝城县', 'Rucheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ARX', '安仁县', 'Anren', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JHX', '嘉禾县', 'Jiahe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LWX', '临武县', 'Linwu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GDX', '桂东县', 'Guidong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '永兴县', 'Yongxing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GYX', '桂阳县', 'Guiyang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '郴州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LST', '冷水滩区', 'Lenshuitan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LLQ', '零陵区', 'Lingling District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QYX', '祁阳县', 'Qiyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '蓝山县', 'Lanshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NYX', '宁远县', 'Ningyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XTX', '新田县', 'Xintian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DAX', '东安县', 'Yongan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYX', '江永县', 'Jiangyong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DXX', '道县', 'Daoxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SPX', '双牌县', 'Shuangpai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JHX', '江华县', 'Jianghua', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '永州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCQ', '鹤城区', 'Hecheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJS', '洪江市', 'Hongjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HTX', '会同县', 'Huitong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLX', '沅陵县', 'Yuanling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CXX', '辰溪县', 'Chenxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XPX', '溆浦县', 'Xupu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZFX', '中方县', 'Zhongfang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHX', '新晃县', 'Xinhuang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZJX', '芷江县', 'Zhijiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TDX', '通道县', 'Tongdao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZX', '靖州县', 'Jingzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MYX', '麻阳县', 'Mayang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '怀化市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXQ', '娄星区', 'Louxing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSJ', '冷水江市', 'Lenshuijiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYS', '涟源市', 'Lianyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHX', '新化县', 'Xinhua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SFX', '双峰县', 'Shuangfeng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '娄底市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCS', '吉首市', 'Jishou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJX', '古丈县', 'Guzhang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HTX', '龙山县', 'Longshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLX', '永顺县', 'Yongshun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CXX', '凤凰县', 'Fenghuang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XPX', '泸西县', 'Luxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZFX', '保靖县', 'Baojing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHX', '花垣县', 'Huayuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '湘西州') where parent_area_id = -1;

---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 吉林省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CCS', '长春市', 'Changchun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JLS', '吉林市', 'Jilin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SPS', '四平市', 'Siping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SLS', '辽源市', 'Liaoyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'THS', '通化市', 'Tonghua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BSS', '白山市', 'Baishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SYS', '松原市', 'Songyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BCS', '白城市', 'Baicheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YBZ', '延边州', 'Yanbian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '吉林') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NGQ', '南关区', 'Nanguan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KCQ', '宽城区', 'Kuancheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYQ', '朝阳区', 'Chaoyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'EDQ', '二道区', 'Erdao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYQ', '绿园区', 'Lvyuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYQ', '双阳区', 'Shuangyang istrict', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JTQ', '九台区', 'Jiutai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSS', '榆树市', 'Yushu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DHS', '德惠市', 'Dehui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NAX', '农安县', 'Nongan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '长春市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYQ', '昌邑区', 'Changyi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LTQ', '龙潭区', 'Longtan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CY#', '船营区', 'Chuanying District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FMQ', '丰满区', 'Fengman District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JHS', '蛟河市', 'Jiaohe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HDS', '桦甸市', 'Huadian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SLS', '舒兰市', 'Shulan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PSS', '磐石市', 'Panshi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJX', '永吉县', 'Yongji', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '吉林市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TXQ', '铁西区', 'Tiexi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TDQ', '铁东区', 'Tiedong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GZL', '公主岭市', 'Gongzhuling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SLS', '双辽市', 'Shuangliao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '梨树县', 'Lishu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YTX', '伊通县', 'Yitong', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '四平市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSQ', '龙山区', 'Longshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XAX', '西安区', 'Xian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DFX', '东丰县', 'Dongfeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DLX', '东辽县', 'Dongliao', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '辽源市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DCQ', '东昌区', 'Dongchang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'EDJ', '二道江区', 'Erdaojiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JAS', '集安市', 'Jian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'THX', '通化县', 'Tonghua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HNX', '辉南县', 'Huinan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHX', '柳河县', 'Liuhe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MHK', '梅河口市', 'Meihekou', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '通化市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJQ', '浑江区', 'Hunjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYQ', '江源区', 'Songyuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LJS', '临江市', 'Linjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FSX', '抚松县', 'Fusong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYX', '靖宇县', 'Jingyu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CBX', '长白县', 'Changbai', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '白山市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NJQ', '宁江区', 'Ningjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FYS', '扶余市', 'Fuyu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QGX', '前郭县', 'Qianguo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CLX', '长岭县', 'Changling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QAX', '乾安县', 'Qianan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '松原市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TBQ', '洮北区', 'Taobei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TNS', '洮南市', 'Taonan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DAS', '大安市', 'Daan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZLX', '镇赉县', 'Zhenlai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TYX', '通榆县', 'Tongyu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '白城市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJS', '延吉市', 'Yanji', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TMS', '图们市', 'Tumen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DHS', '敦化市', 'Dunhua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCS', '珲春市', 'Huichun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LJS', '龙井市', 'Longjing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLS', '和龙市', 'Helong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WQX', '汪清县', 'Wangqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ATX', '安图县', 'Antu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '延边州') where parent_area_id = -1;

---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 江苏省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NJS', '南京市', 'Nanjing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WXS', '无锡市', 'Wuxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XZS', '徐州市', 'Xuzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CZS', '常州市', 'Changzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SZS', '苏州市', 'Suzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NTS', '南通市', 'Nantong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LYG', '连云港市', 'Lianyungang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HAS', '淮安市', 'Huaian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YCS', '盐城市', 'Yancheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YZS', '扬州市', 'Yangzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZJS', '镇江市', 'Zhenjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TZS', '泰州市', 'Taizhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SQS', '宿迁市', 'Suqian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '江苏') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GLQ', '鼓楼区', 'Gulou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HWQ', '玄武区', 'Xuanwu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QHQ', '秦淮区', 'Qinhuai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYQ', '建邺区', 'Jianye District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHT', '雨花台区', 'Yuhuatai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXQ', '栖霞区', 'Xixia District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JNQ', '江宁区', 'Jiangning District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PKQ', '浦口区', 'Pukou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHQ', '六合区', 'Liuhe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSQ', '溧水区', 'Lishui District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GCQ', '高淳区', 'Gaocun District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '南京市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXQ', '梁溪区', 'Liangxi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BHQ', '滨湖区', 'Binhu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XWQ', '新吴区', 'Xinwu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSQ', '锡山区', 'Xishan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSQ', '惠山区', 'Huishan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYS', '江阴市', 'Jiangyin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXS', '宜兴市', 'Yixing', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '无锡市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLQ', '云龙区', 'Yunlong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QSQ', '泉山区', 'Quanshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GLQ', '鼓楼区', 'Gulou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JWQ', '贾汪区', 'Jiawang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TSQ', '铜山区', 'Tongshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SNX', '睢宁县', 'Suining', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PXX', '沛县', 'Peixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FXX', '丰县', 'Fengxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PZS', '邳州市', 'Pizhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYS', '新沂市', 'Xinyi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '徐州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XBQ', '新北区', 'Xinbei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TNQ', '天宁区', 'Tianning District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZLQ', '钟楼区', 'Zhonglou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WJQ', '武进区', 'Wujin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JTQ', '金坛区', 'Jintan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYQ', '溧阳市', 'Liyang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '常州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GSQ', '姑苏区', 'Gusu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WQQ', '虎丘区', 'Huqiu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCQ', '相城区', 'Xiangcheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WZQ', '吴中区', 'Wuzhong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WJQ', '吴江区', 'Wujiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CSS', '常熟市', 'Changshu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZJG', '张家港市', 'Zhangjiagang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TCS', '太仓市', 'Taicang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KSS', '昆山市', 'Kunshan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '苏州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CCQ', '崇川区', 'Chongchuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GZQ', '港闸区', 'Gangzha District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TZQ', '通州区', 'Tongzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RDX', '如东县', 'Rudong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RGX', '如皋县', 'Rugao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HMS', '海门市', 'Haimen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QDS', '启东市', 'Qidong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HAS', '海安市', 'Haian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '南通市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZQ', '海州区', 'Haizhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYQ', '连云区', 'Lianyun District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GYQ', '赣榆区', 'Ganyu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DHX', '东海县', 'DOnghai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GYX', '灌云县', 'Guanyun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GNX', '灌南县', 'Guannan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '连云港市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HAQ', '淮安区', 'Huaian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYQ', '淮阴区', 'Huaiyin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QJP', '清江浦区', 'Qingjiangpu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZQ', '洪泽区', 'Hongze District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '涟水县', 'Lianshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYX', '盱眙县', 'Xuyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JHX', '金湖县', 'Jinhe', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '淮安市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'THQ', '亭湖区', 'Tinghu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YDQ', '盐都区', 'Yandu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DFQ', '大丰区', 'Dafeng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSX', '响水县', 'Xiangshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BHX', '滨海县', 'Binhai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FNX', '阜宁县', 'Funing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYX', '射阳县', 'Sheyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JHX', '建湖县', 'Jianhu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DTS', '东台市', 'Dongtai', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '盐城市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GLQ', '广陵区', 'Guangling District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GJQ', '邗江区', 'Hanjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JDQ', '江都区', 'Jiangdu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BYX', '宝应县', 'Baoying', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZS', '仪征市', 'Yizheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GYS', '高邮市', 'Gaoyou', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '扬州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'THQ', '京口区', 'Tinghu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YDQ', '润州区', 'Yandu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DFQ', '丹徒区', 'Dafeng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSS', '丹阳市', 'Xiangshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BHS', '扬中市', 'Binhai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FNS', '句容市', 'Funing', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '镇江市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLQ', '海陵区', 'Hailing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GGQ', '高港区', 'Gaogang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYQ', '姜堰区', 'Jiangyan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TXS', '泰兴市', 'Taixing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JJS', '靖江市', 'Jingjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHS', '兴化市', 'Xinghua', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '泰州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCQ', '宿城区', 'Sucheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYQ', '宿豫区', 'Suyu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MYX', '沐阳县', 'Muyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYX', '泗阳县', 'Siyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHX', '泗洪县', 'Sihong', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '宿迁市') where parent_area_id = -1;

---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 江西省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NCS', '南昌市', 'Nanchang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JDZ', '景德镇市', 'Jingdezhen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'PXS', '萍乡市', 'Pingxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XYS', '新余市', 'Xinyu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JJS', '九江市', 'Jiujiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YTS', '鹰潭市', 'Yingtan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'GZS', '赣州市', 'Ganzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JAS', '吉安市', 'Jian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YCS', '宜春市', 'Yichun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'FZS', '抚州市', 'Fuzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SRS', '上饶市', 'Shangrao', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '江西') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DHQ', '东湖区', 'Donghu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHQ', '西湖区', 'Xihu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QYP', '青云谱区', 'Qingyunpu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WLQ', '湾里区', 'Wanli District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QSH', '青山湖区', 'Qingshanhu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XJQ', '新建区', 'Xinjian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NCX', '南昌县', 'Nanchang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '进贤县', 'Jinxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AYX', '安义县', 'Anyi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '南昌市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZSQ', '珠山区', 'Zhushan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CJQ', '昌江区', 'Changjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FLX', '浮梁县', 'Fuliang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LPS', '乐平市', 'Leping', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '景德镇市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AYQ', '安源区', 'Anyuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XDQ', '湘东区', 'Xiangdong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHX', '莲花县', 'Lianhua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SLX', '上栗县', 'Shangli', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '泸溪县', 'Luxi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '萍乡市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSQ', '渝水区', 'Yushui District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FYX', '分宜县', 'Fenyi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '新余市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYQ', '浔阳区', 'Xunyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXQ', '濂溪区', 'Lianxi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CSQ', '柴桑区', 'Chaisang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WNX', '武宁县', 'Wuning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PZX', '彭泽县', 'Pengze', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSX', '修水县', 'Xiushui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '永修县', 'Yongxiu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HKX', '湖口县', 'Hukou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DAX', '德安县', 'Dean', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DCX', '都昌县', 'Duchang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RCX', '瑞昌市', 'Ruichang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GQC', '共青城市', 'Gongqingcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSS', '庐山市', 'Lushan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '九江市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CCQ', '月湖区', 'Guihu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GZQ', '余江区', 'Yujiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TZS', '贵溪市', 'Guixi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '鹰潭市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZGQ', '章贡区', 'Zhanggong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NKQ', '南康区', 'Nankang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GXQ', '赣县区', 'Gangxian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCX', '石城县', 'Shicheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AYX', '安远县', 'Anyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NDX', '宁都县', 'Duning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XWX', '寻乌县', 'Xunwu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XGX', '兴国县', 'Xingguo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DNX', '定南县', 'Dingnan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYX', '上犹县', 'Shangyou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GNX', '于都县', 'Yudu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LNX', '龙南县', 'Longnan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYX', '崇义县', 'Chongyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XFX', '信丰县', 'Xinfeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QNX', '全南县', 'Quannan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DYX', '大余县', 'Dayu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCX', '会昌县', 'Huichang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RJS', '瑞金市', 'Ruijin', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '赣州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZQ', '吉州区', 'Jizhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QYQ', '青原区', 'Qingyuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JAX', '吉安县', 'Jian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YFX', '永丰县', 'Yongfeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '永新县', 'Yongxin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XGX', '新干线', 'Xingan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'THX', '泰和县', 'Taihe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XJX', '峡江县', 'Xiajiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCX', '遂川县', 'Suichuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FAX', '福安县', 'Fuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JSX', '吉水县', 'Jishui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WAX', '万安县', 'Wanan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JGS', '井冈山市', 'Jinggangshan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '吉安市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZQ', '袁州区', 'Yuanzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TGX', '铜鼓县', 'Tonggu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JAX', '靖安县', 'Jingan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YFX', '宜丰县', 'Yifeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FXX', '奉新县', 'Fengxin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WZX', '万载县', 'Wanzai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SGX', '上高县', 'Shanggao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FCS', '丰城市', 'Fengcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZSS', '樟树市', 'Zhangshu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GAS', '高安市', 'Gaoan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '宜春市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCQ', '临川区', 'Linchuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DXQ', '东乡区', 'Dongxiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NFX', '南丰县', 'Nanfeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LAX', '乐安县', 'Lean', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '金溪县', 'Jinxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NCX', '南城县', 'Nancheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZXX', '资溪县', 'Zixi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHX', '宜黄县', 'Yihuang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GCX', '广昌县', 'Guangchang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '黎川县', 'Lichuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CRX', '崇仁县', 'Chongren', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '抚州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XZQ', '信州区', 'Xinzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DXQ', '东乡区', 'Dongxiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SRX', '上饶县', 'Shangrao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PYX', '鄱阳县', 'Poyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WYX', '婺源县', 'Wuyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSX', '铅山县', 'Yanshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YGX', '余干县', 'Yugan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HFX', '横峰县', 'Hengfeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YYX', '弋阳县', 'Yiyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YS#', '玉山县', 'Yushan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WNX', '万年县', 'Wannian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DXS', '德兴市', 'Dexing', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '上饶市') where parent_area_id = -1;

---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 辽宁省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SYS', '沈阳市', 'Shenyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DLS', '大连市', 'Dalian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ASS', '鞍山市', 'Anshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'FSS', '抚顺市', 'Fushun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BXS', '本溪市', 'Benxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DDS', '丹东市', 'Dandong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JZS', '锦州市', 'Jinzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YKS', '营口市', 'Yingkou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'FXS', '阜新市', 'Fuxin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LYS', '辽阳市', 'Liaoyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'PJS', '盘锦市', 'Panjin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TLS', '铁岭市', 'Tieling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CYS', '朝阳市', 'Chaoyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HLD', '葫芦岛市', 'Huludao', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '辽宁') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HPQ', '和平区', 'Heping District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHQ', '沈河区', 'Shenhe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HGQ', '皇姑区', 'huanggu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DDQ', '大东区', 'Dadong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TXQ', '铁西区', 'Tiexi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SJT', '苏家屯区', 'Sijiatun District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HNQ', '浑南区', 'Hunnan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SBQ', '沈北新区', 'Shenbei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHQ', '于洪区', 'Yuhong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XMQ', '新民市', 'Xinmin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZQ', '辽中区', 'Liaozhong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KPX', '康平县', 'Kangping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FKX', '法库县', 'Faku', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '沈阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZSQ', '中山区', 'Zhongshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SGQ', '西岗区', 'Xigang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHK', '沙河口区', 'Shahekou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GJZ', '甘井子区', 'Ganjingzi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSK', '旅顺口区', 'Lvshunkou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZQ', '金州区', 'Jinzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PLD', '普兰店区', 'Puliandian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WFD', '瓦房店市', 'Wafangdian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZHS', '庄河市', 'Zhuanghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZHX', '长海县', 'Changhainyi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '大连市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TDQ', '铁东区', 'Tiedong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TXQ', '铁西区', 'Tiexi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSQ', '立山区', 'Lishan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QSQ', '千山区', 'Qianshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCS', '海城市', 'Haicheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TAX', '台安县', 'Taian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYX', '岫岩县', 'Xiuyan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '鞍山市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XFQ', '新抚区', 'Xinfu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DZQ', '东洲区', 'Dongzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WHQ', '望花区', 'Wanghua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCQ', '顺城区', 'Shuncheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FSX', '抚顺县', 'Fushun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QYX', '清原县', 'Qingyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XBX', '新宾县', 'Xinbin', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '抚顺市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PSQ', '平山区', 'Pingshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHQ', '溪湖区', 'Xihu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MSQ', '明山区', 'Mingshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NFQ', '南芬区', 'Nanfen District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BXX', '本溪县', 'Benxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HRX', '恒仁县', 'Hengren', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '本溪市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YBQ', '元宝区', 'Yuanbao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZXQ', '振兴区', 'Zhenxing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZAQ', '振安区', 'Zhenan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DGS', '东港市', 'Donggang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FCS', '凤城市', 'Fengcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KDX', '宽甸县', 'Kuandian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '丹东市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GTQ', '古塔区', 'Guta District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHQ', '凌河区', 'Linghe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'THQ', '太和区', 'Taihe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHS', '凌海市', 'Linghai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BZS', '北镇市', 'Beizhen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSS', '黑山县', 'Heishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '义县', 'Yixian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '锦州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZQQ', '站前区', 'Zhanqian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSQ', '西市区', 'Xishi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BYQ', '鲅鱼圈区', 'Bayuquan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LBQ', '老边区', 'Laobian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GZQ', '盖州市', 'Gaizhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DSQ', '大石桥市', 'Dashiqiao', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '营口市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZQ', '海州区', 'Haizhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XQQ', '新邱区', 'Xinqiu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TPQ', '太平区', 'taiping District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QHM', '清河门区', 'Qinghemen District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHQ', '细河区', 'Xihe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FXX', '阜新县', 'Fuxin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZWX', '彰武县', 'Zhangwu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '阜新市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BTQ', '白塔区', 'Baita District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WSQ', '文圣区', 'Wenshen District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HWQ', '宏伟区', 'Hongwei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GCL', '弓长岭区', 'Gongziling District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TZH', '太子河区', 'taizihe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DTS', '灯塔市', 'Dengta', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYX', '辽阳县', 'Liaoyang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '辽阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'STZ', '双台子区', 'Shuantaizi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XLT', '兴隆台区', 'Xinglongtai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DWQ', '大洼区', 'Dawa District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PSX', '盘山县', 'Panshan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '盘锦市') where parent_area_id = -1;


insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZQ', '银州区', 'Yinzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QHQ', '清河区', 'Qinghe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DBS', '调兵山市', 'Diaobinshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KYS', '开原市', 'Kaiyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TLX', '铁岭县', 'Tieling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XFX', '西丰县', 'Xifeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CTX', '昌图县', 'Changtu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '铁岭市') where parent_area_id = -1;


insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'STQ', '双塔区', 'Shuangta District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCQ', '龙城区', 'Longcheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BPS', '北票市', 'Beipiao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYS', '凌源市', 'Lingyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYX', '朝阳县', 'Chaoyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JPX', '建平县', 'Jianping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KLQ', '喀喇沁县', 'Kalaqin', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '朝阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSQ', '连山区', 'Lianshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LGQ', '龙港区', 'Longgang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NPQ', '南票区', 'Nanpiao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCS', '兴城市', 'Xingcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SZX', '绥中县', 'Suizhong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JCX', '建昌县', 'Jianchang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '葫芦岛市') where parent_area_id = -1;


---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 内蒙
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HHH', '呼和浩特市', 'Huhehaote', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BTS', '包头市', 'Baotou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WHS', '乌海市', 'Wuhai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CFS', '赤峰市', 'Chifeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TLS', '通辽市', 'Tongliao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ERD', '鄂尔多斯市', 'Eerduosi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HLB', '呼伦贝尔市', 'Hulunbeier', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BYN', '巴彦淖尔市', 'Bayannaoer', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WLC', '乌兰察布市', 'Wulanchabu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XAM', '兴安盟', 'Xingan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XLG', '锡林格勒盟', 'Xilingele', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ALS', '阿拉善盟', 'Alashan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '内蒙古') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCQ', '新城区', 'Xingcheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HMQ', '回民区', 'Huimin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YQQ', '玉泉区', 'Yuquan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHQ', '赛罕区', 'Saihan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TMT', '土默特左旗', 'Tumotezuoqi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TKT', '托克托县', 'Tuoketuo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLG', '和林格尔县', 'Helingeer', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QSH', '清水河县', 'Qingshuihe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WCX', '武川县', 'Wuchuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '呼和浩特市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DHQ', '东河区', 'Donghe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KDL', '昆都仑区', 'Kundulun District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QSQ', '青山区', 'Qingshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SGQ', '石拐区', 'Shiguai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BYE', '白云鄂博矿区', 'Baiyunerbo District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYQ', '九原区', 'Jiuyuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TMT', '土默特右旗', 'Tumoteyouqi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GYX', '固阳县', 'Guyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DML', '达茂联合旗', 'Damao', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '包头市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HBW', '海勃湾区', 'Haobowan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HNQ', '海南区', 'Hainan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WDQ', '乌达区', 'Wuda District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '乌海市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSQ', '红山区', 'Hongshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YBS', '元宝山区', 'Yuanbaoshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SSQ', '松山区', 'Songshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ALK', '阿鲁科尔沁旗', 'Alukeqin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BLZ', '巴林左旗', 'Balinzuo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BLY', '巴林右旗', 'Balinyou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '林西县', 'Linxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KSK', '克什克腾旗', 'Keshiketeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WNT', '翁牛特旗', 'Wengniute', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KLQ', '喀喇沁旗', 'Kalaqin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NCX', '宁城县', 'Ningcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AHQ', '敖汉旗', 'Aohan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '赤峰市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KEQ', '科尔沁区', 'Keerqin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KZZ', '科左中旗', 'Kezuozhong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KZH', '科左后旗', 'Kezuohou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KLX', '开鲁县', 'Kailu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KLQ', '库伦旗', 'Kulun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NMQ', '奈曼旗', 'Naiman', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZLT', '扎鲁特旗', 'Zhalute', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLG', '霍林格勒市', 'Helingele', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '通辽市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DSQ', '东胜区', 'Dongshen District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KBS', '康巴什区', 'Kangbashi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DLT', '达拉特旗', 'Dalate', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZGE', '准格尔旗', 'Zhungeer', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ETQ', '鄂托克前旗', 'Etuokeqian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ETK', '鄂托克旗', 'etuoke', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJQ', '杭锦旗', 'Hangjin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJH', '伊金霍洛旗', 'Yijinhuoluo', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '鄂尔多斯市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLE', '海拉尔区', 'Hailaer District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DLN', '扎赉诺尔区', 'Zhalainuoer District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ARQ', '阿荣旗', 'Arong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLD', '莫力达瓦旗', 'Molidawa', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ELC', '鄂伦春旗', 'Elunchun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'EWK', '鄂温克旗', 'Ewenke', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CBE', '陈巴尔虎旗', 'Chenbaerhu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XZQ', '新左旗', 'Xinzuo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYQ', '新右旗', 'Xinyou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MZL', '满洲里市', 'Manzhouli', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YKS', '牙克石市', 'Yakeshi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZLT', '扎兰屯市', 'Zhalantun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'EGN', '额尔古纳市', 'Eerguna', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '呼伦贝尔市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHQ', '临河区', 'Linhe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WYX', '五原县', 'Wuyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DKX', '磴口县', 'Dengkou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WLQ', '乌拉特前旗', 'Wulateqian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WLZ', '乌拉特中旗', 'Wulatezhong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WLH', '乌拉特后旗', 'Wulatehou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJH', '杭锦后旗', 'Hangjinhou', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '巴彦淖尔市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JNQ', '集宁区', 'Jining District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZZX', '卓资县', 'Zhuozi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HDX', '化德县', 'Huade', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SDX', '商都县', 'Shangdu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHX', '兴和县', 'Xinghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '凉城县', 'Liangcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYQ', '察右前旗', 'Chayouqian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYZ', '察右中旗', 'Chayouzhong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYH', '察右后旗', 'Chayouhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SWZ', '四王子旗', 'Siwangzi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FZS', '丰镇市', 'Fengzhen', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '乌兰察布市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WLH', '乌兰浩特市', 'Wulanhaote', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AES', '阿尔山市', 'Aershan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KYQ', '科右前旗', 'Keyouqian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KYZ', '科右中旗', 'Keyouzhong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZLT', '扎赉特旗', 'Zhalaite', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TQX', '突泉县', 'Tuquan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '兴安盟') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ELH', '二连浩特市', 'Erlianhaote', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XLH', '锡林浩特市', 'Xilinhaote', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ABG', '阿巴嘎旗', 'Abaga', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SNZ', '苏尼特左旗', 'Sunitezuo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SNY', '苏尼特右旗', 'Suniteyou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DWQ', '东乌旗', 'Dongwu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XWQ', '西乌旗', 'Xiwu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TPS', '太仆寺旗', 'Taipusi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHQ', '镶黄旗', 'Xianghuang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZXB', '正镶白旗', 'Zhengxiangbai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZLQ', '正蓝旗', 'Zhenglan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DLX', '多伦县', 'Duolun', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '锡林格勒盟') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ALZ', '阿拉善左旗', 'Alashanzuo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ALY', '阿拉善右旗', 'Alashanyou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'EJN', '额济纳旗', 'Ejina', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '阿拉善盟') where parent_area_id = -1;


---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 宁夏
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YCS', '银川市', 'Yinchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SZS', '石嘴山市', 'Shizuishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WZS', '吴忠市', 'Wuzhong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'GYS', '固原市', 'Guyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZWS', '中卫市', 'Zhongwei', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '宁夏') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XQQ', '兴庆区', 'Xingqing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXQ', '西夏区', 'Xixia District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JFQ', '金凤区', 'Jinfeng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YNX', '永宁县', 'Yongning District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLX', '贺兰县', 'Helan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LW',  '灵武市', 'Lingwu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '银川市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DWK', '大武口区', 'Dawukou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HNQ', '惠农区', 'Huinong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PLX', '平罗县', 'Huiluo', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '石嘴山市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LTQ', '利通区', 'Litong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSB', '红寺堡区', 'Hongsibao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCX', '盐池县', 'Yanchi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TXX', '同心县', 'Tongxing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QTX', '青铜峡市', 'Qingtongxia', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '吴忠市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZQ', '原州区', 'Yuanzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XJX', '西吉县', 'Xiji', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LDX', '隆德县', 'Delong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYX', '泾源县', 'Jingyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PYX', '彭阳县', 'Pengyang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '固原市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SPT', '沙坡头区', 'Shapotou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZNQ', '中宁区', 'Zhongning District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYX', '海原县', 'Haiyuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '中卫市') where parent_area_id = -1;

---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 青海省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XNS', '西宁市', 'Xining', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HDS', '海东市', 'Haidong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HBS', '海北州', 'Haibei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HNZ', '黄南州', 'Huangnan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HN#', '海南州', 'Hannan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'GLZ', '果洛州', 'Guoluo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YSZ', '玉树州', 'Yushu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HXZ', '海西州', 'Haixi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '青海') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CDQ', '城东区', 'Chengdong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CZQ', '城中区', 'Chengzhong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CXQ', '城西区', 'Chengxi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CBQ', '城北区', 'Chengbei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DTX', '大通县', 'Datong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZX', '湟中县', 'Huangzhong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYX', '湟源县', 'Huangyuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '西宁市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LDQ', '乐都区', 'Ledu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PAQ', '平安区', 'Pingan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MHX', '民和县', 'Minhe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZX', '互助县', 'Huzhu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLX', '化隆县', 'Halong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHX', '循化县', 'Xunhua', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '海东市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MYX', '门源县', 'Menyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QLX', '祁连县', 'Qilian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYX', '海晏县', 'Haiyan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GCX', '刚察县', 'Gangcha', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '海北州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TRX', '同仁县', 'Tongren', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZX', '尖扎县', 'Jianzha', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZKX', '泽库县', 'Zeku', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HNX', '河南县', 'Henan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '黄南州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GHX', '共和县', 'Gonghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TDX', '同德县', 'Tongde', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GDX', '贵德县', 'Guide', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHX', '兴海县', 'Xinghai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GNX', '贵南县', 'Guinan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '海南州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MQX', '玛沁县', 'Maqin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BMX', '班玛县', 'Banma', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GDX', '甘德县', 'Gande', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DRX', '达日县', 'Dari', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZX', '久治县', 'Jiuzhi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MDX', '玛多县', 'Maduo', -1);    
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '果洛州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSS', '玉树市', 'Yushu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZDX', '杂多县', 'Zhaduo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CDX', '称多县', 'Chengduo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZD#', '治多县', 'Zhiduo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NQX', '囊谦县', 'Nangqian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QML', '曲麻莱县', 'Qumalai', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '玉树州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GEM', '格尔木市', 'Geermu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DLH', '德令哈市', 'Delingha', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WLX', '乌兰县', 'Wulan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DLX', '都兰县', 'Dulan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TJX', '天峻县', 'Tianjun', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '海西州') where parent_area_id = -1;


---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 山东省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JNS', '济南市', 'Jinan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QDS', '青岛市', 'Qingdao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZBS', '淄博市', 'Zibo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZZS', '枣庄市', 'Zaozhuang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DYS', '东营市', 'Dongying', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YTS', '烟台市', 'Yantai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WFS', '潍坊市', 'Weifang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JN#', '济宁市', 'Jining', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TAS', '泰安市', 'Taian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WHS', '威海市', 'Weihai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'RZS', '日照市', 'Rizhao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LWS', '莱芜市', 'Laiwu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LYS', '临沂市', 'Linyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DZS', '德州市', 'Dezhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LCS', '聊城市', 'Liaocheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BZS', '滨州市', 'Binzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HZS', '菏泽市', 'Hezhe', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '山东') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXQ', '历下区', 'Lixia District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SZQ', '市中区', 'Shizhong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYQ', '槐荫区', 'Huaiyin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TQQ', '天桥区', 'Tianqiao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCQ', '历城区', 'Licheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CQQ', '长清区', 'Changqing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZQQ', '章丘区', 'Zhangqiu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PYX', '平阴县', 'Pingyin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYX', '济阳县', 'Jiyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHX', '商河县', 'Shanghe', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '济南市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SNQ', '市南区', 'Shinan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SBQ', '市北区', 'Shibei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HDQ', '黄岛区', 'Huangdao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSQ', '崂山区', 'Laoshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCQ', '李沧区', 'Licang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYQ', '城阳区', 'Changyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZQ', '胶州市', 'Jiaozhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JMS', '即墨市', 'Jimo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PDS', '平度市', 'Pingdu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXS', '莱西市', 'Laixi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '青岛市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZCQ', '淄川区', 'Zichuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZDQ', '张店区', 'Zhangdian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BSQ', '博山区', 'Boshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZQ', '临淄区', 'Linzi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZC#', '周村区', 'Zhoucun District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HTX', '恒台县', 'Hengtai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GQX', '高青县', 'Gaoqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YYX', '沂源县', 'Yiyuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '淄博市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SZQ', '市中区', 'Shizhong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCQ', '薛城区', 'Xucheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FCQ', '峰城区', 'Fengcheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TEZ', '台儿庄区', 'Taierzhuang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'STQ', '山亭区', 'Shanting District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TZS', '滕州市', 'Tengzhou', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '枣庄市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DYQ', '东营区', 'Dongying District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HKQ', '河口区', 'Hekou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KLQ', '垦利区', 'Kunli District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LJX', '利津县', 'Lijin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GRX', '广饶县', 'Guangrao', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '东营市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZFQ', '芝罘区', 'Zhifu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FSQ', '福山区', 'Fushan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MPQ', '牟平区', 'Muping District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSQ', '莱山区', 'Laishan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CDX', '长岛县', 'Changdao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LKS', '龙口市', 'Longkou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYS', '莱阳市', 'Laiyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZS', '莱州市', 'Laizhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PLS', '蓬莱市', 'Penglai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYS', '招远市', 'Zhaoyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXS', '栖霞市', 'Qixia', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYS', '海阳市', 'Haiyang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '烟台市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WCQ', '潍城区', 'Weicheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HTQ', '寒亭区', 'Hangting District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FZQ', '坊子区', 'Fangzi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KWQ', '奎文区', 'Kuiwen District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LQX', '临朐县', 'Linqu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CLX', '昌乐县', 'Changle', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QZS', '青州市', 'Qingzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZCS', '诸城市', 'Zhucheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SGS', '寿光市', 'Shouguang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AQS', '安丘市', 'Anqiu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GMS', '高密市', 'Gaomi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYS', '昌邑市', 'Changyi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '潍坊市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RCQ', '任城区', 'Rencheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZQ', '兖州区', 'Yanzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WSX', '微山县', 'Weishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YTX', '鱼台县', 'Yutai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '金乡县', 'Jinxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JX#', '嘉祥县', 'Jiaxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WS#', '汶上县', 'Wenshang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SSX', '泗水县', 'Sishui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZX', '梁山县', 'Liangshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QFS', '曲阜市', 'Qufu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZCS', '邹城市', 'Zoucheng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '济宁市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TSQ', '泰山区', 'Taishan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DYQ', '岱岳区', 'Daiyue District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NYX', '宁阳县', 'Ningyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DPX', '东平县', 'Dongping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XTS', '新泰市', 'Xintai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FCS', '肥城市', 'Feicheng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '泰安市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCQ', '环翠区', 'Huancui District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WDQ', '文登区', 'Wendeng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RCS', '荣成市', 'Rongcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RSS', '乳山市', 'Rushan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '威海市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DGQ', '东港区', 'Donggang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSQ', '岚山区', 'Lanshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WLX', '五莲县', 'Wulian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '莒县', 'Juxian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '日照市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCQ', '莱城区', 'Laicheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GCQ', '钢城区', 'Gangcheng District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '莱芜市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSQ', '兰山区', 'Lanshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZQ', '罗庄区', 'Luozhuang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HDQ', '河东区', 'Hedong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YNX', '沂南县', 'Yinan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TCX', '郯城县', 'Tancheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSX', '沂水县', 'Yishui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LLX', '兰陵县', 'Lanling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FXX', '费县', 'Feixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PYX', '平邑县', 'Pingyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JNX', '莒南县', 'Junan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MYX', '蒙阴县', 'Mengyin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '临沭县', 'Linshu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '临沂市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DCQ', '德城区', 'Decheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCQ', '陵城区', 'Lingcheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NJX', '宁津县', 'Ningjin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QYX', '庆云县', 'Qingyun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYX', '临邑县', 'Linyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QHX', '齐河县', 'Qihe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PYX', '平原县', 'Pingyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XJX', '夏津县', 'Xiajin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WCX', '武城县', 'Wucheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LLX', '乐陵县', 'Laoling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCS', '禹城市', 'Yucheng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '德州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DCF', '东昌府区', 'Dongchangfu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YGX', '阳谷县', 'Yanggu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXX', '莘县', 'Shenxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CPX', '茌平县', 'Chiping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DEX', '东阿县', 'Donge', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GXX', '冠县', 'Guanxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GTX', '高唐县', 'Gaotang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LQX', '临清县', 'Linqing', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '聊城市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BCQ', '滨城区', 'Bincheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZHQ', '沾化区', 'Zanhua District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HMX', '惠民县', 'Huimin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '阳信县', 'Yangxin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WDX', '无棣县', 'Wudi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BXX', '博兴县', 'Boxing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZPX', '邹平县', 'Zouping', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '滨州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MDQ', '牡丹区', 'Mudan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DTQ', '定陶区', 'Dingtao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CXX', '曹县', 'Caoxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXX', '单县', 'Shanxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CWX', '成武县', 'Chengwu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYX', '巨野县', 'Juye', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCX', '郓城县', 'Yuncheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JCX', '鄄城县', 'Juancheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DMX', '东明县', 'Dongming', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '菏泽市') where parent_area_id = -1;


---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 山西省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TYS', '太原市', 'Taiyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DTS', '大同市', 'Datong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YQS', '阳泉市', 'Yangquan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CZS', '长治市', 'Changzhi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JCS', '晋城市', 'Jincheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SZS', '朔州市', 'Shuzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JZS', '晋中市', 'Jinzhong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YCS', '运城市', 'Yuncheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XZS', '忻州市', 'Xinzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LFS', '临汾市', 'Linfen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LLS', '吕梁市', 'Lvliang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '山西') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XDQ', '小店区', 'Xiaodian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZQ', '迎泽区', 'Yingze District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHL', '杏花岭区', 'Xinghualing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JCP', '尖草坪区', 'Jiancaoping District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WBL', '万柏林区', 'Wanbailin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYQ', '晋源区', 'Jinyuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXX', '清徐县', 'Qingxu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YQX', '阳曲县', 'Yangqu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LFX', '娄烦县', 'Loufan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GJS', '古交市', 'Gujiao', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '太原市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CQQ', '城区', 'Chengqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KQQ', '矿区', 'Kuangqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NJQ', '南郊区', 'Nanjiao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XRQ', '新荣区', 'Xinrong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YGX', '阳高县', 'Yanggao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TZX', '天镇县', 'Tianzhen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GLX', '广灵县', 'Guangling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LQX', '灵丘县', 'Lingqiu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYX', '浑源县', 'HUnyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYX', '左云县', 'Zuoyun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DTX', '大同县', 'Datong', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '大同市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CQQ', '城区', 'Chengqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KQQ', '矿区', 'Kuangqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QQQ', '郊区', 'Jiaoqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PDX', '平定县', 'Pingding', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MXX', '孟县', 'Mengxian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '阳泉市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CQQ', '城区', 'Chengqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QQQ', '郊区', 'Jiaoqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CZX', '长治县', 'Changzhi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYX', '襄垣县', 'Xiangyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TLX', '屯留县', 'Tunliu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PSX', '平顺县', 'Pingshun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '黎城县', 'Licheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HGX', '壶关县', 'Huguan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZZX', '长子县', 'Zhangzi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WXX', '武乡县', 'Wuxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXX', '沁县', 'Qinxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QYX', '沁源县', 'Qinyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCS', '潞城市', 'Lucheng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '长治市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CQQ', '城区', 'Chengqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QSX', '沁水县', 'Qinshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCX', '阳城县', 'Yangcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '陵川县', 'Lingchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZZX', '泽州县', 'Zezhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GPS', '高平市', 'Gaoping', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '晋城市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCQ', '朔城区', 'Shuocheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PLQ', '平鲁区', 'Pinglu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYX', '山阴县', 'Shanyin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '应县', 'Yingxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YYX', '右玉县', 'Youyu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HRX', '怀仁县', 'HUanren', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '朔州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCQ', '榆次区', 'Yuci District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSX', '榆社县', 'Yushe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZQX', '左权县', 'Zuoquan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSX', '和顺县', 'Heshun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYX', '昔阳县', 'Xiyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYX', '寿阳县', 'Shouyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TGX', '太谷县', 'Taigu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXX', '祁县', 'Qixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PYX', '平遥县', 'Pingyao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '灵石县', 'Lingshi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXS', '介休市', 'Jiexiu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '晋中市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHQ', '盐湖区', 'Yanhu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYQ', '临猗县', 'Linyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WRQ', '万荣县', 'Wanrong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WXQ', '闻喜县', 'Wenxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JSQ', '稷山县', 'Jishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XJQ', '新绛县', 'Xinjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '绛县', 'Jiangxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YQQ', '垣曲县', 'Yuanqu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXX', '夏县', 'Xiaixan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PLX', '平陆县', 'Pinglu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RCX', '芮城县', 'Ruicheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJS', '永济市', 'Yongji', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJS', '河津市', 'Hejin', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '运城市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XFQ', '忻府区', 'Xinfu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DXX', '定襄县', 'Dingxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WTX', '五台县', 'Wutai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DX#', '代县', 'Daixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FSX', '繁峙县', 'Fanshi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NWX', '宁武县', 'Ningwu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JLX', '静乐县', 'Jingle', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCX', '神池县', 'Shenchi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WZX', '五寨县', 'Wuzhai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KLX', '岢岚县', 'Kelan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HQX', '河曲县', 'Hequ', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BDX', '保德县', 'Baode', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PGX', '偏关县', 'Pianguan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YPS', '原平市', 'Yuanping', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '忻州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YDQ', '尧都区', 'Yaodu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QWX', '曲沃县', 'Quwo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCX', '翼城县', 'Yicheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XFX', '襄汾县', 'Xiangfen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HTX', '洪洞县', 'Hongtong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GXX', '古县', 'Guxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AZX', '安泽县', 'Anze', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FSX', '浮山县', 'Fushan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '吉县', 'Jixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XNX', '乡宁县', 'Xiangning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DNX', '大宁县', 'Daning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXX', '隰县', 'Xixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHX', '永和县', 'Yonghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PXX', '蒲县', 'Puxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FXX', '汾西县', 'Fenxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HMS', '侯马市', 'Houma', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZS', '霍州市', 'Huozhou', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '临汾市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSQ', '离石区', 'Lishi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WSX', '文水县', 'Wenshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JCX', '交城县', 'Jiaocheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXX', '兴县', 'Xingxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '临县', 'Linxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LLX', '柳林县', 'Liulin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SLX', '石楼县', 'Shilou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LX#', '岚县', 'Lanxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FSX', '方山县', 'Fangshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYX', '中阳县', 'Zhongyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JKX', '交口县', 'Jiaokou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYS', '孝义市', 'Xiaoyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FYS', '汾阳市', 'Fenyang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '吕梁市') where parent_area_id = -1;

---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 陕西省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XAS', '西安市', 'Xian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TCS', '铜川市', 'Tongchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BJS', '宝鸡市', 'Baoji', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XYS', '咸阳市', 'Xianyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WNS', '渭南市', 'Weinan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YAS', '延安市', 'Yanan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HZS', '汉中市', 'Hanzhong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YLS', '榆林市', 'Yulin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'AKS', '安康市', 'Ankang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SLS', '商洛市', 'Shangluo', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '陕西') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCQ', '新城区', 'Xincheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BLQ', '碑林区', 'Beilin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHQ', '莲湖区', 'Lianhu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BQQ', '灞桥区', 'Baqiao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WYQ', '未央区', 'Weiyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YTQ', '雁塔区', 'Yanta District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLQ', '阎良区', 'Yanliang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LTQ', '临潼区', 'Lintong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CAQ', '长安区', 'Changan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GLQ', '高陵区', 'Gaolin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYQ', '鄠邑区', 'Huyi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LTX', '蓝田县', 'Lantian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZZX', '周至县', 'Zhouzhi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '西安市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WYQ', '王益区', 'Wangyi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YTQ', '印台区', 'Yintai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZQ', '耀州区', 'Yaozhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJX', '宜君县', 'Yijun', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '铜川市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WBQ', '渭滨区', 'Weibin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JTQ', '金台区', 'Jintai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CCQ', '陈仓区', 'Chencang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FXX', '凤翔县', 'Fengxiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QSX', '岐山县', 'Qishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FFX', '扶风县', 'Fufeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MXX', '眉县', 'Meixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '陇县', 'Longxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QYX', '千阳县', 'Qianyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYX', '麟游县', 'Linyou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FX#', '凤县', 'Fengxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TBX', '太白县', 'Taibai', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '宝鸡市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QDQ', '秦都区', 'Qindu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLQ', '杨陵区', 'Yangling District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WCQ', '渭城区', 'Weicheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYX', '三原县', 'Sanyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYX', '泾阳县', 'Jingyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXX', '乾县', 'Qianxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LQX', '礼泉县', 'Liquan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSX', '永寿县', 'Yongshou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BXX', '彬县', 'Binxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CWX', '长武县', 'Changwu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYX', '旬邑县', 'Xunyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CHX', '淳化县', 'Chunhua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WGX', '武功县', 'Wugong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XPS', '兴平市', 'Xingping', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '咸阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LWQ', '临渭区', 'Linwei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZQ', '华州区', 'Huazhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TGX', '潼关县', 'Tongguan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DLX', '大荔县', 'Dali', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYX', '合阳县', 'Heyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CCX', '澄城县', 'Chengcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BSX', '白水县', 'Baishui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FPX', '富平县', 'Fuping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PCX', '蒲城县', 'Pucheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCX', '韩城县', 'Hancheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYS', '华阴市', 'HUayin', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '渭南市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BTQ', '宝塔区', 'Baota District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ASQ', '安塞区', 'Anshai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCX', '延长县', 'Yanchang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YC#', '延川县', 'Yanchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZCX', '子长县', 'Zichang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZDX', '志丹县', 'Zhidan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WQX', '吴起县', 'Wuqi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GQX', '甘泉县', 'Ganquan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FXX', '富县', 'Fuxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '洛川县', 'Luochuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YC$', '宜川县', 'Yichuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLX', '黄龙县', 'Huanglong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HL#', '黄陵县', 'HUangling', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '延安市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HTQ', '汉台区', 'Hantai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NZQ', '南郑区', 'Nanzheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CGX', '城固县', 'Chenggu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '洋县', 'Yangxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXX', '西乡县', 'Xixiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MXX', '勉县', 'Mianxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NQX', '宁强县', 'Ningqiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYX', '略阳县', 'Lueyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZBX', '镇巴县', 'Zhenba', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LBX', '留坝县', 'Liuba', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FPX', '佛坪县', 'Foping', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '汉中市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YYQ', '榆阳区', 'Yuyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSQ', '横山区', 'Hengshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FGX', '府谷县', 'Fugu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JBX', '靖边县', 'Jingbian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DBX', '定边县', 'Dingbian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SDX', '绥德县', 'Suide', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MZX', '米脂县', 'Mizhi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JXX', '佳县', 'Jiaxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WBX', '吴堡县', 'Wubao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QJX', '清涧县', 'Qingjian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZZX', '子洲县', 'Zizhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SMS', '神木市', 'Shenmu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '榆林市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HBQ', '汉滨区', 'Hanbin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYX', '汉阴县', 'Hanyin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SQX', '石泉县', 'Shiquan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NSX', '宁陕县', 'Ningshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYX', '紫阳县', 'Ziyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LGX', '岚皋县', 'Langao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PLX', '平利县', 'Pingli', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZPX', '镇坪县', 'Zhenping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYX', '旬阳县', 'Xunyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BHX', '白河县', 'Baihe', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '安康市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SZQ', '商州区', 'Shangzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LNX', '洛南县', 'Luonan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DFX', '丹凤县', 'Danfeng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SNX', '商南县', 'Shangnan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYX', '山阳县', 'Shanyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZAX', '镇安县', 'Zhenan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZSX', '柞水县', 'Zashui', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '商洛市') where parent_area_id = -1;

---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 上海市
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HPQ', '黄浦区', 'Huangpu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XHQ', '徐汇区', 'Xuhui District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CNQ', '长宁区', 'Changning District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JAQ', '静安区', 'Jingan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'PTQ', '普陀区', 'Putuo District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZBQ', '闸北区', 'Zhabei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HKQ', '虹口区', 'Hongkou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YPQ', '杨浦区', 'Yangpu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'MHQ', '闵行区', 'Minhang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BSQ', '宝山区', 'Baoshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JDQ', '嘉定区', 'Jiading District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'PDX', '浦东新区', 'Pudong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JSQ', '金山区', 'Jinshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SJQ', '松江区', 'Songjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SPQ', '青浦区', 'Qingpu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'FXQ', '奉贤区', 'Fengxian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CMQ', '崇明区', 'Chongming District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '上海') where parent_area_id = -1;

---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 四川省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CDS', '成都市', 'Chengdu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZGS', '自贡市', 'Qingdao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'PZH', '攀枝花市', 'Panzhihua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LZS', '泸州市', 'Luzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DYS', '德阳市', 'Deyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'MYS', '绵阳市', 'Mianyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'GYS', '广元市', 'Guangyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SNS', '遂宁市', 'Shuining', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NJS', '内江市', 'neijiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LSS', '乐山市', 'Leshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NCS', '南充市', 'Nanchong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'MSS', '眉山市', 'Meishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YBS', '宜宾市', 'Yibin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'GAS', '广安市', 'Guangan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DZS', '达州市', 'Dazhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YAS', '雅安市', 'Yaan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BZS', '巴中市', 'Bazhong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZYS', '资阳市', 'Zhiyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ABZ', '阿坝州', 'Aba', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'GZZ', '甘孜州', 'Ganzhi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LSZ', '凉山州', 'liangshan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '四川') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JJQ', '锦江区', 'Jinjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QYQ', '青羊区', 'Qingyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JNQ', '金牛区', 'Jinniu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CHQ', '成华区', 'Chenghua District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WHQ', '武侯区', 'Wuhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LQY', '龙泉驿区', 'Longquanyi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QBJ', '青白江区', 'Qingbaijiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XDQ', '新都区', 'Xindu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WJQ', '温江区', 'Wenjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SLQ', '双流区', 'Shuangliu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PDQ', '郫都区', 'Pidu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JTX', '金堂县', 'Jintang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DYX', '大邑县', 'Dayi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PJX', '蒲江县', 'Pujiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XJX', '新津县', 'Xinjin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DJY', '都江堰市', 'Dujiangyan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PZS', '彭州市', 'Pengzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QLS', '邛崃市', 'Qionglai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CZS', '崇州市', 'Chongzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYS', '简阳市', 'Jianyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GXQ', '高新区', 'GaoXing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GXXQ', '高新西区', 'West GaoXing District', -1);

update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '成都市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZLJ', '自流井区', 'Ziliujing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GJQ', '贡井区', 'Gongjing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DAQ', '大安区', 'Daan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DTQ', '沿滩区', 'Yantan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RXX', '荣县', 'Rongxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FSX', '富顺县', 'Fushun', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '自贡市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DQQ', '东区', 'Dongqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XQQ', '西区', 'Xiqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RHQ', '仁和区', 'Renhe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MYX', '米易县', 'Miyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YBX', '盐边县', 'Yanbian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '攀枝花市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYQ', '江阳区', 'Jiangyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NXQ', '纳溪区', 'Naxi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LMT', '龙马潭区', 'Longmatan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '泸县', 'Luxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJX', '合江县', 'Hejiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYX', '叙永县', 'Xuyong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GLX', '古蔺县', 'Gulan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '泸州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYQ', '旌阳区', 'Jingyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZJX', '中江县', 'Zhongjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LJX', '罗江县', 'Luojiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GHS', '广汉市', 'Guanghan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SFS', '什邡市', 'Shifang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MZS', '绵竹市', 'Mianzhu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '德阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FCQ', '涪城区', 'Fucheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXQ', '游仙区', 'Youxian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AZQ', '安州区', 'Anzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'STX', '三台县', 'Santai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YTX', '盐亭县', 'Yanting', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZTX', '梓潼县', 'Zitong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BCX', '北川县', 'Beichuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PWX', '平武县', 'Pingwu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYS', '江油市', 'Jiangyou', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '绵阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZQ', '利州区', 'Lizhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZHQ', '昭化区', 'Zhaohua District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CTQ', '朝天区', 'Chaotian District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CWX', '旺苍县', 'Wangcang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QCX', '青川县', 'Qingchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JGX', '剑阁县', 'Jiange', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CXX', '苍溪县', 'Cangxi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '广元市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CSQ', '船山区', 'Chuanshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AJQ', '安居区', 'Anju District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PXX', '蓬溪县', 'Pengxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHX', '射洪县', 'Shehong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DYX', '大英县', 'Daying', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '遂宁市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SZQ', '市中区', 'Shizhong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DXQ', '东兴区', 'Dongxing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WYX', '威远县', 'Weiyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZZX', '资中县', 'Zizhong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '隆昌县', 'Longchang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '内江市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SZQ', '市中区', 'Shizhong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SWQ', '沙湾区', 'Shawan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WTQ', '五通桥区', 'Wutongqiao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JHK', '金河口区', 'Jinhekou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QWX', '犍为县', 'Qianwei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYX', '井研县', 'Jingyan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JJX', '夹江县', 'Jiajiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MCX', '沐川县', 'Muchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'EBX', '峨边县', 'Ebian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MBX', '马边县', 'Mabian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'EMS', '峨眉山市', 'Emeishan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '乐山市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SQQ', '顺庆区', 'Shunqing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GPQ', '高坪区', 'Gaoping District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JLQ', '嘉陵区', 'Jialing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NBX', '南部县', 'Nanbu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSX', '营山县', 'Yingshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PAX', '蓬安县', 'Pengan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLX', '仪陇县', 'Yilong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCX', '西充县', 'Xichong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZX', '阆中市', 'Langzhong', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '南充市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DPQ', '东坡区', 'Dongpo District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PSQ', '彭山区', 'Pengshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RSX', '仁寿县', 'Renshou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYX', '洪雅县', 'Hongya', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DLX', '丹棱县', 'Danling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QSX', '青神县', 'Qingshen', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '眉山市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CPQ', '翠屏区', 'Cuiping District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NXQ', '南溪区', 'Nanxi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YBX', '宜宾县', 'Yibin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JAX', '江安县', 'Jiangan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CNX', '长宁县', 'Changning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GXX', '高县', 'Gaoxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GX#', '珙县', 'Gongxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JLX', '筠连县', 'Junlian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XWX', '兴文县', 'Xingwen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PSX', '屏山县', 'Pingshan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '宜宾市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GAQ', '广安区', 'Guangan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QFQ', '前锋区', 'Qianfeng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCX', '岳池县', 'Yuechi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WSX', '武胜县', 'Wusheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '邻水县', 'Linshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYS', '华蓥市', 'HUaying', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '广安市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TCQ', '通川区', 'Tongchuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DCQ', '达川区', 'Dachuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHX', '宣汉县', 'Huanhan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KJX', '开江县', 'Kaijiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DZX', '大竹县', 'Dazhu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QXX', '渠县', 'Quxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WYS', '万源市', 'Wanyuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '达州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCQ', '雨城区', 'Yucheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MSQ', '名山区', 'Mingshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJX', '荥经县', 'Yingjing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYX', '汉源县', 'Hanyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SMX', '石棉县', 'Shimian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TQX', '天全县', 'Tianquan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSX', '芦山县', 'Lushan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BXX', '宝兴县', 'Baoxing', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '雅安市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NZQ', '巴州区', 'Bazhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'EYQ', '恩阳区', 'Enyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TJX', '通江县', 'Tongjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NJX', '南江县', 'Nanjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PCX', '平昌县', 'Pinchang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '巴中市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJQ', '雁江区', 'Yanjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AYX', '安岳县', 'Anyue', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZX', '乐至县', 'Lezhi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '资阳市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MEK', '马尔康市', 'Maerkang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WCX', '汶川县', 'Wenchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '理县', 'Lixian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MXX', '茂县', 'Maoxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SPX', '松潘县', 'Songpan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZG', '九寨沟县', 'Jiuzhaigou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JCX', '金川县', 'Jinchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XJX', '小金县', 'Xiaojin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSX', '黑水县', 'Heishui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RTX', '壤塘县', 'Rangtang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ABX', '阿坝县', 'Aba', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'REG', '若尔盖县', 'Ruoergai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYX', '红原县', 'Hongyuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '阿坝州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KDS', '康定市', 'KangDing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LDX', '泸定县', 'Luding', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DBX', '丹巴县', 'Danba', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JLX', '九龙县', 'Jiulong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJX', '雅江县', 'Yajiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DFX', '道孚县', 'Daofu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHX', '炉霍县', 'Luhuo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GZX', '甘孜县', 'Ganzi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XLX', '新龙县', 'Xinlong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DGX', '德格县', 'Dege', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BYX', '白玉县', 'Baiyu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SQX', '石渠县', 'Shiqu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SDX', '色达县', 'Seda', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LTX', '理塘县', 'Litang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BTX', '巴塘县', 'Batang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCX', '乡城县', 'Xiangcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DCX', '稻城县', 'Daocheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DRX', '得荣县', 'Derong', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '甘孜州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCS', '西昌市', 'Xichang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLX', '木里县', 'Muli', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YYX', '盐源县', 'Yanyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DCX', '德昌县', 'Dechang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLX', '会理县', 'Huili', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HDX', '会东县', 'Huidong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NNX', '宁南县', 'Ningnan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PGX', '普格县', 'Puge', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BTX', '布拖县', 'Butuo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYX', '金阳县', 'Jinyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZJX', '昭觉县', 'Zhaojue', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XDX', '喜德县', 'Xide', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MNX', '冕宁县', 'Mianing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '越西县', 'Yuexi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GLX', '甘洛县', 'Ganluo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MGX', '美姑县', 'Meigu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LBX', '雷波县', 'Leibo', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '凉山州') where parent_area_id = -1;

---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 天津市
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HPQ', '和平区', 'Heping District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HDQ', '河东区', 'Hedong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HXQ', '河西区', 'Hexi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NKQ', '南开区', 'Nankai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HBQ', '河北区', 'Hebei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HQQ', '红桥区', 'Hongqiao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DLQ', '东丽区', 'Dongli District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XQQ', '西青区', 'Xiqing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JNQ', '津南区', 'Jinnan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BCQ', '北辰区', 'Beichen District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WQQ', '武清区', 'Wuqing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BDQ', '宝坻区', 'Baidi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BHQ', '滨海新区', 'Binhai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NHQ', '宁河区', 'Ninghe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JHQ', '静海区', 'Jinghai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JZQ', '蓟州区', 'JInzhou District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '天津') where parent_area_id = -1;

---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 西藏
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NSS', '拉萨市', 'Nasa', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'RKZ', '日喀则市', 'Rikaze', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CDS', '昌都市', 'Changdu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LZS', '林芝市', 'Linzhi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SNS', '山南市', 'Shannan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NQA', '那曲地区', 'Naqu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ALA', '阿里地区', 'Ali', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '西藏') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CGQ', '城关区', 'Chengguan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DLD', '堆龙德庆区', 'Duilongdeqing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZX', '林周县', 'Linzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DXX', '当雄县', 'Dangxiong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NMX', '尼木县', 'Nimu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QSX', '曲水县', 'Qushui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DZX', '达孜县', 'Dazi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MZG', '墨竹工卡县', 'Mozhugongka', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '拉萨市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SZZ', '桑珠孜区', 'Sangzhuzi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NLM', '南木林县', 'Nanlinmu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZX', '江孜县', 'Jiangzi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DRX', '定日县', 'Dingri', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SJX', '萨迦县', 'Sajia', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZX', '拉孜县', 'Lazi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ARX', '昂仁县', 'Angren', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XTM', '谢通门县', 'Xietongmen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BLX', '白朗县', 'Bailang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RBX', '仁布县', 'Renbu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KMX', '康马县', 'Kangma', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DJX', '定结县', 'Dingjie', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZBX', '仲巴县', 'Zhongba', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YDX', '亚东县', 'Yadong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JLX', '吉隆县', 'Jilong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NL#', '聂拉木县', 'Nielamu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SGX', '萨嘎县', 'Saga', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GBX', '岗巴县', 'Gangba', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '日喀则市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KRQ', '卡若区', 'Karuo District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JDX', '江达县', 'Jiangda', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GJX', '贡觉县', 'Gongjue', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LWQ', '类乌齐县', 'Leiwuqi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DQ', '丁青县', 'Dingqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYX', '察雅县', 'Chaya', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BSX', '八宿县', 'Basu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZGX', '左贡县', 'Zuogong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MKX', '芒康县', 'Mangkang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LLX', '洛隆县', 'Luolong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BBX', '边坝县', 'Bianba', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '昌都市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BYQ', '巴宜区', 'Bayi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GBJ', '工布江达县', 'Gongbujiangda', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLX', '米林县', 'Milin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MTX', '墨脱县', 'Motuo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BMX', '波密县', 'Bomi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYX', '察隅县', 'Chayu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '朗县', 'Langxian', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '林芝市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NDQ', '乃东区', 'Naidong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZNX', '扎囊县', 'Zanang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GGX', '贡嘎县', 'Gongga', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SRX', '桑日县', 'Sangri', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QJX', '琼结县', 'Qiongjie', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QSX', '曲松县', 'Qusong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CMX', '措美县', 'Cuomei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZX', '洛扎县', 'Luoza', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JZX', '加查县', 'Jiacha', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LZ#', '隆子县', 'Longzi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CNX', '错那县', 'Cuona', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LKX', '浪卡子县', 'Langzika', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '山南市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NQX', '那曲县', 'Naqu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JLX', '嘉黎县', 'Jiali', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BRX', '比如县', 'Biru', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NRX', '聂荣县', 'Nierong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ADX', '安多县', 'Anduo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SZX', '申扎县', 'Shenza', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXX', '索县', 'Suoxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BGX', '班戈县', 'Bange', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BQX', '巴青县', 'Baqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NMX', '尼玛县', 'Nima', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHX', '双湖县', 'Shuanghu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '那曲地区') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PLX', '普兰县', 'Pulan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZDX', '札达县', 'Zada', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GEX', '噶尔县', 'Gaer', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RTX', '日土县', 'Ritu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GJX', '革吉县', 'Geji', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GZX', '改则县', 'Gaize', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CQX', '措勤县', 'Cuoqin', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '阿里地区') where parent_area_id = -1;

---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 新疆
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WLM', '乌鲁木齐市', 'Wulumuqi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'KLM', '克拉玛依市', 'Kalamayi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TLF', '吐鲁番市', 'Tulufan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HMS', '哈密市', 'Hami', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CJZ', '昌吉州', 'Changji', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BET', '博尔塔拉州', 'Boertala', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BYG', '巴音郭楞州', 'Bayinguoleng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'AKS', '阿克苏地区', 'Akesu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'KZL', '克孜勒苏州', 'Kezilesu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'KSQ', '喀什地区', 'Kashi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HTQ', '和田地区', 'Hetian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YLZ', '伊犁州', 'Yili', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TCQ', '塔城地区', 'Tacheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ALT', '阿勒泰地区', 'Aletai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZXX', '自治区直辖', 'Zhixia', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '新疆') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TSQ', '天山区', 'Tianshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYB', '沙依巴克区', 'Shayibake District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSQ', '新市区', 'Xinshi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SMG', '水磨沟区', 'Shuimogou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TTH', '头屯河区', 'Toutunhe District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DBC', '达坂城区', 'Dabancheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MDQ', '米东区', 'Midong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WLM', '乌鲁木齐县', 'Wulumuqi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '乌鲁木齐市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KLM', '克拉玛依', 'Kelamayi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DSZ', '独山子区', 'Dushanzi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BJT', '白碱滩区', 'Baijiantan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WEH', '乌尔禾区', 'Wuerhe District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '克拉玛依市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZQ', '伊州区', 'Yizhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BLK', '巴里坤县', 'Balikun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TKX', '托克逊县', 'Tuokexun', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '哈密市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CJS', '昌吉市', 'Changji', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FKS', '阜康市', 'Fukang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HTB', '呼图壁县', 'Hutubi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MNS', '玛纳斯县', 'Manasi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QTX', '奇台县', 'Qitai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JMS', '吉木萨尔县', 'Jimushaer', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLX', '木垒县', 'Mulei', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '昌吉州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BLS', '博乐市', 'Bole District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ALS', '阿拉山口市', 'Alashankou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JHX', '精河县', 'Jinghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WQX', '温泉县', 'Wenquan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '博尔塔拉州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KEL', '库尔勒市', 'Kuerle', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LTX', '轮台县', 'Luntai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLX', '尉犁县', 'Yuli', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RQX', '若羌县', 'Ruoqiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QMX', '且末县', 'Qiemo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YQX', '焉耆县', 'Yanqi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HJX', '和静县', 'Hejing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSX', '和硕县', 'Heshuo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BHX', '博湖县', 'Bihu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '巴音郭楞州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AKS', '阿克苏市', 'Akeshu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WSX', '温宿县', 'Wensu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KCX', '库车县', 'Kuche', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYX', '沙雅县', 'Shaya', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHX', '新和县', 'Xinhe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BCX', '拜城县', 'Baicheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WS#', '乌什县', 'Wushi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AWT', '阿瓦提县', 'Awati', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KPX', '柯坪县', 'Keping', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '阿克苏地区') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ATS', '阿图什市', 'Atushi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AKT', '阿克陶县', 'Aketao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AHQ', '阿合奇县', 'Aheqi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WQX', '乌恰县', 'Wuqia', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '克孜勒苏州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KSS', '喀什市', 'Kashi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SFX', '疏附县', 'Shufu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SLX', '疏勒县', 'Shule', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJS', '英吉沙县', 'Yingjisha', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PZX', '泽普县', 'Zepu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCX', '莎车县', 'Shache', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCX', '叶城县', 'Yecheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AMT', '麦盖提县', 'Maigaiti', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YPH', '岳普湖县', 'Yuepuhu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JSX', '伽师县', 'Jiashi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BCX', '巴楚县', 'Bachu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TSK', '塔什库尔干县', 'Tashikuergan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '喀什地区') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HTS', '和田市', 'Hetian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HTX', '和田县', 'Hetian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MYX', '墨玉县', 'Moyu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PSX', '皮山县', 'Pishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LPX', '洛浦县', 'Luopu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CLX', '策勒县', 'Chele', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YTX', '于田县', 'Yutian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MFX', '民丰县', 'Minfeng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '和田地区') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YNS', '伊宁市', 'Yining', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KTS', '奎屯市', 'Kuntun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HEG', '霍尔果斯市', 'Huoerguosi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YNX', '伊宁县', 'Yining', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CBC', '察布查尔县', 'Chabuchaer', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCX', '霍城县', 'Huocheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GLX', '巩留县', 'Gongliu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYX', '新源县', 'Xinyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZSX', '昭苏县', 'Zhaosu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TKS', '特克斯县', 'Tekesi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NLK', '尼勒克县', 'Nileke', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '伊犁州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TCS', '塔城市', 'Tacheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WSS', '乌苏市', 'Wusu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'EMX', '额敏县', 'Emin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SWX', '沙湾县', 'Shawan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TLX', '托里县', 'Tuoli', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YMX', '裕民县', 'Yumin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HBK', '和布克赛尔县', 'Hebutuokeshaier', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '塔城地区') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ALT', '阿勒泰市', 'Aletai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BEJ', '布尔津县', 'Buerjin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FYX', '富蕴县', 'Fuyun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FHX', '福海县', 'Fuhai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HBH', '哈巴河县', 'Habahe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QHX', '清河县', 'Qinghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JMN', '吉木乃县', 'Jimunai', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '阿勒泰地区') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHZ', '石河子市', 'Shihezi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ALE', '阿拉尔市', 'Alaer', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TMK', '图木舒克市', 'Tumushuke', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WJQ', '五家渠市', 'Wujiaqu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BTS', '北屯市', 'Beitun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TMG', '铁门关市', 'Tiemenguan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SHS', '双河市', 'Shuanghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KKD', '可克达拉市', 'Kekedala', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KYS', '昆玉市', 'Kunyu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '阿勒泰地区') where parent_area_id = -1;


---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 云南省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'KMS', '昆明市', 'Kunming', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QJS', '曲靖市', 'Qujing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YXS', '玉溪市', 'Yuxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BSS', '保山市', 'Baoshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZTS', '昭通市', 'Zhaotong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LJS', '丽江市', 'Lijiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'PES', '普洱市', 'Puer', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LCS', '临沧市', 'Lincang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CXZ', '楚雄州', 'Chuxiong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HHZ', '红河州', 'Honghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WSZ', '文山州', 'Wenshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XNB', '西双版纳州', 'Xishuangbanna', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DLZ', '大理州', 'Dali', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DHZ', '德宏州', 'Dehong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NJZ', '怒江州', 'Nujiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DQZ', '迪庆州', 'Diqing', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '云南') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WHQ', '五华区', 'Wuhua District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PLQ', '盘龙区', 'Panlong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GDQ', '官渡区', 'Guandu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SXQ', '西山区', 'Xishan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DCQ', '东川区', 'Dongchuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CGQ', '呈贡区', 'Chenggong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JNQ', '晋宁区', 'Jinning District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FMX', '富民县', 'Fumin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLX', '宜良县', 'Yiliang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SLX', '石林县', 'Shilin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SMX', '嵩明县', 'Songming', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LQX', '禄劝县', 'Luquan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XDX', '寻甸县', 'Xundian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ANS', '安宁市', 'Anning', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '昆明市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QLQ', '麒麟区', 'Qiling District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYQ', '沾益区', 'Zhanyi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLX', '马龙县', 'Nalong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LLX', '陆良县', 'Luliang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SZX', '师宗县', 'Shizong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LPX', '罗平县', 'Luoping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FYX', '富源县', 'Fuyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HZX', '会泽县', 'Huize', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XWS', '宣威市', 'Xuanwei', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '曲靖市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HTQ', '红塔区', 'Hongta District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BCQ', '江川区', 'Jiangchuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CJX', '澄江县', 'Cengjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'THX', '通海县', 'Tonghai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HNX', '华宁县', 'HUaning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YMX', '易门县', 'Yimen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ESX', '峨山县', 'Eshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XPX', '新平县', 'Xinping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJX', '元江县', 'Yuanjiang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '玉溪市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYQ', '隆阳区', 'Longyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SDX', '施甸县', 'Shidian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LLX', '龙陵县', 'Longling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CNX', '昌宁县', 'Changning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TCS', '腾冲市', 'Tengchong', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '保山市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYQ', '昭阳区', 'Zhaoyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LDX', '鲁甸县', 'Ludian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QJX', '巧家县', 'Qiaojia', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJX', '延津县', 'Yanjin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DGX', '大关县', 'Daguan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSX', '永善县', 'Yongshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SJX', '绥江县', 'Suijiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZXX', '镇雄县', 'Zhenxiong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLX', '彝良县', 'Yiliang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WXX', '威信县', 'Weixin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SFX', '水富县', 'Shuifu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '昭通市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GCQ', '古城区', 'Gucheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLX', '玉龙县', 'Yulong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSX', '永胜县', 'Yongsheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HPX', '华坪县', 'Huaping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NLX', '宁蒗县', 'Ninglang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '丽江市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SMQ', '思茅区', 'Simao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NEX', '宁洱县', 'Ninger', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MJX', '墨江县', 'Mojiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JDX', '景东县', 'Jingdong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JGX', '景谷县', 'JInggu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYX', '镇沅县', 'Zhenyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JCX', '江城县', 'Jiangcheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLX', '孟连县', 'Menglian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '澜沧县', 'Langcang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XMX', '西盟县', 'Ximeng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '普洱市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXQ', '临翔区', 'Linxiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FQX', '凤庆县', 'Fengqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YXX', '云县', 'Yunxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YDX', '永德县', 'Yongde', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZKX', '镇康县', 'Zhenkang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SJX', '双江县', 'Shuangjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GMX', '耿马县', 'Gengma', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CYX', '沧源县', 'Cangyuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '临沧市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CXS', '楚雄市', 'Chuxiong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SBX', '双柏县', 'Shuangbai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MDX', '牟定县', 'Mouding', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NHX', '南华县', 'Nanhua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YAX', '姚安县', 'Yaoan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DYX', '大姚县', 'Dayao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YRX', '永仁县', 'Yongren', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YMX', '元谋县', 'Yuanmou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WDX', '武定县', 'Wuding', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LFX', '禄丰县', 'Lufeng', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '楚雄州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GJS', '个旧市', 'Gejiu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KYS', '开远市', 'Kaiyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MZS', '蒙自市', 'Mengzi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLS', '弥勒市', 'Mile', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PBX', '屏边县', 'Pingbian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JSX', '建水县', 'Jianshui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SPX', '石屏县', 'Shiping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXX', '泸西县', 'Luxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YYX', '元阳县', 'Yuanyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HHX', '红河县', 'Honghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JPX', '金平县', 'Jinping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '绿春县', 'Lvchun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HKX', '河口县', 'Hekou', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '红河州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WSX', '文山县', 'Wenshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YSX', '砚山县', 'Yanshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCX', '西畴县', 'Xichou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLP', '麻栗坡县', 'Malipo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MGX', '马关县', 'Maguan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QBX', '丘北县', 'Qiubei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GNX', '广南县', 'Guangnan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FNX', '富宁县', 'Funing', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '文山州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JHS', '景洪市', 'Jinghong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MHX', '勐海县', 'Menghai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLX', '勐腊县', 'Mengla', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '西双版纳州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DLS', '大理市', 'Dali', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YBX', '漾濞县', 'Yangbi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYX', '祥云县', 'Xiangyun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BCX', '宾川县', 'Binchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MDX', '弥渡县', 'Midu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NJX', '南涧县', 'Nanjian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WSX', '巍山县', 'Weishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YPX', '永平县', 'Yongping', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLX', '云龙县', 'Yunlong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'EYX', '洱源县', 'Eryuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JCX', '剑川县', 'Jianchuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HQX', '鹤庆县', 'Heqing', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '大理州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RLS', '瑞丽市', 'Ruili', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MSS', '芒市', 'Mangshi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHX', '梁河县', 'Lianghe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJX', '盈江县', 'Yingjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCX', '陇川县', 'Longchuan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '德宏州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LSS', '泸水市', 'Lushui', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FGX', '福贡县', 'Fugong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GSX', '贡山县', 'Gongshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LPX', '兰坪县', 'Lanping', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '怒江州') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XGL', '香格里拉市', 'Xianggelila', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DQX', '德钦县', 'Deqin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WXX', '维西县', 'Weixi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '迪庆州') where parent_area_id = -1;


---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 浙江省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HZS', '杭州市', 'Hangzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NBS', '宁波市', 'Ningbo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WZS', '温州市', 'Wenbo', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JXS', '嘉兴市', 'Jiaxing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HZ#', '湖州市', 'Huzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SSS', '绍兴市', 'Shaoxing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JHS', '金华市', 'Jinhua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QZS', '衢州市', 'Quzhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZSS', '舟山市', 'Zhoushan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TZS', '台州市', 'Taizhou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LSS', '丽水市', 'Lishui', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '浙江') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCQ', '上城区', 'Shangcheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCQ', '下城区', 'Xiacheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JGQ', '江干区', 'Jianggan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GSQ', '拱墅区', 'Gongshu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHQ', '西湖区', 'Xihu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BJQ', '滨江区', 'Binjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSQ', '萧山区', 'Xiaoshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHQ', '余杭区', 'Yuhang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FYQ', '富阳区', 'Fuyang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TLX', '桐庐县', 'Tonglu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CAX', '淳安县', 'Cunan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JDS', '建德市', 'Jiande', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LAS', '临安市', 'Linan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '杭州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HSQ', '海曙区', 'Haishu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JBQ', '江北区', 'Jiangbei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BLQ', '北仑区', 'Beilun District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZHQ', '镇海区', 'Zhenhai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YZQ', '鄞州区', 'Yinzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FHQ', '奉化区', 'Fenghua District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSX', '象山县', 'Xiangshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NHX', '宁海县', 'Ninghai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YYS', '余姚市', 'Yuyao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CXS', '慈溪市', 'Cixi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '宁波市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LCQ', '鹿城区', 'Lucheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LWQ', '龙湾区', 'Longwan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'OHQ', '瓯海区', 'Ouhai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DTQ', '洞头区', 'Dongtou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YJX', '永嘉县', 'Yongjia', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PYX', '平阳县', 'Pingyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CNX', '苍南县', 'Cangnan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WCX', '文成县', 'Wencheng', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TSX', '泰顺县', 'Taishun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RAS', '瑞安市', 'Ruian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YQS', '乐清市', 'Leqing', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '温州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NHQ', '南湖区', 'Nanhu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XZQ', '秀洲区', 'Xiuzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JSX', '嘉善县', 'Jiashan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYX', '海盐县', 'Haiyan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HNS', '海宁市', 'Haining', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PHS', '平湖市', 'Pinghu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TXS', '桐乡市', 'Tongxiang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '嘉兴市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WXQ', '吴兴区', 'Wuxing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NXQ', '南浔区', 'Nanxun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DQX', '德清县', 'Deqing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CXX', '长兴县', 'Changxing', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'AJX', '安吉县', 'Anji', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '湖州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCQ', '越城区', 'Yuecheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KQQ', '柯桥区', 'Keqiao District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYQ', '上虞区', 'Shangyu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HCX', '新昌县', 'Xinchang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZJX', '诸暨市', 'Zhuji', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SZS', '嵊州市', 'Shengzhou', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '绍兴市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WCQ', '婺城区', 'Wucheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JDQ', '金东区', 'Jindong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WYX', '武义县', 'Wuyi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PJX', '浦江县', 'Pujiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PAX', '磐安县', 'Panan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LXS', '兰溪市', 'Lanxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YWS', '义乌市', 'Yiwu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DYS', '东阳市', 'Dongyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YKS', '永康市', 'Yongkang', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '金华市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KCQ', '柯城区', 'Kecheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QJQ', '衢江区', 'Qujiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CSX', '常山县', 'Changshan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'KHX', '开化县', 'Kaihua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYX', '龙游县', 'Longyou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JSS', '江山市', 'Jiangshan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '衢州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DHQ', '定海区', 'Dinghai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PTQ', '普陀区', 'Putuo District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DSX', '岱山县', 'Daishan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SSX', '嵊泗县', 'Shengsi', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '舟山市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JJQ', '椒江区', 'Jiaojiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HYQ', '黄岩区', 'Huangyan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LQQ', '路桥区', 'Luqiao', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHX', '玉环县', 'Yuhuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SMX', '三门县', 'Sanmen', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TTX', '天台县', 'Tiantai', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XJX', '仙居县', 'Xianju', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'WLS', '温岭市', 'Wenling', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LHS', '临海市', 'Linhai', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '台州市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LDQ', '莲都区', 'Liandu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QTX', '青田县', 'Qingtian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYX', '缙云县', 'Jinyun', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCX', '遂昌县', 'Suichang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SYX', '松阳县', 'Songyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHX', '云和县', 'Yunhe', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QYX', '庆元县', 'QIngyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JNX', '景宁县', 'Jingning', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LQQ', '龙泉市', 'Longquan', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '丽水市') where parent_area_id = -1;


---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 重庆市
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YZQ', '渝中区', 'Yuzhong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DDK', '大渡口区', 'Dadukou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BJQ', '江北区', 'Jiangbei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SPB', '沙坪坝区', 'hapingba District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JLP', '九龙坡区', 'Jiulongpo District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'NAQ', '南岸区', 'Nanan District ', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BPQ', '北培区', 'Beipei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YBQ', '渝北区', 'Yubei District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BNQ', '巴南区', 'Banan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WZQ', '万州区', 'Wanzhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'FLQ', '涪陵区', 'Fuling District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QJQ', '黔江区', 'Qianjiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CSQ', '长寿区', 'Changshou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JJQ', '江津区', 'Jiangjin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'HCQ', '合川区', 'Hechuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YCQ', '永川区', 'Yongchuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TCQ', '南川区', 'Nanchuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'QJ#', '綦江区', 'Qijiang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DZQ', '大足区', 'Dazhu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'BSQ', '璧山区', 'Bishan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TLQ', '铜梁区', 'Tongliang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TNQ', '潼南区', 'Tongnan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'RCQ', '荣昌区', 'ongchang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'KZQ', '开州区', 'Kaizhou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'LPQ', '梁平区', 'Liangping District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WLQ', '武隆区', 'Wulong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'CKX', '城口县', 'Chengkou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'FDX', '丰都县', 'Fengdu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'DJX', '垫江县', 'Dianjiang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZXX', '忠县', 'Zhongxian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YYX', '云阳县', 'Yunyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'FJX', '奉节县', 'Fengjie', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WSX', '巫山县', 'Wushan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'WXX', '巫溪县', 'Wuxi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'SZX', '石柱县', 'Shizhu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XSX', '秀山县', 'Xiushan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'YY#', '酉阳县', 'Youyang', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'PSX', '彭水县', 'Pengshui', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '重庆') where parent_area_id = -1;


---------------------------------------------------------------------------------------------------------------------------------------------------------
-- 台湾省
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TBS', '台北市', 'Taibei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'GXS', '高雄市', 'Gaoxiong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JLS', '基隆市', 'Jilong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TZS', '台中市', 'Taizhong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'TNS', '台南市', 'Tainan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'XZS', '新竹市', 'Xinzhu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'JYS', '嘉义市', 'Jiayi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'C', 'ZXX', '直辖县', 'Zhixia', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'P' and area_name = '台湾') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SCQ', '中正区', 'Zhongzheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XCQ', '大同区', 'Datong District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JGQ', '中山区', 'Zhongshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GSQ', '松山区', 'Songshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XHQ', '大安区', 'Daan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BJQ', '万华区', 'Wanhua District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSQ', '信义区', 'Xinyi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YHQ', '士林区', 'Shilin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'FYQ', '北投区', 'Beitou District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TLQ', '内湖区', 'Neihu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'CAQ', '南港区', 'Dangang District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JDQ', '文山区', 'Wenshan District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '台北市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XXQ', '新兴区', 'Xinxing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QJQ', '前金区', 'Qianjin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'LYQ', '芩雅区', 'Lingya District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YCQ', '盐埕区', 'Yancheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GSQ', '鼓山区', 'Gushan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QJ#', '旗津区', 'Qijin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QZQ', '前镇区', 'Qianzhen District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'SMQ', '三民区', 'Sanmin District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZYQ', '左营区', 'Zuoying District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NZQ', '楠梓区', 'Nanxing District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XGQ', '小港区', 'Xiaogang District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '高雄市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'RAQ', '仁爱区', 'Renai District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XYQ', '信义区', 'Xinyi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZZQ', '中正区', 'Zhongzheng District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZSQ', '中山区', 'Zhongshan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ALQ', '安乐区', 'Anle District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NNQ', '暖暖区', 'Nuannuan District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'QDQ', '七堵区', 'Qidu District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '基隆市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZQQ', '中区', 'Zhongqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DQQ', '东区', 'Dongqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NQQ', '南区', 'Nanqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XQQ', '西区', 'Xiqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BQQ', '北区', 'Beiqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BTQ', '北屯区', 'Beitun District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XTQ', '西屯区', 'Xitun District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NTQ', '南屯区', 'Nantun District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '台中市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZXQ', '中西区', 'Zhongxi District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DQQ', '东区', 'Dongqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NQQ', '南区', 'Nanqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BQQ', '北区', 'Beiqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'APQ', '安平区', 'Anping District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ANQ', '安南区', 'Annan District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '台南市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DQQ', '东区', 'Dongqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'BQQ', '北区', 'Beiqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XSQ', '香山区', 'Xiangshan District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '新竹市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'DQQ', '东区', 'Dongqu District', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XQQ', '西区', 'Xiqu District', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '嘉义市') where parent_area_id = -1;

insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TBX', '台北县', 'Taibei', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YLX', '宜兰县', 'Yilan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'XZX', '新竹县', 'Xinzhu', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TYX', '桃源县', 'Taoyuan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'MLX', '苗栗县', 'Miaoli', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TZX', '台中县', 'Taizhong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'ZHX', '彰化县', 'Zhanghua', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'NTX', '南投县', 'Nantou', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'JYX', '嘉义县', 'Jiayi', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'YL#', '云林县', 'Yunlin', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TNX', '台南县', 'Tainan', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'GXX', '高雄县', 'Gaoxiong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PDX', '屏东县', 'Pingdong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'TDX', '台东县', 'Taidong', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'HLX', '花莲县', 'Hualian', -1);
insert into system.area_code (area_type, area_code, area_name, area_name_en, parent_area_id) values( 'D', 'PHX', '澎湖县', 'Penghu', -1);
update system.area_code set parent_area_id = (select id from system.area_code where area_type = 'C' and area_name = '直辖县') where parent_area_id = -1;


