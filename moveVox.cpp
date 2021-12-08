#include"moveVox.h"

void moveVox(vector<double> orig,int thread_id,vector<double>& new_vec,vector<double>& pos_vec, size_t nz, size_t nr, size_t nc, double z, double r, double c, double newZ, double newR, double newC,int n_thread){
  int start = int(thread_id*nz*nr*nc/n_thread);
  int end = std::min((thread_id+1)*(nz*nr*nc)/n_thread,nz*nr*nc);
  end = int(end);
  //cout<<thread_id<<endl;
  int z_index;
  int r_index;
  int c_index;
  for (int ind = start; ind < end; ind++){
   //cout<<"start "<<start;
   c_index = ind%nc; 
   r_index = ((ind-c_index)/nc)%nr;
   z_index = (((ind-c_index)/nc)-r_index)/nr;
   
   new_vec[int((int(z_index*newR*z) + int(r_index*r))*newC) + int(c_index*c)] = orig[int((int(z_index*nr) + r_index)*nc + c_index)];
    
   pos_vec[int((int(z_index*nr) + r_index)*nc + c_index)]= int((int(z_index*newR*z) + int(r_index*r))*newC) + int(c_index*c);  

  }
}

void zrc_pos(vector<double> &z_pos, vector<double> &r_pos, vector<double> &c_pos, size_t nz, size_t nr, size_t nc,double z_mul, double r_mul, double c_mul){
 double newR = nr*r_mul;
 double newC = nc*c_mul;
 double newZ = nz*z_mul;

 for(int r = 0 ; r< nr; ++r)
  for(int c = 0 ; c< nc; ++c)
   for(int z = 0 ; z< nz ; ++z)
    z_pos[(r*nc+c)*nz+z] = int((int(z*newR*z_mul) + int(r*r_mul))*newC) + int(c*c_mul);

 for(int z = 0 ; z< nz; ++z)
  for(int c = 0 ; c< nc; ++c)
   for(int r = 0 ; r< nr ; ++r)
    r_pos[(z*nc+c)*nr + r] = int((int(z*newR*z_mul) + int(r*r_mul))*newC) + int(c*c_mul);


 for(int r = 0 ; r< nr; ++r)
  for(int z = 0 ; z< nz; ++z)
   for(int c = 0 ; c< nc ; ++c)
    c_pos[(r*nz+z)*nc + c] = int((int(z*newR*z_mul) + int(r*r_mul))*newC) + int(c*c_mul);

}
