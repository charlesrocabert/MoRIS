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
import subprocess
import numpy as np

### Load the list of parameters to test ###
def load_list_of_parameter_sets( filename ):
	parameter_sets = {}
	f              = open(filename, "r")
	header         = f.readline()
	variables      = header.strip("\n").split(" ")
	l              = f.readline()
	while l:
		l = l.strip("\n").split(" ")
		set   = {}
		score = 0.0
		for i in range(len(variables)):
			set[variables[i]] = l[i]
			if variables[i] == "score":
				score = float(l[i])
		parameter_sets[score] = set
		l = f.readline()
	f.close()
	return parameter_sets, header, variables

### Build MoRIS run command line from a parameters set ###
def build_command_line( param_set, exec_path ):
	cmd_line  = exec_path
	cmd_line += " -map input/map.txt"
	cmd_line += " -network input/network.txt"
	cmd_line += " -sample input/sample.txt"
	cmd_line += " -typeofdata "+param_set["typeofdata"]
	cmd_line += " -seed "+str(np.random.randint(1,100000000))
	cmd_line += " -reps "+param_set["reps"]
	cmd_line += " -iters "+param_set["iters"]
	cmd_line += " -law "+param_set["law"]
	cmd_line += " -optimfunc "+param_set["optimfunc"]
	cmd_line += " -xintro "+param_set["xintro"]
	cmd_line += " -yintro "+param_set["yintro"]
	cmd_line += " -pintro "+param_set["pintro"]
	cmd_line += " -lambda "+param_set["lambda"]
	cmd_line += " -mu "+param_set["mu"]
	cmd_line += " -sigma "+param_set["sigma"]
	cmd_line += " -gamma "+param_set["gamma"]
	cmd_line += " -w1 "+param_set["w1"]
	cmd_line += " -w2 "+param_set["w2"]
	cmd_line += " -w3 "+param_set["w3"]
	cmd_line += " -w4 "+param_set["w4"]
	cmd_line += " -w5 "+param_set["w5"]
	cmd_line += " -w6 "+param_set["w6"]
	cmd_line += " -wmin "+param_set["wmin"]
	cmd_line += "\n"
	return cmd_line
	
### Read the standard output from MoRIS_run ###
def read_output( output ):
	output           = output.strip("\n").split(" ")
	likelihood       = float(output[0])
	empty_likelihood = float(output[1])
	max_likelihood   = float(output[2])
	empty_score      = float(output[3])
	score            = float(output[4])
	res                     = {}
	res["likelihood"]       = likelihood
	res["empty_likelihood"] = empty_likelihood
	res["max_likelihood"]   = max_likelihood
	res["empty_score"]      = empty_score
	res["score"]            = score
	return res

### Run one MoRIS session ###
def run_moris( param_set, exec_path ):
	cmd_line     = build_command_line(param_set, exec_path)
	moris_stdout = subprocess.Popen([cmd_line], stdout=subprocess.PIPE, shell=True)
	output       = moris_stdout.stdout.read()
	res          = read_output(output)
	return res

### Order CMA-ES scores in a vector ###
def get_ordered_CMAES_scores( parameter_sets ):
	scores = []
	for score in parameter_sets.keys():
		scores.append(score)
	scores.sort()
	return scores

### Print help ###
def printHelp():
	print ""
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	print "                           * MoRIS Evaluate *                         "
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	print " MoRIS (Model of Routes of Invasive Spread)                           "
	print " Copyright (c) 2014-2019 Charles Rocabert, Jérôme Gippet, Serge Fenet "
	print " Web: https://github.com/charlesrocabert/MoRIS                        "
	print "                                                                      "
	print " This program comes with ABSOLUTELY NO WARRANTY.                      "
	print " This is free software, and you are welcome to redistribute           "
	print " it under certain conditions; See the GNU General Public              "
	print " License for details.                                                 "
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	print "Usage: python MoRIS_evaluate.py -h or --help";
	print "   or: python MoRIS_evaluate.py [list of mandatory arguments]";
	print "Options are:"
	print "  -h, --help"
	print "        print this help, then exit"
	print "  -results-filename, --results-filename <results_filename> (mandatory)"
	print "        Specify the emplacement of the file containing MoRIS_optimize results"
	print "  -exec-path, --exec-path <exec_path> (mandatory)"
	print "        Specify the emplacement of MoRIS_run executable"
	print "  -reps, --reps <reps> (mandatory)"
	print "        Specify the number of repetitions needed to estimate the mean score"
	print ""

