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
 
 
 cnpy::npy_save("img_cpp.npy", &vec3d[0], {nz,nr,nc},"w");
 return 0;
 /*z = 1;//dist(drei);
 r = 1;//dist(drei);
 c = 2;//dist(drei);
 

 size_t Z_new = z*nz;
 size_t R_new = r*nr;
 ize_t C_new = c*nc;
 
 vec3d.reserve(nz);
 for (size_t z_index = 0; z_index < nz; ++z_index){
  vec3d.emplace_back(nr);
  for (size_t r_index = 0; r_index < nr; ++r_index){
   vec3d[z_index].emplace_back(nc);
   for (size_t c_index = 0; c_index < nc; ++c_index){
    }
  }
 }
 

 cout<<"new dim = "<<Z_new<<","<<R_new<<","<<C_new<<endl;
 std::vector<std::vector<double> new_arr(Z_new*R_new*C_new);
 */
// make new array
 /*
 for (int i = 0; i < arr.shape[0]; ++i)
  for (int j = 0; j < arr.shape[1]; ++j)
   for (int k = 0; k < arr.shape[2]; ++k){
    new_arr[Z_new*int(i*z) + R_new*int(j*r) + int(k*c)] = loaded_arr[i*arr.shape[0] + j*arr.shape[1] + k] ;
   }
*
 for (int i = 0 ; i< Z_new*R_new*C_new; ++i){
  cout<<new_arr[i];
 }

 cnpy::npy_save("img_cpp.npy",&new_arr,{Z_new,R_new,C_new},"w");
*/
 return 0;
}
