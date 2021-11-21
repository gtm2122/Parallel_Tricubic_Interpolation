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
 for (int i = 0 ; i < 10*10*10; ++i) 
  
   cout<<3.0*loaded_arr[i]; 

 
 double z,r,c;
 unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
 default_random_engine drei(seed1);
 //mt19937 generator(seed1);
 
 uniform_real_distribution<float> dist(0.1,1.5);
 z = dist(drei);
 r = dist(drei);
 c = dist(drei);
 //cout<<"zrc"<<endl;
 //cout<<z<<endl<<r<<endl<<c<<endl;

 int Z_new = z*arr.shape[0];
 int R_new = r*arr.shape[1];
 int C_new = c*arr.shape[2];
 //cout<<"new dim = "<<Z_new<<","<<R_new<<","<<C_new<<endl;
 
 // make new array
 

 return 0;
}
