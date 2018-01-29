CFlag=g++
TargetFile=corr_cpu
ObjectFile=main_v2.o

$(TargetFile):$(ObjectFile)
	$(CFlag) -g $(ObjectFile) -o $(TargetFile)
$(ObjectFile):main_v2.cpp
	$(CFlag) -c main_v2.cpp
clean:
	rm *.o *~ $(TargetFile)



