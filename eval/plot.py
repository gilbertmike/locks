import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

import glob
import os
import sys

def read_csvs(path):
    all_dfs = []
    for fname in glob.iglob(os.path.join(path, '*.csv')):
        df = pd.read_csv(fname, sep='\s*,\s*', engine='python')
        lock_name = os.path.basename(fname).split('.')[0]
        df.drop(0)
        df['lock'] = lock_name
        all_dfs.append(df)
    return pd.concat(all_dfs).reset_index()

def plot(df, path):
    sns.lineplot(data=df, x='threads', y='time', hue='lock')
    plt.savefig(path)

if __name__ == '__main__':
    csvs_path = sys.argv[1]
    plot_path = sys.argv[2]
    df = read_csvs(csvs_path)
    plot(df, plot_path)
