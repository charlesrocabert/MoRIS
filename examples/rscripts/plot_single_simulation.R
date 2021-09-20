options(show.error.messages=FALSE, warn=-1)

suppressMessages(library("ggplot2"))
suppressMessages(library("cowplot"))
suppressMessages(library("sf"))

### Plot the scenario invasion pattern ###
plot_invasion_map <- function( filename, title, xintro, yintro )
{
  #--------------------------#
  # 1) Load and prepare data #
  #--------------------------#
  urban          = st_read("./input/UMZ2006_cut2.shx")
  coordCorrec    = c(st_bbox(urban)[1]+1, st_bbox(urban)[2]+1)
  data           = read.table(filename, h=T)
  data_yobs      = data[data$y_obs>0.0,]
  data_nobs      = data[data$y_obs==0.0&data$n_obs>0.0,]
  data_sim       = data[data$p_sim>0.0,]
  data_sim       = data_sim[order(data_sim$p_sim),]
  data_sim$p_sim = log10(data_sim$p_sim)
  RG             = range(data_sim$p_sim)
  
  #--------------------------#
  # 2) Create figure         #
  #--------------------------#
  p = ggplot() +
    geom_sf(data=st_geometry(urban), color=NA, fill="black") +
    #geom_point(data=data_sim, aes(x=x, y=y, color=p_sim)) +
    geom_tile(data=data_sim, aes(x=x+coordCorrec[1], y=y+coordCorrec[2], fill=p_sim), alpha=0.7, colour="grey50") +
    scale_fill_gradient2(low="turquoise4", mid="brown", high="yellow", midpoint=mean(RG), limits=c(RG[1], RG[2])) +
    #annotate(geom="text", x=data$x, y=data$y, label=data$id) +
    geom_vline(xintercept=xintro+coordCorrec[1], linetype=2) +
    geom_hline(yintercept=yintro+coordCorrec[2], linetype=2) +
    labs(fill="Simulated\nprobability\nof presence\n(log10)") +
    xlab("Longitude") +
    ylab("Latitude") +
    ggtitle(title) +
    theme_classic() +
    theme(
      legend.position = c(1, 1),
      legend.justification = c("right", "top"),
      legend.box.just = "left",
      legend.margin = margin(6, 6, 6, 6),
      legend.background = element_rect(fill=adjustcolor("white", alpha.f=0.8))
    )
  return(p)
}

### Plot the distribution of jump sizes ###
plot_jump_size_distribution <- function( filename )
{
  data = read.table(filename, h=T, sep=" ")
  p = ggplot(data, aes(x=(euclidean_dist), after_stat(density))) +
    scale_color_brewer(palette = "BrBG") +
    scale_fill_brewer(palette = "BrBG") +
    geom_histogram() +
    #geom_density() +
    xlim(1,90000) +
    xlab("Dispersal event distance (meters)") +
    ylab("Density") +
    ggtitle("Distribution of dispersal event distances") +
    theme_classic()
  return(p)
}

### Plot the number of jumps through time ###
plot_nb_jumps <- function( filename )
{
  data        = read.table(filename, h=T, sep=" ")
  tabl        = as.data.frame(table(data[,c("repetition", "iteration")]))
  names(tabl) = c("Repetition", "Timestep", "Count")
  p = ggplot(tabl, aes(x=as.numeric(Timestep), y=as.numeric(Count))) +
    #scale_color_brewer(palette = "BrBG") +
    #scale_fill_brewer(palette = "BrBG") +
    geom_smooth(se=F, color="black") +
    xlab("Simulation time (years)") +
    ylab("Mean number of\ndispersal events") +
    ggtitle("Mean number of dispersal events through time") +
    theme_classic()
  return(p)
}

##################
#      MAIN      #
##################

args = commandArgs(trailingOnly=TRUE)
setwd(args[1])
print(args[1])

p1 = plot_invasion_map("./output/final_state.txt", "Invasion geographical pattern", as.numeric(args[2]), as.numeric(args[3]))
p2 = plot_jump_size_distribution("./output/lineage_tree.txt")
p3 = plot_nb_jumps("./output/lineage_tree.txt")
p  = plot_grid(p1, plot_grid(p2, p3, ncol=1), ncol=2)

ggsave("single_simulation.pdf", p, width=10.65, height=6.05, units="in")


