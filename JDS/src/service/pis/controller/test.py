#coding:utf-8

import sys, time, threading, random
from libs.utils.utils import *
from libs.utils.debug import *
from libs.db.dbHelper import *
from config import PisConfig
from .consts import *
from .message import *
from .msgHandler import *
from .tcpClient import *

msgEncoder = MsgEncoder()

class TestClientThread(threading.Thread) :

	def __init__(self, serverHost) :
		threading.Thread.__init__(self)
		self.serverHost = serverHost

	def sendHeartbeat(self):
		msg = HEARTBEAT()
		msg.setAddr(self.serverHost)
		data = msgEncoder.encode(msg)
		self.udpSend(data)

	def sendRunStatus(self):
		msg = RUN_STATUS()
		msg.setAddr(self.serverHost)
		msg.setParams(random.randint(1, 10000),
			random.randint(1, 10000),
			random.randint(1, 10000),
			random.randint(0, 3))
		data = msgEncoder.encode(msg)
		self.tcpSend(data)

	def sendLog(self):
		msg = RUN_LOG()
		msg.setAddr(self.serverHost)
		msg.random()
		data = msgEncoder.encode(msg)
		self.tcpSend(data)

	def sendInitRequest(self):
		msg = INITIAL_REQUEST()
		msg.setAddr(self.serverHost)
		data = msgEncoder.encode(msg)
		self.udpSend(data)

	def udpSend(self, data):
		self.udpClient.send(self.serverHost, data, port=LOCAL_UDP_PORT)

	def tcpSend(self, data):
		self.tcpClient.send(data)

	def onTcpCallback(self, _type, host, data):
		logD("TestClientThread, on onTcpCallback")
		# printHex(data)
		if _type == "data":
			self.tcpMsgParser.setHost(host)
			self.tcpMsgParser.input(data)

	def onUdpCallback(self, _type, host, data):
		logD("TestClientThread, on onUdpCallback")
		# printHex(data)
		if _type == "data":
			self.udpMsgParser.setHost(host)
			self.udpMsgParser.input(data)

	def sendAck(self, ackedMsg, ACK_MSG, tcp=False):
		msg = ACK_MSG()
		msg.sid = ackedMsg.sid
		msg.addr = ackedMsg.addr
		data = msgEncoder.encode(msg)
		if tcp:
			self.tcpSend(data)
		else:
			self.udpSend(data)

	def onRxMsg(self, msg):
		logD("test onRxMsg : ")
		time.sleep(0.1)
		logD(msg)
		if isinstance(msg, HEARTBEAT):
			self.sendAck(msg, HEARTBEAT_ACK)
		elif isinstance(msg, SEND_PROGRAM_LIST):
			self.sendAck(msg, SEND_PROGRAM_LIST_ACK)
		elif isinstance(msg, SEND_NEWS):
			self.sendAck(msg, SEND_NEWS_ACK)
		elif isinstance(msg, CLEAR_NEWS):
			self.sendAck(msg, CLEAR_NEWS_ACK)
		elif isinstance(msg, SEND_EMERGENCE):
			self.sendAck(msg, SEND_EMERGENCE_ACK)
		elif isinstance(msg, CLEAR_EMERGENCE):
			self.sendAck(msg, CLEAR_EMERGENCE_ACK)
		elif isinstance(msg, SEND_LINE_INFO):
			self.sendAck(msg, SEND_LINE_INFO_ACK)
		elif isinstance(msg, SEND_STATION_TIME):
			self.sendAck(msg, SEND_STATION_TIME_ACK)
		elif isinstance(msg, SEND_FORMAT_FILE):
			self.sendAck(msg, SEND_FORMAT_FILE_ACK)
		elif isinstance(msg, SYSTEM_REBOOT):
			self.sendAck(msg, SYSTEM_REBOOT_ACK, True)
		elif isinstance(msg, SEND_UPDATE):
			self.sendAck(msg, SEND_UPDATE_ACK)
		else:
			logD("test : no handled msg!")

	def run(self):

		time.sleep(1)

		udpServer = UdpServer(self.onUdpCallback, port=REMOTE_UDP_PORT)
		udpServer.start()
		self.udpServer = udpServer

		udpClient = UdpClient()
		self.udpClient = udpClient

		tcpClient = TcpClient(self.serverHost, self.onTcpCallback)
		tcpClient.start()
		self.tcpClient = tcpClient

		self.tcpMsgParser = MsgParser()
		self.udpMsgParser = MsgParser()

		self.tcpMsgParser.setCallback(self.onRxMsg)
		self.udpMsgParser.setCallback(self.onRxMsg)

		time.sleep(2)

		# tcpClient = TcpClient(self.serverHost, self.onTcpCallback)
		# tcpClient.start()

		tick = 0
		while True:

			loop = tick % 3

			if tick == 4:
				self.sendInitRequest()

			"""
			if loop == 0 :
				self.sendLog()

			if loop == 1 :
				self.sendHeartbeat()

			if loop == 2 :
				self.sendRunStatus()
			"""

			# self.sendLog()

			tick += 1
			time.sleep(5)

import socket

#########################################################
if True : #socket.gethostname() == "dog":
	try :
		myAddr = socket.gethostbyname(socket.gethostname())
	except Exception as e:
		myAddr = "127.0.0.1"
		onException(e)

	myAddr = "172.16.10.60"
	#myAddr = "192.168.1.62"
	testClientThread = TestClientThread(myAddr)
	# testClientThread.start()
