library("ggplot2")
library("reshape2")
library("plyr")
data.df <- read.csv("/Users/arrian/Desktop/COMP4540/research-project/R/results-study-3.csv")
names(data.df)[match("how.many.live.coding.performances.have.you.been.to.", names(data.df))] <- "performances.attended"
names(data.df)[match("how.much.music.do.you.regularly.listen.to.", names(data.df))] <- "music.listening"
names(data.df)[match("do.you.play.an.instrument.or.sing.", names(data.df))] <- "musical.ability"
names(data.df)[match("how.much.experience.do.you.have.with.programming.", names(data.df))] <- "programming.experience"
names(data.df)[match("do.you.have.much.experience.with.the.Lisp.family.of.programming.languages.", names(data.df))] <- "lisp.experience"
traj.df <- subset(melt(data.df, id.vars = 1:9, variable.name = "question", value.name = "response"), question %in%
c("unvisualised.enjoyment.beginning", "unvisualised.enjoyment.middle",
"unvisualised.enjoyment.end", "unvisualised.understanding.beginning",
"unvisualised.understanding.middle", "unvisualised.understanding.end",
"visualised.enjoyment.beginning", "visualised.enjoyment.middle",
"visualised.enjoyment.end", "visualised.understanding.beginning",
"visualised.understanding.middle", "visualised.understanding.end"))
## filthy hacks!
traj.df$performances.attended <- as.character(traj.df$performances.attended)
traj.df$performances.attended[traj.df$performances.attended == "first one"] <- "1"
traj.df$performances.attended[traj.df$performances.attended == "one or two"] <- "2"
traj.df$performances.attended[traj.df$performances.attended == "3?"] <- "3"
## combine factor levels
traj.df$programming.experience[traj.df$programming.experience == "some experience"] <- "no experience"
traj.df$performances.attended <- as.numeric(traj.df$performances.attended)
traj.df$response[traj.df$response == "low/medium"] <- "low"
traj.df$response[traj.df$response == "medium/high"] <- "high"
traj.df$response[traj.df$response == "-"] <- NA
## get ordered factor levels right
traj.df$response <- factor(traj.df$response, levels = c("low", "medium", "high"), ordered = TRUE)
traj.df$programming.experience <- factor(traj.df$programming.experience, levels = c("no experience", "some experience", "currently program"), ordered = TRUE)
traj.df$music.listening <- factor(traj.df$music.listening, levels = c("hardly any", "a little", "large amount"), ordered = TRUE)
traj.df$music.listening <- factor(traj.df$music.listening, levels = c("hardly any", "a little", "large amount"), ordered = TRUE)
traj.df$musical.ability <- factor(traj.df$musical.ability, levels = c("no", "occasionally", "yes"), ordered = TRUE)
traj.df$lisp.experience <- factor(traj.df$lisp.experience, levels = c("don't know what you are talking about", "no", "yes"), ordered = TRUE)
traj.df <- cbind(traj.df, as.data.frame(do.call(rbind, strsplit(as.character(traj.df$question), ".", fixed = TRUE))))
names(traj.df)[12:14] <- c("type", "var", "stage")
summary.df <- ddply(traj.df, .(type, var, stage), summarize,
                    mean = mean(map.response.to.numeric(response), na.rm = TRUE),
                    se = se(map.response.to.numeric(response), na.rm = TRUE),
                    low = mean(response == "low", na.rm = TRUE),
                    medium = mean(response == "medium", na.rm = TRUE),
                    high = mean(response == "high", na.rm = TRUE))
map.response.to.numeric <- function(resp){
    c(low = -1, medium = 0, high = 1)[resp]
}
se <- function(x, na.rm) sd(x, na.rm)/sqrt(length(x))

## plots

## THIS ONE, ARRIAN (tweak to size, etc)

library(grid)

condition_dimension_labeller <- function(var, value){
    value <- as.character(value)
    if (var=="var") { 
        value[value=="enjoyment"] <- "Enjoyment"
        value[value=="understanding"]   <- "Understanding"
    }
    else {
    	value[value=="unvisualised"] <- "No Visualisation Condition"
        value[value=="visualised"]   <- "Visualisation Condition"	
    }
    return(value)
}

ggplot(melt(summary.df, id.vars=1:5), aes(as.numeric(stage), value*100, colour = variable, linetype = variable)) + geom_line(size = 1) + scale_x_continuous(breaks = 1:3, labels = c("Beginning", "Middle", "End")) + labs(x = "Stage of Performance", y = "Audience %", linetype="Reported Level", colour="Reported Level") + facet_grid(type~var, labeller=condition_dimension_labeller) + scale_colour_manual(values = c("#f7756d","#00b0f6","#3ab601")) + theme(panel.margin = unit(1, "lines"), legend.position="top", axis.text.y=element_text(colour="black"), axis.text.x=element_text(colour="black"), legend.key.width=unit(1.5,'cm')) + scale_linetype_manual(values = c("longdash","dotted","solid"))

ggsave("option-1.pdf", width=9, height=5)

