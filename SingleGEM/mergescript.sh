#!/bin/bash
# This is a script to merge the coordinate data of all the simulations into a single csv file, useful for plotting histograms later on in ROOT.
# Replace <Enter_DataStorage_Path_here> with the path to the directory where you have stored your data

# Make it executable first in terminal by typing chmod +x <script_name>.sh
mkdir <Enter_DataStorage_Path_here>/cumulative
for a in {1..50}
do
 mv <Enter_DataStorage_Path_here>/$a/log2.csv <Enter_DataStorage_Path_here>/cumulative/"log2_$a.csv"
 mv <Enter_DataStorage_Path_here>/$a/log.csv <Enter_DataStorage_Path_here>/cumulative/"log_$a.csv"
done
cd <Enter_DataStorage_Path_here>/cumulative
awk 'FNR > 1' "log_*.csv" > Final_Log.csv
awk 'FNR > 1' "log2_*.csv" > Final_Log2.csv

rm "log_*.csv"
rm "log2_*.csv"

