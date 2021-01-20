#include "utils.h"

TString getString(Float_t num, int decimal) {
   // this function allows to convert a float to a string with adjusting the number of decimal

   std::ostringstream streamObj3;

   // Set Fixed-Point Notation
   streamObj3 << std::fixed;

   // Set precision to your chosen number of digits
   streamObj3 << std::setprecision(decimal);

   // Add double to stream
   streamObj3 << num;

   // Get string from output string stream
   std::string strObj3 = streamObj3.str();

   return strObj3.c_str();
}


bool comp(float a, float b){
   return (a < b);
}


