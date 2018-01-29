TOTAL_COUNT=144;
TILE=32
for sets in banres_xy blackscholes_xy canneal_xy dedup_xy ferret_xy fluidanimate_xy freqmine_xy raytrace_xy streamcluster_xy swaptions_xy  vips_xy
do
        ./trace_flow_anal4 -N $sets -C $TOTAL_COUNT -T $TILE
done

