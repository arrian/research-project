import sublime, sublime_plugin
import os, time

# Hack for including modules
import os
import sys
sys.path.insert(0, os.path.dirname(__file__))

# OSC imports
import pythonosc

class ExtemporeOscCommand(sublime_plugin.EventListener):

	def __init__(self):
		print("created")
		self.port = 9880
		self.client = udp_client.UDPClient("localhost", self.port)

	def on_selection_modified(self, view):
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

	def send(self, address, *args):
		msg = osc_message_builder.OscMessageBuilder(address = address)
		for arg in args:
			msg.add_arg(arg)
		msg = msg.build()
		self.client.send(msg)


