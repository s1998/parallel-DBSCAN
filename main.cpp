#include <iostream>
#include "src/dbscan.h"
#include <string>
#include <fstream>
#include <sstream>
#include "omp.h"
#include <chrono>
#include <stdlib.h>

using namespace std;
using namespace std::chrono;

void readFile(string fname, vector<vector<float> >& points,
 int repeat)
{
    ifstream file(fname);
    string str;
    float number;
    int start_lines = 1;
    vector<vector<float> > tempPoints;
    while (std::getline(file, str))
    {
        if (start_lines)
        {
          start_lines--;
          continue;
        }
        stringstream iss(str);
        vector<float> point;
        while ( iss >> number ) 
          point.push_back( number / 255.0);
        tempPoints.push_back(point);
    }

    points.resize(tempPoints.size() * repeat);
    for(int i = 0; i < repeat; i++)
      for(int j = 0; j < 10000; j++)
        for(int k = 0; k < tempPoints[0].size(); k++)
          points[i * tempPoints.size() + j].push_back(
            tempPoints[j][k] + (rand() % 100) / 100.0);
}

template <typename t>
void timeit(t& clusterer, 
  vector<vector<float> >& points, 
  string name)
{
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  vector<int> clusterId;
  clusterer.cluster(points, clusterId);
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>( t2 - t1 ).count()/1000000.0;
  cout <<"\n Time taken by clustering policy "<<name<<" : "<< duration<<endl;
}

int main(int argc, const char** argv)
{

  int repeat = 50;
  cout<<"No of processors : "<<omp_get_num_procs()<<"\n";
  omp_set_nested(true);
  vector<vector<float> > points;

  readFile("MINST.txt", points, repeat);
  cout<<endl<<"No of points in data : "<<points.size()<<endl;
  
  float eps = 0.1;
  int minPoints = 5;
  dbscan::DBCAN<rangeSearcherKDTree> clusterer1(minPoints, eps);
  dbscan::DBCAN<> clusterer2(minPoints, eps);
  
  for(int i=4; i < 65; i *= 2)
  {
    cout<<"\nMaximum number of threads used : "<<i<<endl;
    omp_set_num_threads(i); 
    timeit(clusterer1, points, "KDTree Range search parallel");
    // timeit(clusterer2, points, "Naive Range search parallel");
  }
  omp_set_num_threads(1); 
  timeit(clusterer1, points, "KDTree Range search serial");
  // timeit(clusterer2, points, "Naive Range search serial");

}
