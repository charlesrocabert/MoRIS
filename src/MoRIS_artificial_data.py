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

import sys, os, andom

### Write artificial data command line options ###
def artificial_data_command_line_options( prng_seed, x_coord, y_coord, jump_mu, jump_lambda, sample_size ):
  cmdline = ""
  cmdline += "--seed "+str(prng_seed)+" "
  cmdline += "--network network.txt "
  cmdline += "--map map.txt "
  cmdline += "--sample sample.txt "
  cmdline += "--rep 10000 "
  cmdline += "--cycles 40 "
  cmdline += "--xcoord "+str(x_coord)+" "
  cmdline += "--ycoord "+str(y_coord)+" "
  cmdline += "--lambda "+str(jump_lambda)+" "
  cmdline += "--mu "+str(jump_mu)+" "
  cmdline += "--sigma 0 "
  cmdline += "--law DIRAC "
  cmdline += "--wroad1 1.0 "
  cmdline += "--wroad2 1.0 "
  cmdline += "--wroad3 1.0 "
  cmdline += "--wroad4 1.0 "
  cmdline += "--wroad5 0.0 "
  cmdline += "--wroad6 0.0 "
  cmdline += "--min-connectivity 0.001 "
  cmdline += "--sample-size "+str(sample_size)+" "
  cmdline += "--imitate-sample 0"
  return cmdline


##################
#      MAIN      #
##################

GENERATE = "/Users/charlesrocabert/svn/antsp/branches/development/basic-model/build/bin/MoRIS_artificial_data"

X_WINDOW       = [39837.17, 91798.69]
Y_WINDOW       = [39000.0, 107000.0]
LAMBDA_WINDOW  = [0.0, 3.0]
MU_WINDOW      = [0.0, 20.0]
EFFORT_RANGE   = [0.5*1138, 1*1138, 2*1138, 4*1138, 6*1138, 8*1138]

prng_seed   = random.randint(1, 1000000000)
x_coord     = random.random()*(X_WINDOW[1]-X_WINDOW[0])+X_WINDOW[0]
y_coord     = random.random()*(Y_WINDOW[1]-Y_WINDOW[0])+Y_WINDOW[0]
jump_lambda = random.random()*(LAMBDA_WINDOW[1]-LAMBDA_WINDOW[0])+LAMBDA_WINDOW[0]
jump_mu     = random.random()*(MU_WINDOW[1]-MU_WINDOW[0])+MU_WINDOW[0]
effort      = random.choice(EFFORT_RANGE)

### RUN ARTIFICIAL DATA GENERATION ###
artdatacmdline = artificial_data_command_line_options(prng_seed, x_coord, y_coord, jump_mu, jump_lambda, effort)
f = open("cmdline_save.txt", "w")
f.write(artdatacmdline)
f.close()
os.system(GENERATE+" "+artdatacmdline)
