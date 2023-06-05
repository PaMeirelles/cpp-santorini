import pandas as pd
from math import log10
import csv
import os
import numpy as np
import matplotlib.pyplot as plt

def elo_diff(e):
    if e == 0:
        return -800
    if e == 1:
        return 800
    return -400 * log10(1 / e - 1)


def fill_data(time):
    df = pd.read_csv("meta/matches.csv")
    elos = pd.read_csv("elos.csv")
    players = elos["player"]
    data = {p1: {p2: {"wins": 0, "losses": 0, "matches": 0} for p2 in players} for p1 in players}
    for i, row in df.iterrows():
        if row["time_a"] != time and time != "any":
            continue
        pa, pb = row["player_a"], row["player_b"]
        if pa == pb:
            continue
        if row["result"] > 0:
            winner, loser = pa, pb
        else:
            loser, winner = pa, pb

        data[pa][pb]["matches"] += 1
        data[pb][pa]["matches"] += 1

        data[winner][loser]["wins"] += 1
        data[loser][winner]["losses"] += 1

    for p in players:
        with open(f"data/{p}.csv", "w") as f:
            f.write("opponent,wins,losses,matches,eed,wr\n")
            for op in data[p].keys():
                w = data[p][op]['wins']
                l = data[p][op]['losses']
                m = data[p][op]['matches']
                if m != 0:
                    f.write(f"{op},{w},{l},{m},{round(elo_diff(w / m), 2)},{round(100 * w / m, 2)}%\n")


def adjust_elos(n, anchor, first=True):
    if n == 0:
        return
    avg = anchor[1]
    new_elos = {}
    elos = pd.read_csv("elos.csv")
    player = elos["player"]
    for p in player:
        df = pd.read_csv(f"data/{p}.csv")
        temp = df.merge(elos, left_on="opponent", right_on="player")
        if first:
            temp["elo"] = avg
        temp["expected_elo"] = temp["elo"] + temp["eed"]

        d = temp["expected_elo"]
        w = temp["matches"]

        new_elos[p] = (d * w).sum() / w.sum()

    diff = new_elos[anchor[0]] - anchor[1]
    new_elos = {k: v - diff for k, v in new_elos.items()}
    with open("elos.csv", "w") as f:
        f.write("player,elo\n")
        for p in new_elos.keys():
            new_elo = new_elos[p] + elos.loc[elos["player"] == p, "elo"].values[0]
            f.write(f"{p},{round(new_elo/2, 2)}\n")

    print(f"Adjusting...{n} left")
    return adjust_elos(n - 1, anchor, False)

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

    # Initialize a list to store the number of lines per file
    lines_per_file = []

    # Iterate through each file
    for file_name in files:
        file_path = os.path.join(folder_path, file_name)
        
        # Read the file and count the number of lines
        with open(file_path, 'r') as file:
            num_lines = sum(1 for line in file)
            lines_per_file.append(num_lines)

    # Calculate percentiles
    percentile_values = [0, 25, 50, 75, 90, 95, 99, 100]
    percentiles = np.percentile(lines_per_file, percentile_values)

    # Print percentiles
    print("Percentiles:")
    for i in range(len(percentile_values)):
        print("{}th percentile: {}".format(percentile_values[i], percentiles[i]))

    # Plot the histogram
    plt.hist(lines_per_file, bins=10)
    plt.xlabel('Number of Lines')
    plt.ylabel('Frequency')
    plt.title('Number of Lines per File')
    plt.show()


fill_data(60000)
adjust_elos(10, ("Titan", 1000))
print(calculate_white_wr())
match_lenght()