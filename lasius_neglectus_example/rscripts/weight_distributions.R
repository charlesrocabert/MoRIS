library("caTools")

setwd("/Users/charlesrocabert/git/MoRIS-development/example_lasius_neglectus_squares")

d = read.table("input/map.txt", sep=" ", h=F)

# ID X Y AREA SUITABLE_AREA POP POP_DENSITY ROAD_DENSITY
# 1  2 3   4         5       6      7           8

pop = sort(d[,6])
popd = sort(d[,7])
road = sort(d[,8])

par(mfrow=c(3,4), mar=c(2,2,2,2))

### Pop ratio ###
plot(pop/max(pop), pch=20, main="Population")
hist((pop/max(pop)), col="lightgrey", main="Population")

### Pop log ratio ###
plot(log(pop)/max(log(pop)), pch=20, main="Log population")
hist(log(pop)/max(log(pop)), col="lightgrey", main="Log population")

### Pop density ratio ###
plot(popd/max(popd), pch=20, main="Population density")
hist((popd/max(popd)), col="lightgrey", main="Population density")

### Pop density log ratio ###
plot(log(popd)/max(log(popd)), pch=20, main="Log population density")
hist(log(popd)/max(log(popd)), col="lightgrey", main="Log population density")

### Raod density ratio ###
plot(road/max(road), pch=20, main="Road density")
hist((road/max(road)), col="lightgrey", main="Road density")

### Raod density log ratio ###
plot(log(road)/max(log(road)), pch=20, main="Log road density")
hist((log(road)/max(log(road))), col="lightgrey", main="Log road density")



