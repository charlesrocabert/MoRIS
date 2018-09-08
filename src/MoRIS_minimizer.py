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

DEFAULT_PARAMETERS_FILE  = "parameters.txt"
DEFAULT_MAP_FILENAME     = "map.txt"
DEFAULT_NETWORK_FILENAME = "network.txt"
DEFAULT_SAMPLE_FILENAME  = "sample.txt"
DEFAULT_RUN_PATH         = "../build/bin/MoRIS_run"
DEFAULT_REPETITIONS      = 100
TIME_THRESHOLD           = 180

#~~~~~~~~~~~~~~~~~~~~~~~~~#
# Define class Parameters #
#~~~~~~~~~~~~~~~~~~~~~~~~~#
class Parameters:

	### Constructor ###
	def __init__( self ):
		self.executable_path                  = ""
		self.parameters_filename              = ""
		self.seed                             = 0
		self.type_of_data                     = "PRESENCE_ONLY"
		self.map_filename                     = ""
		self.network_filename                 = ""
		self.sample_filename                  = ""
		self.repetitions                      = 0
		self.iterations                       = 0
		self.optimize_coordinates             = False
		self.optimize_lambda                  = False
		self.optimize_mu                      = False
		self.optimize_sigma                   = False
		self.optimize_road_linear_combination = False
		self.optimize_minimal_connectivity    = False
		self.optimization_vector_length       = 0
		self.x_boundaries                     = [0.0, 0.0]
		self.y_boundaries                     = [0.0, 0.0]
		self.jump_lambda                      = [0.0, 0.0]
		self.jump_mu                          = [0.0, 0.0]
		self.jump_sigma                       = [0.0, 0.0]
		self.jump_law                         = ""
		self.road_linear_combination          = [[0.0, 0.0], [0.0, 0.0], [0.0, 0.0], [0.0, 0.0], [0.0, 0.0], [0.0, 0.0]]
		self.minimal_connectivity             = [0.0, 0.0]

	### Load parameters from file ###
	def load_parameters_from_file( self, parameters_filename, executable_path ):
		self.executable_path = executable_path
		try:
			f = open(parameters_filename, "r")
		except:
			print "Error: file "+parameters_filename+ " not found."
			sys.exit()
		else:
			#---------------------------------------------#
			# 1) Parse the file and fill parameter states #
			#---------------------------------------------#
			self.parameters_filename = parameters_filename
			l = f.readline()
			while l:
				l = l.strip("\n")
				
				if l.startswith("SEED"):
					data = self.parse_line(l)
					self.seed = int(data[1])

				elif l.startswith("TYPE_OF_DATA"):
					data = self.parse_line(l)
					self.type_of_data = data[1]

				elif l.startswith("MAP_FILE"):
					data = self.parse_line(l)
					self.map_filename = data[1]
					if data[1] == "DEFAULT":
						self.map_filename = DEFAULT_MAP_FILENAME

				elif l.startswith("NETWORK_FILE"):
					data = self.parse_line(l)
					self.network_filename = data[1]
					if data[1] == "DEFAULT":
						self.network_filename = DEFAULT_NETWORK_FILENAME

				elif l.startswith("SAMPLE_FILE"):
					data = self.parse_line(l)
					self.sample_filename = data[1]
					if data[1] == "DEFAULT":
						self.sample_filename = DEFAULT_SAMPLE_FILENAME
				
				elif l.startswith("REPETITIONS_BY_SIMULATION"):
					data = self.parse_line(l)
					self.repetitions = int(data[1])

				elif l.startswith("NUMBER_OF_ITERATIONS"):
					data = self.parse_line(l)
					self.iterations = int(data[1])

				elif l.startswith("OPTIMIZE_COORDINATES"):
					data = self.parse_line(l)
					if data[1] == "FALSE" or data[1] == "false":
						self.optimize_coordinates = False
					elif data[1] == "TRUE" or data[1] == "true":
						self.optimize_coordinates = True

				elif l.startswith("OPTIMIZE_LAMBDA"):
					data = self.parse_line(l)
					if data[1] == "FALSE" or data[1] == "false":
						self.optimize_lambda = False
					elif data[1] == "TRUE" or data[1] == "true":
						self.optimize_lambda = True

				elif l.startswith("OPTIMIZE_MU"):
					data = self.parse_line(l)
					if data[1] == "FALSE" or data[1] == "false":
						self.optimize_mu = False
					elif data[1] == "TRUE" or data[1] == "true":
						self.optimize_mu = True

				elif l.startswith("OPTIMIZE_SIGMA"):
					data = self.parse_line(l)
					if data[1] == "FALSE" or data[1] == "false":
						self.optimize_sigma = False
					elif data[1] == "TRUE" or data[1] == "true":
						self.optimize_sigma = True

				elif l.startswith("OPTIMIZE_ROAD_LINEAR_COMBINATION"):
					data = self.parse_line(l)
					if data[1] == "FALSE" or data[1] == "false":
						self.optimize_road_linear_combination = False
					elif data[1] == "TRUE" or data[1] == "true":
						self.optimize_road_linear_combination = True

				elif l.startswith("OPTIMIZE_MINIMAL_CONNECTIVITY"):
					data = self.parse_line(l)
					if data[1] == "FALSE" or data[1] == "false":
						self.optimize_minimal_connectivity = False
					elif data[1] == "TRUE" or data[1] == "true":
						self.optimize_minimal_connectivity = True

				elif l.startswith("X_MIN"):
					data = self.parse_line(l)
					self.x_boundaries[0] = float(data[1])

				elif l.startswith("X_MAX"):
					data = self.parse_line(l)
					self.x_boundaries[1] = float(data[1])

				elif l.startswith("Y_MIN"):
					data = self.parse_line(l)
					self.y_boundaries[0] = float(data[1])

				elif l.startswith("Y_MAX"):
					data = self.parse_line(l)
					self.y_boundaries[1] = float(data[1])

				elif l.startswith("LAMBDA_MIN"):
					data = self.parse_line(l)
					self.jump_lambda[0] = float(data[1])

				elif l.startswith("LAMBDA_MAX"):
					data = self.parse_line(l)
					self.jump_lambda[1] = float(data[1])

				elif l.startswith("MU_MIN"):
					data = self.parse_line(l)
					self.jump_mu[0] = float(data[1])

				elif l.startswith("MU_MAX"):
					data = self.parse_line(l)
					self.jump_mu[1] = float(data[1])

				elif l.startswith("SIGMA_MIN"):
					data = self.parse_line(l)
					self.jump_sigma[0] = float(data[1])

				elif l.startswith("SIGMA_MAX"):
					data = self.parse_line(l)
					self.jump_sigma[1] = float(data[1])

				elif l.startswith("JUMP_LAW"):
					data = self.parse_line(l)
					self.jump_law = data[1]

				elif l.startswith("ROAD_LINEAR_COMBINATION_MIN"):
					data = self.parse_line(l)
					self.road_linear_combination[0][0] = float(data[1])
					self.road_linear_combination[1][0] = float(data[2])
					self.road_linear_combination[2][0] = float(data[3])
					self.road_linear_combination[3][0] = float(data[4])
					self.road_linear_combination[4][0] = float(data[5])
					self.road_linear_combination[5][0] = float(data[6])

				elif l.startswith("ROAD_LINEAR_COMBINATION_MAX"):
					data = self.parse_line(l)
					self.road_linear_combination[0][1] = float(data[1])
					self.road_linear_combination[1][1] = float(data[2])
					self.road_linear_combination[2][1] = float(data[3])
					self.road_linear_combination[3][1] = float(data[4])
					self.road_linear_combination[4][1] = float(data[5])
					self.road_linear_combination[5][1] = float(data[6])

				elif l.startswith("MINIMAL_CONNECTIVITY_MIN"):
					data = self.parse_line(l)
					self.minimal_connectivity[0] = float(data[1])

				elif l.startswith("MINIMAL_CONNECTIVITY_MAX"):
					data = self.parse_line(l)
					self.minimal_connectivity[1] = float(data[1])

				l = f.readline()
			f.close()
			#---------------------------------------------#
			# 2) Compute the optimization vector length   #
			#---------------------------------------------#
			self.optimization_vector_length = 0
			if self.optimize_coordinates:
				self.optimization_vector_length += 2
			if self.optimize_lambda:
				self.optimization_vector_length += 1
			if self.optimize_mu:
				self.optimization_vector_length += 1
			if self.optimize_sigma:
				self.optimization_vector_length += 1
			if self.optimize_road_linear_combination:
				self.optimization_vector_length += 6
			if self.optimize_minimal_connectivity:
				self.optimization_vector_length += 1
	
	#### Parse a line and get the list of words ###
	def parse_line( self, line ):
		l = line.strip("\n")
		l = l.split(" ")
		data = []
		for i in range(len(l)):
			if l[i] != "":
				data.append(l[i].strip(" "))
		return data

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
# Define class SimulationManager #
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
class SimulationManager:

	### Constructor ###
	def __init__( self ):
		self.params = Parameters()
		self.current_coord                   = [0.0, 0.0]
		self.current_lambda                  = 0.0
		self.current_mu                      = 0.0
		self.current_sigma                   = 0.0
		self.current_road_linear_combination = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
		self.current_minimal_connectivity    = 0.0
		self.current_score                   = 0.0
		self.command_line_options            = ""
		self.output_file                     = open("output.tmp", "w", 0)

	### Load parameters from file ###
	def load_parameters_from_file( self, parameters_filename, executable_path ):
		self.params.load_parameters_from_file(parameters_filename, executable_path)

	### Draw new parameter values from parameters range ###
	def draw_new_parameter_values( self ):
		self.output_file.close()
		self.output_file                        = open("output.tmp", "w", 0)
		self.current_coord[0]                   = np.random.random()*(self.params.x_boundaries[1]-self.params.x_boundaries[0]) + self.params.x_boundaries[0]
		self.current_coord[1]                   = np.random.random()*(self.params.y_boundaries[1]-self.params.y_boundaries[0]) + self.params.y_boundaries[0]
		self.current_lambda                     = np.random.random()*(self.params.jump_lambda[1]-self.params.jump_lambda[0]) + self.params.jump_lambda[0]
		self.current_mu                         = np.random.random()*(self.params.jump_mu[1]-self.params.jump_mu[0]) + self.params.jump_mu[0]
		self.current_sigma                      = np.random.random()*(self.params.jump_sigma[1]-self.params.jump_sigma[0]) + self.params.jump_sigma[0]
		self.current_road_linear_combination[0] = np.random.random()*(self.params.road_linear_combination[0][1]-self.params.road_linear_combination[0][0]) + self.params.road_linear_combination[0][0]
		self.current_road_linear_combination[1] = np.random.random()*(self.params.road_linear_combination[1][1]-self.params.road_linear_combination[1][0]) + self.params.road_linear_combination[1][0]
		self.current_road_linear_combination[2] = np.random.random()*(self.params.road_linear_combination[2][1]-self.params.road_linear_combination[2][0]) + self.params.road_linear_combination[2][0]
		self.current_road_linear_combination[3] = np.random.random()*(self.params.road_linear_combination[3][1]-self.params.road_linear_combination[3][0]) + self.params.road_linear_combination[3][0]
		self.current_road_linear_combination[4] = np.random.random()*(self.params.road_linear_combination[4][1]-self.params.road_linear_combination[4][0]) + self.params.road_linear_combination[4][0]
		self.current_road_linear_combination[5] = np.random.random()*(self.params.road_linear_combination[5][1]-self.params.road_linear_combination[5][0]) + self.params.road_linear_combination[5][0]
		self.current_minimal_connectivity       = np.random.random()*(self.params.minimal_connectivity[1]-self.params.minimal_connectivity[0]) + self.params.minimal_connectivity[0]

	### Write command line options ###
	def write_command_line_options( self ):
		self.command_line_options  = ""
		self.command_line_options += "--seed "+str(np.random.randint(1,1000000))+" "
		self.command_line_options += "--type-of-data "+str(self.params.type_of_data)+" "
		self.command_line_options += "--map "+str(self.params.map_filename)+" "
		self.command_line_options += "--network "+str(self.params.network_filename)+" "
		self.command_line_options += "--sample "+str(self.params.sample_filename)+" "
		self.command_line_options += "--rep "+str(self.params.repetitions)+" "
		self.command_line_options += "--iterations "+str(self.params.iterations)+" "
		self.command_line_options += "--xcoord "+str(self.current_coord[0])+" "
		self.command_line_options += "--ycoord "+str(self.current_coord[1])+" "
		self.command_line_options += "--lambda "+str(self.current_lambda)+" "
		self.command_line_options += "--mu "+str(self.current_mu)+" "
		self.command_line_options += "--sigma "+str(self.current_sigma)+" "
		self.command_line_options += "--law "+str(self.params.jump_law)+" "
		self.command_line_options += "--wroad1 "+str(self.current_road_linear_combination[0])+" "
		self.command_line_options += "--wroad2 "+str(self.current_road_linear_combination[1])+" "
		self.command_line_options += "--wroad3 "+str(self.current_road_linear_combination[2])+" "
		self.command_line_options += "--wroad4 "+str(self.current_road_linear_combination[3])+" "
		self.command_line_options += "--wroad5 "+str(self.current_road_linear_combination[4])+" "
		self.command_line_options += "--wroad6 "+str(self.current_road_linear_combination[5])+" "
		self.command_line_options += "--min-connectivity "+str(self.current_minimal_connectivity)

	### Read the standard output ###
	def read_params_out( self, output ):
		output             = output.strip("\n")
		self.current_score = float(output)

	### Get initial parameters list ###
	def get_parameters_list( self ):
		norm_x                = 0.0
		norm_y                = 0.0
		norm_lambda           = 0.0
		norm_mu               = 0.0
		norm_sigma            = 0.0
		norm_roads1           = 0.0
		norm_roads2           = 0.0
		norm_roads3           = 0.0
		norm_roads4           = 0.0
		norm_roads5           = 0.0
		norm_roads6           = 0.0
		norm_min_connectivity = 0.0

		optimization_vector = []
		#################################
		if self.params.optimize_coordinates:
			if self.params.x_boundaries[1] != self.params.x_boundaries[0]:
				norm_x = (self.current_coord[0]-self.params.x_boundaries[0])/(self.params.x_boundaries[1]-self.params.x_boundaries[0])
			else:
				norm_x = self.params.x_boundaries[0]
			if self.params.y_boundaries[1] != self.params.y_boundaries[0]:
				norm_y = (self.current_coord[1]-self.params.y_boundaries[0])/(self.params.y_boundaries[1]-self.params.y_boundaries[0])
			else:
				norm_y = self.params.y_boundaries[0]
			optimization_vector.append(norm_x)
			optimization_vector.append(norm_y)
		#################################
		if self.params.optimize_lambda:
			if self.params.jump_lambda[1] != self.params.jump_lambda[0]:
				norm_lambda = (self.current_lambda-self.params.jump_lambda[0])/(self.params.jump_lambda[1]-self.params.jump_lambda[0])
			else:
				norm_lambda = self.params.jump_lambda[0]
			optimization_vector.append(norm_lambda)
		#################################
		if self.params.optimize_mu:
			if self.params.jump_mu[1] != self.params.jump_mu[0]:
				norm_mu = (self.current_mu-self.params.jump_mu[0])/(self.params.jump_mu[1]-self.params.jump_mu[0])
			else:
				norm_mu = self.params.jump_mu[0]
			optimization_vector.append(norm_mu)
		#################################
		if self.params.optimize_sigma:
			if self.params.jump_sigma[1] != self.params.jump_sigma[0]:
				norm_sigma = (self.current_sigma-self.params.jump_sigma[0])/(self.params.jump_sigma[1]-self.params.jump_sigma[0])
			else:
				norm_sigma = self.params.jump_sigma[0]
			optimization_vector.append(norm_sigma)
		#################################
		if self.params.optimize_road_linear_combination:
			if self.params.road_linear_combination[0][1] != self.params.road_linear_combination[0][0]:
				norm_roads1 = (self.current_road_linear_combination[0]-self.params.road_linear_combination[0][0])/(self.params.road_linear_combination[0][1]-self.params.road_linear_combination[0][0])
			else:
				norm_roads1 = self.params.road_linear_combination[0][0]
			if self.params.road_linear_combination[1][1] != self.params.road_linear_combination[1][0]:
				norm_roads2 = (self.current_road_linear_combination[1]-self.params.road_linear_combination[1][0])/(self.params.road_linear_combination[1][1]-self.params.road_linear_combination[1][0])
			else:
				norm_roads2 = self.params.road_linear_combination[1][0]
			if self.params.road_linear_combination[2][1] != self.params.road_linear_combination[2][0]:
				norm_roads3 = (self.current_road_linear_combination[2]-self.params.road_linear_combination[2][0])/(self.params.road_linear_combination[2][1]-self.params.road_linear_combination[2][0])
			else:
				norm_roads3 = self.params.road_linear_combination[2][0]
			if self.params.road_linear_combination[3][1] != self.params.road_linear_combination[3][0]:
				norm_roads4 = (self.current_road_linear_combination[3]-self.params.road_linear_combination[3][0])/(self.params.road_linear_combination[3][1]-self.params.road_linear_combination[3][0])
			else:
				norm_roads4 = self.params.road_linear_combination[3][0]
			if self.params.road_linear_combination[4][1] != self.params.road_linear_combination[4][0]:
				norm_roads5 = (self.current_road_linear_combination[4]-self.params.road_linear_combination[4][0])/(self.params.road_linear_combination[4][1]-self.params.road_linear_combination[4][0])
			else:
				norm_roads5 = self.params.road_linear_combination[4][0]
			if self.params.road_linear_combination[5][1] != self.params.road_linear_combination[5][0]:
				norm_roads6 = (self.current_road_linear_combination[5]-self.params.road_linear_combination[5][0])/(self.params.road_linear_combination[5][1]-self.params.road_linear_combination[5][0])
			else:
				norm_roads6 = self.params.road_linear_combination[5][0]
			optimization_vector.append(norm_roads1)
			optimization_vector.append(norm_roads2)
			optimization_vector.append(norm_roads3)
			optimization_vector.append(norm_roads4)
			optimization_vector.append(norm_roads5)
			optimization_vector.append(norm_roads6)
		#################################
		if self.params.optimize_minimal_connectivity:
			if self.params.minimal_connectivity[1] != self.params.minimal_connectivity[0]:
				norm_min_connectivity = (self.current_minimal_connectivity-self.params.minimal_connectivity[0])/(self.params.minimal_connectivity[1]-self.params.minimal_connectivity[0])
			else:
				norm_min_connectivity = self.params.minimal_connectivity[0]
			optimization_vector.append(norm_min_connectivity)
		#################################
		return optimization_vector

	### Function used by the CMA-ES minimizer ###
	def minimization_function( self, params_list ):
		#----------------------------------------------------------#
		# 1) transform normalized parameters                       #
		#----------------------------------------------------------#
		norm_x                = 0.0
		norm_y                = 0.0
		norm_lambda           = 0.0
		norm_mu               = 0.0
		norm_sigma            = 0.0
		norm_roads1           = 0.0
		norm_roads2           = 0.0
		norm_roads3           = 0.0
		norm_roads4           = 0.0
		norm_roads5           = 0.0
		norm_roads6           = 0.0
		norm_min_connectivity = 0.0
		counter               = 0

		### A) if coordinates are optimized ###
		if self.params.optimize_coordinates:
			norm_x = abs(params_list[counter])
			counter += 1
			norm_y = abs(params_list[counter])
			counter += 1
			self.current_coord[0] = norm_x*(self.params.x_boundaries[1]-self.params.x_boundaries[0])+self.params.x_boundaries[0]
			self.current_coord[1] = norm_y*(self.params.y_boundaries[1]-self.params.y_boundaries[0])+self.params.y_boundaries[0]
		
		### B) if lambda is optimized ###
		if self.params.optimize_lambda:
			norm_lambda = abs(params_list[counter])
			counter += 1
			self.current_lambda = norm_lambda*(self.params.jump_lambda[1]-self.params.jump_lambda[0])+self.params.jump_lambda[0]
		
		### C) if mu is optimized ###
		if self.params.optimize_mu:
			norm_mu = abs(params_list[counter])
			counter += 1
			self.current_mu = norm_mu*(self.params.jump_mu[1]-self.params.jump_mu[0])+self.params.jump_mu[0]
		
		### D) if sigma is optimized ###
		if self.params.optimize_sigma:
			norm_sigma = abs(params_list[counter])
			counter += 1
			self.current_sigma = norm_sigma*(self.params.jump_sigma[1]-self.params.jump_sigma[0])+self.params.jump_sigma[0]
		
		### E) if road linear combination is optimized ###
		if self.params.optimize_road_linear_combination:
			norm_roads1 = abs(params_list[counter])
			counter += 1
			norm_roads2 = abs(params_list[counter])
			counter += 1
			norm_roads3 = abs(params_list[counter])
			counter += 1
			norm_roads4 = abs(params_list[counter])
			counter += 1
			norm_roads5 = abs(params_list[counter])
			counter += 1
			norm_roads6 = abs(params_list[counter])
			counter += 1
			self.current_road_linear_combination[0] = norm_roads1*(self.params.road_linear_combination[0][1]-self.params.road_linear_combination[0][0])+self.params.road_linear_combination[0][0]
			self.current_road_linear_combination[1] = norm_roads2*(self.params.road_linear_combination[1][1]-self.params.road_linear_combination[1][0])+self.params.road_linear_combination[1][0]
			self.current_road_linear_combination[2] = norm_roads3*(self.params.road_linear_combination[2][1]-self.params.road_linear_combination[2][0])+self.params.road_linear_combination[2][0]
			self.current_road_linear_combination[3] = norm_roads4*(self.params.road_linear_combination[3][1]-self.params.road_linear_combination[3][0])+self.params.road_linear_combination[3][0]
			self.current_road_linear_combination[4] = norm_roads5*(self.params.road_linear_combination[4][1]-self.params.road_linear_combination[4][0])+self.params.road_linear_combination[4][0]
			self.current_road_linear_combination[5] = norm_roads6*(self.params.road_linear_combination[5][1]-self.params.road_linear_combination[5][0])+self.params.road_linear_combination[5][0]
		
		### F) if minimal connectivity is optimized ###
		if self.params.optimize_minimal_connectivity:
			norm_min_connectivity = abs(params_list[counter])
			counter += 1
			self.current_minimal_connectivity = norm_min_connectivity*(self.params.minimal_connectivity[1]-self.params.minimal_connectivity[0])+self.params.minimal_connectivity[0]

		#----------------------------------------------------------#
		# 2) exit with penalty if parameters are out of boundaries #
		#----------------------------------------------------------#
		if self.current_coord[0] < self.params.x_boundaries[0] or self.current_coord[0] > self.params.x_boundaries[1]:
			return 1000.0
		if self.current_coord[1] < self.params.y_boundaries[0] or self.current_coord[1] > self.params.y_boundaries[1]:
			return 1000.0
		if self.current_lambda < self.params.jump_lambda[0] or self.current_lambda > self.params.jump_lambda[1]:
			return 1000.0
		if self.current_mu < self.params.jump_mu[0] or self.current_mu > self.params.jump_mu[1]:
			return 1000.0
		if self.current_sigma < self.params.jump_sigma[0] or self.current_sigma > self.params.jump_sigma[1]:
			return 1000.0
		if self.current_road_linear_combination[0] < self.params.road_linear_combination[0][0] or self.current_road_linear_combination[0] > self.params.road_linear_combination[0][1]:
			return 1000.0
		if self.current_road_linear_combination[1] < self.params.road_linear_combination[1][0] or self.current_road_linear_combination[1] > self.params.road_linear_combination[1][1]:
			return 1000.0
		if self.current_road_linear_combination[2] < self.params.road_linear_combination[2][0] or self.current_road_linear_combination[2] > self.params.road_linear_combination[2][1]:
			return 1000.0
		if self.current_road_linear_combination[3] < self.params.road_linear_combination[3][0] or self.current_road_linear_combination[3] > self.params.road_linear_combination[3][1]:
			return 1000.0
		if self.current_road_linear_combination[4] < self.params.road_linear_combination[4][0] or self.current_road_linear_combination[4] > self.params.road_linear_combination[4][1]:
			return 1000.0
		if self.current_road_linear_combination[5] < self.params.road_linear_combination[5][0] or self.current_road_linear_combination[5] > self.params.road_linear_combination[5][1]:
			return 1000.0
		if self.current_minimal_connectivity < self.params.minimal_connectivity[0] or self.current_minimal_connectivity > self.params.minimal_connectivity[1]:
			return 1000.0

		#----------------------------------------------------------#
		# 3) create the command line options                       #
		#----------------------------------------------------------#
		self.write_command_line_options()

		#----------------------------------------------------------#
		# 4) run simulation                                        #
		#----------------------------------------------------------#
		exec_line       = self.params.executable_path+" "+self.command_line_options
		process         = subprocess.Popen([exec_line], stdout=subprocess.PIPE, shell=True)
		standard_output = process.stdout.read()

		#----------------------------------------------------------#
		# 5) read params.out and return score                      #
		#----------------------------------------------------------#
		self.read_params_out(standard_output)
		self.output_file.write(
			str(self.current_coord[0])+" "+
			str(self.current_coord[1])+" "+
			str(self.current_lambda)+" "+
			str(self.current_mu)+" "+
			str(self.current_sigma)+" "+
			str(self.current_road_linear_combination[0])+" "+
			str(self.current_road_linear_combination[1])+" "+
			str(self.current_road_linear_combination[2])+" "+
			str(self.current_road_linear_combination[3])+" "+
			str(self.current_road_linear_combination[4])+" "+
			str(self.current_road_linear_combination[5])+" "+
			str(self.current_minimal_connectivity)+" "+
			str(self.current_score)+"\n")
		os.system("Rscript plot.R > /dev/null &")
		return self.current_score

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
# Read command line arguments    #
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
def readArgs( args ):
	parameters_filename = ""
	run_path            = ""
	repetitions         = 0
	for i in range(len(args)):
		if args[i] == "-h" or args[i] == "--help":
			printUsage()
			sys.exit()
		if args[i] == "-p" or args[i] == "--params":
			parameters_filename = args[i+1]
		if args[i] == "-run" or args[i] == "--run-path":
			run_path = args[i+1]
		if args[i] == "-rep" or args[i] == "--repetitions":
			repetitions = int(args[i+1])
	return parameters_filename, run_path, repetitions

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
# Print usage                    #
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
def printUsage():
	print ""
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	print "                           * MoRIS minimizer *                        "
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
	print "Usage: python MoRIS_minimizer.py -h or --help"
	print "   or: python MoRIS_minimizer.py [options]"
	print "Options are:"
	print "  -h, --help"
	print "      print this help, then exit"
	print "  -p, --params <params_file>"
	print "      specify the parameters file (default: parameters.txt)"
	print "  -run, --run-path <run_path>"
	print "      specify the run executable path (default: ../src)"
	print "  -rep, --repetitions <repetitions>"
	print "      specify the number of repetitions (default: 100)"
	print ""

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
# Print header                   #
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
def printHeader():
	print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	print "                           * MoRIS minimizer *                        "
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


