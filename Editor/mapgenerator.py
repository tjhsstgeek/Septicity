#!/usr/bin/python

import sys, os
from random import *
from ctypes import *

def generate_heights(board, width, height):
	[explode(board, i, width, randint(-100, 100)) for i, x in enumerate(board) if randint(0, 10) > 7]


def explode(board, i, width, rand, jump = 0):
	if rand < 10:
		return
	try:
		board[i+jump] += rand
	except: 
		pass
	try:
		board[i-jump] += rand
	except: 
		pass
	try:
		board[i+width] += rand
	except: 
		pass
	try:
		board[i-width] += rand
	except: 
		pass
	try:
		board[i+width+1] += rand
	except: 
		pass
	try:
		board[i+width-1] += rand
	except: 
		pass
	try:
		board[i-width+1] += rand
	except:
		pass
	try:
		board[i-width-1] += rand
	except: 
		pass
	explode(board, i, width, int(rand*0.25), jump+1)
	
def write_map(name, width, height, board, terrain, numplayers, texfile, filename):
	wr = open(filename, 'wb')
	tex = open(filename, 'rb').read()
	wr.write(c_uint(ord('p')*256*256*256+ord('a')*256*256+ord('m')*256))
	wr.write(c_uint(width/32))
	wr.write(c_uint(height/32))
	wr.write(c_ubyte(len(name)))
	wr.write(str(name))
	wr.write(c_ubyte(numplayers))
	for x in xrange(width*height):
		wr.write(c_int(board[x]))
		wr.write(c_uint(terrain[x]))
	wr.write(tex)
	wr.close()

def main(args):
	name = args[1]
	width, height = int(args[2]), int(args[3])
	board = [0 for x in xrange(width*height)]
	generate_heights(board, width, height)
	print board 
	terrain = [0 for x in xrange(width*height)]
	write_map(name, width, height, board, terrain, int(args[4]), args[5], args[6])


if __name__ == '__main__':
	main(sys.argv)
