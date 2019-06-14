#!/bin/bash

# Bash script to run the avalanche simulation multiple times in a loop. The generated data is moved to a separate directory to prevent overwriting. 


for a in {1..50}
do
  echo $a
  name="$a"
  echo $name
  <Enter_mesh_location_path_here>/avalanche1 
  mkdir <Enter_data_storage_here>/$a
  mv <Enter_mesh_location_path_here>/log2.csv <Enter_data_storage_here>/$a/log2.csv
  mv <Enter_mesh_location_path_here>/log.csv <Enter_data_storage_here>/$a/log.csv
  mv <Enter_mesh_location_path_here>/details.txt <Enter_data_storage_here>/$a/details.txt
  mv <Enter_mesh_location_path_here>/gain.csv <Enter_data_storage_here>/Logs/"$name.csv"
done
cd <Enter_data_storage_here>/Logs
cat *.csv >> final.csv  
#This merges the gain data into a single CSV file which can be plotted later on.

