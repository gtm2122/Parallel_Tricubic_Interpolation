#include<iostream>
using std::cout;using std::endl;
#include"cnpy.h"
#include<random>
using std::default_random_engine; using std::uniform_real_distribution;
using std::mt19937;
#include<ctime>
#include<chrono>
#include<complex>
using std::vector;using std::complex;
int main(){
 cnpy::NpyArray arr = cnpy::npy_load("a.npy");  
 
 complex<double>* loaded_arr = arr.data<std::complex<double>>();

 cout<<arr.shape[0]<<endl;
 cout<<arr.shape[1]<<endl;
 cout<<arr.shape[2]<<endl;
 //cout<<arr.shape;

 double z,r,c;
 unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
 default_random_engine drei(seed1);
 //mt19937 generator(seed1);
 
 uniform_real_distribution<float> dist(1,2);
 z = 1;//dist(drei);
 r = 1;//dist(drei);
 c = 2;//dist(drei);
 cout<<z<<endl<<r<<endl<<c<<endl;

 int Z_new = z*arr.shape[0];
 int R_new = r*arr.shape[1];
 int C_new = c*arr.shape[2];
 cout<<"new dim = "<<Z_new<<","<<R_new<<","<<C_new<<endl;
 vector<complex<double>> new_arr(Z_new*R_new*C_new);

// make new array
 
 for (int i = 0; i < arr.shape[0]; ++i)
  for (int j = 0; j < arr.shape[1]; ++j)
   for (int k = 0; k < arr.shape[2]; ++k){
    new_arr[arr.shape[0]*int(i*z) + arr.shape[1]*int(j*r) + int(k*c)] = loaded_arr[i*arr.shape[0] + j*arr.shape[1] + k] ;
   }

 for (int i = 0 ; i< Z_new*R_new*C_new; ++i){
  cout<<new_arr[i];
 }
 return 0;
}
