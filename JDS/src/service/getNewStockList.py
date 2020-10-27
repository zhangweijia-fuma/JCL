#coding:utf-8

import os
import sys
import time
import datetime

import xlrd, xlwt
import openpyxl

from sys import path
from libs.utils.debug import *

path.append("./libs")                                                   # 库文件

from libs.db.dbHelper import *

# 从excel文件中读取所有股票清单
# A列：代码
# B列：名称
# C列：上市日期
def getAllStockList():

    stockFile = []
    stockFile.append("szList.xlsx")
    stockFile.append("shList.xlsx")

    stockList = []

    for file in stockFile:
        logI("Process file: %s"%file)
        wb = openpyxl.load_workbook(file)
        sheet = wb.active
        nRows = sheet.max_row                                       # 行总数
        if nRows < 2:
            logI("文件无内容！")
            return

        rowNum = 2
        while rowNum <= nRows:
            # logI("File %s: %d"%(file, rowNum))
            # logI("value: %s"%sheet["A%d"%rowNum].value
            data = {}
            if isinstance(sheet["A%d"%rowNum].value, int):
                data['code'] = "%d"%sheet["A%d"%rowNum].value
            else:
                data['code'] = sheet["A%d"%rowNum].value.strip()

            data['name'] = sheet["B%d"%rowNum].value.strip()
            data['list_date'] = sheet["C%d"%rowNum].value.strip()

            stockList.append(data)
            rowNum += 1

    logI("All Stock Count: %s"%len(stockList))
    return stockList


def getNewStockList():

    stockList = []
    allStockList = getAllStockList()

    dbHelper = DbHelper()
    db = dbHelper.getDB("stock.stock")
    cur = dbHelper.getCursor()

    sql = "select code, name from stock.stock"

    cur.execute(sql)
    rows = cur.fetchall()
    dbHelper.closeDB()

    currentStockList = []

    for row in rows:
        keys = ["code", "name"]
        item = dict(zip(keys, row))
        currentStockList.append(item)

    logI("Current database Stock count: %s"%len(currentStockList))

    newStockList = []
    for stock in allStockList:
        found = False
        for item in currentStockList:
            if item['code'] == stock['code']:
                found = True
                break

        if found == False:
            # logI("New stock: %s, %s"%(stock['code'], stock['name']))
            newStockList.append(stock)


    logI("New Stock Count: %s"%len(newStockList))

    # 输出到文件，用于加载到数据库，有3个字段，分别是名称，代码，上市日期
    with open( 'newStockList.txt', 'w', encoding='UTF-8') as stockListFile:
        for stock in newStockList:
            line = "%s|%s|%s\r\n"%(stock['name'], stock['code'], stock['list_date'])
            stockListFile.write(line)

        stockListFile.close()

    # 输出到文件，用于数据下载，有2个字段，分别是名称和代码
    with open( 'newStockListData.txt', 'w', encoding='UTF-8') as stockListFile:
        for stock in newStockList:
            line = "|%s|%s|\r\n"%(stock['name'], stock['code'])
            stockListFile.write(line)

        stockListFile.close()

getNewStockList()

