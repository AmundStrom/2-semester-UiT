n=1000
terning=sample(seq(1:6),n,replace=T)
frekvenser=hist(terning,breaks=c(0,1,2,3,4,5,6)+.5,plot=F)
frekvenser$counts #dette er resultat av hist()
barplot(frekvenser$counts, names.arg=c(1,2,3,4,5,6),
        xlab = "Terning Øyner", ylab = "Antall", 
        width=.1,space=c(5)) #strekdiagram
summary(terning)
mean(terning) #gjennomsnitt
median(terning) #median
var(terning) #varians
sqrt(var(terning))#standardavvik
sd(terning) #standardavvik