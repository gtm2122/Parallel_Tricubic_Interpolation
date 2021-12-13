#include "cnpy.h"
#include<iostream>
using std::cout; using std::endl;
#include<complex>
using std::vector; using std::complex;
#include<ctime>
#include<chrono>
using std::chrono::milliseconds;
using std::chrono::duration;
//#include "cubic_int.cpp"
//using std::cubicInt_serial;
#include "moveVox.h"
void getSpline(int pos0,int pos1, double slope0, double slope1, double y0, double y1, vector<double> &vec,double scale){
 
 double m1 = slope1*(pos1-pos0);
 double m0 = slope0*(pos1-pos0);
 double c0 = 0.5*(y0 + y1 + 0.125*(m0-m1));
 double c1 = 0.03125*(18*(y1-y0)-m0-m1);
 double c2 = 0.0625*(m1-m0);
 double c3 = 0.03125*(2*(y0-y1)+m0+m1);
 double b1,b2,u;
 pos0=double(pos0);
 pos1=double(pos1);
 //cout<<pos0<<" "<<pos1<<endl;
 for (double i = pos0+1; i<pos1; ++i){
  u = -1 + 2*(i-pos0)/(pos1-pos0);
  //cout<<u<<endl;
  //u = scale*(2*u - pos1 - pos0);
  b1 = c3;
  b2 = c2+2*u*b1;
  b1 = c1+2*u*b2 - b1;
  
  vec[i] = int(c0 + u*b1 - b2); 
  //cout<<vec[i]<<endl;
 }
}

void cubicInt_serial(vector <double> &vec1d,vector <double> pos){
 size_t n = pos.size()-1;
 
 vector<double> t(n+3);
 vector<double> y(n+3);
 vector<double> s(n+2);
 vector<double> m(n+3);
 vector<double> d(n+2);
 
 std::fill(t.begin(),t.end(),0);
 std::fill(y.begin(),y.end(),0);
 std::fill(s.begin(),s.end(),0);
 std::fill(m.begin(),m.end(),0);
 std::fill(d.begin(),d.end(),0);
 
 for(int i = 1; i<pos.size();i++){
 t[i] = i-1;  
 y[i] = vec1d[pos[i-1]];
 }
 
 for(int i = 1; i<n+1;++i){
  s[i] = 1/(t[i+1]-t[i]);
  m[i+1] += 3*s[i]*s[i]*(y[i+1]-y[i]);
  m[i] += m[i+1];
 }

 d[1]=0.5/s[1];
 m[1]=d[1]*m[1];
 
 for(int i = 2;i<n+1;++i){
  d[i] = 1/(2*(s[i] + s[i-1]) - s[i-1]*s[i-1]*d[i-1]);
  m[i] = d[i]*(m[i] - s[i-1]*m[i-1]);
 }
 
 for(int i =n; i>0; --i){
   m[i] -= d[i]*s[i]*m[i+1];
 }

 t[0] = t[1] - 1;
 t[n+2] = t[n+1] +1;
 y[0] = y[1]-m[1];
 y[n+2] = y[n+1]+m[n+1];
 s[0] = s[n+1] = 1;
 m[0] = m[1];
 m[n+2] = m[n+1];

 // after obtaining solution, apply the hermite bases to get value at those points
 double y0,y1,m0,m1,h,u;
 int pos1,pos2;
 for (int i = 0 ; i < n+2; ++i){
    h = t[i+1] - t[i];
    y0 = y[i];
    y1 = y[i+1];
    m0 = h*m[i];
    m1 = h*m[i+1];
    
    pos1 = pos[t[i]];
    pos2 = pos[t[i+1]];
    double scale = s[i];
    getSpline(pos1,pos2,m0,m1,y0,y1,vec1d,scale);

 }
 
 //cnpy::npy_save("interp_vec.npy", &vec1d[0], {vec1d.size()},"w");
   
}

