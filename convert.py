from PIL import Image
import sys
import random

def get_colors(pixels):
	colors_list = []
	height = len(pixels)
	width = len(pixels[0])  # Remove the division by depth
	
	for y in range(height):
		for x in range(width):
			pixel = pixels[y][x]  # Access pixel directly
			if isinstance(pixel, int):  # Handle single channel images
				color = format(pixel, "02X") * 3
			else:
				if len(pixel) == 4:  # RGBA
					if pixel[3] < 128:  # If mostly transparent
						colors_list.append('None')
						continue
					color = ''.join(format(pixel[j], "02X") for j in range(3))
				else:  # RGB
					color = ''.join(format(pixel[j], "02X") for j in range(3))
			colors_list.append(color)
	return colors_list

def pixels_to_xpm(pixels, filename):
	pixels_list = get_colors(pixels)
	rows = len(pixels)
	columns = len(pixels[0])
	
	# Create a mapping of colors to two-character combinations
	unique_colors = list(set(pixels_list))
	# Generate two-character combinations using letters and numbers
	chars = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'
	color_chars = []
	for _ in range(len(unique_colors)):
		char_pair = random.choice(chars) + random.choice(chars)
		while char_pair in color_chars:  # Ensure uniqueness
			char_pair = random.choice(chars) + random.choice(chars)
		color_chars.append(char_pair)
	
	color_map = dict(zip(unique_colors, color_chars))
	
	# Write XPM file
	with open(filename, 'w') as f:
		f.write("/* XPM */\n")
		f.write("static char *result[] = {\n")
		f.write("/* columns rows colors chars-per-pixel */\n")
		f.write(f"\"{columns} {rows} {len(unique_colors)} 2 \",\n")
		
		# Write color definitions
		for color, char in color_map.items():
			if color == 'None':
				f.write(f"\"{char} c None\",\n")
			else:
				f.write(f"\"{char} c #{color}\",\n")
		
		# Write pixel data
		for y in range(rows):
			row = ''.join(color_map[pixels_list[y * columns + x]] for x in range(columns))
			if y == rows - 1:
				f.write(f"\"{row}\"\n")
			else:
				f.write(f"\"{row}\",\n")
		f.write("};\n")

if __name__ == "__main__":
	if len(sys.argv) != 3:
		print("Usage: python3 convert.py input.png output.xpm")
		sys.exit(1)
	
	input_file = sys.argv[1]
	output_file = sys.argv[2]
	
	# Open and convert the image
	img = Image.open(input_file)
	
	# Convert image to RGBA if it isn't already
	img = img.convert('RGBA')
	
	# Get pixel data as a list
	pixels = list(img.getdata())
	
	# Convert to 2D list with correct dimensions
	width, height = img.size
	pixels = [pixels[i:i+width] for i in range(0, len(pixels), width)]
	
	# Convert to XPM
	pixels_to_xpm(pixels, output_file)
