#!/usr/bin/env python3

import png
import numpy as np
import sys

np.set_printoptions(threshold=np.inf)

def get_list1():
	list_keys1 = []
	for i in range(32, 126):
		if i != 34:
			list_keys1.append(f'{chr(i)}')
	return list_keys1

def get_list2():
	list_keys2 = []
	for i in range(32, 126):
		if i != 34:
			for j in range(32, 126):
				if j != 34:
					list_keys2.append(f'{chr(i)+ chr(j)}')
	return list_keys2

def get_list3(): #804357 size
	list_keys3 = []
	for i in range(32, 126):
		if i != 34:
			for j in range(32, 126):
				if j != 34:
					for k in range(32, 126):
						if k != 34:
							list_keys3.append(f'{chr(i)+ chr(j) + chr(k)}')
	return list_keys3

def get_colors(pixels):
	global depth
	colors_list = []
	colums = len(pixels)
	rows = len(pixels[0])
	for colums in range(colums):
		i = 0
		if depth == 4:
			while i < rows:
				color = \
				str(format(pixels[colums][i+3], "02X")) \
				+ str(format(pixels[colums][i+0], "02X")) \
				+ str(format(pixels[colums][i+1], "02X")) \
				+ str(format(pixels[colums][i+2], "02X"))
				colors_list.append(color)
				i += depth
		else:
			while i < rows:
				color = \
				  str(format(pixels[colums][i+0], "02X")) \
				+ str(format(pixels[colums][i+1], "02X")) \
				+ str(format(pixels[colums][i+2], "02X"))
				colors_list.append(color)
				i += depth
			
	return colors_list


def pixels_to_xpm(pixels, filename):
	final_dict = {}
	pixels_list = get_colors(pixels)
	pixels_len = len(set(list(pixels_list)))
	char_per_pixels = 1
	colums = len(pixels)
	rows = round(len(pixels[0]) / depth)
	if pixels_len <= 93:
		list_keys = get_list1()
	elif pixels_len <= 8649:
		char_per_pixels = 2
		list_keys = get_list2()
	else:
		char_per_pixels = 3
		list_keys = get_list3()
	with open(filename, "w") as f:
		f.write("/* XPM */\n")
		f.write("static char *result[] = {\n")
		f.write("/* columns rows colors chars-per-pixel */\n")
		nb_keys = 0
		for pixel in pixels_list:
			if pixel not in final_dict:
				final_dict[pixel] = list_keys[nb_keys]
				nb_keys += 1
		size_fin_colors = len(final_dict)
		f.write(f"\"{rows} {colums} {size_fin_colors} {char_per_pixels} \",\n")
		for col in final_dict:
			f.write(f"\"{final_dict[col]} c #{col}\",\n")
		f.write("/* pixels */\n\"")
		i = 0
		for color in pixels_list:
			f.write(str(final_dict[color]))
			i += 1
			if i % rows == 0:
				f.write("\"")
				if i != colums * rows:
					f.write(",\n\"")
				else:
					f.write("\n};")

depth = 0

def parse_pixels(arg):
	global depth
	if len(arg) < 2:
		print("Usage : 'python3 png_to_xpm.py input_file.png [output_name.xpm]'")
		return
	try:
		with open(arg[1], "rb") as f:
			reader = png.Reader(f)
			_, _, pixels, meta = reader.asDirect()
			if meta['alpha'] == True:
				depth = 4
			else:
				depth = 3
			pixel_array = np.array(list(pixels))
			if len(arg) == 3:
				pixels_to_xpm(pixel_array, arg[2])
			else:
				if arg[1][-4:] == "png":
					pixels_to_xpm(pixel_array, arg[1][:-3] + "xpm")
				else:
					pixels_to_xpm(pixel_array, arg[1] + ".xpm")
	except (IOError, png.FormatError, OSError):
		print("Input file does not exist or is corrupted png file.")

def main():
	parse_pixels(sys.argv)

if __name__ == "__main__":
	main()
