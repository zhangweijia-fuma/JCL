#coding:utf-8

import sys, time, threading
from socket import *  
from config import PisConfig
from utils.utils import *
from utils.debug import *
from .consts import *
from .message import *

Socks = {}
localLock = threading.Lock()

# print("TcpServerHandler ..................")

class TcpServerHandler(threading.Thread) :

	def __init__(self, addr, sock, parent, unique=True) :
		threading.Thread.__init__(self)
		self.host = addr[0]
		self.port = addr[1]
		self.sock = sock
		self.addr = (addr[0], addr[1], sock)
		self.parent = parent
		self.unique = unique
		self.closed = False

		localLock.acquire()

		# print("TcpServerHandler __init__ %s" % str(addr))
		# print(Socks)

		try:
			if self.unique:
				try :
					if haskey(Socks, self.host):
						threads = Socks[self.host]
						Socks[self.host] = {}
						for k,v in threads.items():
							v.closed = True
				except Exception as e:
					onException(e)
				Socks[self.host] = {
					self.port : self
				}
			else:
				try :
					if not haskey(Socks, self.host):
						Socks[self.host] = {}
					threads = Socks[self.host]

					threads[self.port] = self
				except Exception as e:
					onException(e)
		except Exception as e:
			onException(e)

		localLock.release()

	def onCallback(self, _type, addr, data):
		try:
			if self.parent:
				self.parent.onCallback(_type, addr, data)
		except Exception as e:
			onException(e)
			logE("TcpServerHandler onRxData exception")

	def close(self):
		try :
			logW("TcpServerHandler close")
			self.sock.close() 
			self.sock = None
		except Exception as e:
			onException(e)
			logE("TcpServerHandler close failed")

	def send(self, data):
		try:
			if self.sock:
				return self.sock.send(data)
		except Exception as e:
			onException(e)
		return False

	def run(self):
		logI("TcpServerHandler start!!!")
		while True:
			try :
				self.sock.settimeout(5)
				data = self.sock.recv(RX_BUF_LEN)
				if data:
					self.onCallback('data', self.addr, data)
				else:
					logI("no data..")
					self.closed = True
			except timeout as e:
				# logI("timeout....")
				pass
			except Exception as e:
				onException(e)
				self.closed = True

			if self.closed:
				self.close()
				self.onCallback('disconn', self.addr, None)
				break

		localLock.acquire()
		try:
			if haskey(Socks, self.host) and haskey(Socks[self.host], self.port):
				del Socks[self.host][self.port]
		except Exception as e:
			onException(e)
		localLock.release()

class TcpServer(threading.Thread) :

	def __init__(self, callback = None, port=LOCAL_TCP_PORT, unique=True) :
		threading.Thread.__init__(self)
		self.host = '0.0.0.0'
		self.port = port
		self.callback = callback
		self.unique = unique
		self.sock = None

	def open(self):
		try :
			addr = (self.host, self.port)
			sock = socket(AF_INET, SOCK_STREAM)  
			sock.bind(addr)
			sock.listen(10000)

			self.sock = sock
		except Exception as e:
			logE("TcpServer open failed %s:%s" % (self.host, self.port))
			onException(e)

	def close(self):
		logI("TcpServer try close!!!")
		try :
			self.sock.close() 
			self.sock = None
		except Exception as e:
			logE("TcpServer close failed")
			onException(e)

	def setCallback(self, callback):
		self.callback = callback

	def onCallback(self, _type, host, data):
		try:
			if self.callback:
				self.callback(_type, host, data)
		except Exception as e:
			logE("TcpServer onRxData exception")
			onException(e)

	def send(self, host, data):
		try:
			# print(Socks)
			if haskey(Socks, str(host)):
				logI("TcpServer send to  %s" % host)
				threads = Socks[host]
				for k,v in threads.items():
					v.send(data)
				return True
			else:
				logW("TcpServer send to  %s, not found" % host)

		except Exception as e:
			logE("TcpServer send exception")
			onException(e)

	def run(self):
		# logI("TcpServer start, port : %s" % self.port)
		while True:
			if not self.sock:
				time.sleep(CONN_ERROR_WAIT)
				self.open()
				continue

			try :
				sock, addr = self.sock.accept()
				logI("TcpServer got conn : %s" % (str(addr)))
				handler = TcpServerHandler(addr, sock, self, self.unique)
				handler.start()
				self.onCallback('conn', addr, sock)
			except Exception as e:
				self.close()
				onException(e)

