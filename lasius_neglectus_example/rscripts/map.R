library("maptools")

setwd("/Users/charlesrocabert/git/MoRIS-development/example_lasius_neglectus_squares")

# id x y y_obs n_obs p_obs n_sim y_sim p_sim total_nb_intros mean_nb_intros var_nb_intros max_ml empty_ml ml empty_score score

### Plot invasion data ###
plot_map <- function( d, xintro, yintro )
{
	plot(urban, col="grey", border=NA, xlab="X", ylab="Y", main="Invasion map")
	STEP   = 0.01
	PVEC   = seq(0.0, max(d$p_sim), by=STEP)
	colors = colorRampPalette(c("white", "blue", "green", "orange", "red"))(n=length(PVEC))
	colors = adjustcolor(colors, alpha.f=0.5)
	for(i in seq(1, length(PVEC)))
	{
		p = PVEC[i]
		dobs = d[d$p_sim > p & d$p_sim <= p+STEP,]
		points(dobs$x, dobs$y, pch=15, cex=0.6, col=colors[i])
	}
	dth = d[d$n_obs > 0,]
	points(dth$x, dth$y, pch=20, col="black", cex=0.2)
	dth = d[d$y_obs>0,]
	points(dth$x, dth$y, pch=20, col="purple", cex=0.6)
	abline(v=xintro, lty=2)
	abline(h=yintro, lty=2)
	plot(x=NULL, xlim=c(0,length(d$x)), ylim=c(0,1), xaxt="n", yaxt="n", axes=F, xlab="", ylab="")
	legend("topleft", legend=c("Positive observation", "Sampling", "Introduction"), col=c("purple", "black", "black"), pch=c(20, 20, NA), lty=c(NA, NA, 2), cex=0.8)
}

d = read.table("mean_output/final_state.txt", h=T, sep=" ")
p = read.table("mean_output/parameters.txt", h=T, sep=" ")
urban = readShapePoly("input/urban2.shp")

N = length(d[,1])

par(mfrow=c(2,2), mar=c(2,2,2,2))

plot_map(d, p$xintro, p$yintro)




factor = 1

dplus = d[d$y_obs>0,]
pos = sum(dplus[dplus$p_sim>0,1])
neg = sum(dplus[dplus$p_sim==0,1])

dminus = d[d$y_obs==0 & d$n_obs>0,]
pos = sum(dminus[dminus$p_sim==0,1])
neg = sum(dminus[dminus$p_sim>0,1])

dnz = d[d$n_obs>0,]
plot(dnz$p_obs, dnz$p_sim*factor, pch=20, main="Observed/presence probability", xlab="Observed", ylab="Simulated")
abline(a=0, b=1, lty=2)
abline(lm(dnz$p_sim*factor~dnz$p_obs), col="purple", lty=2)

plot(x=NULL, xlim=c(0,length(d$x)), ylim=c(0,1), xaxt="n", yaxt="n", axes=F, xlab="", ylab="")
	legend("topleft", legend=c("Perfect match (x=y)", "Observed vs. simulated probability"), col=c("black", "purple"), lty=c(2, 2), cex=0.6)