ggplot(summary.df, aes(as.numeric(stage), mean, colour = type)) + geom_line(size = 2)  + geom_errorbar(aes(ymin=mean-se, ymax=mean+se), size = 1, width = 0.1) + scale_x_continuous(breaks = 1:3, labels = c("beginning", "middle", "end")) + scale_y_continuous(limits = c(-1, 1), breaks = c(-1, 0, 1), labels = c("low", "medium", "high")) + labs(x = "stage of performance", y = "mean response") + facet_wrap(~var)

ggsave("option-2.pdf")

## split by another variable

ozchi.plot1 <- function(split.var){
    df <- ddply(traj.df, c(split.var, "type", "var", "stage"), summarize,
                mean = mean(map.response.to.numeric(response), na.rm = TRUE),
                se = se(map.response.to.numeric(response), na.rm = TRUE),
                low = mean(response == "low", na.rm = TRUE),
                med = mean(response == "medium", na.rm = TRUE),
                high = mean(response == "high", na.rm = TRUE))
    ggplot(melt(df, id.vars=1:6), aes(as.numeric(stage), value*100, colour = variable, linetype = type)) + geom_line(size = 2) + scale_x_continuous(breaks = 1:3, labels = c("beginning", "middle", "end")) + labs(x = "stage of performance", y = "percentage of respondents") + facet_grid(paste("var", split.var, sep="~")) + scale_colour_manual(values = c("red", "orange", "green"))
}

ozchi.plot2 <- function(split.var){
    df <- ddply(traj.df, c(split.var, "type", "var", "stage"), summarize,
                mean = mean(map.response.to.numeric(response), na.rm = TRUE),
                se = se(map.response.to.numeric(response), na.rm = TRUE),
                low = mean(response == "low", na.rm = TRUE),
                med = mean(response == "medium", na.rm = TRUE),
                high = mean(response == "high", na.rm = TRUE))
    ## browser()
    ggplot(df, aes(as.numeric(stage), mean, colour = type)) + geom_line(size = 2)  + geom_errorbar(aes(ymin=mean-se, ymax=mean+se), size = 1, width = 0.1) + scale_x_continuous(breaks = 1:3, labels = c("beginning", "middle", "end")) + scale_y_continuous(limits = c(-1, 1), breaks = c(-1, 0, 1), labels = c("low", "medium", "high")) + labs(x = "stage of performance", y = "mean response") + facet_grid(paste("var", split.var, sep="~"))
}

ggsave("option-2.pdf")



ggplot(traj.df, aes(stage)) + geom_bar(aes(fill = response)) + facet_grid(type~var) + scale_fill_manual(values = c("red", "orange", "green", "grey50"))

## by performance

summary2.df <- melt(summary.df, id.vars=1:4)
strsplit(as.character(summary2.df$variable), ".", fixed = TRUE)

summary2.df <- cbind(summary2.df, as.data.frame(do.call(rbind, strsplit(as.character(summary2.df$variable), ".", fixed = TRUE))))

ggplot(summary2.df, aes(as.numeric(stage), value*100, colour = V1, linetype = V2)) + geom_line(size = 2) + scale_x_continuous(breaks = 1:3, labels = c("beginning", "middle", "end"))  + labs(x = "stage of performance", y = "percentage of respondents") + facet_wrap(~var)
ggsave("option-1.pdf")

## positive/negative ones

sum.df <- melt(ddply(traj.df, c("type", "var", "stage"),
                     summarize,
                     low = -100*mean(response == "low", na.rm = TRUE),
                     high = 100*mean(response == "high", na.rm = TRUE)))
sum.df$stage <- factor(sum.df$stage, levels = c("beginning", "middle", "end"), ordered = TRUE)

## positive/negative bar plot (need to eval sum.df above) ###############

ggplot() + geom_bar(data = subset(sum.df, value <=0), aes(stage, value, fill = variable), fill = "#FF6669", stat = "identity") + geom_bar(data = subset(sum.df, value >0), aes(stage, value, fill = variable), fill = "#67E38E", stat = "identity") + scale_y_continuous(breaks = seq(-50,50,25), labels = c("50%", "25%", "0%", "25%", "50%")) + labs(x = "Stage of Performance", y = "Audience %") + facet_grid(type~var, labeller=condition_dimension_labeller) + theme(panel.margin = unit(1, "lines"), axis.text.y=element_text(colour="black"), axis.text.x=element_text(colour="black"))  + geom_hline(yintercept=0)

ggsave("pos-neg.pdf", width=9, height=5)

## ozchi.plot3 <- function(split.var){
##     df <- melt(ddply(traj.df, c(split.var, "type", "var", "stage"),
##                      summarize,
##                      low = -1*sum(response == "low", na.rm = TRUE),
##                      high = sum(response == "high", na.rm = TRUE)),
##                id.vars = 1:3)
##     ggplot() + geom_bar(data = subset(df, value <=0), aes(stage, value, fill = variable), fill = "red", stat = "identity") + geom_bar(data = subset(df, value >0), aes(stage, value, fill = variable), fill = "green", stat = "identity") + labs(x = "stage of performance", y = "number of responses") + facet_grid(paste("var", split.var, sep="~"))
## }
