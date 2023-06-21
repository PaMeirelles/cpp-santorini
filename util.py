import os, csv

def remove_whisper_entries(csv_file_path):
    temp_file_path = "temp.csv"  # Temporary file to store filtered entries

    with open(csv_file_path, "r", newline="") as file:
        with open(temp_file_path, "w", newline="") as temp_file:
            reader = csv.DictReader(file)
            fieldnames = reader.fieldnames
            writer = csv.DictWriter(temp_file, fieldnames=fieldnames)
            writer.writeheader()

            for row in reader:
                if row["player_a"] != "Whisper" and row["player_b"] != "Whisper":
                    writer.writerow(row)

    # Replace the original file with the filtered one
    os.replace(temp_file_path, csv_file_path)


remove_whisper_entries("matches.csv")