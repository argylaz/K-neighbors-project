# Define the CSV file path
csv_file="csv/create_stitched_R.csv"

# Add headers if the file doesn't exist
if [[ ! -f $csv_file ]]; then
  echo "K,L,R,Rstitched,a,UserTime,ElapsedTime" > "$csv_file"
fi

# Define the values of 'R (small)' to test
R_values=(10 20 30 40 50 60 70 80 90 100 110)

# Loop over the values of 'R'
for R in "${R_values[@]}"; do
  # Run the executable and process its output
  ./bin/main -f dummy -k 100 -L 150 -R "$R" -Rst 96 -a 1.2 -v stitched -x create | while read -r K L R Rstitched a_val UserTime ElapsedTime; do
    # Append each line to the CSV file
    echo "$K,$L,$R,$Rstitched,$a_val,$UserTime,$ElapsedTime" >> "$csv_file"
  done
done
