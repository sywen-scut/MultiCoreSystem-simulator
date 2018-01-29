#./corr -n barnes_xy -i 100000 -f 0.1
interval=10000
freq=0.5
for sets in barnes_xy 
do
        ./corr_cpu -n $sets -i $interval -f $freq
done

