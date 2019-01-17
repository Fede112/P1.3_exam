#!/bin/bash


DIR=./DATA
MERGE_DATA=${DIR}/avg_std_time
DAT=.dat

PLOT_SCRIPT=${DIR}/plot.plt 
FIG=${DIR}/plot.png

cat <<EOF > ${PLOT_SCRIPT}
set terminal png  size 1200, 800   
set output  "${FIG}" 
set multiplot layout 1, 2 title ""

set title "tree vs map"
set xlabel "Size"
set ylabel "Time (ns)"
# set logscale y
plot    "${MERGE_DATA}${DAT}" using 1:(\$2+\$5):(\$2-\$5)  \
		 with filledcurves title "+/- STD", \
	    "${MERGE_DATA}${DAT}" using 1:2  \
		 with lines lw 3 lc rgb "red" title \
		 "Time for naive Tree", \
	    "${MERGE_DATA}${DAT}" using 1:(\$3+\$6):(\$3-\$6)  \
		 with filledcurves lc rgb "green" title "+/- STD", \
	    "${MERGE_DATA}${DAT}" using 1:3  \
		 with lines lw 3 lc rgb "yellow" title \
		 "Time for naive Tree"

set xlabel "Size"
set ylabel "Time (ns)"
# set logscale x 
plot    "${MERGE_DATA}${DAT}" using 1:(\$4+\$7):(\$4-\$7) \
		with filledcurves lc rgb "violet" title "+/- STD", \
	    "${MERGE_DATA}${DAT}" using 1:4 \
		 with lines lw 3 lc rgb "blue" title \
		"Time for std::map"
EOF

gnuplot ${PLOT_SCRIPT}

rm ${PLOT_SCRIPT} 