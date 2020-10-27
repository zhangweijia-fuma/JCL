#coding:utf-8

import sys, time, threading
from config import PisConfig
from socket import *  
from utils.utils import *
from utils.debug import *
from .consts import *
from .message import *

class TcpClient(threading.Thread) :

	def __init__(self, host, callback = None, port=LOCAL_TCP_PORT) :
		threading.Thread.__init__(self)
		self.host = host
		self.port = port
		self.addr = (self.host, self.port)
		self.callback = callback
		self.sock = None

	def open(self):
		try :
			logI("TcpClient open : %s" % str(self.addr))
			sock = socket(AF_INET, SOCK_STREAM)  
			sock.connect(self.addr)
			self.sock = sock
		except Exception as e:
			logE("TcpClient open failed")
			onException(e)

	def close(self):
		try :
			logI("TcpClient close")
			self.sock.close() 
			self.sock = None
		except Exception as e:
			logE("TcpClient close failed")
			onException(e)

	def setCallback(self, callback):
		self.callback = callback

	def onCallback(self, _type, addr, data):
		try:
			if self.callback:
				threadCallback(self.callback, _type, addr, data)
				# self.callback(_type, host, data)
		except Exception as e:
			logE("TcpClient onRxData exception")
			onException(e)

	def send(self, data):
		try:
			if self.sock:
				return self.sock.send(data)
		except Exception as e:
			logE("TcpClient send exception")
			onException(e)
		return None

	def run(self):
		logI("TcpClient start!!!")
		while True:
			if not self.sock:
				time.sleep(CONN_ERROR_WAIT)
				self.open()
				continue

			try :
				data = self.sock.recv(RX_BUF_LEN)
			except Exception as e:
				data = None
				onException(e)

			if not data:
				self.close()
				self.onCallback('disconn', self.addr, None)
				break
			self.onCallback('data', self.addr, data)

