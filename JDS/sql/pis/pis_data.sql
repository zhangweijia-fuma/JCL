-- 成都地铁运营线路、车站等相关数据

-- 涉及到时间、IP地址等数据需要根据当时情况进行修正

-- 新增运营线路，成都地铁规划共38条线
delete from pis.operation_line;
create or replace function InsertOperationLine() returns 
void AS $$
declare i integer;
    begin
        i:=1;
        FOR i IN 1..40 LOOP
        	if (i < 10) then
        		insert into pis.operation_line (name, name_en, code, memo) 
	                values ( '成都地铁'||to_char(i, 'fm9')||'号线', 'Chengdu Metro '||to_char(i, 'fm9'), to_char(i, 'fm9'), '');
        	else
	            insert into pis.operation_line (name, name_en, code, memo) 
	                values ( '成都地铁'||to_char(i, 'fm09')||'号线', 'Chengdu Metro '||to_char(i, 'fm09'), to_char(i, 'fm09'), '');
	        end if;
        end loop;
  end;
$$ LANGUAGE plpgsql;

select InsertOperationLine();



-- ----------------------------
-- Records of sub_line_info
-- ----------------------------
-- INSERT INTO "pis"."sub_line_info" (sub_line_name, sub_line_name_en, up_start_time, up_end_time, down_start_time, down_end_time)
--     VALUES ( '主线', 'MainLine', '37', '6', '6', '37', '06:00', '23:30', '06:00', '23:30');
-- INSERT INTO "pis"."sub_line_info" (sub_line_name, sub_line_name_en, up_start_time, up_end_time, down_start_time, down_end_time) 
--     VALUES ( '支线1', 'sublin1', '33', '7', '7', '33', '06:00', '23:55', '06:00', '23:45');

-- -- 支线的起始站点ID需要单独更新
-- update pis.sub_line_info set line_id = ( select id from pis.operation_line where name='成都地铁2号线' );


-- ----------------------------
-- Records of station
-- ----------------------------

delete from pis.station;

-- 成都地铁1号线车站
INSERT INTO "pis"."station" (name, name_en) VALUES ('韦家碾', 'Weijianian');
INSERT INTO "pis"."station" (name, name_en) VALUES ('升仙湖', 'Shengxian Lake');
INSERT INTO "pis"."station" (name, name_en) VALUES ('火车北站', 'North Railway Station');
INSERT INTO "pis"."station" (name, name_en) VALUES ('人民北路', 'North Renmin Road');
INSERT INTO "pis"."station" (name, name_en) VALUES ('文殊院', 'Wenshu Monastery');
INSERT INTO "pis"."station" (name, name_en) VALUES ('骡马市', 'Luomashi');
INSERT INTO "pis"."station" (name, name_en) VALUES ('天府广场', 'Tianfu Square');
INSERT INTO "pis"."station" (name, name_en) VALUES ('锦江宾馆', 'Jinjiang Hotel');
INSERT INTO "pis"."station" (name, name_en) VALUES ('华西坝', 'Huaxiba');
INSERT INTO "pis"."station" (name, name_en) VALUES ('省体育馆', 'Sichuan Gymnasium');
INSERT INTO "pis"."station" (name, name_en) VALUES ('倪家桥', 'Nijiaqiao');
INSERT INTO "pis"."station" (name, name_en) VALUES ('桐梓林', 'Tongzilin');
INSERT INTO "pis"."station" (name, name_en) VALUES ('火车南站', 'South Railway Station');
INSERT INTO "pis"."station" (name, name_en) VALUES ('高新', 'Hi-Tech Zone');
INSERT INTO "pis"."station" (name, name_en) VALUES ('金融城', 'Financial City');
INSERT INTO "pis"."station" (name, name_en) VALUES ('孵化园', 'Incubation Park');
INSERT INTO "pis"."station" (name, name_en) VALUES ('锦城广场', 'Jincheng Plaza');
INSERT INTO "pis"."station" (name, name_en) VALUES ('世纪城', 'Century City');
INSERT INTO "pis"."station" (name, name_en) VALUES ('天府三街', 'Tianfu 3rd Street');
INSERT INTO "pis"."station" (name, name_en) VALUES ('天府五街', 'Tianfu 5rd Street');
INSERT INTO "pis"."station" (name, name_en) VALUES ('华府大道', 'Huafu Avenue');
INSERT INTO "pis"."station" (name, name_en) VALUES ('四河', 'Sihe');
INSERT INTO "pis"."station" (name, name_en) VALUES ('广都', 'Guangdu');
INSERT INTO "pis"."station" (name, name_en) VALUES ('五根松', 'Wugensong');
INSERT INTO "pis"."station" (name, name_en) VALUES ('华阳', 'Huayang');
INSERT INTO "pis"."station" (name, name_en) VALUES ('海昌路', 'Haichang Road');
INSERT INTO "pis"."station" (name, name_en) VALUES ('广福', 'Guangfu');
INSERT INTO "pis"."station" (name, name_en) VALUES ('红石公园', 'Hongshi Park');
INSERT INTO "pis"."station" (name, name_en) VALUES ('麓湖', 'Luhu Lake');
INSERT INTO "pis"."station" (name, name_en) VALUES ('武汉路', 'Wuhan Road');
INSERT INTO "pis"."station" (name, name_en) VALUES ('天府公园', 'Tianfu Park');
INSERT INTO "pis"."station" (name, name_en) VALUES ('西博城', 'Western China International Expo City');
INSERT INTO "pis"."station" (name, name_en) VALUES ('广州路', 'Guangzhou Road');
INSERT INTO "pis"."station" (name, name_en) VALUES ('兴隆湖', 'Xinglong Lake');
INSERT INTO "pis"."station" (name, name_en) VALUES ('科学城', 'Science City');
update pis.station set type = 'N', status = '5', code = 'N-'||to_char(id, 'fm099999');


