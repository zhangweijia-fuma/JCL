#!/bin/python3
#coding:utf-8

#apt-get install libpcap-dev
#pip3 install pypcap
#pip3 install dpkt

import os
import sys
import time
import threading
import pcap
import dpkt
import traceback
from socket import *  

Config = {
	"PCAP_INTF" : "eth0",
	"PCAP_FILTER" : "tcp port 502 and not ip dst 191.164.6.3",
	"CAP_DST_IP" : "192.168.1.10",
	"CAP_DST_PORT" : 502,
	"DST_HOST": "localhost",
	"DST_PORT": 502,
}

def onException(e):
	print("onException : %s" % type(e))
	print(str(e))
	traceback.print_exc()

class TcpReplayClient(threading.Thread) :

	def __init__(self, host, port, callback = None) :
		threading.Thread.__init__(self)
		self.host = host
		self.port = port
		self.addr = (self.host, self.port)
		self.callback = callback
		self.sock = None

	def open(self):
		try :
			print("TcpReplayClient open")
			sock = socket(AF_INET, SOCK_STREAM)  
			sock.connect(self.addr)
			self.sock = sock
			print("TcpReplayClient open ok")
		except Exception as e:
			print("TcpReplayClient open failed")
			onException(e)

	def close(self):
		try :
			print("TcpReplayClient close")
			self.sock.close() 
			print("TcpReplayClient close ok")
		except Exception as e:
			print("TcpReplayClient close failed")
			onException(e)
		self.sock = None

	def setCallback(self, callback):
		self.callback = callback

	def onCallback(self, _type, addr, data):
		try:
			if self.callback:
				self.callback(_type, addr, data)
		except Exception as e:
			print("TcpReplayClient onRxData exception")
			onException(e)

	def send(self, data):
		try:
			if not self.sock:
				self.open()

			if self.sock:
				return self.sock.send(data)
		except Exception as e:
			print("TcpReplayClient send exception")
			onException(e)
		return None

	def run(self):
		print("TcpReplayClient start!!!")
		while True:
			if not self.sock:
				time.sleep(2)
				self.open()
				continue

			try :
				data = self.sock.recv(4096)
			except Exception as e:
				data = None
				onException(e)

			if not data:
				self.close()
				self.onCallback('disconn', self.addr, None)
				continue
			self.onCallback('data', self.addr, data)

def printData(data):
	print(" ".join(["%02x" % d for d in data]))

def onRxData(_type, addr, data):
	print("onRxData : %s" % _type)

def main():
	print("Relay start.....")
	c = TcpReplayClient(Config["DST_HOST"], Config["DST_PORT"])
	c.setCallback(onRxData)
	c.start()

	pc = pcap.pcap(Config["PCAP_INTF"])
	pc.setfilter(Config["PCAP_FILTER"])   

	isDst = None

	for ptime, pdata in pc:
		pkt = dpkt.ethernet.Ethernet(pdata)
		if not pkt:
			continue

		if type(pkt.data) != dpkt.ip.IP:
			print("not ip pkt");
			continue

		ip = pkt.data
		if type(ip.data) != dpkt.tcp.TCP:
			print("not tcp pkt");
			continue

		tcp = ip.data

		dip = "%s.%s.%s.%s" % (ip.dst[0], ip.dst[1], ip.dst[2], ip.dst[3])
		sip = "%s.%s.%s.%s" % (ip.src[0], ip.src[1], ip.src[2], ip.src[3])
		if dip == Config["CAP_DST_IP"] and tcp.dport == Config["CAP_DST_PORT"]:
			isDst = True
		elif sip == Config["CAP_DST_IP"] and tcp.sport == Config["CAP_DST_PORT"]:
			isDst = False
		else:
			print("%s:%s -> %s:%s len:%d, ip/port not match" % (sip, tcp.sport, dip, tcp.dport, len(tcp.data)))
			continue

		if not tcp.data or len(tcp.data) <= 12:
			continue

		print("%s:%s -> %s:%s len:%d, dst:%s" % (sip, tcp.sport, dip, tcp.dport, len(tcp.data), isDst))

		printData(tcp.data)

		print(type(tcp.data))

		if isDst:
			c.send(tcp.data)

if __name__ == "__main__":
	main()
