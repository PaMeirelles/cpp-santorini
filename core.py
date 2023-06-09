import pandas as pd
from math import log10, sqrt
import numpy as np
import matplotlib.pyplot as plt

def elo_diff(e):
    if e == 0:
        return -800
    if e == 1:
        return 800
    return -400 * log10(1 / e - 1)


def fill_data(time):
    df = pd.read_csv("matches.csv")
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
        df = pd.read_csv(f"data/{p}.csv", dtype={'opponent': str})
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

def calculate_confidence_interval(n, p):
    q = 1-p
    a = sqrt(p * q / n) * 1.64
    return elo_diff(0.5+a)

def process_player_csv(player):
    df = pd.read_csv(f"data/{player}.csv", dtype={'opponent': str})
    df["confidence interval"] = df.apply(lambda row: calculate_confidence_interval(row["matches"], float(row["wr"].strip("%")) / 100), axis=1)
    df["player"] = player
    return df[["player", "opponent", "confidence interval"]]

def fill_confidence_interval():
    elos = pd.read_csv("elos.csv")
    player = elos["player"]
    df = pd.DataFrame()
    for p in player:
        df = pd.concat([df, process_player_csv(p)])
    ordered =  df.sort_values(["confidence interval"], ascending=False).reset_index(drop=True)
    unique = ordered[ordered.index % 2 == 0].reset_index(drop=True)
    unique.to_csv("confidence_interval.csv", index=False, float_format='%.2f')

fill_data(60000)
adjust_elos(600, ("Titan", 1000))
fill_confidence_interval()

