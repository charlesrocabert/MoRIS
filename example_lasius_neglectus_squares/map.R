setwd("/Users/charlesrocabert/git/MoRIS-development/example_lasius_neglectus_squares")

# id xcoord ycoord y n f_obs f_sim

### Plot invasion data ###
plot_map <- function( d, xintro, yintro )
{
	RGX = range(d$xcoord)
	RGY = range(d$ycoord)
	plot(x=NULL, xlim=RGX, ylim=RGY, xlab="X", ylab="Y", main="Invasion map")
	STEP   = 0.01
	FVEC   = seq(0.0, 0.3-STEP, by=STEP)
	colors = colorRampPalette(c("white", "blue"))(n=length(FVEC))
	#colors = adjustcolor(colors, alpha.f=0.7)
	for(i in seq(1, length(FVEC)))
	{
		f = FVEC[i]
		dobs = d[d$f_sim > f & d$f_sim <= f+STEP,]
		points(dobs$xcoord, dobs$ycoord, pch=15, cex=0.6, col=colors[i])
	}
	dth = d[d$n > 0,]
	points(dth$xcoord, dth$ycoord, pch=20, col="black", cex=0.2)
	dth = d[d$y>0,]
	points(dth$xcoord, dth$ycoord, pch=20, col="green", cex=0.6)
	abline(v=xintro, lty=2)
	abline(h=yintro, lty=2)
	#plot(x=NULL, xlim=c(0,length(d$x)), ylim=c(0,1), xaxt="n", yaxt="n", axes=F, xlab="", ylab="")
	#legend("topleft", legend=c("0.01 < f <= 0.1", "0.1 < f <= 0.2", "0.2 < f <= 0.3", "0.3 < f <= 0.4", "0.4 < f <= 0.5", "0.5 < f <= 0.6", "0.6 < f <= 0.7", "0.7 < f <= 0.8", "0.8 < f <= 0.9", "0.9 < f <= 1.0", "Positive observation", "Sampling", "Introduction"), col=c(colors, "green", "black", "black"), pch=c(rep(15, 11), 20, NA), lty=c(rep(NA, 12), 2), cex=0.8)
}

d = read.table("output/final_state.txt", h=T, sep=" ")
p = read.table("output/parameters.txt", h=T, sep=" ")

N = length(d[,1])


plot_map(d, p$x_introduction, p$y_introduction)



# # 

factor = 362/56
factor = 1

par(mfrow=c(1,2))
dplus = d[d$y>0,]
pos = sum(dplus[dplus$f_sim>0,1])
neg = sum(dplus[dplus$f_sim==0,1])

dminus = d[d$y==0 & d$n>0,]
pos = sum(dminus[dminus$f_sim==0,1])
neg = sum(dminus[dminus$f_sim>0,1])

dnz = d[d$n>0,]
plot(dnz$f_obs, dnz$f_sim*factor, pch=20, main="Observed/presence probability", xlab="Observed", ylab="Simulated")
abline(a=0, b=1, lty=2)
abline(lm(dnz$f_sim*factor~dnz$f_obs), col="purple", lty=2)

plot(x=NULL, xlim=c(0,length(d$x)), ylim=c(0,1), xaxt="n", yaxt="n", axes=F, xlab="", ylab="")
	legend("topleft", legend=c("Perfect match (x=y)", "Observed vs. simulated probability"), col=c("black", "purple"), lty=c(2, 2), cex=0.6)
