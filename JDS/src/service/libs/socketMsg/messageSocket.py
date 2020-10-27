#coding:utf-8

import sys, time, threading
from socket import *  

# python 自带一个socketserver基类，需要重写handle函数
# import socketserver

from libs.utils.debug import *

from .messageConfig import *


# 业务部不处理socket异常

# socket 基类
class baseSocket:
	def __init__(self, socket_type = SOCK_STREAM, port = 0, callback = None) :
		
		self.localAddr = gethostbyname(gethostname())

		self.port = port

		# 默认连接方式为TCP
		self.socket_type = socket_type

		# 回调函数
		self.callback = callback

		self.sock = None

		return

	def __del__(self):
		self.close()
		return

	def close(self):
		if self.sock != None:
			self.sock.close()

	# 设置回调函数
	def setCallback(self, callback):
		self.callback = callback
		
		return

	# 发送消息给对端
	def sendMessage(self, data, addr = None):
		if self.sock == None:
			logE("socket is not open now")
			return False

		try:
			if self.socket_type == SOCK_STREAM:
				self.sock.send( data )
			else:
				self.sock.sendto( data, addr )

		except Exception as e:
			logE("send data to peer failed: %d, %s"%(e.errno, e.strerror))
			return False

		return True


# socket Server，基于线程，在接收到消息后调用回调函数由业务层对数据进行处理
class socketServer(threading.Thread, baseSocket):
	def __init__(self, socket_type, port, callback) :
		threading.Thread.__init__(self)
		baseSocket.__init__(self, socket_type, port, callback)

		self.runFlag = True

		return

	def __del__(self): 
		self.close()
		
		return

	def open(self):
		try :
			addr = (self.localAddr, self.port)
			sock = socket(AF_INET, self.socket_type)
			sock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)

			self.sock = sock

			self.sock.bind(addr)
			
			# TCP 连接
			if self.socket_type == SOCK_STREAM:
				self.sock.listen(5)

		except Exception as e:
			logE("create socket server error: code: %d, desciption: %s"%(e.errno, e.strerror))
			self.sock = None
			return

		return

	def close(self):
		self.runFlag = False
		if self.sock:
			self.sock.close()

		return

	# 线程启动，一直等待接收客户端消息，直到运行标志为False
	# 根据socket类型，选择TCP或UDP服务端
	def run(self):

		if self.socket_type == SOCK_STREAM:
			self.runTcpServer()
		else:
			self.runUdpServer()


	# TCP服务端
	def runTcpServer(self):

		while self.runFlag:
			if self.sock == None:
				self.open()
				time.sleep(2)
				continue

			try:
				sock, clientAddr = self.sock.accept()

				clientSock = baseSocket()
				clientSock.sock = sock

			except Exception as e:
				logE("accept connection from client error, code: %d, desciption: %s"%(e.errno, e.strerror))
				self.sock.close()
				self.sock = None
				continue

			# 在接收到连接后， 使用一个线程来处理这个连接
			t = threading.Thread( target = self.receiveTCPMessage, args = (clientSock, clientAddr))
			t.start()


	# 处理TCP连接
	# clientSock: 基于baseSocket的类
	# clientAddr：包含IP地址及端口
	def receiveTCPMessage(self, clientSock, clientAddr):
		logI("accept new connection from %s:%s..." %(clientAddr[0], clientAddr[1]))

		while self.runFlag:
			try:
				data = clientSock.sock.recv(MESSAGE_MAX_LEN)
				if not data:
					continue
			except Exception as e:
				logE("receive data from client error, code: %d, desciption: %s"%(e.errno, e.strerror))
				break

			# socket 不解析消息，由业务处理部分来进行消息解析和处理
			if self.callback: 
				# 把接收的数据、客户端地址、端口及连接客户端到socket返回给业务层
				self.callback(data, clientSock, clientAddr)
			else:
				logE("系统错误：TCP socket 无回调函数！")

		# 
		clientSock.close()
		return


	# UDP服务端
	def runUdpServer(self):

		while self.runFlag:
			if self.sock == None:
				self.open()
				time.sleep(2)
				continue

			# logI("waiting meesage...")
			try :
				data, clientAddr = self.sock.recvfrom(MESSAGE_MAX_LEN)

			except Exception as e:
				logE("receive data from client error, code: %d, desciption: %s"%(e.errno, e.strerror))
				break

			# 在接收到消息后， 使用一个线程来处理这个消息
			t = threading.Thread( target = self.receiveUDPMessage, args = (data, clientAddr))
			t.start()

	# 调用回调函数由业务处理UDP消息
	def receiveUDPMessage(self, data, clientAddr):

		# socket 不解析消息，由业务处理部分来进行消息解析和处理
		if self.callback: 
			self.callback(data, self, clientAddr)
		else:
			logE("系统错误：UDP socket 无回调函数！")

		return

# socketClient，在接收到消息后调用回调函数
class socketClient(baseSocket):
	def __init__(self, socket_type = SOCK_DGRAM, callback = None) :
		baseSocket.__init__(self, socket_type, 0, callback)
		
		self.srvAddr = ""
		self.srvPort = 0
		
		self.socket_type = socket_type

		# 设置socket超时时间为2秒
		setdefaulttimeout(2)

		return

	def __del__(self):
		self.close()
		# LogI("socket client exit")
		return

	# 连接到服务端，如果连接不成功则持续连接
	def connect(self, srvAddr, srvPort):

		self.srvAddr = srvAddr
		self.srvPort = srvPort

		while True:

			try:
				self.sock = socket(AF_INET,  self.socket_type)
				
				if  self.socket_type == SOCK_STREAM:
					logI("connect to server: %s:%d"%(srvAddr, srvPort))
					self.sock.connect( (srvAddr, srvPort) )

			except Exception as e:
				logE("connect to server: %s:%d failed, code: %d, error desciption: %s"%(srvAddr, srvPort, e.errno, e.strerror))
				time.sleep(5)
				continue

			break

		return


	# 接收服务端数据
	def receiveMessage(self):
		if self.sock == None:
			return None

		try:
			receive = self.sock.recv(MESSAGE_MAX_LEN)
			if not receive:
				return None

		except Exception as e:
			# 接收数据超时或者出现异常
			# logE("receive data from server failed")
			return None

		if self.callback:
			self.callback(receive)

		return receive
