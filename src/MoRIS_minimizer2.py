#!/usr/bin/env python
# coding: utf-8

#***********************************************************************
# MoRIS (Model of Routes of Invasive Spread)
# Copyright (c) 2014-2018 Charles Rocabert, Jérôme Gippet, Serge Fenet
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
import cma
import time
import math
import subprocess
import numpy as np

#~~~~~~~~~~~~~~~~~~~~~~~#
# MoRIS Minimizer class #
#~~~~~~~~~~~~~~~~~~~~~~~#
class MoRIS_Minimizer:

	### Consructor ###
	def __init__( self ):

		#------------------------- PATHS #

		self.param_path = ""
		self.exec_path  = ""

		#------------------------- INPUTS #

		self.map_filename     = ""
		self.network_filename = ""
		self.sample_filename  = ""

		#------------------------- PARAMETERS #

		self.seed         = 0
		self.type_of_data = ""
		self.repetitions  = 0
		self.iterations   = 0
		self.jump_law     = ""

		#------------------------- DEFAULT PARAMETERS #

		self.default_parameters = {}
		self.default_parameters["x"]       = 0.0
		self.default_parameters["y"]       = 0.0
		self.default_parameters["p_intro"] = 0.0
		self.default_parameters["lambda"]  = 0.0
		self.default_parameters["mu"]      = 0.0
		self.default_parameters["sigma"]   = 0.0
		self.default_parameters["w1"]      = 0.0
		self.default_parameters["w2"]      = 0.0
		self.default_parameters["w3"]      = 0.0
		self.default_parameters["w4"]      = 0.0
		self.default_parameters["w5"]      = 0.0
		self.default_parameters["w6"]      = 0.0
		self.default_parameters["min_w"]   = 0.0

		#------------------------- PARAMETERS TO OPTIMIZE #

		self.to_optimize = {}
		self.to_optimize["x"]       = False
		self.to_optimize["y"]       = False
		self.to_optimize["p_intro"] = False
		self.to_optimize["lambda"]  = False
		self.to_optimize["mu"]      = False
		self.to_optimize["sigma"]   = False
		self.to_optimize["w1"]      = False
		self.to_optimize["w2"]      = False
		self.to_optimize["w3"]      = False
		self.to_optimize["w4"]      = False
		self.to_optimize["w5"]      = False
		self.to_optimize["w6"]      = False
		self.to_optimize["min_w"]   = False

		#------------------------- PARAMETERS RANGE #

		self.min_parameters = {}
		self.min_parameters["x"]       = 0.0
		self.min_parameters["y"]       = 0.0
		self.min_parameters["p_intro"] = 0.0
		self.min_parameters["lambda"]  = 0.0
		self.min_parameters["mu"]      = 0.0
		self.min_parameters["sigma"]   = 0.0
		self.min_parameters["w1"]      = 0.0
		self.min_parameters["w2"]      = 0.0
		self.min_parameters["w3"]      = 0.0
		self.min_parameters["w4"]      = 0.0
		self.min_parameters["w5"]      = 0.0
		self.min_parameters["w6"]      = 0.0
		self.min_parameters["min_w"]   = 0.0

		self.max_parameters = {}
		self.max_parameters["x"]       = 0.0
		self.max_parameters["y"]       = 0.0
		self.max_parameters["p_intro"] = 0.0
		self.max_parameters["lambda"]  = 0.0
		self.max_parameters["mu"]      = 0.0
		self.max_parameters["sigma"]   = 0.0
		self.max_parameters["w1"]      = 0.0
		self.max_parameters["w2"]      = 0.0
		self.max_parameters["w3"]      = 0.0
		self.max_parameters["w4"]      = 0.0
		self.max_parameters["w5"]      = 0.0
		self.max_parameters["w6"]      = 0.0
		self.max_parameters["min_w"]   = 0.0

		#------------------------- OPTIMIZATION #

		self.vector_length      = 0
		self.parameters_index   = {}
		self.current_parameters = {}
		self.current_score      = 0.0

	#### Parse a line and get the list of words ###
	def parse_line( self, line ):
		l = line.strip("\n")
		l = l.split(" ")
		data = []
		for i in range(len(l)):
			if l[i] != "":
				data.append(l[i].strip(" "))
		return data

	### Load parameters from file ###
	def load_parameters( self, params_path ):
		self.param_path = params_path
		try:
			f = open(self.param_path, "r")
		except:
			print "Error: file "+self.param_path+ " not found."
			sys.exit()
		else:
			l = f.readline()
			while l:
				l = l.strip("\n")
				
				#------------------------- EXEC PATH #

				if l.startswith("EXEC_PATH"):
					data = self.parse_line(l)
					self.exec_path = data[1]

				#------------------------- INPUTS #

				elif l.startswith("MAP_FILE"):
					data = self.parse_line(l)
					self.map_filename = data[1]
				elif l.startswith("NETWORK_FILE"):
					data = self.parse_line(l)
					self.network_filename = data[1]
				elif l.startswith("SAMPLE_FILE"):
					data = self.parse_line(l)
					self.sample_filename = data[1]
				
				#------------------------- PARAMETERS #

				elif l.startswith("SEED"):
					data = self.parse_line(l)
					self.seed = int(data[1])
				elif l.startswith("TYPE_OF_DATA"):
					data = self.parse_line(l)
					self.type_of_data = data[1]
				elif l.startswith("REPETITIONS_BY_SIMULATION"):
					data = self.parse_line(l)
					self.repetitions = int(data[1])
				elif l.startswith("NUMBER_OF_ITERATIONS"):
					data = self.parse_line(l)
					self.iterations = int(data[1])
				elif l.startswith("JUMP_LAW"):
					data = self.parse_line(l)
					self.jump_law = data[1]

				#------------------------- DEFAULT PARAMETERS #

				elif l.startswith("X_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["x"] = float(data[1])
				elif l.startswith("Y_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["y"] = float(data[1])
				elif l.startswith("P_INTRO_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["p_intro"] = float(data[1])
				elif l.startswith("LAMBDA_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["lambda"] = float(data[1])
				elif l.startswith("MU_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["mu"] = float(data[1])
				elif l.startswith("SIGMA_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["sigma"] = float(data[1])
				elif l.startswith("W1_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["w1"] = float(data[1])
				elif l.startswith("W2_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["w2"] = float(data[1])
				elif l.startswith("W3_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["w3"] = float(data[1])
				elif l.startswith("W4_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["w4"] = float(data[1])
				elif l.startswith("W5_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["w5"] = float(data[1])
				elif l.startswith("W6_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["w6"] = float(data[1])
				elif l.startswith("MIN_W_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["min_w"] = float(data[1])

				#------------------------- PARAMETERS TO OPTIMIZE #

				elif l.startswith("OPTIMIZE_X"):
					data = self.parse_line(l)
					if data[1] == "FALSE":
						self.to_optimize["x"] = False
					elif data[1] == "TRUE":
						self.to_optimize["x"] = True
					else:
						print data[0]+" must be FALSE or TRUE."
						sys.exit()
				elif l.startswith("OPTIMIZE_Y"):
					data = self.parse_line(l)
					if data[1] == "FALSE":
						self.to_optimize["y"] = False
					elif data[1] == "TRUE":
						self.to_optimize["y"] = True
					else:
						print data[0]+" must be FALSE or TRUE."
						sys.exit()
				elif l.startswith("OPTIMIZE_P_INTRO"):
					data = self.parse_line(l)
					if data[1] == "FALSE":
						self.to_optimize["p_intro"] = False
					elif data[1] == "TRUE":
						self.to_optimize["p_intro"] = True
					else:
						print data[0]+" must be FALSE or TRUE."
						sys.exit()
				elif l.startswith("OPTIMIZE_LAMBDA"):
					data = self.parse_line(l)
					if data[1] == "FALSE":
						self.to_optimize["lambda"] = False
					elif data[1] == "TRUE":
						self.to_optimize["lambda"] = True
					else:
						print data[0]+" must be FALSE or TRUE."
						sys.exit()
				elif l.startswith("OPTIMIZE_MU"):
					data = self.parse_line(l)
					if data[1] == "FALSE":
						self.to_optimize["mu"] = False
					elif data[1] == "TRUE":
						self.to_optimize["mu"] = True
					else:
						print data[0]+" must be FALSE or TRUE."
						sys.exit()
				elif l.startswith("OPTIMIZE_SIGMA"):
					data = self.parse_line(l)
					if data[1] == "FALSE":
						self.to_optimize["sigma"] = False
					elif data[1] == "TRUE":
						self.to_optimize["sigma"] = True
					else:
						print data[0]+" must be FALSE or TRUE."
						sys.exit()
				elif l.startswith("OPTIMIZE_W1"):
					data = self.parse_line(l)
					if data[1] == "FALSE":
						self.to_optimize["w1"] = False
					elif data[1] == "TRUE":
						self.to_optimize["w1"] = True
					else:
						print data[0]+" must be FALSE or TRUE."
						sys.exit()
				elif l.startswith("OPTIMIZE_W2"):
					data = self.parse_line(l)
					if data[1] == "FALSE":
						self.to_optimize["w2"] = False
					elif data[1] == "TRUE":
						self.to_optimize["w2"] = True
					else:
						print data[0]+" must be FALSE or TRUE."
						sys.exit()
				elif l.startswith("OPTIMIZE_W3"):
					data = self.parse_line(l)
					if data[1] == "FALSE":
						self.to_optimize["w3"] = False
					elif data[1] == "TRUE":
						self.to_optimize["w3"] = True
					else:
						print data[0]+" must be FALSE or TRUE."
						sys.exit()
				elif l.startswith("OPTIMIZE_W4"):
					data = self.parse_line(l)
					if data[1] == "FALSE":
						self.to_optimize["w4"] = False
					elif data[1] == "TRUE":
						self.to_optimize["w4"] = True
					else:
						print data[0]+" must be FALSE or TRUE."
						sys.exit()
				elif l.startswith("OPTIMIZE_W5"):
					data = self.parse_line(l)
					if data[1] == "FALSE":
						self.to_optimize["w5"] = False
					elif data[1] == "TRUE":
						self.to_optimize["w5"] = True
					else:
						print data[0]+" must be FALSE or TRUE."
						sys.exit()
				elif l.startswith("OPTIMIZE_W6"):
					data = self.parse_line(l)
					if data[1] == "FALSE":
						self.to_optimize["w6"] = False
					elif data[1] == "TRUE":
						self.to_optimize["w6"] = True
					else:
						print data[0]+" must be FALSE or TRUE."
						sys.exit()
				elif l.startswith("OPTIMIZE_MIN_W"):
					data = self.parse_line(l)
					if data[1] == "FALSE":
						self.to_optimize["min_w"] = False
					elif data[1] == "TRUE":
						self.to_optimize["min_w"] = True
					else:
						print data[0]+" must be FALSE or TRUE."
						sys.exit()

				#------------------------- PARAMETERS RANGE #

				elif l.startswith("X_MIN"):
					data = self.parse_line(l)
					self.min_parameters["x"] = float(data[1])
				elif l.startswith("Y_MIN"):
					data = self.parse_line(l)
					self.min_parameters["y"] = float(data[1])
				elif l.startswith("P_INTRO_MIN"):
					data = self.parse_line(l)
					self.min_parameters["p_intro"] = float(data[1])
				elif l.startswith("LAMBDA_MIN"):
					data = self.parse_line(l)
					self.min_parameters["lambda"] = float(data[1])
				elif l.startswith("MU_MIN"):
					data = self.parse_line(l)
					self.min_parameters["mu"] = float(data[1])
				elif l.startswith("SIGMA_MIN"):
					data = self.parse_line(l)
					self.min_parameters["sigma"] = float(data[1])
				elif l.startswith("W1_MIN"):
					data = self.parse_line(l)
					self.min_parameters["w1"] = float(data[1])
				elif l.startswith("W2_MIN"):
					data = self.parse_line(l)
					self.min_parameters["w2"] = float(data[1])
				elif l.startswith("W3_MIN"):
					data = self.parse_line(l)
					self.min_parameters["w3"] = float(data[1])
				elif l.startswith("W4_MIN"):
					data = self.parse_line(l)
					self.min_parameters["w4"] = float(data[1])
				elif l.startswith("W5_MIN"):
					data = self.parse_line(l)
					self.min_parameters["w5"] = float(data[1])
				elif l.startswith("W6_MIN"):
					data = self.parse_line(l)
					self.min_parameters["w6"] = float(data[1])
				elif l.startswith("MIN_W_MIN"):
					data = self.parse_line(l)
					self.min_parameters["min_w"] = float(data[1])

				elif l.startswith("X_MAX"):
					data = self.parse_line(l)
					self.max_parameters["x"] = float(data[1])
				elif l.startswith("Y_MAX"):
					data = self.parse_line(l)
					self.max_parameters["y"] = float(data[1])
				elif l.startswith("P_INTRO_MAX"):
					data = self.parse_line(l)
					self.max_parameters["p_intro"] = float(data[1])
				elif l.startswith("LAMBDA_MAX"):
					data = self.parse_line(l)
					self.max_parameters["lambda"] = float(data[1])
				elif l.startswith("MU_MAX"):
					data = self.parse_line(l)
					self.max_parameters["mu"] = float(data[1])
				elif l.startswith("SIGMA_MAX"):
					data = self.parse_line(l)
					self.max_parameters["sigma"] = float(data[1])
				elif l.startswith("W1_MAX"):
					data = self.parse_line(l)
					self.max_parameters["w1"] = float(data[1])
				elif l.startswith("W2_MAX"):
					data = self.parse_line(l)
					self.max_parameters["w2"] = float(data[1])
				elif l.startswith("W3_MAX"):
					data = self.parse_line(l)
					self.max_parameters["w3"] = float(data[1])
				elif l.startswith("W4_MAX"):
					data = self.parse_line(l)
					self.max_parameters["w4"] = float(data[1])
				elif l.startswith("W5_MAX"):
					data = self.parse_line(l)
					self.max_parameters["w5"] = float(data[1])
				elif l.startswith("W6_MAX"):
					data = self.parse_line(l)
					self.max_parameters["w6"] = float(data[1])
				elif l.startswith("MIN_W_MAX"):
					data = self.parse_line(l)
					self.max_parameters["min_w"] = float(data[1])

				l = f.readline()

			f.close()

	### Generate random vector ###
	def build_vector_of_parameters( self ):
		vector                  = []
		self.vector_length      = 0
		self.parameters_index   = {}
		self.current_parameters = {}
		for param in self.to_optimize.keys():
			if self.to_optimize[param]:
				vector.append(np.random.random())
				self.parameters_index[param]    = self.vector_length
				self.current_parameters[param]  = 0.0
				self.vector_length             += 1
		return vector

	### Compute the current parameters from the optimization vector
	def vectorToCurrentParameters( self, params_vector ):
		for param in self.parameters_index.keys():
			index   = self.parameters_index[param]
			normv   = params_vector[index]
			minv    = self.min_parameters[param]
			maxv    = self.max_parameters[param]
			raw_val = normv*(maxv-minv)+minv
			self.current_parameters[param] = (raw_val%(maxv-minv))+minv

	### Compute parameter values from a vector
	def getParametersValues( self, params_vector ):
		converted_values = {}
		for param in self.to_optimize.keys():
			if self.to_optimize[param]:
				index   = self.parameters_index[param]
				normv   = params_vector[index]
				minv    = self.min_parameters[param]
				maxv    = self.max_parameters[param]
				raw_val = normv*(maxv-minv)+minv
				converted_values[param] = (raw_val%(maxv-minv))+minv
			else:
				converted_values[param] = self.default_parameters[param]
		return converted_values

	### Get command line ###
	def get_command_line( self ):
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 1) Retrieve parameters    #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		command_line_params = {}
		for param in self.to_optimize.keys():
			if self.to_optimize[param]:
				command_line_params[param] = self.current_parameters[param]
			else:
				command_line_params[param] = self.default_parameters[param]

		#~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 2) Build the command line #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		cmd_line = self.exec_path
		cmd_line += " -map "+self.map_filename
		cmd_line += " -network "+self.network_filename
		cmd_line += " -sample "+self.sample_filename
		cmd_line += " -typeofdata "+self.type_of_data
		cmd_line += " -seed "+str(self.seed)
		cmd_line += " -rep "+str(self.repetitions)
		cmd_line += " -iterations "+str(self.iterations)
		cmd_line += " -law "+self.jump_law
		cmd_line += " -xcoord "+str(command_line_params["x"])
		cmd_line += " -ycoord "+str(command_line_params["y"])
		#cmd_line += " -p-intro "+str(command_line_params["p_intro"])
		cmd_line += " -lambda "+str(command_line_params["lambda"])
		cmd_line += " -mu "+str(command_line_params["mu"])
		cmd_line += " -sigma "+str(command_line_params["sigma"])
		cmd_line += " -wroad1 "+str(command_line_params["w1"])
		cmd_line += " -wroad2 "+str(command_line_params["w2"])
		cmd_line += " -wroad3 "+str(command_line_params["w3"])
		cmd_line += " -wroad4 "+str(command_line_params["w4"])
		cmd_line += " -wroad5 "+str(command_line_params["w5"])
		cmd_line += " -wroad6 "+str(command_line_params["w6"])
		cmd_line += " -min-connectivity "+str(command_line_params["min_w"])
		return cmd_line

	### Read the standard output ###
	def read_exec_ouput( self, exec_output ):
		output             = exec_output.strip("\n")
		self.current_score = float(exec_output)

	### Open output file header ###
	def open_output_file_header( self ):
		self.output_file = open("optim.txt", "w", 0)

	### Close output file header ###
	def close_output_file_header( self ):
		self.output_file.close()

	### Write output file header ###
	def write_output_file_header( self ):
		line = "score"
		for param in self.default_parameters.keys():
			line += " "+param
		self.output_file.write(line+"\n")

	### Write output file data ###
	def write_output_file_data( self, score ):
		line = str(score)
		for param in self.to_optimize.keys():
			if self.to_optimize[param]:
				line += " "+str(self.current_parameters[param])
			else:
				line += " "+str(self.default_parameters[param])
		self.output_file.write(line+"\n")

	### Function used by CMA-ES ###
	def minimization_function( self, params_vector ):
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 1) Return penalty if out of boundaries #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		#print params_vector
		#for val in params_vector:
		#	if val < 0.0 or val > 1.0:
		#		return 1000.0
		# COMMENT --> MODULO APPLIED IN BELOW FUNCTION

		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 2) Get current parameter values        #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		self.vectorToCurrentParameters(params_vector)

		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 3) Execute MoRIS                       #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		cmd_line    = self.get_command_line()
		process     = subprocess.Popen([cmd_line], stdout=subprocess.PIPE, shell=True)
		exec_output = process.stdout.read()
		self.read_exec_ouput(exec_output)

		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 4) Save the data and return the score  #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		self.write_output_file_data(self.current_score)
		return self.current_score

#~~~~~~~~~~~~~~#
# Print header #
#~~~~~~~~~~~~~~#
def printHeader():
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	print "                           * MoRIS Minimizer *                        "
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	print " MoRIS (Model of Routes of Invasive Spread)                           "
	print "                                                                      "
	print " Copyright (c) 2014-2018 Charles Rocabert, Jérôme Gippet, Serge Fenet "
	print "                                                                      "
	print " This program comes with ABSOLUTELY NO WARRANTY.                      "
	print " This is free software, and you are welcome to redistribute           "
	print " it under certain conditions; See the GNU General Public              "
	print " License for details.                                                 "
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

#~~~~~~~~~~~~~~~~~~#
# Write the result #
#~~~~~~~~~~~~~~~~~~#
def saveResults( xbest, xmean ):
	f1     = open("best.txt", "w")
	f2     = open("mean.txt", "w")
	header = ""
	for param in xbest.keys():
		header += param+" "
	f1.write(header.strip(" ")+"\n")
	f2.write(header.strip(" ")+"\n")
	l1 = ""
	l2 = ""
	for param in xbest.keys():
		l1 += str(xbest[param])+" "
		l2 += str(xmean[param])+" "
	f1.write(l1.strip(" ")+"\n")
	f2.write(l2.strip(" ")+"\n")
	f1.close()
	f2.close()



##############
#    MAIN    #
##############

TIME_THRESHOLD  = 180
PARAMETERS_PATH = "parameters.txt"

if __name__ == '__main__':
	#~~~~~~~~~~~~~~~~~~~~~~~~~#
	# 1) Create the minimizer #
	#~~~~~~~~~~~~~~~~~~~~~~~~~#
	minimizer = MoRIS_Minimizer()
	minimizer.load_parameters(PARAMETERS_PATH)
	np.random.seed(minimizer.seed)

	#~~~~~~~~~~~~~~~~~~~~~~~~~#
	# 2) Run the CMA-ES       #
	#~~~~~~~~~~~~~~~~~~~~~~~~~#
	success = False
	while not success:
		minimizer.open_output_file_header()
		minimizer.write_output_file_header()
		vector    = minimizer.build_vector_of_parameters()
		start     = time.time()
		cmaes_res = cma.fmin(minimizer.minimization_function, vector, 0.8)
		end       = time.time()
		minimizer.close_output_file_header()
		if end-start < TIME_THRESHOLD:
			print ">> Minimization Failure, restart"
		else:
			xbest = cmaes_res[0]
			xmean = cmaes_res[5]
			params_best = minimizer.getParametersValues(xbest)
			params_mean = minimizer.getParametersValues(xmean)
			saveResults(params_best, params_mean)
			success = True

# res[0] (xopt) -- best evaluated solution
# res[1] (fopt) -- respective function value
# res[2] (evalsopt) -- respective number of function evaluations
# res[3] (evals) -- number of overall conducted objective function evaluations
# res[4] (iterations) -- number of overall conducted iterations
# res[5] (xmean) -- mean of the final sample distribution
# res[6] (stds) -- effective stds of the final sample distribution
# res[-3] (stop) -- termination condition(s) in a dictionary
# res[-2] (cmaes) -- class CMAEvolutionStrategy instance
# res[-1] (logger) -- class CMADataLogger instance

