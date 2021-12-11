#include "cnpy.h"
#include<iostream>
using std::cout; using std::endl;
#include<complex>
using std::vector; using std::complex;
#include<ctime>
#include<chrono>
using std::chrono::milliseconds;
using std::chrono::duration;
// based on what is outlined in https://observablehq.com/@jrus/cubic-spline



void get_A_y(vector <double> vec1d, vector<double> pos, vector<double> &A, vector<double> &y){
 
 size_t a = pos.size()-1;
 cout<<a;
 
 //vector<double> A(a*a);
 //vector<double> y(a);
 A[0] = 2;
 A[(a-1)*(a)+(a-1)] = 2;
 A[(a-2)*a+(a-1)] = 1;
 A[(a-1)*a+a-2]=1;
 
 for(int r = 1 ; r< a-1; ++r){
   A[a*(r-1)+r] = 1;
   A[a*(r)+(r-1)]=1;
   A[r*a+r] = 4;
   
  }

 for(int r =0; r< a;++r){
  y[r] = 3*(vec1d[pos[r+1]] - vec1d[pos[r]])/(pos[1]-pos[0]);
 }
/* 
 cnpy::npy_save("A_mat.npy", &A[0], {a,a},"w");
 cnpy::npy_save("y_vec.npy", &y[0], {a},"w");
*/ 
}

void cubicInt(vector <double> vec1d,vector <double> pos){
 // FIGURE OUT HOW TO TO SOLVE THE TRIDIAGONAL MATRIX AFTER OBTAINING IT
 size_t a = pos.size()-1;
 vector<double> A(a*a);
 vector<double> y(a);
 get_A_y(vec1d,pos,A,y);
  
  }

int main(){
 cnpy::NpyArray arr = cnpy::npy_load("vec_img.npy");
 double *arr_p = arr.data<double>();
 size_t arr_p_shape = arr.shape[0];

 cnpy::NpyArray row_pos = cnpy::npy_load("coord_r0.npy");
 size_t row_pos_shape = row_pos.shape[0]; 
 double *row_pos_p = row_pos.data<double>();
 
 vector<double> arr_vec(arr_p_shape);
 vector<double> row_pos_vec(row_pos_shape);
 for (size_t c_index = 0; c_index<arr_p_shape; ++c_index)
  {
   arr_vec[c_index] = arr_p[c_index];
 }

 for (size_t c_index = 0; c_index<row_pos_shape; ++c_index)
{
  row_pos_vec[c_index] = row_pos_p[c_index];
 }
 // always convert cnpy::NpyArray to std::vector<double>

 
 cubicInt(arr_vec,row_pos_vec); 
 //get_A_y(arr_vec, row_pos_vec);  
 return 0;
}
