#coding:utf-8

import sys, time, threading
from config import PisConfig
from utils.utils import *
from utils.debug import *
from .consts import *
from socket import *  

class UdpServer(threading.Thread) :

	def __init__(self, callback = None, port=LOCAL_UDP_PORT) :
		threading.Thread.__init__(self)

		self.host = '0.0.0.0'
		self.port = port
		self.callback = callback
		self.sock = None

	def open(self):
		try :
			addr = (self.host, self.port)
			sock = socket(AF_INET, SOCK_DGRAM)  
			sock.bind(addr)
			self.sock = sock
		except Exception as e:
			onException(e)
			logE("UDP Server open failed")

	def close(self):
		try :
			self.sock.close() 
			self.sock = None
		except Exception as e:
			onException(e)
			logE("UDP Server close failed")

	def setCallback(self, callback):
		self.callback = callback

	def onRxData(self, host, data):
		# logI("UDP Server receive data from %s" % str(host))
		try:
			if self.callback:
				self.callback('data', host, data)
		except Exception as e:
			onException(e)
			logE("UDP Server onRxData exception")

	def run(self):
		# logI("UDP Server start, port : %s" % self.port)
		while True:
			if not self.sock:
				time.sleep(CONN_ERROR_WAIT)
				self.open()
				continue

			try :
				data, addr = self.sock.recvfrom(MAX_PKT_LEN) 
				self.onRxData(addr, data)
			except Exception as e:
				onException(e)
				self.close()

# udpServer = UdpServer(None)
# udpServer.start()