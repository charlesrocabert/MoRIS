#!/usr/bin/env python
# coding: utf-8

#***************************************************************************
# MoRIS (Model of Routes of Invasive Spread)
# Copyright (c) 2014-2020 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet
# Web: https://github.com/charlesrocabert/MoRIS
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#***************************************************************************

import os
import sys
import time
import numpy as np
import networkx as nx
import scipy.stats.mstats

### Load lineage tree data ###
def load_lineage_tree_data():
	# repetition start_node end_node geodesic_distance euclidean_dist iteration
	lineage_tree_data = {}
	f = open("output/lineage_tree.txt", "r")
	l = f.readline()
	l = f.readline()
	while l:
		l = l.strip("\n").split(" ")
		repetition     = int(l[0])
		start_node     = int(l[1])
		end_node       = int(l[2])
		geodesic_dist  = float(l[3])
		euclidean_dist = float(l[4])
		iteration      = int(l[5])
		if repetition not in lineage_tree_data.keys():
			lineage_tree_data[repetition]                   = {}
			lineage_tree_data[repetition]["start_node"]     = []
			lineage_tree_data[repetition]["end_node"]       = []
			lineage_tree_data[repetition]["geodesic_dist"]  = []
			lineage_tree_data[repetition]["euclidean_dist"] = []
			lineage_tree_data[repetition]["iteration"]      = []
		lineage_tree_data[repetition]["start_node"].append(start_node)
		lineage_tree_data[repetition]["end_node"].append(end_node)
		lineage_tree_data[repetition]["geodesic_dist"].append(geodesic_dist)
		lineage_tree_data[repetition]["euclidean_dist"].append(euclidean_dist)
		lineage_tree_data[repetition]["iteration"].append(iteration)
		l = f.readline()
	f.close()
	return lineage_tree_data

### Load hexagon coordinates ###
def load_coordinates():
	coordinates = {}
	f = open("input/map.txt", "r")
	l = f.readline()
	while l:
		l = l.strip("\n")
		l = l.split(" ")
		coordinates[int(l[0])] = [float(l[1]), float(l[2])]
		l = f.readline()
	f.close()
	return coordinates

### Compute euclidean distance matrices for each repetition ###
def compute_euclidean_distance_matrices( lineage_tree_data, coordinates ):
	for repetition in lineage_tree_data.keys():
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 1) Get list of unique nodes                      #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		unique_nodes = []
		data         = lineage_tree_data[repetition]
		for i in range(len(data["start_node"])):
			if data["start_node"][i] not in unique_nodes:
				unique_nodes.append(data["start_node"][i])
			if data["end_node"][i] not in unique_nodes:
				unique_nodes.append(data["end_node"][i])
		N = len(unique_nodes)
		
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 2) Build index list                              #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		index     = 0
		index_map = {}
		for node in unique_nodes:
			index_map[node] = index
			index += 1
		
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 3) Create and fill the euclidean distance matrix #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		euclidean_mat = []
		for i in range(N):
			euclidean_mat.append([0.0]*N)
		for node1 in index_map.keys():
			for node2 in index_map.keys():
				x1 = coordinates[node1][0]
				y1 = coordinates[node1][1]
				x2 = coordinates[node2][0]
				y2 = coordinates[node2][1]
				d  = np.sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))
				euclidean_mat[index_map[node1]][index_map[node2]] = d
				euclidean_mat[index_map[node2]][index_map[node1]] = d
		lineage_tree_data[repetition]["index_map"]     = index_map
		lineage_tree_data[repetition]["euclidean_mat"] = euclidean_mat
	return lineage_tree_data

