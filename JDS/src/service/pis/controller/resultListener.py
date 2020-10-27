#coding:utf-8
import sys, struct, time, threading
from utils.utils import *
from .consts import *
from .msgLogger import *

class MsgResultListener():
	def __init__(self) :
		self.msgs = {}
		self.pend = 0
		self.fail = 0
		self.success = 0
		self.sinal = threading.Event()

	def onResult(self, msg, result):
		_id = id(msg)
		if not haskey(self.msgs, _id):
			return
		if not msg._pend:
			return

		msg._pend = False
		self.pend -= 1

		if result:
			self.success += 1
		else:
			self.fail += 1
			m = MSG_UNACKED()
			m.addr = msg.addr
			m.oldMsg = msg
			m.controller = msg.controller
			msgLogger.logMsg(m)

		if not self.pend:
			self.sinal.set()

	def addMsg(self, msg):
		_id = id(msg)
		self.msgs[_id] = msg
		msg.setCallback(self.onResult)
		msg._pend = True
		self.pend += 1

	def wait(self, timeout=REQUEST_TIMEOUT):
		if not self.pend:
			result = True
		else:
			result = self.sinal.wait(timeout=timeout)

		self.result = result