// do 1 D interpolation on each vector indexed by pos
void interpolate(vector<double> pos, vector<double> &vec, int size1, int size2, int size3, 
int numR, int numC, double mult){
 //cout<<numR<<" "<<numC<<endl; 
 for (int i = 0; i < numR ; i++){
  vector<double> local_pos (numC);
  vector <double> local_vec (int(numC*mult));
  //cout<<i<<endl;
  int start_pos = int(pos[i*numC]);  
  int ind_3 =  start_pos%size3; // C
  int ind_2 = (start_pos- ind_3)/size3 % size1; // R
  int ind_1 = ((start_pos- ind_3)/size3 - ind_2)/size1; // Z

  for(int j = 0 ; j < numC ; ++j){
   //cout<<"j "<<j<<endl;
   //cout<<pos[i*numC+j]<<endl;
   local_pos[j] = pos[i*numC+j] - pos[i*numC];
   cout<<"pos "<<pos[i*numC+j]<<endl;
   cout<<"pos2 "<<local_pos[j]<<endl;
   cout<<"val "<<vec[pos[i*numC+j]]<<endl;
   local_vec[local_pos[j]] = vec[pos[i*numC+j]];
   cout<<"val2 "<<local_vec[local_pos[j]]<<endl;
  }
  cout<<ind_1<<","<<ind_2<<endl;
  //cout<<std::max(pos);
  cubicInt_serial(local_vec,local_pos);
     
  for(int j = 0 ; j < numC*mult ; ++j){
   cout<<local_vec[j]<<endl;
   cout<<(ind_2*size2+ind_1)*size3+j<<endl;
   vec[(ind_2*size2+ind_1)*size3+j] = local_vec[j];
   //local_pos[j] = local_pos[j] + pos[i*numC];
   //cout<<"local pos "<<local_pos[j]<<endl;
   //cout<<local_vec[j]<<endl;
   //vec[local_pos[j]] = local_vec[j];
  }
    
 }

}

int main()
{
// First load 3D image
// compute zrc coord arrays and the enlarged image
// run cubic interpolation using each row for each zrc
// later run above using mpi
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

 double z = 1;
 double r = 2;
 double c = 3;
 size_t newZ = arr.shape[0]*z;
 size_t newR = arr.shape[1]*r;
 size_t newC = arr.shape[2]*c;
 cout<<" "<<newZ<<" "<<newR<<" "<<newC<<endl;
 std::vector<double> vec3d_new(newZ*newR*newC);
 std::vector<double> vec3d_new_pos(nz*nr*nc);
 
 std::vector<double> z_pos(nr*nc*nz);
 std::vector<double> r_pos(nz*nc*nr);
 std::vector<double> c_pos(nz*nr*nc);

 int n_thread=10; 
 std::thread thrd_move[n_thread];

 for(int i = 0; i<n_thread; ++i){ 
  thrd_move[i] = std::thread(moveVox,vec3d,i,std::ref(vec3d_new),std::ref(vec3d_new_pos),nz,nr,nc,z,r,c,newZ,newR,newC,n_thread);   
 }

 for(int i = 0; i<n_thread; ++i){ 
  if (thrd_move[i].joinable())
   thrd_move[i].join(); 
   //cout<<"joined"<<i<<endl; 
 }
 
 zrc_pos(z_pos,r_pos,c_pos,nz,nr,nc,z,r,c);
  
 // after obtaining zrc pos and big image, we now run interpolation for all columns in parallel using threading

 int n_thread = 10;
 
 for(int i = 0 ; i<n_thread;++i){
  int step_size = (nz*nr+(n_thread-nz*nr%n_thread))/n_thread;
  int start_row = i*step_size;
  int end_row = std::min((i+1)*step_size,nz*nr);
  vector<double> c_pos_temp(nc*(end_row-start_row))
  for(int j =start_row; j < end_row; ++j){
   for(int col = 0 ; col < nc; ++col){
    c_pos_temp[(j-end_row)*nc + col] = c_pos[j*nc+col]
   }
  }
  interpolate(c_pos_temp, vec3d_new,newR,newZ, newC, int((end_row-start_row)*nc), nc, c);
 
 }

// interpolate(c_pos, vec3d_new,newR,newZ, newC, int(nz*nr), nc, c);
 //interpolate(r_pos, vec3d_new,int(z*nz),int(c*nc), int(z*nz), int(nc*nz), nr, r);
 cout<<"int done";
 //interpolate(z_pos, vec3d_new,int(r*nr),int(c*nc), int(z*nc), int(nc*nr), nz, z);
 
 cnpy::npy_save("img_cpp_interpc.npy", &vec3d_new[0], {newZ,newR,newC},"w");
 

return 0;
}
