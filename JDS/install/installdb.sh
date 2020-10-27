#!/bin/bash


# 脚本由应用程序启动
# 参数1：固定为 JamesSoft，以防止手动启动脚本
Usage()
{
    echo
    echo "Usage: This script should be run by application and can not be run from common line."
    echo
}

# 创建数据表格
CreateTable()
{
    schemaname=$1
    tablename=$2

    # 执行sql文件
    filename=$SRVDIR/sql/$schemaname/$tablename.sql
    # psql -U postgres -d $databaseName -f $filename

    # 创建历史数据表
    sequencename=$tablename"_history"

    # psql -U postgres -d JamesSoft -c "drop table history.$tablename"
    psql -U postgres -d $databaseName -c "create table history.$tablename as (select * from $schemaname.$tablename limit 0)"
    psql -U postgres -d $databaseName -c "ALTER TABLE history.$tablename ADD PRIMARY KEY ("id");"
    # psql -U postgres -d $databaseName -c "drop SEQUENCE $sequencename;"   
    psql -U postgres -d $databaseName -c "CREATE SEQUENCE $sequencename START WITH 1;"
    psql -U postgres -d $databaseName -c "ALTER TABLE history.$tablename ALTER COLUMN id SET DEFAULT nextval('$sequencename'::regclass);"

    psql -U postgres -d $databaseName -c "ALTER TABLE history.$tablename add COLUMN op_user_id int"
    psql -U postgres -d $databaseName -c "ALTER TABLE history.$tablename add COLUMN op_datetime varchar(50)"
    psql -U postgres -d $databaseName -c "ALTER TABLE history.$tablename add COLUMN orginal_id int"

    psql -U postgres -d $databaseName -c "CREATE INDEX $tablename_op_user_INDEX ON history.$tablename USING btree ("op_user_id")"
    psql -U postgres -d $databaseName -c "CREATE INDEX $tablename_op_date_INDEX ON history.$tablename USING btree ("op_datetime")"
}


if [ $# -eq 0 ]
then
    Usage
    exit
fi

if [ $1 != "JamesSoft" ]
then
    Usage
    exit
fi

databaseName=$2

echo
echo "Create Database $databaseName..."
echo

# psql -U postgres -c "alter user postgres with password 'postgres'"

# psql -U postgres -c "drop database $databaseName"
# psql -U postgres -c "create database $databaseName"

# 创建模式
# psql -U postgres -d $databaseName -c "create schema system"
# # psql -U postgres -d $databaseName -c "create schema public"
# psql -U postgres -d $databaseName -c "create schema pis"
# psql -U postgres -d $databaseName -c "create schema iot"
# psql -U postgres -d $databaseName -c "create schema oa"
# psql -U postgres -d $databaseName -c "create schema stock"
# psql -U postgres -d $databaseName -c "create schema mro"
# psql -U postgres -d $databaseName -c "create schema rsu"
psql -U postgres -d $databaseName -c "create schema history"

# # system tables
# supervisor user's password: james2019


CreateTable system system_user_list 
CreateTable system system_user_menu 
CreateTable system account 
CreateTable system code_type 
CreateTable system code_value 
CreateTable system login_record 
CreateTable system menu 
CreateTable system menu_item 
CreateTable system operation_log 
CreateTable system user_group 
CreateTable system user_group_member 
CreateTable system user_group_menu 
CreateTable system area_code 

# public tables

CreateTable public component_list
CreateTable public component_property_list
CreateTable public device_component_list
CreateTable public device_config
CreateTable public device_list 
CreateTable public device_status 
CreateTable public device_type 
CreateTable public file 
CreateTable public organization 
CreateTable public property_list
CreateTable public vendor_list 
CreateTable public vendor_check_history 

# PIS tables
CreateTable pis controller 
CreateTable pis controller_log 
CreateTable pis controller_log_history 
CreateTable pis controller_status 
CreateTable pis controller_status_history 
CreateTable pis format_file 
CreateTable pis message_list 
CreateTable pis message_list_history 
CreateTable pis message_log 
CreateTable pis message_log_history 
CreateTable pis operation_line 
CreateTable pis program_check_history 
CreateTable pis program_info 
CreateTable pis program_unit 
CreateTable pis program_list 
CreateTable pis program_file 
CreateTable pis station 
CreateTable pis station_line 
CreateTable pis sub_line_info 
CreateTable pis train 

# IOT tables\
CreateTable iot gps_module 
CreateTable iot gps_module_trace 

# oa Tables
# 
CreateTable oa bulletinboard 
CreateTable oa oa_system_parameter 

# stock Tables
CreateTable stock stock 
CreateTable stock stock_day_data 
CreateTable stock stock_minute_1_data 
CreateTable stock stock_minute_5_data 
CreateTable stock favority_list 

# MRO

# RSU
CreateTable rsu rsu_project 
CreateTable rsu rsu_list 

# 加载数据
# psql -U postgres -d $databaseName -f $SRVDIR/sql/system/system_data.sql
# psql -U postgres -d $databaseName -f $SRVDIR/sql/public/public_system_data.sql

# psql -U postgres -d $databaseName -f $SRVDIR/sql/pis/pis_system_data.sql
# psql -U postgres -d $databaseName -f $SRVDIR/sql/pis/pis_data.sql
# psql -U postgres -d $databaseName -f $SRVDIR/sql/iot/iot_system_data.sql
# psql -U postgres -d $databaseName -f $SRVDIR/sql/oa/system_data.sql
# psql -U postgres -d $databaseName -f $SRVDIR/sql/stock/stock_system_data.sql
# psql -U postgres -d $databaseName -f $SRVDIR/sql/rsu/rsu_system_data.sql

