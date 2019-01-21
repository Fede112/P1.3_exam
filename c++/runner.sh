#!/bin/bash

# Average calculator definition -----------------------
calculate_average()
{
	# X axis
	x=1
	# Colums to average
	c1=2
	c2=3
	c3=4

	# Calculate
	awk 'BEGIN {
				v1=0;
				v2=0;
				v3=0
		 } 
		 {
		 		v1+=$c1; 
		 		v2+=$c2;
		 		v3+=$c3
		 }
	 	 END {
	 	 		printf("%d   %0.3f   %0.3f   %0.3f\n", $x,
	 	 		v1/NR,
	 	 		v2/NR,
	 	 		v3/NR)
	 	}
	 	' "x=$x" "c1=$c1" "c2=$c2" "c3=$c3" \
	 	$1 1>> $2
}

# Standard Deviation calculator definition ------------
calculate_std()
{
	# X axis
	x=1
	# Colums to std
	c1=2
	c2=3
	c3=4
	# Calculate
	awk 'BEGIN { 
				sum1=0; sumsq1=0;
				sum2=0; sumsq2=0;
				sum3=0; sumsq3=0;
		 }
		 {
				sum1 += $c1; sumsq1 += ($c1)^2;
				sum2 += $c2; sumsq2 += ($c2)^2;
				sum3 += $c3; sumsq3 += ($c3)^2;
		 } 
	
		 END {
				printf ("%0.3f   %0.3f   %0.3f\n",
				sqrt((sumsq1-sum1^2/NR)/NR),
				sqrt((sumsq2-sum2^2/NR)/NR),
				sqrt((sumsq3-sum3^2/NR)/NR))
		}
	 	'  "c1=$c1" "c2=$c2" "c3=$c3" \
	 	$1 1>> $2
}


# Variable definitions --------------------------------
DIR=./DATA
DATA=${DIR}/time
AVG_DATA=${DIR}/avg_time
STD_DATA=${DIR}/std_time
MERGE_DATA=${DIR}/avg_std_time
DAT=.dat
RAW=.raw
EXE=main.x

INNER_REPETIONS=1000
REPETITIONS=10
SIZES=`seq 100 1000 50100` 
# Compiling ------------------------------------------
make

# Remove previous data ------------------------------
rm ${DIR}/*.dat ${DIR}/*raw 2> /dev/null

# Running --------------------------------------------
for size in ${SIZES[*]}; do
	for repetition in `seq 1 ${REPETITIONS}`; do
		 ./${EXE} ${size} ${INNER_REPETIONS}\
		 1>> ${DATA}_${size}${RAW}		
	done

		calculate_average ${DATA}_${size}${RAW} \
						  ${AVG_DATA}${RAW}
		calculate_std 	  ${DATA}_${size}${RAW} \
					  	  ${STD_DATA}${RAW}
		rm ${DATA}_${size}${RAW}
done

paste 			  ${AVG_DATA}${RAW} ${STD_DATA}${RAW} \
	  			  1>> ${MERGE_DATA}${DAT}

rm 	${AVG_DATA}${RAW} ${STD_DATA}${RAW}


# ------------------------------------
# sed -i '1s/^/Size\tNon balanced tree\t\Balanced tree\tstd::map\n/' \
				# ${MERGE_DATA}${DAT}
