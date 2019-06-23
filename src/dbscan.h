#include <vector>
#include <chrono> 
#include <cmath>
#include <numeric> 
#include "omp.h"
#include "rangeSearch.cpp"

#define NOT_CLASSIFIED -1
#define NOISE 0

namespace dbscan{

  template<typename rangeSearcher = rangeSearcherNaive>
  class DBCAN {
    
    public:
      int minPts;
      float eps;
      std::vector<std::vector<float> > points;
      std::vector<std::vector<size_t> > adjPoints;
      std::vector<bool> visited;
      std::vector<int > clusterId;
      int nClusters = 0;
      rangeSearcher r;
      
      DBCAN(int minPts, float eps) {
          this->eps = eps;
          this->minPts = minPts;
      }

      void cluster(std::vector<std::vector<float> >const& points, std::vector<int>& clusterId)
      {
        this->points = points;
        this->adjPoints.resize(points.size());
        this->clusterId = clusterId;
        this->clusterId.assign(points.size(), NOT_CLASSIFIED);
        this->visited.assign(points.size(), false);
        cluster_();
      }

    private:
      void cluster_() {
        std::cout<<"Running to find adjacent points\n";
        auto start = std::chrono::high_resolution_clock::now(); 
        r.rangeSearch(points, adjPoints, eps);
        auto end1  = std::chrono::high_resolution_clock::now(); 
        for(int i=0;i<points.size();i++) {
          if(clusterId[i] != NOT_CLASSIFIED) continue;
          
          if(isCoreObject(i))
            dfs(i, nClusters++);
          else
            clusterId[i] = NOISE;
          
      }
        auto end2  = std::chrono::high_resolution_clock::now(); 
        auto duration1 = std::chrono::duration_cast<
          std::chrono::microseconds>(end1 - start); 
        auto duration2 = std::chrono::duration_cast<
          std::chrono::microseconds>(end2 - start); 
        // std::cout<<"Time taken in microseconds: "
        //   <<duration1.count()<<" "<<duration2.count()<<"\n";
    }
      
      void dfs (int now, int c) {
          clusterId[now] = c;
          if(!isCoreObject(now)) return;          
          for(auto&next:adjPoints[now]) {
              if(clusterId[next] != NOT_CLASSIFIED) continue;
              dfs(next, c);
          }
      }
      
      // is idx'th point core object?
      bool inline isCoreObject(int idx) {
          return adjPoints[idx].size() >= minPts;
      }      
  };
}
