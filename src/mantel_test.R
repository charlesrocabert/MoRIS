options(show.error.messages=FALSE, warn=-1)
args <- commandArgs(trailingOnly = TRUE)
WORKDIR    = args[1]
BREAKPOINT = as.numeric(args[2])

# WORKDIR = "/Users/charlesrocabert/git/MoRIS-development/RESULTS_2019/post_treatment/best_scenario/mantel_test_tmp"
# BREAKPOINT = 5000.0

setwd(WORKDIR)

library(ade4)
library(vegan)

euclidean = read.table("euclidean.txt", h=F, sep=" ")
genetic   = read.table("genetic.txt", h=F, sep=" ")

euclidean = as.dist(euclidean)
genetic   = as.dist(genetic)

maxeuclidean = max(euclidean)
nclass       = round(maxeuclidean/BREAKPOINT)
breakpoints  = seq(0, nclass)
breakpoints  = breakpoints*BREAKPOINT

test = mantel.correlog(genetic, euclidean, nperm=999, break.pts=breakpoints)
write.table(file="mantel_test.txt", test$mantel.res, quote=F)

#Signif. codes:  0 ‘***’ 0.001 ‘**’ 0.01 ‘*’ 0.05 ‘.’ 0.1 ‘ ’ 1
