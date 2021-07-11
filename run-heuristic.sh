#!/bin/bash

echo "--SCP benchmark heuristic--"

cd heuristic
make

k=1
for instance in ../instances/A/*; do
	echo "Running $instance"
	echo "Instance $k of 193" 
	
	echo $instance
	./scp-heuristic ${instance}
	
	k=$(($k + 1))
done

k=1
for instance in ../instances/D/*; do
	echo "Running $instance"
	echo "Instance $k of 193" 
	
	echo $instance
	./scp-heuristic ${instance}
	
	k=$(($k + 1))
done

k=1
for instance in ../instances/C/*; do
	echo "Running $instance"
	echo "Instance $k of 193" 
	
	echo $instance
	./scp-heuristic ${instance}
	
	k=$(($k + 1))
done

k=1
for instance in ../instances/B/*; do
	echo "Running $instance"
	echo "Instance $k of 193" 
	
	echo $instance
	./scp-heuristic ${instance}
	
	k=$(($k + 1))
done
