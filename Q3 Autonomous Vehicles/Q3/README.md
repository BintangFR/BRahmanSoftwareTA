This is more of an open ended extension task, so don't worry if you find it difficult. If you do get stuck you may find hints.txt helpful.

## Task: 
    Given a CSV of unordered cone coordinates (metres) local to an imaginary car at the origin, create a new CSV file with mid line waypoints. The coordinates in the CSV make up a track's inner and outer boundaries. The output file should have the same format as the provided track CSVs.

    Your implementation should be completed in C++ in Question-3.cc
## Constraints:
-   25 < number of cones < 300
-   maximum distance between consecutive cones = 5m
-   minimum track width = 3m
-   minimum turning diameter = 9m
-   maximum distance between waypoints = 0.5m
## Testing:
-   To help visualize the both the track and your waypoints, visualize.py has been provided.
-   ### example usage:
    -   python visualize.py track.csv
    -   python visualize.py track.csv waypoints.csv

-   *assuming you have python and matplotlib installed

-   3 track CSV files have also been provided to help with testing.
