import pandas as pd
from math import log10
import csv
import os
import numpy as np
import matplotlib.pyplot as plt

def calculate_white_wr():
    total_count = 0
    positive_count = 0

    with open("meta/matches.csv", 'r') as file:
        reader = csv.DictReader(file)
        for row in reader:
            result = int(row['result'])
            if result > 0:
                positive_count += 1
            total_count += 1

    if total_count == 0:
        return "No matches found"

    positive_percentage = (positive_count / total_count) * 100
    return f"White win rate: {round(positive_percentage, 2)}% (Matches analyzed: {total_count})"

def match_lenght():
    folder_path = "games"
    # List all files in the folder
    files = os.listdir(folder_path)

    # Initialize variables for maximum and minimum line counts
    max_lines = float('-inf')
    min_lines = float('inf')
    max_game = ""
    min_game = ""

    # Initialize a list to store the number of lines per file
    lines_per_file = []

    # Iterate through each file
    for file_name in files:
        file_path = os.path.join(folder_path, file_name)
        
        # Read the file and count the number of lines
        with open(file_path, 'r') as file:
            num_lines = sum(1 for line in file)
            lines_per_file.append(num_lines)

            # Update maximum and minimum line counts
            if num_lines > max_lines:
                max_lines = num_lines
                max_game = file_name
            if num_lines < min_lines:
                min_lines = num_lines
                min_game = file_name

    # Calculate percentiles
    percentile_values = [0, 25, 50, 75, 90, 95, 99, 100]
    percentiles = np.percentile(lines_per_file, percentile_values)

    # Print percentiles
    print("Percentiles:")
    for i in range(len(percentile_values)):
        print("{}th percentile: {}".format(percentile_values[i], round(percentiles[i], 2)))

    # Print game with most and least lines
    print("Game with the most lines: {} ({} lines)".format(max_game, max_lines))
    print("Game with the least lines: {} ({} lines)".format(min_game, min_lines))

    # Plot the histogram
    plt.hist(lines_per_file, bins=10)
    plt.xlabel('Number of moves')
    plt.ylabel('Frequency')
    plt.title('Number of moves per game')
    plt.show()

def generate_game_report():
    csv_file = "meta/matches.csv"
    # Define counters for each player and result type
    players = {}
    overall_stats = {"ascension": 0, "suffocation": 0, "timeout": 0}

    # Open the CSV file and read the data
    with open(csv_file, 'r') as file:
        reader = csv.DictReader(file)
        for row in reader:
            player_a = row['player_a']
            player_b = row['player_b']
            result = int(row['result'])

            # Update player A's stats
            if player_a not in players:
                players[player_a] = {"wins": {"ascension": 0, "suffocation": 0, "timeout": 0, "total": 0},
                                     "losses": {"ascension": 0, "suffocation": 0, "timeout": 0, "total": 0}}
            
            if result > 0:
                to_update = players[player_a]["wins"]
            else:
                to_update = players[player_a]["losses"]
            
            if abs(result) == 1:
                to_update["ascension"] += 1
                overall_stats["ascension"] += 1
            elif abs(result) == 2:
                to_update["suffocation"] += 1 
                overall_stats["suffocation"] += 1
            else:
                to_update["timeout"] += 1 
                overall_stats["timeout"] += 1
            
            to_update["total"] += 1

            # Update player B's stats
            if player_b not in players:
                players[player_b] = {"wins": {"ascension": 0, "suffocation": 0, "timeout": 0, "total": 0},
                                     "losses": {"ascension": 0, "suffocation": 0, "timeout": 0, "total": 0}}
            
            if result < 0:
                to_update = players[player_b]["wins"]
            else:
                to_update = players[player_b]["losses"]
            
            if abs(result) == 1:
                to_update["ascension"] += 1
                overall_stats["ascension"] += 1
            elif abs(result) == 2:
                to_update["suffocation"] += 1 
                overall_stats["suffocation"] += 1
            else:
                to_update["timeout"] += 1 
                overall_stats["timeout"] += 1
            
            to_update["total"] += 1


    # Print the player report in a pretty format
    print("Player Report:")
    print("-----------------------")
    for player, stats in players.items():
        print(f"Player: {player}")
        print("Wins:")
        print(f"  Ascension: {stats['wins']['ascension']} ({stats['wins']['ascension'] / stats['wins']['total'] * 100:.2f}%)")
        print(f"  Suffocation: {stats['wins']['suffocation']} ({stats['wins']['suffocation'] / stats['wins']['total'] * 100:.2f}%)")
        print(f"  Timeout: {stats['wins']['timeout']} ({stats['wins']['timeout'] / stats['wins']['total'] * 100:.2f}%)")
        print("Losses:")
        print(f"  Ascension: {stats['losses']['ascension']} ({stats['losses']['ascension'] / stats['losses']['total'] * 100:.2f}%)")
        print(f"  Suffocation: {stats['losses']['suffocation']} ({stats['losses']['suffocation'] / stats['losses']['total'] * 100:.2f}%)")
        print(f"  Timeout: {stats['losses']['timeout']} ({stats['losses']['timeout'] / stats['losses']['total'] * 100:.2f}%)")
        print(f"Total Games: {stats['wins']['total'] + stats['losses']['total']}")
        print("-----------------------")
    
    # Print overall statistics
    print("Overall Statistics:")
    print("-----------------------")
    overall_total = overall_stats["ascension"] + overall_stats["suffocation"] + overall_stats["timeout"]
    print(f"Overall Ascension: {overall_stats['ascension']} ({overall_stats['ascension'] / overall_total * 100:.2f}%)")
    print(f"Overall Suffocation: {overall_stats['suffocation']} ({overall_stats['suffocation'] / overall_total * 100:.2f}%)")
    print(f"Overall Timeout: {overall_stats['timeout']} ({overall_stats['timeout'] / overall_total * 100:.2f}%)")
    print("-----------------------")



print(calculate_white_wr())
match_lenght()
generate_game_report()
