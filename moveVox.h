#define MOVEVOX_H
#include<iostream>
using std::cout;using std::endl;
#include"cnpy.h"
#include<random>
//using std::default_random_engine; using std::uniform_real_distribution;
//using std::mt19937;
#include<ctime>
#include<chrono>
//using std::chrono::milliseconds;
//using std::chrono::duration;
#include<complex>
using std::vector;using std::complex;
#include<thread>


void moveVox(vector<double> orig,int thread_id,  vector<double>& new_vec, size_t nz, size_t nr, size_t nc, double z, double r, double c, double newZ, double newR, double newC,int n_thread);

