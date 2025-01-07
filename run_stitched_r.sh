# Define the CSV file path
csv_file="csv/run_stitched_R.csv"

# Add headers if the file doesn't exist
if [[ ! -f $csv_file ]]; then
  echo "K,L,R,Rstitched,a,Recall,UserTime,ElapsedTime" > "$csv_file"
fi

# Define the values of 'a' to test
R_values=(10 20 30 40 50 60 70 80 90 100 110)

# Loop over the values of 'a'
for R in "${R_values[@]}"; do
  # Run the executable and process its output
  ./bin/main -f dummy -k 100 -L 150 -R "$R" -Rst 96 -a 1.2 -v stitched -x run | while read -r K L R Rstitched a_val Recall UserTime ElapsedTime; do
    # Append each line to the CSV file
    echo "$K,$L,$R,$Rstitched,$a_val,$Recall,$UserTime,$ElapsedTime" >> "$csv_file"
  done
done
