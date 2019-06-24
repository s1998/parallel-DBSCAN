# parallel-DBSCAN

A parallel implementaion of DBSCAN using c++. The code has been written in c++ using policy based design and uses OpenMP.

## Instructions to use

Firstly clone the repsitory using:
```
git clone https://github.com/s1998/parallel-DBSCAN
```

To compile, use the following command
```
g++ --std=c++11 -O3 -fopenmp ./main.cpp -o main.out
```

### Prerequisites

Make sure you have c++11 installed to get better performance. Also, dont foret to turn on the optimization flag -O3 while compiling for fast results.

### Sample usage

The current implementaion supports neighborhood query using naive search and using KD tree.

To use the KD tree based neighborhood query, do:
```
dbscan::DBCAN<rangeSearcherKDTree> clusterer1(minPoints, eps);
vector<int> clusterId;
vector<vector<t> > points;
clusterer.cluster(points, clusterId);
```

To use the naive range search, replace the first line with:
```
dbscan::DBCAN<> clusterer(minPoints, eps);
```
Cureently supports l2 metric. WIP to enable custom metrics.

## Performance details

Obtained upto 13.5x speed gain from parallel implementation using a 36 core machine. The implementation performs competitively (w.r.t time taken) with scikit's DBSCAN implementation and uses lesser memory (subject to setting same options).

The experiments where run using c5.9xlarge on AWS which has 2nd generation Xeon processor (3.4GHz).

### Choosing the range search policy

The amount of time taken by the algorithm depends on the dataset and the hyperparameters used. 
The bottleneck of the entire algorithm is the neighborhood query time. Also, a single point (say x) neighborhood query time would rely on the amount of depth it needs to explore in the KD tree. If the data is sparse and the constraint: |x<sub>j</sub> - y<sub>j</sub>| > eps (where y denotes the point at current node of the tree) is violated too often, it would be better to use naive search or some other heuristic designed for DBSCAN.

