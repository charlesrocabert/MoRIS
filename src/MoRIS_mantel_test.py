#!/usr/bin/env python
# coding: utf-8

#***********************************************************************
# MoRIS (Model of Routes of Invasive Spread)
# Copyright (c) 2014-2019 Charles Rocabert, Jérôme Gippet, Serge Fenet
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
#***********************************************************************

import os
import sys
import time
import numpy as np
import networkx as nx
import scipy.stats.mstats

### Load lineage tree data ###
def load_lineage_tree_data( lineage_tree_file ):
	# repetition start_node end_node geodesic_distance euclidean_dist iteration
	lineage_tree_data = {}
	f = open(lineage_tree_file, "r")
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
def load_coordinates( map_file ):
	coordinates = {}
	f = open(map_file, "r")
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
		edges = []
		for i in range(len(data["start_node"])):
			start_node = data["start_node"][i]
			end_node   = data["end_node"][i]
			if (start_node,end_node) not in edges:
				edges.append((start_node,end_node))
		
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 3) Build the lineage tree                  #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		G = nx.Graph()
		for node in index_map:
			G.add_node(node)
		for edge in edges:
			G.add_edge(edge[0], edge[1])
		#S = nx.all_pairs_dijkstra_path(G)
		L = nx.all_pairs_dijkstra_path_length(G)
		
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 4) Create and fill genetic distance matrix #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		genetic_mat = []
		for i in range(N):
			genetic_mat.append([0.0]*N)
		for node1 in index_map.keys():
			for node2 in index_map.keys():
				gdist = L[node1][node2]
				genetic_mat[index_map[node1]][index_map[node2]] = gdist
				genetic_mat[index_map[node2]][index_map[node1]] = gdist
		lineage_tree_data[repetition]["genetic_mat"] = genetic_mat
	return lineage_tree_data

### Create the temporary mantel test folder ###
def create_mantel_test_folder():
	os.mkdir("mantel_test_tmp")

### Delete the temporary mantel test folder ###
def delete_mantel_test_folder():
	os.system("rm -rf mantel_test_tmp")

### Write distance matrices ###
def write_matrices( euclidean_mat, genetic_mat ):
	f1 = open("mantel_test_tmp/euclidean.txt", "w")
	f2 = open("mantel_test_tmp/genetic.txt", "w")
	N  = len(euclidean_mat)
	for i in range(N):
		l1 = ""
		l2 = ""
		for j in range(N):
			l1 += str(euclidean_mat[i][j])+" "
			l2 += str(genetic_mat[i][j])+" "
		f1.write(l1.strip(" ")+"\n")
		f2.write(l2.strip(" ")+"\n")
	f1.close()
	f2.close()
	
### Compute Mantel test ###
def compute_mantel_test( mantel_test_rscript, working_directory, breakpoint ):
	os.system("Rscript "+mantel_test_rscript+" "+working_directory+"/mantel_test_tmp "+str(breakpoint))

### Collect the mantel test result ###
def collect_mantel_test_result( repetition ):
	try:
		f = open("mantel_test_tmp/mantel_test.txt", "r")
	except:
		return False, "", ""
	else:
		header = "repetition class_name "+f.readline().strip("\n")
		data   = []
		l = f.readline()
		while l:
			line = str(repetition)+" "+l.strip("\n")
			if "NA" not in line:
				data.append(line)
			l = f.readline()
		f.close()
		return True, header, data

##################
#      MAIN      #
##################

LINEAGE_TREE_FILE   = "output/lineage_tree.txt"
MAP_FILE            = "input/map.txt"
MANTEL_TEST_RSCRIPT = "/Users/charlesrocabert/git/MoRIS-development/src/mantel_test.R"
WORKING_DIRECTORY   = "/Users/charlesrocabert/git/MoRIS-development/RESULTS_2019/post_treatment/best_scenario"
BREAKPOINT          = 5000.0

coordinates       = load_coordinates(MAP_FILE)
lineage_tree_data = load_lineage_tree_data(LINEAGE_TREE_FILE)
lineage_tree_data = compute_euclidean_distance_matrices(lineage_tree_data, coordinates)
lineage_tree_data = compute_lineage_distance_matrices(lineage_tree_data)

all_tests = []
delete_mantel_test_folder()
f = open("MANTEL_TEST_RESULTS.txt", "w")
header_saved = False
for repetition in lineage_tree_data.keys():
	data = lineage_tree_data[repetition]
	create_mantel_test_folder()
	write_matrices(data["euclidean_mat"], data["genetic_mat"])
	compute_mantel_test(MANTEL_TEST_RSCRIPT, WORKING_DIRECTORY, BREAKPOINT)
	success, header, res = collect_mantel_test_result(repetition)
	if success:
		all_tests += res
		if not header_saved:
			f.write(header+"\n")
			header_saved = True
		for line in res:
			f.write(line+"\n")
		f.flush()
	delete_mantel_test_folder()
	time.sleep(0.1)
f.close()