##############
#    MAIN    #
##############

if __name__ == '__main__':

	#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
	# 1) Read command line arguments #
	#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
	parameters_filename, run_path, repetitions = readArgs(sys.argv)
	if parameters_filename == "":
		parameters_filename = DEFAULT_PARAMETERS_FILE
	if run_path == "":
		run_path = DEFAULT_RUN_PATH
	if repetitions == 0:
		repetitions = DEFAULT_REPETITIONS

	#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
	# 2) Load simulation manager     #
	#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
	simManager = SimulationManager()
	simManager.load_parameters_from_file(parameters_filename, run_path)
	np.random.seed(simManager.params.seed)

	#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
	# 3) Start the loop              #
	#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
	f = open("output.res", "w", 0)
	rep = 0
	while rep < repetitions:

		print ">> ------------------------------------------------------"
		print ">> Running minimization "+str(rep)+" on "+str(repetitions)
		print ">> ------------------------------------------------------"

		start = time.time()

		simManager.draw_new_parameter_values()
		paramsList = simManager.get_parameters_list()
		
		res = cma.fmin(simManager.minimization_function, paramsList, 1)
		
		end = time.time()
		if end-start < TIME_THRESHOLD:
			print ">> Minimization Failure, restart"
		else:
			#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
			# 3.A) Compute optimal values #
			#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
			opt_x       = simManager.current_coord[0]
			opt_y       = simManager.current_coord[1]
			opt_lambda  = simManager.current_lambda
			opt_mu      = simManager.current_mu
			opt_sigma   = simManager.current_sigma
			opt_roads1  = simManager.current_road_linear_combination[0]
			opt_roads2  = simManager.current_road_linear_combination[1]
			opt_roads3  = simManager.current_road_linear_combination[2]
			opt_roads4  = simManager.current_road_linear_combination[3]
			opt_roads5  = simManager.current_road_linear_combination[4]
			opt_roads6  = simManager.current_road_linear_combination[5]
			opt_connect = simManager.current_minimal_connectivity
			counter     = 0
			#################################
			if simManager.params.optimize_coordinates:
				opt_x = abs(res[0][counter])
				counter +=1
				opt_y = abs(res[0][counter])
				counter += 1
				opt_x = opt_x*(simManager.params.x_boundaries[1]-simManager.params.x_boundaries[0])+simManager.params.x_boundaries[0]
				opt_y = opt_y*(simManager.params.y_boundaries[1]-simManager.params.y_boundaries[0])+simManager.params.y_boundaries[0]
			#################################
			if simManager.params.optimize_lambda:
				opt_lambda = abs(res[0][counter])
				counter += 1
				opt_lambda = opt_lambda*(simManager.params.jump_lambda[1]-simManager.params.jump_lambda[0])+simManager.params.jump_lambda[0]
			#################################
			if simManager.params.optimize_mu:
				opt_mu = abs(res[0][counter])
				counter += 1
				opt_mu = opt_mu*(simManager.params.jump_mu[1]-simManager.params.jump_mu[0])+simManager.params.jump_mu[0]
			#################################
			if simManager.params.optimize_sigma:
				opt_sigma = abs(res[0][counter])
				counter += 1
				opt_sigma = opt_sigma*(simManager.params.jump_sigma[1]-simManager.params.jump_sigma[0])+simManager.params.jump_sigma[0]
			#################################
			if simManager.params.optimize_road_linear_combination:
				opt_roads1 = abs(res[0][counter])
				counter += 1
				opt_roads2 = abs(res[0][counter])
				counter += 1
				opt_roads3 = abs(res[0][counter])
				counter += 1
				opt_roads4 = abs(res[0][counter])
				counter += 1
				opt_roads5 = abs(res[0][counter])
				counter += 1
				opt_roads6 = abs(res[0][counter])
				counter += 1
				opt_roads1 = opt_roads1*(simManager.params.road_linear_combination[0][1]-simManager.params.road_linear_combination[0][0])+simManager.params.road_linear_combination[0][0]
				opt_roads2 = opt_roads2*(simManager.params.road_linear_combination[1][1]-simManager.params.road_linear_combination[1][0])+simManager.params.road_linear_combination[1][0]
				opt_roads3 = opt_roads3*(simManager.params.road_linear_combination[2][1]-simManager.params.road_linear_combination[2][0])+simManager.params.road_linear_combination[2][0]
				opt_roads4 = opt_roads4*(simManager.params.road_linear_combination[3][1]-simManager.params.road_linear_combination[3][0])+simManager.params.road_linear_combination[3][0]
				opt_roads5 = opt_roads5*(simManager.params.road_linear_combination[4][1]-simManager.params.road_linear_combination[4][0])+simManager.params.road_linear_combination[4][0]
				opt_roads6 = opt_roads6*(simManager.params.road_linear_combination[5][1]-simManager.params.road_linear_combination[5][0])+simManager.params.road_linear_combination[5][0]
			#################################
			if simManager.params.optimize_minimal_connectivity:
				opt_connect = abs(res[0][counter])
				counter += 1
				opt_connect = opt_connect*(simManager.params.minimal_connectivity[1]-simManager.params.minimal_connectivity[0])+simManager.params.minimal_connectivity[0]
			
			#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
			# 3.B) Get optimized score    #
			#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
			opt_score = simManager.current_score

			#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
			# 3.C) Save values in output  #
			#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
			f.write(
				str(opt_x)+" "+
				str(opt_y)+" "+
				str(opt_lambda)+" "+
				str(opt_mu)+" "+
				str(opt_sigma)+" "+
				str(opt_roads1)+" "+
				str(opt_roads2)+" "+
				str(opt_roads3)+" "+
				str(opt_roads4)+" "+
				str(opt_roads5)+" "+
				str(opt_roads6)+" "+
				str(opt_connect)+" "+
				str(opt_score)+"\n")
				
			#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
			# 3.D) Increment repetition   #
			#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
			rep += 1
	f.close()


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

