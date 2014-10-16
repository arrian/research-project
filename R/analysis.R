library("ggplot2")
library("reshape2")
library("plyr")
data.df <- read.csv("survey-data.csv")
traj.df <- subset(melt(data.df, id.vars = 1:4, variable.name = "question", value.name = "response"), question %in%
c("aesthetic.enjoyment.beginning", "aesthetic.enjoyment.middle",
"aesthetic.enjoyment.end", "aesthetic.understanding.beginning",
"aesthetic.understanding.middle", "aesthetic.understanding.end",
"didactic.enjoyment.beginning", "didactic.enjoyment.middle",
"didactic.enjoyment.end", "didactic.understanding.beginning",
"didactic.understanding.middle", "didactic.understanding.end"))
traj.df$response[traj.df$response %in% c("-", "low/medium", "medium/high")] <- NA
traj.df$response <- factor(traj.df$response, levels = c("low", "medium", "high"), ordered = TRUE)
traj.df <- cbind(traj.df, as.data.frame(do.call(rbind, strsplit(as.character(traj.df$question), ".", fixed = TRUE))))
names(traj.df)[7:9] <- c("type", "var", "stage")
summary.df <- ddply(traj.df, .(type, var, stage), summarize,
                    mean = mean(map.response.to.numeric(response), na.rm = TRUE),
                    se = se(map.response.to.numeric(response), na.rm = TRUE),
                    low = mean(response == "low", na.rm = TRUE),
                    med = mean(response == "medium", na.rm = TRUE),
                    high = mean(response == "high", na.rm = TRUE))
map.response.to.numeric <- function(resp){
    c(low = -1, med = 0, high = 1)[resp]
}
se <- function(x, na.rm) sd(x, na.rm)/sqrt(length(x))

## plots

ggplot(melt(summary.df, id.vars=1:5), aes(as.numeric(stage), value*100, colour = variable, linetype = type)) + geom_line(size = 2) + scale_x_continuous(breaks = 1:3, labels = c("beginning", "middle", "end"))  + labs(x = "stage of performance", y = "percentage of respondents") + facet_wrap(~var)

ggplot(summary.df, aes(as.numeric(stage), mean, colour = type)) + geom_line(size = 2)  + geom_errorbar(aes(ymin=mean-se, ymax=mean+se), size = 1, width = 0.1) + scale_x_continuous(breaks = 1:3, labels = c("beginning", "middle", "end")) + scale_y_continuous(limits = c(-1, 1), breaks = c(-1, 0, 1), labels = c("low", "medium", "high")) + labs(x = "stage of performance", y = "mean response") + facet_wrap(~var)

ggplot(traj.df, aes(stage)) + geom_bar(aes(fill = response)) + facet_grid(type~var) + scale_fill_manual(values = c("red", "orange", "green", "grey50"))
