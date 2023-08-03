#!/bin/bash

# files=("cfg0210-int.4" "cfg0210-int.6" "cfg0210-int.8" "cfg0211-intq.4" "cfg0211-intq.6" "cfg0211-intl.4" "cfg0211-intl.6" "cfg0211.13")
# files=("cfg0210-int.5" "cfg0211-intq.5" "cfg0211-intl.5")
# files=( "cfg0211/link-smoothed/cfg0211-intl-sm.13" "cfg0211/queue-smoothed/cfg0211-intq-sm.13" "cfg0211/no-int/cfg0211.13")
files=( "cfg0211/int/cfg0211-int." "cfg0211/link/cfg0211-intl." ) #"cfg0211/no-int/cfg0211."" "cfg0211/queue/cfg0211-intq."
gens=( 2 3 4 5 )
seed=10

for file in ${files[*]}; do
    for gen in ${gens[*]}; do
        echo "Evaluating $file$gen"
        ./sender-runner if=/home/eecs/sarah/remy/results/$file$gen cfg=/home/eecs/sarah/remy/configs/config0211.cfg seed=$seed sample=10000 > ../results/evals/$(basename $file)$gen-$seed-2.txt
    done
done