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
// Hermite spline functions to get the cubic polynoimial
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
 for (double i = pos0+1; i<pos1; ++i){
  u = -1 + 2*(i-pos0)/(pos1-pos0);
  cout<<u<<endl;
  //u = scale*(2*u - pos1 - pos0);
  b1 = c3;
  b2 = c2+2*u*b1;
  b1 = c1+2*u*b2 - b1;
  
  vec[i] = int(c0 + u*b1 - b2); 
  cout<<vec[i]<<endl;
 }
}

void cubicInt_serial(vector <double> vec1d,vector <double> pos){
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

 
 cubicInt_serial(arr_vec,row_pos_vec); 
 //get_A_y(arr_vec, row_pos_vec);  
 return 0;
}
