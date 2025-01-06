#!/bin/bash

# Define the CSV file path
csv_file="csv/create_stitched_L.csv"

# Add headers if the file doesn't exist
if [[ ! -f $csv_file ]]; then
  echo "K,L,R,Rstitched,a,Recall,UserTime,ElapsedTime" > "$csv_file"
fi

# Define the values of 'R' to test
L_values=(20 40 50 70 100 120 150 200)

# Loop over the values of 'a'
for L in "${L_values[@]}"; do
  # Run the executable and process its output
  ./bin/main -f dummy -k 100 -L "$L" -R 40 -a 1.2 -Rst 96 -v stitched -x create | while read -r K L R Rstitched a_val Recall UserTime ElapsedTime; do
    # Append each line to the CSV file
    echo "$K,$L,$R,$Rstitched,$a_val,$Recall,$UserTime,$ElapsedTime" >> "$csv_file"
  done
done