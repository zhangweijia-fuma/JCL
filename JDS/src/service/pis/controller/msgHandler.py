#coding:utf-8

import sys, time, threading
import multiprocessing as mp
from multiprocessing import Queue
from queue import Empty
from utils.utils import *
from utils.debug import *
from config import PisConfig
from .consts import *
from .message import *
from .tcpServer import *
from .udpClient import *
from .udpServer import *
from .msgParser import *
from .test import *
from .msgLogger import *

class MsgHandler(threading.Thread) :

	def __init__(self, callback = None, queueSize = 10000) :
		threading.Thread.__init__(self)

		self.queueSize = queueSize
		self.msgQueue = Queue(self.queueSize)

		self.callback = callback
		self.sentMsgList = None

		# 下发节目单及安全垫片消息需要等待下载完成消息
		self.downloadMsgList = None

	def init(self):
		self.tcpServer 		= TcpServer()
		self.udpServer 		= UdpServer()
		self.udpClient 		= UdpClient()

		self.msgEncoder 	= MsgEncoder()
		self.tcpMsgParser 	= MsgParser()
		self.udpMsgParser 	= MsgParser()

		self.tcpServer.setCallback(self.onTcpRxCallback)
		self.udpServer.setCallback(self.onUdpRxCallback)

		self.tcpMsgParser.setCallback(self.onRxMsg)
		self.udpMsgParser.setCallback(self.onRxMsg)

		self.tcpServer.start()
		self.udpServer.start()

		self.mutex = threading.Lock()
		self.msgCache = {}

	def putMsg(self, msg):
		self.mutex.acquire()
		key = id(msg)
		try :
			self.msgCache[key] = msg
		except Exception as e:
			onException(e)
		self.mutex.release()
		return key

	def getMsg(self, key):
		if not key:
			return None
		self.mutex.acquire()
		msg = None
		try :
			if haskey(self.msgCache, key):
				msg = self.msgCache[key]
				del self.msgCache[key]
		except Exception as e:
			onException(e)
		self.mutex.release()
		return msg

	def addSentMsg(self, msg, msgList):
		if not msgList:
			msgList = msg
			return

		# msg.initAck()

		prev = None
		head = msgList
		while head:
			if msg.timeout <= head.timeout:
				msg.next = head
				if prev:
					prev.next = msg
				else:
					self.sentMsgList = msg
				return
			prev = head
			head = head.next

		prev.next = msg

	def setCallback(self, callback):
		self.callback = callback

	def sendMsg(self, msg):
		try :
			key = self.putMsg(msg)
			self.msgQueue.put(key, block=True)
			return True
		except Exception as e:
			onException(e)
			logE("TxMsgQueue.put failed")
			return False

	def onRxMsg(self, msg):
		# logD("MsgHandler onRxMsg")
		msg.tx = False
		self.sendMsg(msg)

	def getSleepTime(self):
		if self.sentMsgList:
			t = self.sentMsgList.timeout - time.time() + 0.01
			if t < 0:
				t = 0
			return t
		return 100

	def send(self, msg):
		if not hasattr(msg, "packet"):
			packet = self.msgEncoder.encode(msg)
			msg.packet = packet
		else:
			packet = msg.packet

		if msg.getMsgType() != HeartBeatAck and msg.getMsgType() != RunStatusAck:
			logI("Send message %s to %s, len: %s, parameter: %s" % (msg.getMsgTypeStr(), msg.addr, len(packet), msg.toString() )) 
		# else:
		# 	logD("Send message %s to %s, len: %s, parameter: %s" % (msg.getMsgTypeStr(), msg.addr, len(packet), msg.toString() )) 
		
		# , "TCP" if msg.tcp else "UDP"
		# printHex(packet)
		if msg.tcp:
			self.tcpServer.send(msg.addr, packet)
		else:
			self.udpClient.send(msg.addr, packet)

	def removeSentMsg(self, ackMsg, msgList):
		ackedType 	= ackMsg.getAckedType()
		ackedSid 	= ackMsg.sid
		ackedAddr 	= ackMsg.addr

		prev = None
		head = msgList
		while head:
			#D("removeSentMsg %s:%s, %s:%s, %s:%s" % (head.type, ackedType, head.sid, ackedSid, head.addr, ackedAddr))
			if head.type == ackedType and head.sid == ackedSid and head.addr == ackedAddr:
				if prev:
					prev.next = head.next
				else:
					msgList = head.next
				return head
			prev = head
			head = head.next

	def handle(self, msg):
		if msg.tx:
			# 发送消息给控制器
			self.send(msg)
			if not msg.ack:
				# 如果发送的是请求消息，则将消息放到已发送消息队列中，等待响应消息或超时出发
				msg.initAck(False)
				self.addSentMsg(msg, self.sentMsgList)

			msgLogger.logMsg(msg)

		else:
			# 从控制器接收消息
			if msg.ack:
				# 如果是响应消息，则查找对应的请求消息
				ackedMsg = self.removeSentMsg(msg, self.sentMsgList)

				# 如果该消息需要再次接收下载完成消息，则把该消息放入到队列中，在接收到下载完成消息后再进行处理
				if ackedMsg.getMsgType() in msgForDownloadList:
					self.addSentMsg(msg, self.downloadMsgList)

				msg.ackedMsg = ackedMsg
				if ackedMsg:
					ackedMsg.setAckMsg(msg)
					# logI("MsgHandler found ackedMsg : %s %s" % (ackedMsg.addr, ackedMsg.sid))
				else:
					logW("MsgHandler no found ackedMsg : %s %s" % (msg.addr, msg.sid))
			else:
				# 接收到控制器发出的请求消息
				if msg.getMsgType() == DownloadCompleteMsg:
					# 接收到下载完成消息，查找原来的请求消息

					# 构造一个响应消息
					if msg.requestMsgType == SendProgramListMsg:
						simulateAckMsg = SEND_PROGRAM_LIST_ACK()

					elif msg.requestMsgType == SendSafeVideoMsg:
						simulateAckMsg = SEND_SAFE_FILE_ACK()

					else:
						logE("Receive DownloadCompleteMsg but requestMsgType %s is not support"%msg.getMsgTypeStr() )
						return

					# 添加参数，SID可能无法匹配
					simulateAckMsg.sid 	= msg.sid
					simulateAckMsg.addr = msg.addr
					ackedMsg = self.removeSentMsg(simulateAckMsg, self.downloadMsgList)
					if ackedMsg == None:
						logE("msgHandler not found sent request message in downloadMsgList")
						return

					msg.ackedMsg = ackedMsg
					# 在msgLogger 里面更新原有的发送记录 

			msgLogger.logMsg(msg)
			if self.callback:
				m = {
					"type" : "msg",
					"msg" : msg
				}
				threadCallback(self.callback, m)

	def reTx(self):
		t = time.time()
		prev = None
		head = self.sentMsgList
		while head:
			if t < head.timeout:
				break

			self.sentMsgList = head.next
			head.next = None

			if head.resent < RESEND_LIMIT:
				logI("MsgHandler resend a msg : %s %s" % (head.addr, head.sid))
				self.send(head)
				head.initAck(True)
				self.addSentMsg(head, self.sentMsgList)
			else:
				logI("MsgHandler resend timeout : %s %s" % (head.addr, head.sid))
				head.setAckMsg(None)
			head = self.sentMsgList

	def onTcpRxCallback(self, _type, host, data):
		# logD("onTcpRxCallback... %s %s" % (_type, host))
		if _type == "data":
			if PACKET_DEBUG:
				printHex(data)

			self.tcpMsgParser.setHost(host)

			# 解析消息，然后通过回调函数将解析后的消息送到队列中进行处理
			msg = self.tcpMsgParser.input(data)

			if msg.getMsgType() != HeartBeatMsg and msg.getMsgType() != RunStatusMsg:
				logI("Receive message %s from %s, parameter: %s"%(msg.getMsgTypeStr(), msg.addr, msg.toString() ))
			else:
				logD("Receive message %s from %s, parameter: %s"%(msg.getMsgTypeStr(), msg.addr, msg.toString() ))
			

	def onUdpRxCallback(self, _type, host, data):
		# logD("onUdpRxCallback... %s %s" % (_type, host))
		if _type == "data":
			if PACKET_DEBUG:
				printHex(data)

			self.udpMsgParser.setHost(host)

			# 解析消息，然后通过回调函数将解析后的消息送到队列中进行处理			
			msg = self.udpMsgParser.input(data)

			if msg.getMsgType() != HeartBeatMsg and msg.getMsgType() != RunStatusMsg:
				logI("Receive message %s from %s, parameter: %s"%(msg.getMsgTypeStr(), msg.addr, msg.toString() ))
			else:
				logD("Receive message %s from %s, parameter: %s"%(msg.getMsgTypeStr(), msg.addr, msg.toString() ))
			
	def run(self):
		logD("TxMsgQueue Starting...")
		self.init()
		while True:
			msg = None
			try :
				to = self.getSleepTime()
				# logD("getSleepTime : %s" % to)
				if not to:
					time.sleep(1)
				msg = self.msgQueue.get(timeout = to)
			except Empty as e:
				pass
			except Exception as e:
				logE("TxMsgQueue run")
				onException(e)

			# logD("MsgHandler, got msg %s" % msg)
			msg = self.getMsg(msg)
			if msg:
				# logD("MsgHandler, got msg valid")
				try:
					self.handle(msg)
				except Exception as e:
					logE("TxMsgQueue handle failed")
					onException(e)

			try :
				self.reTx()
			except Exception as e:
				logE("TxMsgQueue reTx failed")
				onException(e)

msgHandler = MsgHandler()
if USING_MSGER:
	msgHandler.start()

