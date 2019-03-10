options(show.error.messages=FALSE, warn=-1)

suppressMessages(library("RColorBrewer"))
suppressMessages(library("caTools"))
suppressMessages(library("maptools"))

setwd("/Users/charlesrocabert/git/MoRIS-development/lasius_neglectus_example")

d = read.table("optimization.txt", sep=" ", h=T)
urban = readShapePoly("input/urban2.shp")

N = length(d$x)
L = 200
if (N < L)
{
	L = N-1
}

png(filename="output.png", width=800, height=800)

par(mfrow=c(3,3), mar=c(2,2,2,2))

plot(d$score, pch=20, main="Score", col="tomato", log="y")
lines(runmean(d$score, k=100), col="blue", lwd=2)
lines(d$empty_score)
abline(h=0)

maxL   = abs(d$max_L[N])
emptyL = abs(d$empty_L[N])
L      = abs(d$L[N])
barplot(c(maxL/maxL, emptyL/maxL, L/maxL), ylim=range(c(maxL/maxL, emptyL/maxL, L/maxL)), col=c("tomato", "lightgrey", "darkolivegreen3"), names.arg=c("Max.L","Empty.L","L"))

#plot(d$x, pch=20, main="X")
#lines(runmean(d$x, k=100), col="green", lwd=2)

#plot(d$y, pch=20, main="Y")
#lines(runmean(d$y, k=100), col="green", lwd=2)

plot(urban, col="black", border=NA)
colors = colorRampPalette(c("blue", "green", "orange", "red"))(n=length(d$x))
colors = adjustcolor(colors, alpha.f=0.5)
points(d$xintro, d$yintro, pch=20, col=colors, main="(X, Y)")
rect(20000, 14000, 88000, 104000, lty=2)

plot(d$pintro, pch=20, main="P intro")
lines(runmean(d$pintro, k=100), col="blue", lwd=2)

plot(d$lambda, pch=20, main="Lambda")
lines(runmean(d$lambda, k=100), col="blue", lwd=2)

plot(d$mu, pch=20, main="Mu")
lines(runmean(d$mu, k=100), col="blue", lwd=2)

plot(d$sigma, pch=20, main="Sigma")
lines(runmean(d$sigma, k=100), col="blue", lwd=2)

#plot(d$gamma, pch=20, main="Gamma")
#lines(runmean(d$gamma, k=100), col="blue", lwd=2)

W = 1#d$w1+d$w2+d$w3+d$w4
plot(x=NULL, xlim=c(0,length(d$x)), ylim=c(0,1))
points(d$w1/W, col="blue")
lines(d$w1/W, col="blue", lty=2)
points(d$w2/W, col="red")
lines(d$w2/W, col="red", lty=2)
points(d$w3/W, col="green")
lines(d$w3/W, col="green", lty=2)
points(d$w4/W, col="purple")
lines(d$w4/W, col="purple", lty=2)
legend("topleft", legend=c("w1","w2","w3","w4"), col=c("blue", "red", "green", "purple"), lwd=c(2,2,2,2))

#plot(d$min_w, pch=20, main="Min connectivity")
#lines(runmean(d$min_w, k=100), col="green", lwd=2)

dev.off()