### Compute genetic distance matrices for each repetition ###
def compute_lineage_distance_matrices( lineage_tree_data ):
	for repetition in lineage_tree_data.keys():
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 1) Get index map                           #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		data      = lineage_tree_data[repetition]
		index_map = data["index_map"]
		N         = len(index_map.keys())
		
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 2) Extract unique edges                    #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# edges = {}
		# for i in range(len(data["start_node"])):
		# 	start_node = data["start_node"][i]
		# 	end_node   = data["end_node"][i]
		# 	if (start_node,end_node) not in edges.keys():
		# 		edges[(start_node,end_node)] = 1.0
		# 	else:
		# 		edges[(start_node,end_node)] /= 2.0
		edges_all   = []
		edges_first = []
		for i in range(len(data["start_node"])):
			start_node = data["start_node"][i]
			end_node   = data["end_node"][i]
			edges_all.append((start_node,end_node))
			if (start_node,end_node) not in edges_first and (end_node,start_node) not in edges_first:
				edges_first.append((start_node,end_node))
				
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 3) Build the lineage tree                  #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		G_all   = nx.Graph()
		G_first = nx.Graph()
		for node in index_map:
			G_all.add_node(node)
			G_first.add_node(node)
		for edge in edges_all:
			G_all.add_edge(edge[0], edge[1], weight=1)
		for edge in edges_first:
			G_first.add_edge(edge[0], edge[1], weight=1)
		L_all   = nx.all_pairs_dijkstra_path_length(G_all)
		L_first = nx.all_pairs_dijkstra_path_length(G_first)
		
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 4) Create and fill genetic distance matrix #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		genetic_mat_all   = []
		genetic_mat_first = []
		for i in range(N):
			genetic_mat_all.append([0.0]*N)
			genetic_mat_first.append([0.0]*N)
		for node1 in index_map.keys():
			for node2 in index_map.keys():
				gdist_all   = L_all[node1][node2]
				gdist_first = L_first[node1][node2]
				genetic_mat_all[index_map[node1]][index_map[node2]]   = gdist_all
				genetic_mat_all[index_map[node2]][index_map[node1]]   = gdist_all
				genetic_mat_first[index_map[node1]][index_map[node2]] = gdist_first
				genetic_mat_first[index_map[node2]][index_map[node1]] = gdist_first
		lineage_tree_data[repetition]["genetic_mat_all"] = genetic_mat_all
		lineage_tree_data[repetition]["genetic_mat_first"] = genetic_mat_first
	return lineage_tree_data

### Create the temporary mantel test folder ###
def create_mantel_test_folder():
	os.mkdir("mantel_test_tmp")

### Delete the temporary mantel test folder ###
def delete_mantel_test_folder():
	os.system("rm -rf mantel_test_tmp")

### Write distance matrices ###
def write_matrices( euclidean_mat, genetic_mat_all, genetic_mat_first ):
	f1      = open("mantel_test_tmp/euclidean.txt", "w")
	f2      = open("mantel_test_tmp/genetic_all.txt", "w")
	f3      = open("mantel_test_tmp/genetic_first.txt", "w")
	N       = len(euclidean_mat)
	INDEXES = range(0,N)
	if N > 30:
		np.random.shuffle(INDEXES)
		INDEXES = INDEXES[0:30]
	for i in INDEXES:
		l1 = ""
		l2 = ""
		l3 = ""
		for j in INDEXES:
			l1 += str(euclidean_mat[i][j])+" "
			l2 += str(genetic_mat_all[i][j])+" "
			l3 += str(genetic_mat_first[i][j])+" "
		f1.write(l1.strip(" ")+"\n")
		f2.write(l2.strip(" ")+"\n")
		f3.write(l3.strip(" ")+"\n")
	f1.close()
	f2.close()
	f3.close()
	
### Compute Mantel test ###
def compute_mantel_test( mantel_test_rscript, working_directory, breakpoint ):
	os.system("Rscript "+mantel_test_rscript+" "+working_directory+"/mantel_test_tmp "+str(breakpoint))

### Collect the mantel test result for all jumps ###
def collect_mantel_test_result_all( repetition ):
	try:
		f = open("mantel_test_tmp/mantel_test_all.txt", "r")
	except:
		return False, "", []
	else:
		header = "repetition class_name "+f.readline().strip("\n")
		data   = []
		l      = f.readline()
		while l:
			line = str(repetition)+" "+l.strip("\n")
			if "NA" not in line:
				data.append(line)
			l = f.readline()
		f.close()
		return True, header, data

### Collect the mantel test result for first jumps ###
def collect_mantel_test_result_first( repetition ):
	try:
		f = open("mantel_test_tmp/mantel_test_first.txt", "r")
	except:
		return False, "", []
	else:
		header = "repetition class_name "+f.readline().strip("\n")
		data   = []
		l      = f.readline()
		while l:
			line = str(repetition)+" "+l.strip("\n")
			if "NA" not in line:
				data.append(line)
			l = f.readline()
		f.close()
		return True, header, data	
		
