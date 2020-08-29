
setwd("/Users/charlesrocabert/git/MoRIS-development/lasius_neglectus_example")

eval = read.table("mean_output/evaluated_euclidean_distribution.txt", sep=" ", h=T)
obs = read.table("mean_output/observed_euclidean_distribution.txt", sep=" ", h=T)
sim = read.table("mean_output/simulated_euclidean_distribution.txt", sep=" ", h=T)


hist(obs$euclidean_dist, freq=F, col=adjustcolor("blue", alpha.f=0.2), ylim=c(0, 4e-5), main="Distance between colonies", xlab="Euclidean distance", ylab="Density")
#hist(obs$euclidean_dist, add=T, freq=F, col=adjustcolor("blue", alpha.f=0.3))
hist(sim$euclidean_dist, add=T, freq=F, col=adjustcolor("green", alpha.f=0.3))
abline(v=mean(eval$euclidean_dist), col="red", lwd=3, lty=2)
abline(v=mean(obs$euclidean_dist), col="blue", lwd=3, lty=2)
abline(v=mean(sim$euclidean_dist), col="green", lwd=3, lty=2)

lines(density(eval$euclidean_dist), col="red", ylim=c(0, 4e-5))
lines(density(obs$euclidean_dist), col="blue", lwd=3)
lines(density(sim$euclidean_dist), col="green", lwd=3)

legend("topright", legend=c("Evaluated", "Observed", "Simulated"), lwd=c(1,3,3), col=c("red", "blue", "green"))
