#include <vector>
#include "metrics.h"
#include "KDTree.hpp"
#include "omp.h"

class rangeSearcherNaive
{
  public:
    template<typename t>
    void rangeSearch(std::vector<std::vector<t> >& points,
      std::vector<std::vector<size_t> >& adjPoints,
      t eps)
    {
      #pragma omp parallel for 
      for(int i=0;i<points.size();i++) {
        if (i % 1000 == 0)
          std::cout<<"** "<<i<<" ";
        for(int j=0;j<points.size();j++) {
          if (i == j) continue;
          if(l2_norm(points[i], points[j]) <= eps) {
            adjPoints[i].push_back(j);
            // adjPoints[j].push_back(i);
          }
        }
        // std::cout<<adjPoints[i].size()<<std::endl;
      }
    }
};


class rangeSearcherKDTree
{
  public:
    template<typename t>
    void rangeSearch(std::vector<std::vector<t> >& points,
      std::vector<std::vector<size_t> >& adjPoints,
      t eps)
    {
      #pragma omp parallel 
      {
        #pragma omp single
        {
          KDTree tree(points);
          
          #pragma omp parallel for 
          for(int i=0; i < points.size(); i++)
            {
              if (i % 1000 == 0)
                std::cout<<"** "<<i<<" ";
              adjPoints[i] = tree.neighborhood_indices(points[i], eps);
              // std::cout<<adjPoints[i].size()<<std::endl;
            }
        }
      }
    }
};