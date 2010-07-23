#!/usr/bin/python

import wx, sys, os
from wx.lib import intctrl

class MWindow(wx.Frame):
	def __init__(self, parent, title):
		wx.Frame.__init__(self, parent, title=title, size=(800, 600))
		self.CreateStatusBar()
		self.existing = False
		self.heightmap = True
		self.curmap = Map()
		self.mapdir = ''
		self.create_menu()
		self.Show(True)
	
	def create_menu(self):
		menubar = wx.MenuBar()

		filemenu = wx.Menu()
		newitem = filemenu.Append(wx.ID_NEW, "&New", "New map")
		self.Bind(wx.EVT_MENU, self.newmap, newitem)
		openitem = filemenu.Append(wx.ID_OPEN, "&Open", "Open existing map")
		self.Bind(wx.EVT_MENU, self.openmap, openitem)
		saveitem = filemenu.Append(wx.ID_SAVE, "&Save", "Save map to file")
		self.Bind(wx.EVT_MENU, self.savemap, saveitem)
		exititem = filemenu.Append(wx.ID_EXIT, "E&xit", "End program")
		self.Bind(wx.EVT_MENU, self.exit, exititem)

		helpmenu = wx.Menu()
		aboutitem = helpmenu.Append(wx.ID_ABOUT, "&About", "About")
		self.Bind(wx.EVT_MENU, self.about, aboutitem)
		
		menubar.Append(filemenu, "&File")
		menubar.Append(helpmenu, "&Help")
		self.SetMenuBar(menubar)

	
	def create_map_panel(self):
		panel = wx.Panel(self)
		mappanel = wx.Panel(panel)
		self.drawmap(mappanel)
		sidepanel = wx.Panel(panel, pos=(600, 0))
		grid = wx.GridSizer(4, 2)
		caption = wx.StaticText(sidepanel, label="Map Name: ", size=(100, 50))
		field = wx.TextCtrl(sidepanel, size=(100, 50))
		grid.Add(caption)
		grid.Add(field)
		sidepanel.SetSizer(grid)


	def drawmap(self, mappanel):
		pass

	def newmap(self, event):
		if self.existing: # implement some dialog asking to save file
			pass
		self.existing = False
		width, height = 0, 0
		dlg = wx.NumberEntryDialog(self, "Enter width for new map:", "width: ", "", 0, 32, 2048)
		if dlg.ShowModal() == wx.ID_OK:
			width = dlg.GetValue()
		dlg.Destroy()
		if width != 0:
			dlg = wx.NumberEntryDialog(self, "Enter height for new map:", "height: ","", 0, 32, 2048)
			if dlg.ShowModal() == wx.ID_OK:
				height = dlg.GetValue()
			dlg.Destroy()
			self.curmap.new(width, height)
			self.create_map_panel()
	
	def openmap(self, event):
		if self.existing:
#		wxMessageDialog( <-- implement some dialog asking to save file
			pass
		self.existing = True
		dlg = wx.FileDialog(self, "Choose a file to open:", self.mapdir, "", "*.*", wx.OPEN)
		if dlg.ShowModal() == wx.ID_OK:
			self.mapdir = dlg.GetDirectory()
			self.mapname = dlg.GetFilename()
			self.curmap.open(os.path.join(self.mapdir, self.mapname))
		dlg.Destroy()

	def savemap(self, event):
		if not self.existing:
			self.mapdir = ''
			dlg = wx.FileDialog(self, "Choose a file to save to:", self.mapdir, "", "*.*", wx.SAVE_AS)
			if dlg.ShowModal() == wx.ID_OK:
				self.mapname= dlg.GetFilename()
				self.mapdir = dlg.GetDirectory()
				self.existing = True
			dlg.Destroy()
		if self.existing:
			self.curmap.save(os.path.join(self.mapdir, self.mapname))

	def exit(self, event):
		if self.existing: # implement do you really want to exit?
			pass
		sys.exit(0)

	def about(self, event):
		box = wx.MessageDialog(self, "A Map Editor for Septicity", "About Septicity Map Editor")
		box.ShowModal()
		box.Destroy()


class CreationDialog(wx.Dialog):  #useless
	def __init__(self, parent, title):
		wx.Dialog.__init__(self, parent, title=title)
		vbox = wx.BoxSizer(wx.VERTICAL)
		hbox1 = wx.BoxSizer(wx.HORIZONTAL)
		widthbox = intctrl.IntCtrl(self)
		hbox2 = wx.BoxSizer(wx.HORIZONTAL)
		heightbox = intctrl.IntCtrl(self)
		hbox3 = wx.BoxSizer(wx.HORIZONTAL)
		ok = wx.Button(self, wx.ID_OK)
		cancel = wx.Button(self, wx.ID_CANCEL)
		hbox1.Add(wx.StaticText(self, -1, 'Width: '), 0, wx.RIGHT, 10)
		hbox1.Add(widthbox, 0, wx.EXPAND|wx.RIGHT|wx.LEFT, 10)
		hbox2.Add(wx.StaticText(self, -1, 'Height: '), 0, wx.RIGHT, 10)
		hbox2.Add(heightbox, 0, wx.EXPAND|wx.RIGHT|wx.LEFT, 10)
		hbox3.Add(ok, 0, wx.RIGHT | wx.BOTTOM, 10)
		hbox3.Add(cancel, 0, wx.RIGHT | wx.BOTTOM, 10)
		vbox.Add(hbox1, 0)
		vbox.Add(hbox2, 0)
		vbox.Add(hbox3, 0)
		self.SetSizer(vbox)
		self.ShowModal()
		self.Destroy()

class Map:
	def __init__(self):
		pass

	def new(self, width, height):
		self.width = width
		self.height = height
		self.location = []
		for x in xrange(width*height):
			self.location.append((0, 0))

	def open(self, filename):
		self.filename = filename
		self.mapfile = open(filename, 'rb')
		magic = self.mapfile.read(4)
		if magic != 'p'*256*256*256+'a'*256*256+'m'*256:
			print "Error, invalid magic number"
			return
		self.width = self.mapfile.read(4)
		self.height= self.mapfile.read(4)
		self.namesize = self.mapfile.read(1)
		self.name = self.mapfile.read(self.namesize)
		self.players = self.mapfile.read(1)
		self.texsize = self.mapfile.read(1)
		self.texfile = self.mapfile.read(self.texsize)
		self.location = []
		for i in xrange(self.width*self.height):
			self.location.append(self.mapfile.read(4), self.mapfile.read(4))
		self.mapfile.close()
	
	def save(self, filename):
		f = open(filename, 'wb')
		f.write(int('p'*256*256*256*256+'a'*256*256+'m'*256))



def main(args):
	app = wx.App(False)
	frame = MWindow(None, "Septicity Map Editor")
	app.MainLoop()


if __name__ == '__main__':
	main(sys.argv)
