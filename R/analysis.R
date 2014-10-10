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

ggplot(traj.df, aes(stage)) + geom_bar(aes(fill = response)) + facet_grid(type~var) + scale_fill_manual(values = c("red", "yellow", "green", "grey50"))

ggplot(traj.df, aes(stage)) + geom_bar(aes(fill = response)) + facet_grid(type~var) + scale_fill_manual(values = c("red", "yellow", "green", "grey50"))

map.response.to.numeric <- function(resp){
    c(low = -1, med = 0, high = 1)[resp]
}

map.response.to.numeric(traj.df$response)

sapply(traj.df$response, map.response.to.numeric)

revalue(traj.df$response, c())

ggplot(transform(traj.df, response = map.response.to.numeric(response)), aes(stage, response, fill = type, style = var)) + geom_boxplot() + facet_wrap(~var)

ggplot(melt(ddply(traj.df, .(type, var, stage), summarize,
           mean = mean(map.response.to.numeric(response), na.rm = TRUE),
           sd = sd(map.response.to.numeric(response), na.rm = TRUE)),
     id.vars = 1:3),
       aes(as.numeric(stage), value, colour = type, linetype = variable)) + geom_path() + facet_wrap(~var)
