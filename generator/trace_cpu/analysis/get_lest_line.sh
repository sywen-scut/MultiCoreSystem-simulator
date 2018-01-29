for name in barnes blackscholes canneal dedup ferret fluidanimate freqmine raytrace streamcluster swaptions swaptions
do
        for method in xy
        do
                dir=cpu_"$name"_"$method".txt
                dir_out=last_line_"$name"_"$method".txt
                echo $dir
                echo $dir_out
                if [ -f "./last_line/$dir_out" ]
                then
                        rm ./last_line/$dir_out
                fi
                tail -n 1 ../$dir >> ./last_line/$dir_out
        done
done

