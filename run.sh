#!/bin/bash

# Define the CSV file path
csv_file="output.csv"

# Add headers if the file doesn't exist
if [[ ! -f $csv_file ]]; then
  echo "Name,Age,Country" > "$csv_file"
fi

# Run the executable and process its output
./test_main | while read -r name age country; do
  # Append each line to the CSV file
  echo "$name,$age,$country" >> "$csv_file"
done
