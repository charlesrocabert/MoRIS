library("caTools")

setwd("/Users/charlesrocabert/git/MoRIS-development/example_lasius_neglectus_squares")

d = read.table("optim.txt", sep=" ", h=T)

N = length(d$x)
L = 200
if (N < L)
{
	L = N-1
}

png(filename="output.png", width=800, height=800)

par(mfrow=c(3,3), mar=c(2,2,2,2))

plot(d$score, pch=20, main="Score", col="tomato")
lines(runmean(d$score, k=100), col="blue", lwd=2)
lines(d$empty_score)

plot(d$x, pch=20, main="X")
lines(runmean(d$x, k=100), col="green", lwd=2)

plot(d$y, pch=20, main="Y")
lines(runmean(d$y, k=100), col="green", lwd=2)

plot(d$p_intro, pch=20, main="P intro")
lines(runmean(d$p_intro, k=100), col="green", lwd=2)

plot(d$lambda, pch=20, main="Lambda")
lines(runmean(d$lambda, k=100), col="green", lwd=2)

plot(d$mu, pch=20, main="Mu")
lines(runmean(d$mu, k=100), col="green", lwd=2)

plot(d$sigma, pch=20, main="Sigma")
lines(runmean(d$sigma, k=100), col="green", lwd=2)

plot(x=NULL, xlim=c(0,length(d$x)), ylim=c(0,1))
points(d$w1, col="blue")
lines(d$w1, col="blue", lty=2)
points(d$w2, col="red")
lines(d$w2, col="red", lty=2)
points(d$w3, col="green")
lines(d$w3, col="green", lty=2)
points(d$w4, col="purple")
lines(d$w4, col="purple", lty=2)
legend("topleft", legend=c("w1","w2","w3","w4"), col=c("blue", "red", "green", "purple"), lwd=c(2,2,2,2))

plot(d$min_w, pch=20, main="Min connectivity")
lines(runmean(d$min_w, k=100), col="green", lwd=2)

dev.off()
