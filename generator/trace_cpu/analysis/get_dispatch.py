import sys
import re

for i in file('./name'):
    name1='./dispatch/'+i.strip('\n')+'_dispatch.txt'
    name2='../'+i.strip('\n')+'.txt'
    f=file(name1,'w')
    for line in file(name2):
            if(filter(str.isalpha,line)=='dispatch'):
#                    print line
                    f.write(line)
    print i.strip('\n')+' finished'
    f.close();
