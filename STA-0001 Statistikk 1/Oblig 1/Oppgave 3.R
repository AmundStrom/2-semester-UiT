lifetime=rexp(1000,rate=1/2500)
lifetime=round(lifetime,digits=0)

A=mean(lifetime)-2*sd(lifetime)#intervallets nedre grense
B=mean(lifetime)+2*sd(lifetime) #intervallets øvre grense
A #nedre grense
B #øvre grense
x=hist(lifetime,breaks=c(A,B,max(lifetime),include.lowest=F),plot=F)
x
attributes(x)
x$counts
