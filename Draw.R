modelname="Station1"
data=read.table(modelname);
attach(data);

temps=V1
moyenne=V2

plot(temps,moyenne,type="l",xlab="temps",ylab="E[N]",col="red",main="Simulation M/M/1")
