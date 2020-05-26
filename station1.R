modelname="Station1"
data=read.table(modelname);
attach(data);

temps=V1
moyenne=V2
nomFichier = paste("station1.pdf")
pdf(file = nomFichier)
plot(temps,moyenne,type="l",xlab="temps",ylab="moyenne paquets",col="red",main="Station 1")
