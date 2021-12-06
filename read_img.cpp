#include<iostream>
using std::cout;using std::endl;
#include"cnpy.h"
#include<random>
using std::default_random_engine; using std::uniform_real_distribution;
using std::mt19937;
#include<ctime>
#include<chrono>
using std::chrono::milliseconds;
using std::chrono::duration;
#include<complex>
using std::vector;using std::complex;
#include<thread>
#include "moveVox.h"
//using std::moveVox;
int main(){
 cnpy::NpyArray arr = cnpy::npy_load("four.npy");  
 
 double *loaded_data = arr.data<double>();
 
 size_t nz = arr.shape[0];
 size_t nr = arr.shape[1];
 size_t nc = arr.shape[2];

 std::vector<double> vec3d(nz*nr*nc);
 
 for (size_t z_index = 0; z_index< nz; ++z_index){
  for(size_t r_index = 0;r_index<nr; ++r_index){
   for(size_t c_index=0; c_index<nc ; ++c_index){
     vec3d[(z_index*nr + r_index)*nc + c_index] = loaded_data[(z_index*nr + r_index)*nc + c_index];
    }
  }
 }
  
 // voxelTeleportation

 double z = 1;
 double r = 0.5;
 double c = 0.5;
 size_t newZ = arr.shape[0]*z;
 size_t newR = arr.shape[1]*r;
 size_t newC = arr.shape[2]*c;
 cout<<" "<<newZ<<" "<<newR<<" "<<newC<<endl;
 std::vector<double> vec3d_new(newZ*newR* newC);
 int n_thread=10; 
 std::thread thrd[n_thread];
 for(int i = 0; i<n_thread; ++i){ 
    
  thrd[i] = std::thread(moveVox,vec3d,i,std::ref(vec3d_new),nz,nr,nc,z,r,c,newZ,newR,newC,n_thread); 
  
 }

 for(int i = 0; i<n_thread; ++i){ 
  if (thrd[i].joinable())
   thrd[i].join(); 
   //cout<<"joined"<<i<<endl; 
 }
 cout<<vec3d_new.size();
 //cout<<vec3d_new[0].size();
 
 //cout<<vec3d_new[0][0].size();
 cout<<endl;
 // saving enlarged image waiting to be interpolated 
 cnpy::npy_save("img_cpp_new.npy", &vec3d_new[0], {newZ,newR,newC},"w");
 
 return 0;
}