-- 设置车站和运营线路的关系
delete from pis.station_line;
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='韦家碾';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='升仙湖';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='火车北站';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='人民北路';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='文殊院';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='骡马市';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='天府广场';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='锦江宾馆';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='华西坝';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='省体育馆';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='倪家桥';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='桐梓林';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='火车南站';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='高新';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='金融城';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='孵化园';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='锦城广场';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='世纪城';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='天府三街';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='天府五街';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='华府大道';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='四河';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='广都';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='五根松';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='华阳';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='海昌路';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='广福';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='红石公园';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='麓湖';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='武汉路';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='天府公园';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='西博城';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='广州路';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='兴隆湖';
INSERT INTO pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁1号线' where a.name ='科学城';
update pis.station_line set sort = ( 201 + id - (SELECT Min (ID) FROM pis.station_line A WHERE A.line_id = (SELECT ID FROM pis.operation_line WHERE NAME = '成都地铁1号线') ) ),  
    up_start_time='06:00', up_end_time='23:30', down_start_time='06:00', down_end_time='23:00'  where line_id = (select id from pis.operation_line where name='成都地铁1号线');


-- 2号线车站
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('龙泉驿', 'Longquanyi', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('龙平路', 'Longping Road', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('书房', 'Shufang', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('界牌', 'Jiepai', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('连山坡', 'Lianshanpo', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('大面铺', 'Damianpu', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('成都行政学院', 'Chengdu Institute of Public Administration', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('洪河', 'Honghe', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('惠王陵', 'Huiwanglin', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('成渝立交', 'Chengyu Flyover', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('成都东客站', 'East Chengdu', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('塔子山公园', 'Tazishan Park', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('东大路', 'Dongda Road', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('牛市口', 'Niushikou', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('牛王庙', 'Niuwangmiao', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('东门大桥', 'Dongmen Bridge', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('春熙路', 'Chunxi Road', 'N');
-- INSERT INTO "pis"."station" (name, name_en, type) VALUES ('天府广场', 'Tianfu Square', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('人民公园', 'People‘s Park', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('通惠门', 'Tonghuimen', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('中医药大学', 'Chengdu University of TCM & Sichuan Provincial People‘s Hospital', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('白果林', 'Baiguolin', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('蜀汉路东', 'East Shuhan Road', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('一品天下', 'Yipintianxia', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('羊犀立交', 'Yangxi Flyover', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('茶店子', 'Chadianzi Bus Terminal', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('迎宾大道', 'Yingbin Avenue', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('金科北路', 'Jinke Road North', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('金周路', 'Jinzhou Road', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('百草路', 'Baicao Road', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('天河路', 'Tianhe Road', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('犀浦', 'Xipu', 'N');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('洪柳车辆段', 'Hongliu', 'S');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('红光停车场', 'Hongguang', 'S');
INSERT INTO "pis"."station" (name, name_en, type) VALUES ('控制中心', 'ControlCneter', 'S');
update pis.station set status = '5', code = 'N-'||to_char(id, 'fm099999') where code is null and type = 'N';
update pis.station set status = '5', code = 'S-'||to_char(id, 'fm099999') where code is null and type = 'S';

-- ----------------------------
-- Records of station_line
-- ----------------------------

-- 设置车站和运营线路的关系
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='龙泉驿';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='龙平路';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='书房';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='界牌';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='连山坡';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='大面铺';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='成都行政学院';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='洪河';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='惠王陵';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='成渝立交';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='成都东客站';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='塔子山公园';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='东大路';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='牛市口';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='牛王庙';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='东门大桥';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='春熙路';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='天府广场';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='人民公园';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='通惠门';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='中医药大学';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='白果林';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='蜀汉路东';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='一品天下';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='羊犀立交';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='茶店子';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='迎宾大道';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='金科北路';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='金周路';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='百草路';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='天河路';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='犀浦';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='洪柳车辆段';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='红光停车场';
insert into pis.station_line (station_id, line_id ) select a.id, b.id from pis.station a inner join pis.operation_line b on b.name = '成都地铁2号线' where a.name ='控制中心';
update pis.station_line set sort = ( 401 + id - (SELECT Min (ID) FROM pis.station_line A WHERE A.line_id = (SELECT ID FROM pis.operation_line WHERE NAME = '成都地铁2号线') ) ),  
    up_start_time='06:00', up_end_time='23:30', down_start_time='06:00', down_end_time='23:00'  where line_id = (select id from pis.operation_line where name='成都地铁2号线');

-- ----------------------------
-- Records of train
-- ----------------------------

-- 新增列车，每条线各60列
delete from pis.train;
create or replace function InsertTrain() returns 
void AS $$
declare i integer;
    begin
        i:=1;
        FOR i IN 1..60 LOOP
            insert into pis.train (name, name_en, code, description) 
                values ( '和谐号'||to_char(i, 'fm09'), 'Hexie' ||to_char(i, 'fm09'), 'LC-001-'||to_char(i, 'fm099999'), '');
        end loop;

        FOR i IN 1..60 LOOP
            insert into pis.train (name, name_en, code, description) 
                values ( '复兴号'||to_char(i, 'fm09'), 'Fuxing'||to_char(i, 'fm09'), 'LC-002-'||to_char(i, 'fm099999'), '');
        end loop;

        update pis.train set line_id = (select id from pis.operation_line where name = '成都地铁1号线' ) where name_en like 'He%';
        update pis.train set line_id = (select id from pis.operation_line where name = '成都地铁2号线' ) where name_en like 'Fu%';
        update pis.train set sort = id;

  end;
$$ LANGUAGE plpgsql;

select InsertTrain();


-- ----------------------------
-- Records of controller
-- ----------------------------

delete from pis.controller;
create or replace function InsertControllerLine() returns 
void AS $$
declare i integer;
    begin

    -- 1号线车站控制器
    i:=1;
    FOR i IN 1..35 LOOP
        insert into pis.controller (serial_no, name, device_type, install_type, sort, open_time, close_time, status) 
            values ('N-'||to_char(i, 'fm099999'), '上行控制器', '1', '2', (200+i)*10 + 2, '06:00', '23:30', '5' );
        insert into pis.controller (serial_no, name, device_type, install_type, sort, open_time, close_time, status)     
            values ('N-'||to_char(i, 'fm099999'), '下行控制器', '1', '3', (200+i)*10 + 3, '06:00', '23:30', '5' );
        insert into pis.controller (serial_no, name, device_type, install_type, sort, open_time, close_time, status) 
            values ('N-'||to_char(i, 'fm099999'), '站厅控制器', '1', '4', (200+i)*10 + 4, '06:00', '23:30', '5' );
    end loop;

    -- 1号线列车控制器
    i:=1;
    FOR i IN 1..60 LOOP
        insert into pis.controller (serial_no, name, device_type, install_type, sort, open_time, close_time, status) 
            values ('LC-001-'||to_char(i, 'fm099999'), '车头控制器', '1', '0', i*10,  '06:00', '23:30', '5' );
        insert into pis.controller (serial_no, name, device_type, install_type, sort, open_time, close_time, status) 
            values ('LC-001-'||to_char(i, 'fm099999'), '车尾控制器', '1', '1', i*10 + 1, '06:00', '23:30', '5' );
    end loop;
    update pis.controller set line_id = ( select id from pis.operation_line where name='成都地铁1号线' ) where line_id is null;

    -- 2号线车站控制器
    i:=36;
    FOR i IN 36..66 LOOP
        insert into pis.controller (serial_no, name, device_type, install_type, sort, open_time, close_time, status) 
            values ('N-'||to_char(i, 'fm099999'), '上行控制器', '1', '2', (200+i-35)*10 + 2, '06:00', '23:30', '5' );
        insert into pis.controller (serial_no, name, device_type, install_type, sort, open_time, close_time, status)     
            values ('N-'||to_char(i, 'fm099999'), '下行控制器', '1', '3', (200+i-35)*10 + 3, '06:00', '23:30', '5' );
        insert into pis.controller (serial_no, name, device_type, install_type, sort, open_time, close_time, status) 
            values ('N-'||to_char(i, 'fm099999'), '站厅控制器', '1', '4', (200+i-35)*10 + 4, '06:00', '23:30', '5' );
    end loop;

    -- 车辆段控制器
    i:=67;
    FOR i IN 67..69 LOOP
        insert into pis.controller (serial_no, name, device_type, install_type, sort, open_time, close_time, status) 
            values ('S-'||to_char(i, 'fm099999'), '上行控制器', '1', '2', (200+i-35)*10 + 2, '06:00', '23:30', '5' );
        insert into pis.controller (serial_no, name, device_type, install_type, sort, open_time, close_time, status)     
            values ('S-'||to_char(i, 'fm099999'), '下行控制器', '1', '3', (200+i-35)*10 + 3, '06:00', '23:30', '5' );
        insert into pis.controller (serial_no, name, device_type, install_type, sort, open_time, close_time, status) 
            values ('S-'||to_char(i, 'fm099999'), '站厅控制器', '1', '4', (200+i-35)*10 + 4, '06:00', '23:30', '5' );
    end loop;

    -- 2号线列车控制器
    i:=61;
    FOR i IN 61..120 LOOP
        insert into pis.controller (serial_no, name, device_type, install_type, sort, open_time, close_time, status) 
            values ('LC-002-'||to_char(i-60, 'fm099999'), '车头控制器', '1', '0', i*10,  '06:00', '23:30', '5' );
        insert into pis.controller (serial_no, name, device_type, install_type, sort, open_time, close_time, status) 
            values ('LC-002-'||to_char(i-60, 'fm099999'), '车尾控制器', '1', '1', i*10 + 1, '06:00', '23:30', '5' );
    end loop;
    update pis.controller set line_id = ( select id from pis.operation_line where name='成都地铁2号线' ) where line_id is null;

    update pis.controller a set install_id = ( select id from pis.train b where a.serial_no = b.code ) where a.install_type < '2';
    update pis.controller a set install_id = ( select id from pis.station b where a.serial_no = b.code ) where a.install_type > '1';

    update pis.controller set code = 'S-'||to_char( (select b.sort from pis.station_line b inner join pis.operation_line c on c.name = '成都地铁1号线' where b.station_id = install_id and line_id = c.id), 'fm099999') || '-' || install_type where install_type > '1';
    update pis.controller set code = 'S-'||to_char( (select b.sort from pis.station_line b inner join pis.operation_line c on c.name = '成都地铁2号线' where b.station_id = install_id and line_id = c.id), 'fm099999') || '-' || install_type where install_type > '1' and code is null;
    update pis.controller set code = 'T-'||to_char( (select b.sort from pis.train b where b.id = install_id), 'fm099999') || '-' || install_type where install_type < '2';

    update pis.controller set serial_no = null;

  end;
$$ LANGUAGE plpgsql;

select InsertControllerLine();
