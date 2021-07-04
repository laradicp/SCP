#!/bin/bash

echo "--SCP benchmark iterative algorithm ub--"

cd iterative-algo-ub
make

k=1
for instance in ../instances/A/*; do
	echo "Running $instance"
	echo "Instance $k of 193" 
	
	echo $instance >> ./benchmark/A/bm-$k
	./scp ${instance} >> ./benchmark/A/bm-$k
	
	echo "$k: $instance" >> ./benchmark/A/bm
	grep 'Maximum profit\|Primal\|Dual\|Status\|Time:' ./benchmark/A/bm-$k >> ./benchmark/A/bm
	echo "" >> ./benchmark/A/bm

	k=$(($k + 1))
done
echo "-" >> ./benchmark/A/bm

k=1
for instance in ../instances/D/*; do
	echo "Running $instance"
	echo "Instance $k of 193" 
	
	echo $instance >> ./benchmark/D/bm-$k
	./scp ${instance} >> ./benchmark/D/bm-$k
	
	echo "$k: $instance" >> ./benchmark/D/bm
	grep 'Maximum profit\|Primal\|Dual\|Status\|Time:' ./benchmark/D/bm-$k >> ./benchmark/D/bm
	echo "" >> ./benchmark/D/bm

	k=$(($k + 1))
done
echo "-" >> ./benchmark/D/bm

k=1
for instance in ../instances/C/*; do
	echo "Running $instance"
	echo "Instance $k of 193" 
	
	echo $instance >> ./benchmark/C/bm-$k
	./scp ${instance} >> ./benchmark/C/bm-$k
	
	echo "$k: $instance" >> ./benchmark/C/bm
	grep 'Maximum profit\|Primal\|Dual\|Status\|Time:' ./benchmark/C/bm-$k >> ./benchmark/C/bm
	echo "" >> ./benchmark/C/bm

	k=$(($k + 1))
done
echo "-" >> ./benchmark/C/bm

k=1
for instance in ../instances/B/*; do
	echo "Running $instance"
	echo "Instance $k of 193" 
	
	echo $instance >> ./benchmark/B/bm-$k
	./scp ${instance} >> ./benchmark/B/bm-$k
	
	echo "$k: $instance" >> ./benchmark/B/bm
	grep 'Maximum profit\|Primal\|Dual\|Status\|Time:' ./benchmark/B/bm-$k >> ./benchmark/B/bm
	echo "" >> ./benchmark/B/bm

	k=$(($k + 1))
done
echo "-" >> ./benchmark/B/bm
