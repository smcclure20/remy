#!/usr/bin/env Rscript

# Read in results
# Sort configs in some systematic way
# Plot a summary of all runs by getting sender summary stats

args <- commandArgs(trailingOnly=TRUE)

data <- read.csv(args[1])

# Get summary stats for each network configuration
agg_tput <- aggregate(data$Throughput, list(data$Config), FUN=function(x) c(mean = mean(x), sd = sd(x) ))
agg_tput_df <- data.frame(agg_tput$Group.1, agg_tput$x[,"mean"], agg_tput$x[,"sd"])
colnames(agg_tput_df) <- c("Config", "Avg.Tput", "Tput.SD")

agg_del <- aggregate(data$Delay, list(data$Config), FUN=function(x) c(mean = mean(x), sd = sd(x) ))
agg_del_df <- data.frame(agg_del$Group.1, agg_del$x[,"mean"], agg_del$x[,"sd"])
colnames(agg_del_df) <- c("Config", "Avg.Delay", "Delay.SD")

all_summary <- merge(x=agg_tput_df, y=agg_del_df, by="Config", all=TRUE)

# Get configuration information
cfg_data <- read.csv(args[2])

# Plot
pdf(args[3])

# Plot averages
plot(all_summary$Avg.Tput, all_summary$Avg.Delay, main="RemyCC Average Throughput and Delay", xlab="Average Throughput", ylab="Average Delay", type="p")

# Plot standard deviations
plot(all_summary$Tput.SD, all_summary$Delay.SD, main="RemyCC Throughput and Delay Std Dev", xlab="Throughput Std Dev", ylab="Delay Std Dev", type="p")

# Plot both at the same time
plot(all_summary$Avg.Tput, all_summary$Avg.Delay, main="RemyCC Throughput and Delay", xlab="Average Throughput", ylab="Average Delay", type="p", pch=20, cex=1.5)
arrows(x0=all_summary$Avg.Tput-all_summary$Tput.SD,
       y0=all_summary$Avg.Delay, 
       x1=all_summary$Avg.Tput+all_summary$Tput.SD, 
       y1=all_summary$Avg.Delay,
       code=3, angle=90, length=0.05, col="gray")
arrows(x0=all_summary$Avg.Tput,
       y0=all_summary$Avg.Delay-all_summary$Delay.SD, 
       x1=all_summary$Avg.Tput, 
       y1=all_summary$Avg.Delay+all_summary$Delay.SD,
       code=3, angle=90, length=0.05, col="gray")

# Plot with color for a network configuration feature
rbPal <- colorRampPalette(c('red','blue'))
net_features <- c("MeanOn", "MeanOff", "Nsrc", "LinkPpt", "Delay", "BufferSize")
for (i in 1:length(net_features)){
    feat <- net_features[i]

    # Divide feature into cuts for all configs
    feature_cut <- cut(cfg_data[,feat], breaks=10)
    cfg_data$color <- as.numeric(feature_cut)
    cfg_data$color_lab <- feature_cut
    # print(cfg_data)

    # For each configuration in the run data, get the color of its feature
    all_summary$col <- rbPal(10)[cfg_data$color[which(cfg_data$ConfigNum == all_summary$Config)]]

    # Plot the same plot as above, but with the color coding for this feature
    plot(all_summary$Avg.Tput, all_summary$Avg.Delay, main=paste("RemyCC Throughput and Delay (", feat, ")", sep=" "), xlab="Average Throughput", ylab="Average Delay", type="p", 
    col=all_summary$col, pch = 20, cex=1.5)
    arrows(x0=all_summary$Avg.Tput-all_summary$Tput.SD,
        y0=all_summary$Avg.Delay, 
        x1=all_summary$Avg.Tput+all_summary$Tput.SD, 
        y1=all_summary$Avg.Delay,
        code=3, angle=90, length=0.05, col="gray")
    arrows(x0=all_summary$Avg.Tput,
        y0=all_summary$Avg.Delay-all_summary$Delay.SD, 
        x1=all_summary$Avg.Tput, 
        y1=all_summary$Avg.Delay+all_summary$Delay.SD,
        code=3, angle=90, length=0.05, col="gray")
    legend("topleft",title=feat,legend=levels(feature_cut), col=rbPal(10), pch=20)
}