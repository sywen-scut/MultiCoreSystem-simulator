data<-read.table(text="
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	
0	600	0	0	0	0	0	99	0	0	0	",header=FALSE, as.is=TRUE)
data$V1 <- 0:8
data$V8 <-0
data$V2 <- 600
data$V3<- 0
d1<-data[,1:2]
d2<-data[,3:11]
d2
for (i in 1:8) {
  if(i!=8){
  d2[i,i+1]=-100
  d2[i+1,i] =100
  }
}
d2[1,9]=-100
d2[9,1] = 100
d2
data<-cbind(d1,d2)
write.table(data, file="~/simulator/app/single/temp.txt", sep ="   ", row.names =FALSE,col.names =FALSE, quote =TRUE)

  