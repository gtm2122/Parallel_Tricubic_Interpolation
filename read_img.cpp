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

void moveVox(vector<double> orig,int threadid,  vector<double>& new_vec, size_t nz, size_t nr, size_t nc, double z, double r, double c, double newZ, double newR, double newC){
  int start ;
  int end;
  cout<<thread_id<<endl;
  int z_index;
  int r_index;
  int c_index;

  for (int ind = start; ind < end; ind++){
   c_index = ind%nc; 
   r_index = ((ind-c_index)/nc)%nr;
   z_index = ((((ind-c_index)/nc)-r_index)/nr;
   new_vec[(z_index*newR*z + r_index*r)*newC + c_index*c] = orig[(z_index*nr + r_index)*nc + c_index];
//TODO EVALUATE ABOVE, SEE IF IT WORKS 
  }
/*
  for(size_t z_index = 0; z_index<nz; ++z_index){
   for(size_t r_index = 0; r_index<nr; ++r_index){
    for(size_t c_index = 0; c_index<nc; ++c_index){
     new_vec[(z_index*newR*z + r_index*r)*newC + c_index*c] = orig[(z_index*nr + r_index)*nc + c_index];
    }
   }
  }
*/
}

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

 double z = 2;
 double r = 2;
 double c = 2;
 size_t newZ = arr.shape[0]*z;
 size_t newR = arr.shape[1]*r;
 size_t newC = arr.shape[2]*c;

 std::vector<double> vec3d_new(newZ*newR* newC);
 int n_thread=4; 
 std::thread thrd[n_thread];

 for(int i = 0; i<n_thread; ++i){ 
    
  thrd[i] = std::thread(moveVox,vec3d,i,std::ref(vec3d_new),nz,nr,nc,z,r,c,newZ,newR,newC); 
  
 }

 for(int i = 0; i<n_thread; ++i){ 
  if (thrd[i].joinable())
   thrd[i].join(); 
  
 }




 cnpy::npy_save("img_cpp_new.npy", &vec3d_new[0], {newZ,newR,newC},"w");
 
 return 0;
}
