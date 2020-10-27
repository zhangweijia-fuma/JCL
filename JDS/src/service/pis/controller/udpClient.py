#coding:utf-8

import sys, struct
from config import PisConfig
from socket import *
from .consts import *
from utils.utils import *  

class UdpClient() :

	def __init__(self):
		self.sock = None

	def open(self):
		sock = socket(AF_INET, SOCK_DGRAM)
		self.sock = sock

	def send(self, host, data, port=REMOTE_UDP_PORT):
		# logI("UdpClient send :%s %s" % (host, port))
		try:
			self.host = host
			self.port = port
			self.data = data
			if not self.sock:
				self.open()
			addr = (host, port)
			return self.sock.sendto(data, addr)
		except Exception as e:
			onException(e)
			self.close()

		return False

	def resend(self):
		self.send(self.data, self.host, self.port)

	def close(self):
		if self.sock:
			try :
				self.sock.close()
			except Exception as e:
				pass
