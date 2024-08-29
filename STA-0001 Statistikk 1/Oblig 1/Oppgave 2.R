lifetime=rexp(1000,rate=1/2500)
lifetime=round(lifetime,digits=0)
hist(lifetime,nclass=25)
sort(lifetime) #Sorterer fra minste til største verdi
mean(lifetime) #Gjennomsnittet
median(lifetime) #Medianen
