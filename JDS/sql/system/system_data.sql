-- 系统部分的基本参数，数据字典


-- 账户类型字典
INSERT INTO "system"."code_type" (code, name, description, status) VALUES ('ACCOUNT_TYPE', '账号类型', '', '1');
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('1', '主账号', '主账号', 'ACCOUNT_TYPE', '0',              '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('2', '子账号', '子账号', 'ACCOUNT_TYPE', '1',              '1' );

-- 账户状态字典
INSERT INTO "system"."code_type" (code, name, description, status) VALUES ('ACCOUNT_STATUS', '用户类型', '', '1');
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('1', '未激活', '未激活',   'ACCOUNT_STATUS', '1', '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('2', '正常',   '正常',     'ACCOUNT_STATUS', '2', '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('3', '冻结',   '冻结',     'ACCOUNT_STATUS', '3', '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('4', '删除',   '删除',     'ACCOUNT_STATUS', '4', '1' );


-- 字典类型
INSERT INTO "system"."code_type" (code, name, description, status) VALUES ('CODE_TYPE_STATUS', '字典类型', '', '1');
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('0', '停用', '停用', 'CODE_TYPE_STATUS', '0',              '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('1', '启用', '启用', 'CODE_TYPE_STATUS', '1',              '1' );


INSERT INTO "system"."code_type" (code, name, description, status) VALUES ('CODE_VALUE_STATUS', '数据字典数据状态', '数据字典数据状态', '1');
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('0', '停用', '停用', 'CODE_VALUE_STATUS', '0',             '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('1', '启用', '启用', 'CODE_VALUE_STATUS', '1',             '1' );

-- 学历字典
INSERT INTO "system"."code_type" (code, name, description, status) VALUES ('EDUCATION_LEVEL', '学历', '学历', '1');
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('1', '博士后', '博士后', 'EDUCATION_LEVEL', '1',           '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('2', '博士', '博士', 'EDUCATION_LEVEL', '2',               '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('3', '硕士', '硕士', 'EDUCATION_LEVEL', '3',               '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('4', '本科', '本科', 'EDUCATION_LEVEL', '4',               '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('5', '大专', '大专', 'EDUCATION_LEVEL', '5',               '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('6', '中专', '中专', 'EDUCATION_LEVEL', '6',               '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('7', '高中', '高中', 'EDUCATION_LEVEL', '7',               '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('8', '初中', '初中', 'EDUCATION_LEVEL', '8',               '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('9', '小学', '小学', 'EDUCATION_LEVEL', '9',               '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('10', '无学历', '无学历', 'EDUCATION_LEVEL', '10',         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('11', '未知', '未知', 'EDUCATION_LEVEL', '11',             '1' );

-- 婚姻状态字典
INSERT INTO "system"."code_type" (code, name, description, status) VALUES ('MARITAL_STATUS', '婚姻状态', '婚姻状态字典', '1');
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('1',  '未婚', '', 'MARITAL_STATUS', '1',                   '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('2',  '已婚', '', 'MARITAL_STATUS', '2',                   '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('3',  '丧偶', '', 'MARITAL_STATUS', '3',                   '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('4',  '离婚', '', 'MARITAL_STATUS', '4',                   '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('5',  '其他', '', 'MARITAL_STATUS', '5',                   '1' );

-- 民族字典
INSERT INTO "system"."code_type" (code, name, description, status) VALUES ('NATION', '民族', '民族字典', '1');
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('01', '汉族', '', 'NATION', '1',                           '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('02', '蒙古族', '', 'NATION', '2',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('03', '回族', '', 'NATION', '3',                           '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('04', '藏族', '', 'NATION', '4',                           '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('05', '维吾尔族', '', 'NATION', '5',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('06', '苗族', '', 'NATION', '6',                           '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('07', '彝族', '', 'NATION', '7',                           '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('08', '壮族', '', 'NATION', '8',                           '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('09', '布依族', '', 'NATION', '9',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('10', '朝鲜族', '', 'NATION', '10',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('11', '满族', '', 'NATION', '11',                          '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('12', '侗族', '', 'NATION', '12',                          '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('13', '瑶族', '', 'NATION', '13',                          '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('14', '白族', '', 'NATION', '14',                          '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('15', '土家族', '', 'NATION', '15',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('16', '哈尼族', '', 'NATION', '16',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('17', '哈萨克族', '', 'NATION', '17',                      '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('18', '傣族', '', 'NATION', '18',                          '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('19', '黎族', '', 'NATION', '19',                          '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('20', '傈僳族', '', 'NATION', '20',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('21', '佤族', '', 'NATION', '21',                          '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('22', '畲族', '', 'NATION', '22',                          '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('23', '高山族', '', 'NATION', '23',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('24', '拉祜族', '', 'NATION', '24',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('25', '水族', '', 'NATION', '25',                          '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('26', '东乡族', '', 'NATION', '26',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('27', '纳西族', '', 'NATION', '27',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('28', '景颇族', '', 'NATION', '28',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('29', '柯尔克孜族', '', 'NATION', '29',                    '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('30', '土族', '', 'NATION', '30',                          '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('31', '达翰尔族', '', 'NATION', '31',                      '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('32', '仫佬族', '', 'NATION', '32',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('33', '羌族', '', 'NATION', '33',                          '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('34', '布朗族', '', 'NATION', '34',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('35', '撒拉族', '', 'NATION', '35',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('36', '毛南族', '', 'NATION', '36',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('37', '仡佬族', '', 'NATION', '37',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('38', '锡伯族', '', 'NATION', '38',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('39', '阿昌族', '', 'NATION', '39',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('40', '普米族', '', 'NATION', '40',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('41', '塔吉克族', '', 'NATION', '41',                      '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('42', '怒族', '', 'NATION', '42',                          '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('43', '乌孜别克族', '', 'NATION', '43',                    '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('44', '俄罗斯族', '', 'NATION', '44',                      '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('45', '鄂温克族', '', 'NATION', '45',                      '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('46', '德昂族', '', 'NATION', '46',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('47', '保安族', '', 'NATION', '47',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('48', '裕固族', '', 'NATION', '48',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('49', '京族', '', 'NATION', '49',                          '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('50', '塔塔尔族', '', 'NATION', '50',                      '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('51', '独龙族', '', 'NATION', '51',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('52', '鄂伦春族', '', 'NATION', '52',                      '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('53', '赫哲族', '', 'NATION', '53',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('54', '门巴族', '', 'NATION', '54',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('55', '珞巴族', '', 'NATION', '55',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('56', '基诺族', '', 'NATION', '56',                        '1' );


-- 性别字典
INSERT INTO "system"."code_type" (code, name, description, status) VALUES ('SEX', '性别', '性别字典', '1');
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('1',   '男', '', 'SEX', '1',                               '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('2',   '女', '', 'SEX', '2',                               '1' );

-- 国家字典
INSERT INTO "system"."code_type" (code, name, description, status) VALUES ('COUNTRY', '国家', '国家', '1');
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('AE', '阿联酋', '', 'COUNTRY', '2',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('AF', '阿富汗', '', 'COUNTRY', '3',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('AG', '安提瓜和巴布达', '', 'COUNTRY', '4',                '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('AI', '安圭拉', '', 'COUNTRY', '5',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('AL', '阿尔巴尼亚', '', 'COUNTRY', '6',                    '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('AM', '赞比亚', '', 'COUNTRY', '237',                      '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('AN', '荷属安的列斯', '', 'COUNTRY', '8',                  '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('AO', '安哥拉', '', 'COUNTRY', '9',                        '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('AQ', '南极洲', '', 'COUNTRY', '10',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('AR', '阿根廷', '', 'COUNTRY', '11',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('AS', '美属萨摩亚', '', 'COUNTRY', '12',                   '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('AT', '奥地利', '', 'COUNTRY', '13',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('AU', '澳大利亚', '', 'COUNTRY', '14',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('AW', '阿鲁巴', '', 'COUNTRY', '15',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('AZ', '阿塞拜疆', '', 'COUNTRY', '16',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BA', '波斯尼亚和黑塞哥维那', '', 'COUNTRY', '17',         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BB', '巴巴多斯', '', 'COUNTRY', '18',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BD', '孟加拉国', '', 'COUNTRY', '19',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BE', '比利时', '', 'COUNTRY', '20',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BF', '布基纳法索', '', 'COUNTRY', '21',                   '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BG', '保加利亚', '', 'COUNTRY', '22',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BH', '巴林', '', 'COUNTRY', '23',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BI', '布隆迪', '', 'COUNTRY', '24',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BJ', '贝宁', '', 'COUNTRY', '25',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BM', '百慕大', '', 'COUNTRY', '26',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BN', '文莱', '', 'COUNTRY', '27',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BO', '玻利维亚', '', 'COUNTRY', '28',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BR', '巴西', '', 'COUNTRY', '29',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BS', '巴哈马', '', 'COUNTRY', '30',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BT', '不丹', '', 'COUNTRY', '31',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BV', '布维岛', '', 'COUNTRY', '32',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BW', '博茨瓦纳', '', 'COUNTRY', '33',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BY', '白俄罗斯', '', 'COUNTRY', '34',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('BZ', '伯利兹', '', 'COUNTRY', '35',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CA', '加拿大', '', 'COUNTRY', '36',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CC', '科科斯（基林）群岛', '', 'COUNTRY', '37',           '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CF', '中非', '', 'COUNTRY', '38',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CG', '刚果', '', 'COUNTRY', '39',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CH', '瑞士', '', 'COUNTRY', '40',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CI', '科特迪瓦', '', 'COUNTRY', '41',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CK', '库克群岛', '', 'COUNTRY', '42',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CL', '智利', '', 'COUNTRY', '43',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CM', '喀麦隆', '', 'COUNTRY', '44',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CN', '中国', '', 'COUNTRY', '45',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CO', '哥伦比亚', '', 'COUNTRY', '46',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CR', '哥斯达黎加', '', 'COUNTRY', '47',                   '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CT', '中国台湾', '', 'COUNTRY', '48',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CU', '古巴', '', 'COUNTRY', '49',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CV', '佛得角', '', 'COUNTRY', '50',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CX', '圣诞岛', '', 'COUNTRY', '51',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CY', '塞浦路斯', '', 'COUNTRY', '52',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('CZ', '捷克', '', 'COUNTRY', '53',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('DE', '德国', '', 'COUNTRY', '54',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('DJ', '吉布提', '', 'COUNTRY', '55',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('DK', '丹麦', '', 'COUNTRY', '56',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('DM', '多米尼克', '', 'COUNTRY', '57',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('DO', '多米尼加共和国', '', 'COUNTRY', '58',               '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('DZ', '阿尔及利亚', '', 'COUNTRY', '59',                   '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('EC', '厄瓜多尔', '', 'COUNTRY', '60',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('EE', '爱沙尼亚', '', 'COUNTRY', '61',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('EG', '埃及', '', 'COUNTRY', '62',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('EH', '西撒哈拉', '', 'COUNTRY', '63',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('ER', '厄立特里亚', '', 'COUNTRY', '64',                   '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('ES', '西班牙', '', 'COUNTRY', '65',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('ET', '埃塞俄比亚', '', 'COUNTRY', '66',                   '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('FI', '芬兰', '', 'COUNTRY', '67',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('FJ', '斐济', '', 'COUNTRY', '68',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('FK', '马尔维纳斯群岛（福克兰群岛）', '', 'COUNTRY', '69', '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('FM', '密克罗尼西亚', '', 'COUNTRY', '70',                 '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('FO', '法罗群岛', '', 'COUNTRY', '71',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('FR', '法国', '', 'COUNTRY', '72',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GA', '加蓬', '', 'COUNTRY', '73',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GB', '英国', '', 'COUNTRY', '74',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GD', '格林纳达', '', 'COUNTRY', '75',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GE', '格鲁吉亚', '', 'COUNTRY', '76',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GF', '法属圭亚那', '', 'COUNTRY', '77',                   '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GH', '加纳', '', 'COUNTRY', '78',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GI', '直布罗陀', '', 'COUNTRY', '79',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GL', '格陵兰', '', 'COUNTRY', '80',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GM', '冈比亚', '', 'COUNTRY', '81',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GN', '几内亚', '', 'COUNTRY', '82',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GP', '瓜德罗普', '', 'COUNTRY', '83',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GQ', '赤道几内亚', '', 'COUNTRY', '84',                   '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GR', '希腊', '', 'COUNTRY', '85',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GS', '南乔治亚岛和南桑德韦奇岛', '', 'COUNTRY', '86',     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GT', '危地马拉', '', 'COUNTRY', '87',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GU', '关岛', '', 'COUNTRY', '88',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GW', '几内亚比绍', '', 'COUNTRY', '89',                   '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('GY', '圭亚那', '', 'COUNTRY', '90',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('HK', '香港', '', 'COUNTRY', '91',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('HM', '赫德岛和麦克唐纳岛', '', 'COUNTRY', '92',           '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('HN', '洪都拉斯', '', 'COUNTRY', '93',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('HR', '克罗地亚', '', 'COUNTRY', '94',                     '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('HT', '海地', '', 'COUNTRY', '95',                         '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('HU', '匈牙利', '', 'COUNTRY', '96',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('ID', '印度尼西亚', '', 'COUNTRY', '97',                   '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('IE', '爱尔兰', '', 'COUNTRY', '98',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('IL', '以色列', '', 'COUNTRY', '99',                       '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('IN', '印度', '', 'COUNTRY', '100',                        '1' );

-- 语言字典
INSERT INTO "system"."code_type" (code, name, description, status) VALUES ('SYSTEM_LANGUAGE', '系统语言', '系统语言', '1');
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('cn', '中文', '中文', 'SYSTEM_LANGUAGE', '0',              '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('en', '英文', '英文', 'SYSTEM_LANGUAGE', '1',              '1' );

-- 流程状态字典
INSERT INTO "system"."code_type" (code, name, description, status) VALUES ('FLOW_STATUS', '审批流程状态', '审批流程状态', '1');
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('1', '未提交', '流程未提交',     'FLOW_STATUS', '1', '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('2', '待审核', '流程待审核',     'FLOW_STATUS', '2', '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('3', '被驳回', '流程待被驳回',   'FLOW_STATUS', '3', '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('4', '取消',   '流程被取消',     'FLOW_STATUS', '4', '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('5', '已审核', '流程已通过审核', 'FLOW_STATUS', '5', '1' );

-- 业务模块列表
INSERT INTO "system"."code_type" (code, name, description, status) VALUES ('USER_SERVICE_TYPE', '用户业务类型', '用户业务类型', '1');
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('1', 'PIS', 'PIS',           'USER_SERVICE_TYPE', '1', '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('2', 'IOT', '物联网设备管理','USER_SERVICE_TYPE', '2', '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('3', 'iMRO', '智能检修业务', 'USER_SERVICE_TYPE', '3', '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('4', 'oa',   'OA办公',       'USER_SERVICE_TYPE', '4', '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('5', 'stock', '股票分析系统','USER_SERVICE_TYPE', '5', '1' );
INSERT INTO "system"."code_value" ( code, name, description, type_code, sort, status ) VALUES ('6', 'rsu', 'rsu管理系统','USER_SERVICE_TYPE', '6', '1' );


-- 设备状态
delete from system.code_type where code='DEVICE_STATUS_TYPE';
insert into system.code_type ( code, name, description, status ) values( 'DEVICE_STATUS_TYPE', '设备状态', '设备状态', '1');

delete from system.code_value where type_code='DEVICE_STATUS_TYPE';
insert into system.code_value ( code, name, description, type_code, sort, status ) values('1', '正常', '正常', 'DEVICE_STATUS_TYPE', '1', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('2', '维护中', '维护中', 'DEVICE_STATUS_TYPE', '2', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('3', '系统高负荷运行', '系统高负荷运行', 'DEVICE_STATUS_TYPE', '3', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('4', '设备部件故障', '设备部件故障', 'DEVICE_STATUS_TYPE', '4', '1');
insert into system.code_value ( code, name, description, type_code, sort, status ) values('5', '设备未连接', '设备未连接', 'DEVICE_STATUS_TYPE', '5', '1');
update system.code_value set type_id = (select ct.id from system.code_type ct where ct.code = type_code);

