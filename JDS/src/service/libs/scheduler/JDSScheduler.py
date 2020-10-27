# coding:utf-8

# ------------------------------------------------------------------------------
# JCCL - Component :: Task Scheduler
# Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
# Author: Zhang Weijia
# ------------------------------------------------------------------------------
# Name:    JDSScheduler.py
# Purpose: Create Schedule Task
#  *----------------------------------------------------------------------------

import time
from apscheduler.schedulers.background import BackgroundScheduler
from apscheduler.triggers.cron import CronTrigger

# 创建一个定时任务，由一个函数来执行定时任务
# 参数：
#   func: 执行任务的函数
#   second、minute、hour、day_of_week：任务启动时间，表示每周的第几天、几点、几分、几秒启动任务
#   如：定义一个每周一早上2点10分的一个定时任务，参数为：
#   second:0, minute: 10, hour:2, day_of_week: *
def CreateScheduleTask(func, second = "0", minute = "*", hour = "*", day_of_week = "*"): 
    scheduler = BackgroundScheduler()
    cronTrigger = CronTrigger(second = second, minute = minute, hour = hour, day_of_week = day_of_week)
    scheduler.add_job(func, cronTrigger)
    scheduler.start()

