import numpy as np
import random
import time
from sklearn.cluster import DBSCAN

repeat = 10
with open("MINST.txt", "r") as f:
  data = f.readlines()[1:]
  points = []
  for line in data:
    for r in range(repeat):
      pt = [int(x) / 255.0 + random.random() for x in line.split()]
      points.append(pt)
  points = np.array(points)

X = points 
start = time.time()
db = DBSCAN(eps=0.1, min_samples=5, n_jobs=-1, leaf_size=1)
print(db)
db.fit(X)
end = time.time()
print((end - start))

