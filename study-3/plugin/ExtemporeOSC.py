import sublime, sublime_plugin
import os, time

# Hack for including modules
import os
import sys
sys.path.insert(0, os.path.dirname(__file__))

# OSC imports
from pythonosc import osc_message_builder
from pythonosc import udp_client

class ExtemporeOscCommand(sublime_plugin.EventListener):

	def __init__(self):
		# print("created")
		self.port = 9880
		self.client = pythonosc.udp_client.UDPClient("localhost", self.port)

	def on_activated(self, view):
		# print("activated")
		focus = view.file_name()
		if focus is None:
			focus = "null"
		self.send("/interface/focus", focus)

	def on_selection_modified(self, view):
		# print("selection modified")
		selections = []
		for sel in view.sel():
			selections.append(sel.begin());
			if sel.begin() != sel.end():
				selections.append(sel.end());
		self.send("/interface/selection", len(selections), *selections);

	def on_modified(self, view):
		self.send("/interface/code", view.substr(sublime.Region(0, view.size())));

	def on_pre_save(self, view):
		self.send("/interface/save", "test")

	def on_text_command(self, view, command_name, args):
		if command_name == "extempore_connect":
			self.send("/interface/connect", 0)
		elif command_name == "extempore_disconnect":
			self.send("/interface/disconnect", 0)
		# print(command_name)
		return None

	def send(self, address, *args):
		# print("sending")
		msg = pythonosc.osc_message_builder.OscMessageBuilder(address = address)
		for arg in args:
			msg.add_arg(arg)
		msg = msg.build()
		self.client.send(msg)
