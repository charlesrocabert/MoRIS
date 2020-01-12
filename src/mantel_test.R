#!/usr/bin/env Rscript
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

#options(show.error.messages=FALSE, warn=-1)
args <- commandArgs(trailingOnly = TRUE)
WORKDIR    = args[1]
BREAKPOINT = as.numeric(args[2])

setwd(WORKDIR)

library(ade4)
library(ecodist)

euclidean     = read.table("euclidean.txt", h=F, sep=" ")
genetic_all   = read.table("genetic_all.txt", h=F, sep=" ")
genetic_first = read.table("genetic_first.txt", h=F, sep=" ")

euclidean     = as.dist(euclidean)
genetic_all   = as.dist(genetic_all)
genetic_first = as.dist(genetic_first)

maxeuclidean = max(euclidean)
nclass       = round(maxeuclidean/BREAKPOINT)
breakpoints  = seq(0, nclass)
breakpoints  = breakpoints*BREAKPOINT
breakpoints  = c(0.000, 5049.538, 9131.990, 12984.375, 17149.253, 19889.579, 24030.381, 29509.548, 36560.753, 43083.206, 70000.000)

test_all <- mgram(genetic_all, euclidean, nperm=999, breaks=breakpoints)
write.table(file="mantel_test_all.txt", test_all$mgram, quote=F)

test_first <- mgram(genetic_first, euclidean, nperm=999, breaks=breakpoints)
write.table(file="mantel_test_first.txt", test_first$mgram, quote=F)

#Signif. codes:  0 ‘***’ 0.001 ‘**’ 0.01 ‘*’ 0.05 ‘.’ 0.1 ‘ ’ 1
