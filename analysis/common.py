import pandas as pd
from matplotlib import pyplot as plt


def read_sat(path: str):
    df = pd.read_csv(path)
    N = int(len(df.columns) / 4)
    t = df.values[:,0]
    x = df.values[:,range(1, N * 4, 4)]
    y = df.values[:,range(2, N * 4, 4)]
    return t, x, y