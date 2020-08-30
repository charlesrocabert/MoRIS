#!/usr/bin/env python3
# coding: utf-8

#***************************************************************************
# MoRIS (Model of Routes of Invasive Spread)
# Copyright © 2014-2020 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet
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
import cma
import time
import math
import subprocess
import numpy as np


### MoRIS optimizer class ###
class MoRIS_Optimizer:

	### Constructor ###
	def __init__( self ):

		#------------------------- PATHS #

		self.param_path = ""
		self.exec_path  = ""

		#------------------------- INPUTS #

		self.map_filename     = ""
		self.network_filename = ""
		self.sample_filename  = ""

		#------------------------- PARAMETERS #

		self.seed                  = 0
		self.type_of_data          = ""
		self.repetitions           = 0
		self.iterations            = 0
		self.jump_law              = ""
		self.optimization_function = ""
		self.human_activity_index  = ""
		self.wmin                  = 0.0

		#------------------------- DEFAULT PARAMETERS #

		self.default_parameters           = {}
		self.default_parameters["xintro"] = 0.0
		self.default_parameters["yintro"] = 0.0
		self.default_parameters["pintro"] = 0.0
		self.default_parameters["lambda"] = 0.0
		self.default_parameters["mu"]     = 0.0
		self.default_parameters["sigma"]  = 0.0
		self.default_parameters["gamma"]  = 0.0
		self.default_parameters["w1"]     = 0.0
		self.default_parameters["w2"]     = 0.0
		self.default_parameters["w3"]     = 0.0
		self.default_parameters["w4"]     = 0.0
		self.default_parameters["w5"]     = 0.0
		self.default_parameters["w6"]     = 0.0

		#------------------------- PARAMETERS TO OPTIMIZE #

		self.to_optimize           = {}
		self.to_optimize["xintro"] = False
		self.to_optimize["yintro"] = False
		self.to_optimize["pintro"] = False
		self.to_optimize["lambda"] = False
		self.to_optimize["mu"]     = False
		self.to_optimize["sigma"]  = False
		self.to_optimize["gamma"]  = False
		self.to_optimize["w1"]     = False
		self.to_optimize["w2"]     = False
		self.to_optimize["w3"]     = False
		self.to_optimize["w4"]     = False
		self.to_optimize["w5"]     = False
		self.to_optimize["w6"]     = False

		#------------------------- PARAMETERS RANGE #

		self.min_parameters           = {}
		self.min_parameters["xintro"] = 0.0
		self.min_parameters["yintro"] = 0.0
		self.min_parameters["pintro"] = 0.0
		self.min_parameters["lambda"] = 0.0
		self.min_parameters["mu"]     = 0.0
		self.min_parameters["sigma"]  = 0.0
		self.min_parameters["gamma"]  = 0.0
		self.min_parameters["w1"]     = 0.0
		self.min_parameters["w2"]     = 0.0
		self.min_parameters["w3"]     = 0.0
		self.min_parameters["w4"]     = 0.0
		self.min_parameters["w5"]     = 0.0
		self.min_parameters["w6"]     = 0.0

		self.max_parameters           = {}
		self.max_parameters["xintro"] = 0.0
		self.max_parameters["yintro"] = 0.0
		self.max_parameters["pintro"] = 0.0
		self.max_parameters["lambda"] = 0.0
		self.max_parameters["mu"]     = 0.0
		self.max_parameters["sigma"]  = 0.0
		self.max_parameters["gamma"]  = 0.0
		self.max_parameters["w1"]     = 0.0
		self.max_parameters["w2"]     = 0.0
		self.max_parameters["w3"]     = 0.0
		self.max_parameters["w4"]     = 0.0
		self.max_parameters["w5"]     = 0.0
		self.max_parameters["w6"]     = 0.0

		#------------------------- OPTIMIZATION #

		self.vector_length      = 0
		self.parameters_index   = {}
		self.current_parameters = {}
		self.best_parameters    = {}
		self.mean_parameters    = {}
		self.max_likelihood     = 0.0
		self.empty_likelihood   = 0.0
		self.likelihood         = 0.0
		self.empty_score        = 0.0
		self.current_score      = 0.0
		self.counter            = 0

	#### Parse a line and get the list of words ###
	def parse_line( self, line ):
		l    = line.strip("\n")
		l    = l.split(" ")
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
			print("Error: file "+self.param_path+ " not found.")
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
				elif l.startswith("OPTIMIZATION_FUNCTION"):
					data = self.parse_line(l)
					self.optimization_function = data[1]
				elif l.startswith("HUMAN_ACTIVITY_INDEX"):
					data = self.parse_line(l)
					if data[1] == "NO" or data[1] == "YES":
						self.human_activity_index = data[1]
					else:
						print(data[0]+" must be NO or YES.")
						sys.exit()
				elif l.startswith("WMIN"):
					data = self.parse_line(l)
					self.wmin = float(data[1])

				#------------------------- DEFAULT PARAMETERS #

				elif l.startswith("XINTRO_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["xintro"] = float(data[1])
				elif l.startswith("YINTRO_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["yintro"] = float(data[1])
				elif l.startswith("PINTRO_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["pintro"] = float(data[1])
				elif l.startswith("LAMBDA_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["lambda"] = float(data[1])
				elif l.startswith("MU_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["mu"] = float(data[1])
				elif l.startswith("SIGMA_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["sigma"] = float(data[1])
				elif l.startswith("GAMMA_DEFAULT"):
					data = self.parse_line(l)
					self.default_parameters["gamma"] = float(data[1])
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

				#------------------------- PARAMETERS TO OPTIMIZE #

				elif l.startswith("OPTIMIZE_XINTRO"):
					data = self.parse_line(l)
					if data[1] == "NO":
						self.to_optimize["xintro"] = False
					elif data[1] == "YES":
						self.to_optimize["xintro"] = True
					else:
						print(data[0]+" must be NO or YES.")
						sys.exit()
				elif l.startswith("OPTIMIZE_YINTRO"):
					data = self.parse_line(l)
					if data[1] == "NO":
						self.to_optimize["yintro"] = False
					elif data[1] == "YES":
						self.to_optimize["yintro"] = True
					else:
						print(data[0]+" must be NO or YES.")
						sys.exit()
				elif l.startswith("OPTIMIZE_PINTRO"):
					data = self.parse_line(l)
					if data[1] == "NO":
						self.to_optimize["pintro"] = False
					elif data[1] == "YES":
						self.to_optimize["pintro"] = True
					else:
						print(data[0]+" must be NO or YES.")
						sys.exit()
				elif l.startswith("OPTIMIZE_LAMBDA"):
					data = self.parse_line(l)
					if data[1] == "NO":
						self.to_optimize["lambda"] = False
					elif data[1] == "YES":
						self.to_optimize["lambda"] = True
					else:
						print(data[0]+" must be NO or YES.")
						sys.exit()
				elif l.startswith("OPTIMIZE_MU"):
					data = self.parse_line(l)
					if data[1] == "NO":
						self.to_optimize["mu"] = False
					elif data[1] == "YES":
						self.to_optimize["mu"] = True
					else:
						print(data[0]+" must be NO or YES.")
						sys.exit()
				elif l.startswith("OPTIMIZE_SIGMA"):
					data = self.parse_line(l)
					if data[1] == "NO":
						self.to_optimize["sigma"] = False
					elif data[1] == "YES":
						self.to_optimize["sigma"] = True
					else:
						print(data[0]+" must be NO or YES.")
						sys.exit()
				elif l.startswith("OPTIMIZE_GAMMA"):
					data = self.parse_line(l)
					if data[1] == "NO":
						self.to_optimize["gamma"] = False
					elif data[1] == "YES":
						self.to_optimize["gamma"] = True
					else:
						print(data[0]+" must be NO or YES.")
						sys.exit()
				elif l.startswith("OPTIMIZE_W1"):
					data = self.parse_line(l)
					if data[1] == "NO":
						self.to_optimize["w1"] = False
					elif data[1] == "YES":
						self.to_optimize["w1"] = True
					else:
						print(data[0]+" must be NO or YES.")
						sys.exit()
				elif l.startswith("OPTIMIZE_W2"):
					data = self.parse_line(l)
					if data[1] == "NO":
						self.to_optimize["w2"] = False
					elif data[1] == "YES":
						self.to_optimize["w2"] = True
					else:
						print(data[0]+" must be NO or YES.")
						sys.exit()
				elif l.startswith("OPTIMIZE_W3"):
					data = self.parse_line(l)
					if data[1] == "NO":
						self.to_optimize["w3"] = False
					elif data[1] == "YES":
						self.to_optimize["w3"] = True
					else:
						print(data[0]+" must be NO or YES.")
						sys.exit()
				elif l.startswith("OPTIMIZE_W4"):
					data = self.parse_line(l)
					if data[1] == "NO":
						self.to_optimize["w4"] = False
					elif data[1] == "YES":
						self.to_optimize["w4"] = True
					else:
						print(data[0]+" must be NO or YES.")
						sys.exit()
				elif l.startswith("OPTIMIZE_W5"):
					data = self.parse_line(l)
					if data[1] == "NO":
						self.to_optimize["w5"] = False
					elif data[1] == "YES":
						self.to_optimize["w5"] = True
					else:
						print(data[0]+" must be NO or YES.")
						sys.exit()
				elif l.startswith("OPTIMIZE_W6"):
					data = self.parse_line(l)
					if data[1] == "NO":
						self.to_optimize["w6"] = False
					elif data[1] == "YES":
						self.to_optimize["w6"] = True
					else:
						print(data[0]+" must be NO or YES.")
						sys.exit()

				#------------------------- PARAMETERS RANGE #

				elif l.startswith("XINTRO_MIN"):
					data = self.parse_line(l)
					self.min_parameters["xintro"] = float(data[1])
				elif l.startswith("YINTRO_MIN"):
					data = self.parse_line(l)
					self.min_parameters["yintro"] = float(data[1])
				elif l.startswith("PINTRO_MIN"):
					data = self.parse_line(l)
					self.min_parameters["pintro"] = float(data[1])
				elif l.startswith("LAMBDA_MIN"):
					data = self.parse_line(l)
					self.min_parameters["lambda"] = float(data[1])
				elif l.startswith("MU_MIN"):
					data = self.parse_line(l)
					self.min_parameters["mu"] = float(data[1])
				elif l.startswith("SIGMA_MIN"):
					data = self.parse_line(l)
					self.min_parameters["sigma"] = float(data[1])
				elif l.startswith("GAMMA_MIN"):
					data = self.parse_line(l)
					self.min_parameters["gamma"] = float(data[1])
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

				elif l.startswith("XINTRO_MAX"):
					data = self.parse_line(l)
					self.max_parameters["xintro"] = float(data[1])
				elif l.startswith("YINTRO_MAX"):
					data = self.parse_line(l)
					self.max_parameters["yintro"] = float(data[1])
				elif l.startswith("PINTRO_MAX"):
					data = self.parse_line(l)
					self.max_parameters["pintro"] = float(data[1])
				elif l.startswith("LAMBDA_MAX"):
					data = self.parse_line(l)
					self.max_parameters["lambda"] = float(data[1])
				elif l.startswith("MU_MAX"):
					data = self.parse_line(l)
					self.max_parameters["mu"] = float(data[1])
				elif l.startswith("SIGMA_MAX"):
					data = self.parse_line(l)
					self.max_parameters["sigma"] = float(data[1])
				elif l.startswith("GAMMA_MAX"):
					data = self.parse_line(l)
					self.max_parameters["gamma"] = float(data[1])
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

	### Compute the current parameters from the optimization vector ###
	def vector_to_current_parameters( self, params_vector ):
		for param in self.parameters_index.keys():
			index = self.parameters_index[param]
			normv = params_vector[index]
			minv  = self.min_parameters[param]
			maxv  = self.max_parameters[param]
			#-----------------#
			# Modulo wrapper  #
			#-----------------#
			#raw_val = normv*(maxv-minv)+minv
			#raw_val = (raw_val%(maxv-minv))+minv
			#-----------------#
			# Cosinus wrapper #
			#-----------------#
			#raw_val = minv + (maxv-minv)*(1.0-np.cos(np.pi*normv))/2.0
			#-----------------#
			# Linear wrapper  #
			#-----------------#
			if np.floor(normv)%2.0 == 0.0:
				raw_val = normv%1.0
			elif np.floor(normv)%2.0 == 1.0:
				raw_val = 1.0-normv%1.0
			raw_val = minv + (maxv-minv)*raw_val
			self.current_parameters[param] = raw_val

	### Compute parameter values from a vector ###
	def get_parameters_values( self, params_vector ):
		converted_values = {}
		for param in self.to_optimize.keys():
			if self.to_optimize[param]:
				index = self.parameters_index[param]
				normv = params_vector[index]
				minv  = self.min_parameters[param]
				maxv  = self.max_parameters[param]
				#-----------------#
				# Modulo wrapper  #
				#-----------------#
				#raw_val = normv*(maxv-minv)+minv
				#raw_val = (raw_val%(maxv-minv))+minv
				#-----------------#
				# Cosinus wrapper #
				#-----------------#
				#raw_val = minv + (maxv-minv)*(1.0-np.cos(np.pi*normv))/2.0
				#-----------------#
				# Linear wrapper  #
				#-----------------#
				if np.floor(normv)%2.0 == 0.0:
					raw_val = normv%1.0
				elif np.floor(normv)%2.0 == 1.0:
					raw_val = 1.0-normv%1.0
				raw_val = minv + (maxv-minv)*raw_val
				converted_values[param] = raw_val
			else:
				converted_values[param] = self.default_parameters[param]
		return converted_values

	### Build MoRIS_run command line ###
	def build_moris_command_line( self ):
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
		cmd_line  = self.exec_path
		cmd_line += " -map "+self.map_filename
		cmd_line += " -network "+self.network_filename
		cmd_line += " -sample "+self.sample_filename
		cmd_line += " -typeofdata "+self.type_of_data
		cmd_line += " -seed "+str(np.random.randint(1,100000000))
		cmd_line += " -reps "+str(self.repetitions)
		cmd_line += " -iters "+str(self.iterations)
		cmd_line += " -law "+self.jump_law
		cmd_line += " -optimfunc "+self.optimization_function
		cmd_line += " -humanactivity "+self.human_activity_index
		cmd_line += " -xintro "+str(command_line_params["xintro"])
		cmd_line += " -yintro "+str(command_line_params["yintro"])
		cmd_line += " -pintro "+str(command_line_params["pintro"])
		cmd_line += " -lambda "+str(command_line_params["lambda"])
		cmd_line += " -mu "+str(command_line_params["mu"])
		cmd_line += " -sigma "+str(command_line_params["sigma"])
		cmd_line += " -gamma "+str(command_line_params["gamma"])
		cmd_line += " -w1 "+str(command_line_params["w1"])
		cmd_line += " -w2 "+str(command_line_params["w2"])
		cmd_line += " -w3 "+str(command_line_params["w3"])
		cmd_line += " -w4 "+str(command_line_params["w4"])
		cmd_line += " -w5 "+str(command_line_params["w5"])
		cmd_line += " -w6 "+str(command_line_params["w6"])
		cmd_line += " -wmin "+str(self.wmin)
		return cmd_line

	### Build best and mean command line ###
	def build_optimized_command_lines( self ):
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 1) Build best parameters command line #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		best_cmd_line  = self.best_parameters["exec"]
		best_cmd_line += " -map "+self.best_parameters["map"]
		best_cmd_line += " -network "+self.best_parameters["network"]
		best_cmd_line += " -sample "+self.best_parameters["sample"]
		best_cmd_line += " -typeofdata "+self.best_parameters["typeofdata"]
		best_cmd_line += " -seed "+self.best_parameters["seed"]
		best_cmd_line += " -reps "+self.best_parameters["reps"]
		best_cmd_line += " -iters "+self.best_parameters["iters"]
		best_cmd_line += " -law "+self.best_parameters["law"]
		best_cmd_line += " -optimfunc "+self.best_parameters["optimfunc"]
		best_cmd_line += " -humanactivity "+self.best_parameters["humanactivity"]
		best_cmd_line += " -xintro "+self.best_parameters["xintro"]
		best_cmd_line += " -yintro "+self.best_parameters["yintro"]
		best_cmd_line += " -pintro "+self.best_parameters["pintro"]
		best_cmd_line += " -lambda "+self.best_parameters["lambda"]
		best_cmd_line += " -mu "+self.best_parameters["mu"]
		best_cmd_line += " -sigma "+self.best_parameters["sigma"]
		best_cmd_line += " -gamma "+self.best_parameters["gamma"]
		best_cmd_line += " -w1 "+self.best_parameters["w1"]
		best_cmd_line += " -w2 "+self.best_parameters["w2"]
		best_cmd_line += " -w3 "+self.best_parameters["w3"]
		best_cmd_line += " -w4 "+self.best_parameters["w4"]
		best_cmd_line += " -w5 "+self.best_parameters["w5"]
		best_cmd_line += " -w6 "+self.best_parameters["w6"]
		best_cmd_line += " -wmin "+self.best_parameters["wmin"]
		best_cmd_line += " -save-outputs\n"
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 2) Build mean parameters command line #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		mean_cmd_line  = self.mean_parameters["exec"]
		mean_cmd_line += " -map "+self.mean_parameters["map"]
		mean_cmd_line += " -network "+self.mean_parameters["network"]
		mean_cmd_line += " -sample "+self.mean_parameters["sample"]
		mean_cmd_line += " -typeofdata "+self.mean_parameters["typeofdata"]
		mean_cmd_line += " -seed "+self.mean_parameters["seed"]
		mean_cmd_line += " -reps "+self.mean_parameters["reps"]
		mean_cmd_line += " -iters "+self.mean_parameters["iters"]
		mean_cmd_line += " -law "+self.mean_parameters["law"]
		mean_cmd_line += " -optimfunc "+self.mean_parameters["optimfunc"]
		mean_cmd_line += " -humanactivity "+self.mean_parameters["humanactivity"]
		mean_cmd_line += " -xintro "+self.mean_parameters["xintro"]
		mean_cmd_line += " -yintro "+self.mean_parameters["yintro"]
		mean_cmd_line += " -pintro "+self.mean_parameters["pintro"]
		mean_cmd_line += " -lambda "+self.mean_parameters["lambda"]
		mean_cmd_line += " -mu "+self.mean_parameters["mu"]
		mean_cmd_line += " -sigma "+self.mean_parameters["sigma"]
		mean_cmd_line += " -gamma "+self.mean_parameters["gamma"]
		mean_cmd_line += " -w1 "+self.mean_parameters["w1"]
		mean_cmd_line += " -w2 "+self.mean_parameters["w2"]
		mean_cmd_line += " -w3 "+self.mean_parameters["w3"]
		mean_cmd_line += " -w4 "+self.mean_parameters["w4"]
		mean_cmd_line += " -w5 "+self.mean_parameters["w5"]
		mean_cmd_line += " -w6 "+self.mean_parameters["w6"]
		mean_cmd_line += " -wmin "+self.mean_parameters["wmin"]
		mean_cmd_line += " -save-outputs\n"
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 3) Return command lines               #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		return best_cmd_line, mean_cmd_line

	### Get cell id from coordinates ###
	def get_cell_from_coordinates( self, cell_x, cell_y ):
		best_dist = 1e+10
		best_id   = -1
		f = open(self.sample_filename, "r")
		l = f.readline()
		while l:
			l = l.strip("\n").split(" ")
			cell_id = int(l[0])
			x       = float(l[1])
			y       = float(l[2])
			dist    = math.sqrt((x-cell_x)*(x-cell_x)+(y-cell_y)*(y-cell_y))
			if best_dist > dist:
				best_dist = dist
				best_id   = cell_id
			l = f.readline()
		f.close()
		if best_id == -1:
			print("Error during cell id extraction from coordinates. Exit.")
			sys.exit()
		return best_id

	### Read the standard output from MoRIS_run ###
	def read_moris_output( self, moris_output ):
		output                = moris_output.strip("\n").split(" ")
		self.likelihood       = float(output[0])
		self.empty_likelihood = float(output[1])
		self.max_likelihood   = float(output[2])
		self.empty_score      = float(output[3])
		self.current_score    = float(output[4])

	### Open output file ###
	def open_output_file( self ):
		self.output_file = open("optimization.txt", "w")

	### Close output file ###
	def close_output_file( self ):
		self.output_file.close()

	### Write output file header ###
	def write_output_file_header( self ):
		line = "L empty_L max_L score empty_score"
		for param in self.default_parameters.keys():
			line += " "+param
		self.output_file.write(line+"\n")
		self.output_file.flush()

	### Write output file data ###
	def write_output_file_data( self ):
		line  = ""
		line += str(self.likelihood)+" "+str(self.empty_likelihood)+" "+str(self.max_likelihood)+" "
		line += str(self.current_score)+" "+str(self.empty_score)
		for param in self.to_optimize.keys():
			if self.to_optimize[param]:
				line += " "+str(self.current_parameters[param])
			else:
				line += " "+str(self.default_parameters[param])
		self.output_file.write(line+"\n")
		self.output_file.flush()

	### Minimization method provided to CMA-ES ###
	def minimization_function( self, params_vector ):
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 1) Get current parameter values        #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		self.vector_to_current_parameters(params_vector)
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 2) Execute MoRIS                       #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		moris_cmd_line = self.build_moris_command_line()
		moris_process  = subprocess.Popen([moris_cmd_line], stdout=subprocess.PIPE, shell=True)
		moris_output   = moris_process.stdout.read().decode('utf8')
		self.read_moris_output(moris_output)
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 3) Save the data and return the score  #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		self.write_output_file_data()
		if self.counter%10 == 0:
			os.system("Rscript ./rscripts/optimization.R > /dev/null &")
		self.counter += 1
		#return np.log10(self.current_score)
		return self.current_score

	### Save the results ###
	def save_results( self, xbest, xmean, score ):
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 1) Convert final optimization vectors #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		params_best = self.get_parameters_values(xbest)
		params_mean = self.get_parameters_values(xmean)
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 2) Build best parameters list         #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		self.best_parameters                  = {}
		self.best_parameters["exec"]          = self.exec_path
		self.best_parameters["map"]           = self.map_filename
		self.best_parameters["network"]       = self.network_filename
		self.best_parameters["sample"]        = self.sample_filename
		self.best_parameters["typeofdata"]    = self.type_of_data
		self.best_parameters["seed"]          = str(self.seed)
		self.best_parameters["reps"]          = str(self.repetitions)
		self.best_parameters["iters"]         = str(self.iterations)
		self.best_parameters["law"]           = self.jump_law
		self.best_parameters["optimfunc"]     = self.optimization_function
		self.best_parameters["humanactivity"] = self.human_activity_index
		self.best_parameters["xintro"]        = str(params_best["xintro"])
		self.best_parameters["yintro"]        = str(params_best["yintro"])
		self.best_parameters["pintro"]        = str(params_best["pintro"])
		self.best_parameters["lambda"]        = str(params_best["lambda"])
		self.best_parameters["mu"]            = str(params_best["mu"])
		self.best_parameters["sigma"]         = str(params_best["sigma"])
		self.best_parameters["gamma"]         = str(params_best["gamma"])
		self.best_parameters["w1"]            = str(params_best["w1"])
		self.best_parameters["w2"]            = str(params_best["w2"])
		self.best_parameters["w3"]            = str(params_best["w3"])
		self.best_parameters["w4"]            = str(params_best["w4"])
		self.best_parameters["w5"]            = str(params_best["w5"])
		self.best_parameters["w6"]            = str(params_best["w6"])
		self.best_parameters["wmin"]          = str(self.wmin)
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 3) Build mean parameters list         #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		self.mean_parameters                  = {}
		self.mean_parameters["exec"]          = self.exec_path
		self.mean_parameters["map"]           = self.map_filename
		self.mean_parameters["network"]       = self.network_filename
		self.mean_parameters["sample"]        = self.sample_filename
		self.mean_parameters["typeofdata"]    = self.type_of_data
		self.mean_parameters["seed"]          = str(self.seed)
		self.mean_parameters["reps"]          = str(self.repetitions)
		self.mean_parameters["iters"]         = str(self.iterations)
		self.mean_parameters["law"]           = self.jump_law
		self.mean_parameters["optimfunc"]     = self.optimization_function
		self.mean_parameters["humanactivity"] = self.human_activity_index
		self.mean_parameters["xintro"]        = str(params_mean["xintro"])
		self.mean_parameters["yintro"]        = str(params_mean["yintro"])
		self.mean_parameters["pintro"]        = str(params_mean["pintro"])
		self.mean_parameters["lambda"]        = str(params_mean["lambda"])
		self.mean_parameters["mu"]            = str(params_mean["mu"])
		self.mean_parameters["sigma"]         = str(params_mean["sigma"])
		self.mean_parameters["gamma"]         = str(params_mean["gamma"])
		self.mean_parameters["w1"]            = str(params_mean["w1"])
		self.mean_parameters["w2"]            = str(params_mean["w2"])
		self.mean_parameters["w3"]            = str(params_mean["w3"])
		self.mean_parameters["w4"]            = str(params_mean["w4"])
		self.mean_parameters["w5"]            = str(params_mean["w5"])
		self.mean_parameters["w6"]            = str(params_mean["w6"])
		self.mean_parameters["wmin"]          = str(self.wmin)
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 4) Save parameters                    #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		f_best = open("best.txt", "w")
		f_mean = open("mean.txt", "w")
		header = "score cell_id"
		for param in self.best_parameters.keys():
			header += " "+param
		f_best.write(header+"\n")
		f_mean.write(header+"\n")
		cell_id_best = self.get_cell_from_coordinates(float(self.best_parameters["xintro"]), float(self.best_parameters["yintro"]))
		cell_id_mean = self.get_cell_from_coordinates(float(self.mean_parameters["xintro"]), float(self.mean_parameters["yintro"]))
		l_best = str(score)+" "+str(cell_id_best)
		l_mean = str(score)+" "+str(cell_id_mean)
		for param in self.best_parameters.keys():
			l_best += " "+self.best_parameters[param]
			l_mean += " "+self.mean_parameters[param]
		f_best.write(l_best+"\n")
		f_mean.write(l_mean+"\n")
		f_best.close()
		f_mean.close()
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 5) Save command lines                 #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		best_cmd_line, mean_cmd_line = self.build_optimized_command_lines()
		f = open("best_cmd_line.sh", "w")
		f.write(best_cmd_line)
		f.close()
		f = open("mean_cmd_line.sh", "w")
		f.write(mean_cmd_line)
		f.close()
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		# 6) Run command lines                  #
		#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
		print("> Run MoRIS with best parameters")
		os.system(best_cmd_line)
		os.system("cp -r output best_output")
		print("> Run MoRIS with mean parameters")
		os.system(mean_cmd_line)
		os.system("cp -r output mean_output")
		os.system("rm -rf output")

