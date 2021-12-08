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

void get_A_y(vector <double> &vec1d, vector<double> &y, vector<double> pos){
 
 size_t a = pos.shape[0];
 A[0] = 2;
 A[(a)*(a)-1] = 2;
 A[(a-2)*a+(a-1)] = 1;
 A[(a-2)*a+a-2]=1;
 
 for(int r = 1 ; r< a-1; ++r){
   A[a*(r-1)+r] = 1;
   A[a*(r)+(r-1)]=1;
   A[r*a+r] = 4;
   
  }

 for(int r =0; r< a-1;++r){
  y[r] = 3*(vec1d[pos[r+1]] - vec1d[pos[r]])/(pos[1]-pos[0]);
 }
 
}


void cubicInt(vector <double> vec1d,vector <double> posi){
 // FIGURE OUT HOW TO TO SOLVE THE TRIDIAGONAL MATRIX AFTER OBTAINING IT
  }

int main(){
 cnpy::NpyArray arr = cnpy::npy_load("vec_img.npy");
 double *vec = arr.data<double>();
 cout<<arr.shape[0]<<endl;

 cnpy::NpyArray row_pos = cnpy::npy_load("coord_r0.npy");
 cout<<row_pos.shape[0]<<endl;
 
 return 0;
}
