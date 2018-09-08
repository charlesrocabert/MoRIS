def convert_to_txt( filename, textfilename ):
	f = open(filename, "r")
	g = open(textfilename, "w")
	l = f.readline()
	while l:
		l = l.strip("\r\n").split(";")
		line = ""
		for elmt in l:
			line += elmt+" "
		g.write(line.strip(" ")+"\n")
		l = f.readline()
	f.close()
	g.close()

convert_to_txt("map_OK.csv", "map.txt")
convert_to_txt("network_OK.csv", "network.txt")
convert_to_txt("sample_OK.csv", "sample.txt")

