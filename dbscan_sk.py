import numpy as np
from sklearn.cluster import DBSCAN

with open("MINST.txt", "r") as f:
  data = f.readlines()[1:]
  points = []
  for line in data:
    pt = [int(x) / 255.0 for x in line.split()]
    points.append(pt)
  points = np.array(points)

X = points 
db = DBSCAN(eps=5.0, min_samples=5, n_jobs=-1)
print(db)
db.fit(X)