### Print header ###
def printHeader():
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	print "                           * MoRIS Evaluate *                         "
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	print " MoRIS (Model of Routes of Invasive Spread)                           "
	print " Copyright (c) 2014-2019 Charles Rocabert, Jérôme Gippet, Serge Fenet "
	print " Web: https://github.com/charlesrocabert/MoRIS                        "
	print "                                                                      "
	print " This program comes with ABSOLUTELY NO WARRANTY.                      "
	print " This is free software, and you are welcome to redistribute           "
	print " it under certain conditions; See the GNU General Public              "
	print " License for details.                                                 "
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

### Read command line arguments ###
def readArgs( argv ):
	arguments                     = {}
	arguments["results-filename"] = ""
	arguments["exec-path"]        = ""
	arguments["reps"]             = 0
	for i in range(len(argv)):
		if argv[i] == "-h" or argv[i] == "--help":
			printHelp()
			exit_thread()
		if argv[i] == "-results-filename" or argv[i] == "--results-filename":
			arguments["results-filename"] = argv[i+1]
		if argv[i] == "-exec-path" or argv[i] == "--exec-path":
			arguments["exec-path"] = argv[i+1]
		if argv[i] == "-reps" or argv[i] == "--reps":
			arguments["reps"] = int(argv[i+1])
	return arguments
	
######################
#        MAIN        #
######################

# PARAMS_FILE = "/Users/charlesrocabert/git/MoRIS-development/RESULTS_2019/results/optimizable_introduction_prevalence_N1000-3-fully_anisotropic_collected_mean.txt"
# EXEC_PATH   = "/Users/charlesrocabert/git/MoRIS-development/build/bin/MoRIS_run"
# REPS        = 1000

if __name__ == '__main__':
	
	#~~~~~~~~~~~~~~~~~~~~~~#
	# 1) Read arguments    #
	#~~~~~~~~~~~~~~~~~~~~~~#
	arguments = readArgs(sys.argv)
	printHeader()
	
	#~~~~~~~~~~~~~~~~~~~~~~#
	# 2) Load datasets     #
	#~~~~~~~~~~~~~~~~~~~~~~#
	parameter_sets, header, variables = load_list_of_parameter_sets(arguments["results-filename"])
	
    #~~~~~~~~~~~~~~~~~~~~~~#
	# 3) Open saving files #
	#~~~~~~~~~~~~~~~~~~~~~~#
	f1 = open("estimations_all.txt", "w")
	f1.write("cmaes test\n")
	f2 = open("estimations_mean.txt", "w")
	f2.write("cmaes test_mean test_var\n")
	f3 = open("rebuilt_list_of_parameter_sets.txt", "w")
	f3.write(header.strip("\n")+" test_mean test_var empty max\n")
	
	#~~~~~~~~~~~~~~~~~~~~~~#
	# 4) Start evaluation  #
	#~~~~~~~~~~~~~~~~~~~~~~#
	ordered_cmaes_scores = get_ordered_CMAES_scores(parameter_sets)
	N = len(ordered_cmaes_scores)
	for i in range(N):
		cmaes_score = ordered_cmaes_scores[i]
		param_set   = parameter_sets[cmaes_score]
		mean_score  = 0.0
		var_score   = 0.0
		mean_empty  = 0.0
		mean_max    = 0.0
		print "> Score "+str(cmaes_score)+" ("+str(round(float(i+1)/float(N), 4))+"%) ..."
		for j in range(arguments["reps"]):
			print "rep "+str(j+1)+" ..."
			res         = run_moris(param_set, arguments["exec-path"])
			test_score  = res["score"]
			test_empty  = res["empty_likelihood"]
			test_max    = res["max_likelihood"]
			mean_score += test_score
			var_score  += test_score*test_score
			mean_empty += test_empty
			mean_max   += test_max
			f1.write(str(cmaes_score)+" "+str(test_score)+"\n")
			f1.flush()
		mean_score /= float(arguments["reps"])
		var_score  /= float(arguments["reps"])
		var_score  -= mean_score*mean_score
		mean_empty /= float(arguments["reps"])
		mean_max   /= float(arguments["reps"])
		f2.write(str(cmaes_score)+" "+str(mean_score)+" "+str(var_score)+"\n")
		f2.flush()
		line = ""
		for var in variables:
			line += str(param_set[var])+" "
		line += str(mean_score)+" "+str(var_score)+" "+str(mean_empty)+" "+str(mean_max)+"\n"
		f3.write(line)
		f3.flush()
	f1.close()
	f2.close()
	f3.close()
	