### Print help ###
def printHelp():
	print("")
	print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
	print("                            * MoRIS Optimize *                             ")
	print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
	print(" MoRIS (Model of Routes of Invasive Spread)                                ")
	print(" Copyright © 2014-2020 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet   ")
	print(" Web: https://github.com/charlesrocabert/MoRIS                             ")
	print("                                                                           ")
	print(" This program comes with ABSOLUTELY NO WARRANTY.                           ")
	print(" This is free software, and you are welcome to redistribute                ")
	print(" it under certain conditions; See the GNU General Public                   ")
	print(" License for details.                                                      ")
	print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
	print("Usage: python MoRIS_optimize.py -h or --help")
	print("   or: python MoRIS_optimize.py [no arguments needed]")
	print("To run, MoRIS_optimize needs a parameters file and the input files.")
	print("")

### Print header ###
def printHeader():
	print("")
	print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
	print("                            * MoRIS Optimize *                             ")
	print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
	print(" MoRIS (Model of Routes of Invasive Spread)                                ")
	print(" Copyright © 2014-2020 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet   ")
	print(" Web: https://github.com/charlesrocabert/MoRIS                             ")
	print("                                                                           ")
	print(" This program comes with ABSOLUTELY NO WARRANTY.                           ")
	print(" This is free software, and you are welcome to redistribute                ")
	print(" it under certain conditions; See the GNU General Public                   ")
	print(" License for details.                                                      ")
	print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
	print("")

