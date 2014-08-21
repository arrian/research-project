import sublime, sublime_plugin
import os, time

# Hack for including modules
import os
import sys
sys.path.insert(0, os.path.dirname(__file__))

# OSC imports
import pythonosc
from pythonosc import osc_message_builder
from pythonosc import udp_client

MAX_LENGTH = 2048

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
		self.send(view, "/interface/focus", focus)

	def on_selection_modified(self, view):
		# print("selection modified")
		selections = []
		for sel in view.sel():
			selections.append(sel.begin());
			if sel.begin() != sel.end():
				selections.append(sel.end());
		self.send(view, "/interface/selection", len(selections), *selections);

	def on_modified(self, view):
		result = view.substr(sublime.Region(0, view.size()))
		self.send(view, "/interface/code", result[:MAX_LENGTH]);

	def on_pre_save(self, view):
		self.send(view, "/interface/save", "test")

	def on_text_command(self, view, command_name, args):
		if command_name == "extempore_connect":
			self.send(view, "/interface/connect", 0)
		elif command_name == "extempore_disconnect":
			self.send(view, "/interface/disconnect", 0)
		elif command_name == "extempore_evaluate":
			result = self.evaluate_hack(view)
			# print(result)
			self.send(view, "/interface/evaluate", result)
		# print(command_name)
		return None

	def send(self, view, address, *args):

		# only send for extempore files
		if view.file_name() is None or not view.file_name().endswith(".xtm"):
			return
			
		# print(view.file_name())
		# print("sending to %s - temp disabled" % address)
		# return
		msg = pythonosc.osc_message_builder.OscMessageBuilder(address = address)
		for arg in args:
			msg.add_arg(arg)
		msg = msg.build()
		self.client.send(msg)


	def evaluate_hack(self, view):
		v = view
		result = ''
		if v.sel()[0].empty():
			result = self.evaluate_hack_top_level_definition(view)
		else:
			result = v.substr(v.sel()[0])
		return result[:MAX_LENGTH] # trim string to max length of 2048

	def evaluate_hack_top_level_definition(self, view): 
		v = view
		v.run_command("single_selection")
		initial_reg = v.sel()[0]
		reg = initial_reg
		old_reg = None
		# loop until the region stabilises or starts at the beginning of a line
		while reg != old_reg and v.rowcol(reg.a)[1] != 0:
			v.run_command("expand_selection", {"to": "brackets"})
			old_reg = reg
			reg = v.sel()[0]
		def_str = v.substr(v.sel()[0])

		# return the point to where it was
		v.sel().clear()
		v.sel().add(initial_reg)
		return def_str
