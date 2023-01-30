from argparse import ArgumentParser

import numpy as np
from common import read_sat, plot_important_points, animate_base, render_plt_video
from matplotlib import animation
from matplotlib import pyplot as plt

if __name__ == "__main__":
    arg_parser = ArgumentParser(
        prog="sim-comparator",
        description="Compares two or more simulations with identical save interval. Useful for comparing different timesteps"
    )
    arg_parser.add_argument("inputFiles", nargs="+")
    arg_parser.add_argument("--save")
    arg_parser.add_argument("--vid-fps", default="30")
    arg_parser.add_argument("--vid-dur", default="60")
    arg_parser.add_argument("--x-range", default="-6e8;6e8")
    arg_parser.add_argument("--y-range", default="-6e8;6e8")
    arg_parser.add_argument("-s", "--point-size", default="30")
    arg_parser.add_argument("--labels", default=[], nargs="*")

    args = arg_parser.parse_args()
    print("Arguments:")
    print(args)
    vid_dur = float(args.vid_dur)
    vid_fps = float(args.vid_fps)
    filepaths = args.inputFiles

    ts = []
    xs = []
    ys = []
    for filepath in filepaths:
        print(f"Loading file {filepath}")
        t, x, y = read_sat(filepath)
        if not len(ts) == 0 and (len(ts[0]) != len(t)):
            raise RuntimeError(f"Compared file has different number of save times: {filepath} has {len(t)}, original has {ts[0]}.")
        ts.append(t)
        xs.append(x)
        ys.append(y)
    
    # Plot setup
    plt_xrange_from = float(args.x_range.split(";")[0])
    plt_xrange_to = float(args.x_range.split(";")[1])
    plt_yrange_from = float(args.y_range.split(";")[0])
    plt_yrange_to = float(args.y_range.split(";")[1])
    plt.xlim(plt_xrange_from, plt_xrange_to)
    plt.ylim(plt_yrange_from, plt_yrange_to)
    ax = plt.gca()
    ax.set_aspect(1)
    plt.gcf().set_size_inches(15, 10)
    plot_important_points(ax)
    
    scatters = []
    point_size = int(args.point_size)
    for t, x, y, i in zip(ts, xs, ys, range(len(ts))):
        label = None if len(args.labels) != len(ts) else args.labels[i]
        scatters.append(ax.scatter(x, y, s=point_size, alpha=.5, label=label))
    
    def animate(frame_i):
        i = int(frame_i / vid_dur / vid_fps * len(ts[0]))
        for x, y, scatter in zip(xs, ys, scatters):
            data = np.transpose(np.array((x[i,:], y[i,:]), dtype=np.float64))
            scatter.set_offsets(data)

        animate_base(i, ts[0])
        return scatters
    
    def anim_save_callback(current_frame: int, total_frames: int):
        print(current_frame / total_frames * 100, "%")
    
    plt.legend(loc="upper right")
    ani = animation.FuncAnimation(plt.gcf(), animate, interval=(1000 / vid_fps), frames=range(int(vid_dur * vid_fps)), repeat=True)
    if not args.save is None:
        render_plt_video(args, ani)
    
    plt.show()