### Read command line arguments ###
def readArgs( argv ):
	for i in range(len(argv)):
		if argv[i] == "-h" or argv[i] == "--help":
			printHelp()
			sys.exit()


######################
#        MAIN        #
######################

TIME_THRESHOLD  = 180
PARAMETERS_PATH = "parameters.txt"

if __name__ == '__main__':
	#~~~~~~~~~~~~~~~~~~~~~~~~~~~#
	# 1) Print help if required #
	#~~~~~~~~~~~~~~~~~~~~~~~~~~~#
	readArgs(sys.argv)
	printHeader()

	#~~~~~~~~~~~~~~~~~~~~~~~~~~~#
	# 2) Create the optimizer   #
	#~~~~~~~~~~~~~~~~~~~~~~~~~~~#
	optimizer = MoRIS_Optimizer()
	optimizer.load_parameters(PARAMETERS_PATH)
	np.random.seed(optimizer.seed)

	#~~~~~~~~~~~~~~~~~~~~~~~~~~~#
	# 3) Run the optimizer      #
	#~~~~~~~~~~~~~~~~~~~~~~~~~~~#
	success = False
	while not success:
		optimizer.open_output_file()
		optimizer.write_output_file_header()
		vector    = optimizer.build_vector_of_parameters()
		start     = time.time()
		cmaes_res = cma.fmin(optimizer.minimization_function, vector, 0.2)
		end       = time.time()
		optimizer.close_output_file()
		if end-start < TIME_THRESHOLD:
			print(">> Optimization failure, restart")
		else:
			score   = cmaes_res[1]
			xbest   = cmaes_res[0]
			xmean   = cmaes_res[5]
			success = True
			optimizer.save_results(xbest, xmean, score)
