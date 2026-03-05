# Q3 Waypoint Generation - Development Notes

## V1 Status (Known Problems)
This is **V1** and the result is currently **terrible / not production-ready**.

Main reasons:
- The code cannot reliably find the car's starting position on the centerline.
- The code can break or produce incorrect waypoint ordering when two track sections are too close together (for example, near parallel segments or overlapping loops in local distance).

These issues are currently unresolved in V1 and should be treated as known limitations.

## Summary
Implemented `Q3/Question-3.cc` to:
- Read an unordered cone CSV (`x,y`).
- Infer centerline waypoint candidates geometrically.
- Order and smooth the centerline into a closed loop.
- Resample to enforce `<= 0.5 m` spacing.
- Write waypoints to output CSV with the same `x,y` format.

## Hurdles Encountered

1. `Question-3.cc` started as an empty/stub file
- Hurdle: There was no runnable implementation logic to extend.
- Action: Rebuilt the solution from scratch with full CSV parsing, geometry utilities, candidate generation, ordering, and output.

2. Midline extraction without cone labels (inner vs outer unknown)
- Hurdle: The input is unordered and does not identify boundary membership.
- Action: Estimated local tangents for each cone using neighborhood covariance, then selected cone pairs likely across track width using orientation checks:
  - tangent parallelism between paired cones,
  - across-track vector approximately perpendicular to both tangents,
  - plausible separation bounds.
- Result: Produced strong midpoint candidates without explicit boundary classification.

3. Large number of noisy/outlier midpoint candidates
- Hurdle: Raw pairing creates many false midpoint candidates, especially on larger tracks.
- Action:
  - Grid-based deduplication (keep best-score candidate per spatial cell),
  - local density filtering,
  - score sorting and capped retention based on cone count.
- Result: Candidate cloud became tractable and better aligned to track center.

4. Ordering unordered midpoint cloud into a continuous loop
- Hurdle: Midpoints are still unordered; naive sorting can create self-crossing paths.
- Action: Implemented a greedy geometric walker using:
  - local nearest-neighbor graph,
  - turn-minimizing cost,
  - explicit segment intersection avoidance,
  - closure detection.
- Fallback: nearest-neighbor tour + 2-opt untangling when greedy closure is weak.

5. Unstable path quality on complex track (`track3.csv`)
- Hurdle: Early versions produced overly long zig-zag loops (too many output waypoints after resampling).
- Action:
  - tightened midpoint filtering,
  - added path plausibility checks,
  - added cyclic smoothing and decimation before final resampling.
- Result: Output became significantly more stable and compact.

6. CMake Tools build configuration unavailable in environment
- Hurdle: `Build_CMakeTools` failed with configure error in this VS Code session.
- Action: Verified correctness by compiling `Q3/Question-3.cc` directly with `g++` and running on all supplied tracks.
- Result: End-to-end generation succeeded for all three provided track CSV files.

## Validation Runs Performed
Compiled and ran:
- `Q3/track1.csv -> Q3/waypoints_out_track1.csv`
- `Q3/track2.csv -> Q3/waypoints_out_track2.csv`
- `Q3/track3.csv -> Q3/waypoints_out_track3.csv`

Latest run produced:
- `track1`: 121 waypoints
- `track2`: 313 waypoints
- `track3`: 896 waypoints

## Remaining Tradeoffs
- The approach is fully geometric and avoids external libraries, but it is heuristic rather than exact Voronoi/Delaunay centerline extraction.
- On unusually complex or degenerate cone layouts, fallback ordering may still produce suboptimal smoothness compared to full computational-geometry solutions.
