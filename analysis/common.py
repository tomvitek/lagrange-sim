import pandas as pd
from matplotlib import axes, animation
from matplotlib import pyplot as plt

MOON_A = 3.844e8
MOON_M = 7.3482e22
EARTH_M = 5.974e24
D = 3.844e8
d = 4.669e6

L12_DST = (MOON_A * pow(MOON_M / 3 / EARTH_M, 1/3))
L3_X = -MOON_A * (1 + 7 * MOON_M / (12 * EARTH_M))
L4_X = MOON_A / 2
L4_Y = MOON_A * (3/4)**0.5

def read_sat(path: str):
    '''
    Reads output of a simulation in form of csv file.
    Returns the result as time, x, y
    '''
    df = pd.read_csv(path)
    N = int(len(df.columns) / 4)
    t = df.values[:,0]
    x = df.values[:,range(1, N * 4, 4)]
    y = df.values[:,range(2, N * 4, 4)]
    return t, x, y

def plot_l_point(ax: axes, x: float, y: float, name: str):
    '''Plots Lagrange point on the given matplotlib axes'''
    ax.plot(x, y, 'x', label=name)
    ax.annotate(name, (x,y), textcoords="offset points", xytext=(0, 10), ha='center')

def plot_important_points(ax: axes):
    '''Plots important points which should be present on every plot with Lagrange simulation'''
    ax.scatter(-d, 0, s=120, label="Earth")
    ax.scatter(D -d, 0, s=100, label="Moon")
    plot_l_point(ax, MOON_A - L12_DST, 0, 'L1')
    plot_l_point(ax, MOON_A + L12_DST, 0, "L2")
    plot_l_point(ax, L3_X, 0, "L3")
    plot_l_point(ax, L4_X, L4_Y, "L4")
    plot_l_point(ax, L4_X, -L4_Y, "L5")

def animate_base(i, t):
    '''Function that should be run in every animation loop. Currently adds title to the plot with current time'''
    if t[i] < 3600 * 24:
        titleStr = f"{t[i] / 3600:.2f} hours"
    elif t[i] < 3600 * 24 * 30:
        titleStr = f"{t[i] / 3600 / 24:.2f} days"
    elif t[i] < 3600 * 24 * 365.256:
        titleStr = f"{t[i] / 3600 / 24 / 30:.2f} months"
    else:
        titleStr = f"{t[i] / 3600 / 24 / 365.256:.2f} years"

    plt.suptitle(titleStr)

def anim_save_callback(current_frame: int, total_frames: int):
    print(current_frame / total_frames * 100, "%")

def render_plt_video(args, ani):
    '''Renders animation to an mp4 video to the file defined in args (output of ArgumentParser) using "--save" option'''
    video_output_filepath = args.save
    video_fps = int(args.vid_fps)
    FFwriter = animation.FFMpegWriter(fps=video_fps)
    ani.save(video_output_filepath, writer=FFwriter, dpi=150, progress_callback=anim_save_callback)