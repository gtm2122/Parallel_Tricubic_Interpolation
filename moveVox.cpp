#include"moveVox.h"


void moveVox(vector<double> orig,int thread_id,  vector<double>& new_vec, size_t nz, size_t nr, size_t nc, double z, double r, double c, double newZ, double newR, double newC,int n_thread){
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
   /*cout<<"c_index "<<c_index<<endl;
   cout<<"r_index "<<r_index<<endl;
   cout<<"z_index "<<z_index<<endl;
   cout<<"arr index "<<(int((int(z_index*newR*z) + int(r_index*r))*newC) + int(c_index*c))<<endl;
   */
   new_vec[int((int(z_index*newR*z) + int(r_index*r))*newC) + int(c_index*c)] = orig[int((int(z_index*nr) + r_index)*nc + c_index)];
 }

}
