# coding:utf-8

# 获取CPU、内存、硬盘使用率
# 返回值：为万分比，即返回值为234，为2.34%

import os, time

last_worktime = 0
last_idletime = 0

def getCPUUsage():
	global last_worktime, last_idletime
	f = open("/proc/stat","r")
	line = ""
	while not "cpu " in line: 
		line = f.readline()

	f.close()
	
	spl = line.split(" ")
	worktime  = int(spl[2]) + int(spl[3]) + int(spl[4])
	idletime  = int(spl[5])
	dworktime = (worktime - last_worktime)
	didletime = (idletime - last_idletime)
	rate = float( dworktime) / (didletime + dworktime )
	last_worktime = worktime
	last_idletime = idletime
	if( last_worktime == 0): 
		return 0

	return int(rate * 10000)

def getMemoryUsage():
	try:
		f = open('/proc/meminfo', 'r')
		for line in f:
			if line.startswith('MemTotal:'):
				mem_total = int(line.split()[1])
			elif line.startswith('MemFree:'):
				mem_free = int(line.split()[1])
			elif line.startswith('Buffers:'):
				mem_buffer = int(line.split()[1])
			elif line.startswith('Cached:'):
				mem_cache = int(line.split()[1])
			elif line.startswith('SwapTotal:'):
				vmem_total = int(line.split()[1])
			elif line.startswith('SwapFree:'):
				vmem_free = int(line.split()[1])
			else:
				continue
		f.close()

	except:
		return None

	physical_percent = usagePercent(mem_total - (mem_free + mem_buffer + mem_cache), mem_total)
	virtual_percent = 0
	if vmem_total > 0:
		virtual_percent = usagePercent((vmem_total - vmem_free), vmem_total)
	
	# 仅返回物理内存使用率
	mem_usage = int(physical_percent * 100)
	return mem_usage

def usagePercent(use, total):
	try:

		ret = (float(use) / total) * 100

	except ZeroDivisionError:
		raise Exception("ERROR - zero division error")

	return ret

def getDiskUsage():

	statvfs = os.statvfs('/')
	total_disk_space = statvfs.f_frsize * statvfs.f_blocks
	free_disk_space = statvfs.f_frsize * statvfs.f_bfree
	disk_usage = (total_disk_space - free_disk_space) * 100.0 * 100 / total_disk_space
	disk_usage = int(disk_usage)

	return disk_usage

