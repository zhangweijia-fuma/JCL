#!/usr/bin/python3
import socket
import time
from sys import argv
from array import array
from struct import unpack, pack
from math import ceil
from argparse import ArgumentParser
from random import randint

__all__ = ["client"]

def Help():
	print("modbusTest.py <host> <port> <fcode> <addr> <data>")

class client:
	def __init__(self, host='localhost', unit=1):
		self.host = host
		self.unit = unit
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.connect((host, 502))

	def read(self, FC, ADD, LEN):
		print("FC:%d, ADD:%d, LEN:%d"%(FC, ADD, LEN))
		if FC not in [1, 2, 3, 4]: 
			Help()
		else:
			lADD = ADD & 0x00FF
			mADD = ADD >> 8
			lLEN = LEN & 0x00FF
			mLEN = LEN >> 8
			if (FC < 3):
				BYT = ceil(LEN / 8)  # Round off the no. of bytes
			else:
				BYT = LEN * 2
			cmd = array('B', [0, randint(0, 255), 0, 0, 0, 6,
							  self.unit, FC, mADD, lADD, mLEN, lLEN])

			print("TX : " + " ".join(["%02x" % d for d in cmd]))
			self.sock.send(cmd)

			buf = self.sock.recv(1000)
			print("RX : " + " ".join(["%02x" % d for d in buf]))

	def writeData(self, data):
		print("TX : " + " ".join(["%02x" % d for d in data]))
		self.sock.send(data)
		
		buf = self.sock.recv(1000)
		print("RX : " + " ".join(["%02x" % d for d in buf]))

	def write(self, FC, ADD, DAT):
		if FC not in [5, 6, 15, 16]: 
			Help()
		else:
			lADD = ADD & 0x00FF
			mADD = ADD >> 8
			VAL = DAT
			# for i in DAT:
			# 	VAL = VAL + pack('>H', int(i))

			if FC == 5 or FC == 6:
				VAL = VAL[0:2]

			# print(DAT)
			# print(VAL)

			if FC == 5 or FC == 15:
				LEN = len(VAL) * 8
			else:
				LEN = int(len(VAL) / 2)

			LEN = 1

			lLEN = LEN & 0x00FF
			mLEN = LEN >> 8
			cmd = array('B', [0, 0, 0, 0, 0, 7 + len(VAL),
				self.unit, FC, mADD, lADD, mLEN, lLEN, len(VAL)])
			cmd.extend(VAL)

			# cmd = cmd + cmd + cmd

			print("TX : " + " ".join(["%02x" % d for d in cmd]))
			self.sock.send(cmd)

			buf = self.sock.recv(1000)
			print("RX : " + " ".join(["%02x" % d for d in buf]))

			# buf = self.sock.recv(1000)
			# print("RX : " + " ".join(["%02x" % d for d in buf]))

			# buf = self.sock.recv(1000)
			# print("RX : " + " ".join(["%02x" % d for d in buf]))

charHexMap = {
	"0" : 0,
	"1" : 1,
	"2" : 2,
	"3" : 3,
	"4" : 4,
	"5" : 5,
	"6" : 6,
	"7" : 7,
	"8" : 8,
	"9" : 9,
	"a" : 10,
	"b" : 11,
	"c" : 12,
	"d" : 13,
	"e" : 14,
	"f" : 15,
	"A" : 10,
	"B" : 11,
	"C" : 12,
	"D" : 13,
	"E" : 14,
	"F" : 15,
}

def parseHex(text):
	try :
		ret = 0
		for c in text:
			ret = (ret << 4) + charHexMap[c]
		return ret
	except Exception as e:
		print("parseHex failed : %s" % text)
		onException(e)
		exit()

def test():

	#print("argc : %d, argv : %s" % (len(argv), str(argv)))

	alen = len(argv)
	if alen < 6:
		Help()
		exit()

	host = argv[1]
	port = argv[2]
	fcode = parseHex(argv[3])
	addr = parseHex(argv[4])

	c = client(host, 0x88)

	if fcode == 0x10:
		if addr == 0x33:
			data = argv[5]
			data = data.encode()
		else:
			data = parseHex(argv[5])
			data = array('B', [(data & 0xFF00) >> 8, data & 0xFF])
			# data = [(data & 0xFF00) >> 8, data & 0xFF]
		c.write(fcode, addr, data)
	elif fcode == 0x3:
		data = parseHex(argv[5])
		c.read(fcode, addr, data)
	elif fcode == 0x00:
		data = bytearray(alen - 4)
		i = 4
		j = 0
		while i < alen:
			data[j] = parseHex(argv[i])
			j += 1
			i += 1

		c.writeData(data)

if __name__ == "__main__":
	test()


# python3 modbusTest.py 192.168.1.62 502 10 c9 00
# python3 modbusTest.py 192.168.1.62 502 10 03 01

"""

cd /JamesSoft/JDS/src/service/pis/controller/

#d1 下行 本次列车到站信息
python3 modbusTest.py 172.16.10.136 502 10 c9 01
python3 modbusTest.py 172.16.10.136 502 10 cd 11
python3 modbusTest.py 172.16.10.136 502 10 cc 21

#d2 下行 下次列车到站信息
python3 modbusTest.py 172.16.10.136 502 10 cf 02
python3 modbusTest.py 172.16.10.136 502 10 d3 12
python3 modbusTest.py 172.16.10.136 502 10 d2 22

#u1 上行 本次列车到站信息
python3 modbusTest.py 172.16.10.136 502 10 c8 03
python3 modbusTest.py 172.16.10.136 502 10 cb 13
python3 modbusTest.py 172.16.10.136 502 10 ca 23

#u2 上行 下次列车到站信息
python3 modbusTest.py 172.16.10.136 502 10 ce 04
python3 modbusTest.py 172.16.10.136 502 10 d1 14
python3 modbusTest.py 172.16.10.136 502 10 d0 24

#msg
#python3 modbusTest.py 192.168.1.62 502 00 00 a9 00 00 00 15 ff 10 00 33 00 07 0e f4 bd b1 bc a3 c4 bd ca e2 b2 d4 ca 00 00


"""
