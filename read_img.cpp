#include<iostream>
using std::cout;using std::endl;
#include"cnpy.h"


int main(){
 cnpy::NpyArray arr = cnpy::npy_load("a.npy");  
  
 cout<<arr.shape[0];
 return 0;
}
