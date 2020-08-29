
setwd("/Users/charlesrocabert/git/MoRIS-development/example_lasius_neglectus_squares")

d = read.table("best_output/lineage_tree.txt", sep=" ", h=T)

par(mfrow=c(2,2))

hist(d$geodesic_distance, col="bisque")
abline(v=mean(d$geodesic_distance))
mean(d$euclidean_dist)

hist(d$euclidean_dist, col="bisque")
abline(v=mean(d$euclidean_dist))
mean(d$euclidean_dist)

mu = 1.914672
sigma = 0.5409641
X = rlnorm(1000, log(mu), sigma)

hist(X, col="bisque")

d1 = density(d$geodesic_distance)
d2 = density(d$euclidean_dist)
d3 = density(X)


plot(d1$x, d1$y/max(d1$y), col="red", type="l")
lines(d2$x/1000, d2$y/max(d2$y), col="green")
lines(d3$x, d3$y/max(d3$y), col="blue")

mean(d$euclidean_dist)