### Print help ###
def printHelp():
	print ""
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	print "                           * MoRIS Mantel Test *                           "
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	print " MoRIS (Model of Routes of Invasive Spread)                                "
	print " Copyright (c) 2014-2020 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet "
	print " Web: https://github.com/charlesrocabert/MoRIS                             "
	print "                                                                           "
	print " This program comes with ABSOLUTELY NO WARRANTY.                           "
	print " This is free software, and you are welcome to redistribute                "
	print " it under certain conditions; See the GNU General Public                   "
	print " License for details.                                                      "
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	print "Usage: python MoRIS_mantel_test.py -h or --help";
	print "   or: python MoRIS_mantel_test.py [list of mandatory arguments]";
	print "Options are:"
	print "  -h, --help"
	print "        print this help, then exit"
	print "  -mantel-test-rscript, --mantel-test-rscript <mantel_test_tmprscript> (mandatory)"
	print "        Specify the emplacement of the Mantel test Rscript"
	print "  -breakpoint, --breakpoint <breakpoint> (mandatory)"
	print "        Specify the distance classes breakpoint"
	print ""

### Print header ###
def printHeader():
	print ""
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	print "                           * MoRIS Mantel Test *                           "
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	print " MoRIS (Model of Routes of Invasive Spread)                                "
	print " Copyright (c) 2014-2020 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet "
	print " Web: https://github.com/charlesrocabert/MoRIS                             "
	print "                                                                           "
	print " This program comes with ABSOLUTELY NO WARRANTY.                           "
	print " This is free software, and you are welcome to redistribute                "
	print " it under certain conditions; See the GNU General Public                   "
	print " License for details.                                                      "
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	print ""

### Read command line arguments ###
def readArgs( argv ):
	arguments                        = {}
	arguments["mantel-test-rscript"] = ""
	arguments["breakpoint"]          = 0
	for i in range(len(argv)):
		if argv[i] == "-h" or argv[i] == "--help":
			printHelp()
			sys.exit()
		if argv[i] == "-mantel-test-rscript" or argv[i] == "--mantel-test-rscript":
			arguments["mantel-test-rscript"] = argv[i+1]
		if argv[i] == "-breakpoint" or argv[i] == "--breakpoint":
			arguments["breakpoint"] = int(argv[i+1])
	return arguments


######################
#        MAIN        #
######################

if __name__ == '__main__':
	#~~~~~~~~~~~~~~~~~~~~~~~~~#
	# 1) Read arguments       #
	#~~~~~~~~~~~~~~~~~~~~~~~~~#
	arguments = readArgs(sys.argv)
	printHeader()
	
	#~~~~~~~~~~~~~~~~~~~~~~~~~#
	# 2) Load data            #
	#~~~~~~~~~~~~~~~~~~~~~~~~~#
	print ">>> Load coordinates"
	coordinates = load_coordinates()
	print ">>> Load lineage tree data"
	lineage_tree_data = load_lineage_tree_data()
	print ">>> Compute euclidean distances"
	lineage_tree_data = compute_euclidean_distance_matrices(lineage_tree_data, coordinates)
	print ">>> Compute genetic distances"
	lineage_tree_data = compute_lineage_distance_matrices(lineage_tree_data)

	#~~~~~~~~~~~~~~~~~~~~~~~~~#
	# 3) Compute Mantel tests #
	#~~~~~~~~~~~~~~~~~~~~~~~~~#
	print ">>> Compute Mantel autocorrelograms"
	delete_mantel_test_folder()
	f1                 = open("output/mantel_autocorrelogram_all.txt", "w")
	f2                 = open("output/mantel_autocorrelogram_first.txt", "w")
	header_saved_all   = False
	header_saved_first = False
	for repetition in lineage_tree_data.keys():
		print "    rep "+str(repetition)
		data = lineage_tree_data[repetition]
		create_mantel_test_folder()
		write_matrices(data["euclidean_mat"], data["genetic_mat_all"], data["genetic_mat_first"])
		compute_mantel_test(arguments["mantel-test-rscript"], os.getcwd(), arguments["breakpoint"])
		success_all, header_all, res_all       = collect_mantel_test_result_all(repetition)
		success_first, header_first, res_first = collect_mantel_test_result_first(repetition)
		if success_all:
			if not header_saved_all:
				f1.write(header_all+"\n")
				header_saved_all = True
			for line in res_all:
				f1.write(line+"\n")
			f1.flush()
		if success_first:
			if not header_saved_first:
				f2.write(header_first+"\n")
				header_saved_first = True
			for line in res_first:
				f2.write(line+"\n")
			f2.flush()
		delete_mantel_test_folder()
		time.sleep(0.1)
	f1.close()
	f2.close()
