library(ggplot2)
library(ggthemr)
library(reshape2)
ggthemr("dust")
dat <- read.table(text="
waittingTime  executionTime  responseTime  migrationCost
1   1   1  1
1.4   1.15   1.2  0.29
0.01   1.27   0.93   0.26", header=TRUE, as.is=TRUE)
dat$Methods <- factor(c("Square", "ChessBoard", "GlobalCoolest"), 
                  levels=c("Square", "ChessBoard", "GlobalCoolest"))
mdat <- melt(dat, id.vars="Methods")
head(mdat)
ggplot(mdat, aes(variable, value, fill=Methods)) + 
  xlab("Perfomance category") +
  ylab("Normalized Time") +
  theme(axis.text.x = element_text(angle = 45, hjust = 0.5, vjust = 0.5))+
  geom_bar(stat="identity", position="dodge")
