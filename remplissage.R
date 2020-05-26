modelname="remplissageAnneau"
data=read.table(modelname);
attach(data);

temps=V1
moyenne=V2
 nomFichier = paste("remplissage Anneau.pdf")
  pdf(file = nomFichier)
plot(temps,moyenne,type="l",xlab="temps",ylab="nombre de paquets",col="red",main="Anneau")
