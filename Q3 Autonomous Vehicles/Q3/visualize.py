import sys
import csv
import matplotlib.pyplot as plt


def read_csv(path):
    points = []
    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            x = row.get("x") or row.get("X")
            y = row.get("y") or row.get("Y")
            if x is not None and y is not None and x.strip() != "" and y.strip() != "":
                points.append((float(x), float(y)))
    return points


def main():
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print("Usage: python visualize.py track.csv [waypoints.csv]")
        sys.exit(1)

    track_path = sys.argv[1]
    waypoints_path = sys.argv[2] if len(sys.argv) == 3 else None

    cones = read_csv(track_path)
    if not cones:
        print(f"Error: no cone data found in '{track_path}'")
        sys.exit(1)

    cx, cy = zip(*cones)

    fig, ax = plt.subplots(figsize=(10, 10))

    ax.scatter(cx, cy, c="orange", s=50, zorder=3, label="Cones")

    # Mark the origin (car start position)
    ax.scatter(0, 0, c="blue", s=100, marker="^", zorder=4, label="Car (origin)")

    if waypoints_path:
        waypoints = read_csv(waypoints_path)
        if not waypoints:
            print(f"Warning: no waypoint data found in '{waypoints_path}'")
        else:
            wx, wy = zip(*waypoints)
            ax.plot(wx, wy, c="green", linewidth=1.5, zorder=2, label="Midline waypoints")
            ax.scatter(wx, wy, c="green", s=20, zorder=3)

    ax.set_aspect("equal")
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.set_xlabel("X (m)")
    ax.set_ylabel("Y (m)")
    ax.set_title("Track visualisation")
    ax.legend()

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()
