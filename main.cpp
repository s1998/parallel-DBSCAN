#include <iostream>
#include "src/dbscan.h"
#include <string>
#include <fstream>
#include <sstream>
#include "omp.h"
#include "argparse/argparse.hpp"
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
          point.push_back( number / 255.0 + (rand() % 10) / 10.0);
        points.push_back(point);
    }
    for(int i = 0; i < repeat; i++)
      for(int j = 0; j < 10000; j++)
        points.push_back(points[j]);
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
  cout <<"Time taken by clustering policy "<<name<<" : "<< duration<<endl;
}

int main(int argc, const char** argv)
{
  // ArgumentParser parser;
  // parser.addArgument("-r", "--repeat", 0);
  // // parser.addArgument("-f", "--fileName", "MINST.txt");
  // parser.parse(argc, argv);

  // // if we get here, the configuration is valid
  // int repeat = parser.retrieve<int>("repeat");
  // string fname = parser.retrieve<string>("fileName");
  
  int repeat = 0;
  cout<<"No of processors : "<<omp_get_num_procs()<<"\n";
  omp_set_nested(true);
  vector<vector<float> > points;

  readFile("MINST.txt", points, repeat);
  cout<<endl<<"No of points in data : "<<points.size()<<endl;
  
  float eps = 0.1;
  int minPoints = 5;
  dbscan::DBCAN<rangeSearcherKDTree> clusterer1(minPoints, eps);
  dbscan::DBCAN<> clusterer2(minPoints, eps);
  
  omp_set_num_threads(16); 
  timeit(clusterer1, points, "KDTree Range search parallel");
  timeit(clusterer2, points, "Naive Range search parallel");
  // omp_set_num_threads(1); 
  // timeit(clusterer1, points, "KDTree Range search serial");
  // timeit(clusterer2, points, "Naive Range search serial");

}
