/*
 * This file (syshelpers.cpp) is part of h4r_ev3_ctrl.
 * Date: 22.11.2015
 *
 * Author: Christian Holl
 * http://github.com/Hacks4ROS
 *
 * h4r_ev3_ctrl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * h4r_ev3_ctrl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with h4r_ev3_ctrl.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "syshelpers.h"


bool pathExists(const std::string &path)
{
	 return (access( path.c_str(), F_OK ) != -1);
}

bool writeIntToSysFile(FILE *fileptr,int value)
{
	   std::string out;
	   bool negative= (value<0);
	   for (int i = 0; value >0 ; ++i)
	   {
		   out+=(value%10)+'0';
		   value/=10;
	   }
	   if(negative)
		   out+='-';

	   out=string ( out.rbegin(), out.rend() );
	   out+='\n';


	   int wrote=fwrite(out.c_str(),1,out.size(),fileptr);


	   if(fflush(fileptr))
		   return false;
	   rewind(fileptr);

	   return wrote==out.size();
}

bool readIntFromSysFile(FILE *fileptr, int &value)
{
		fflush(fileptr);
		rewind(fileptr);
	   int64_t value_out;
	   value=0;

	   char buffer[256];
	   char *buf=&buffer[0];
	   char **bufptr=&buf;

	   bool negative;
	   ssize_t read;
	   size_t len=256;
	   int l=0;
       while ((read = getline(bufptr, &len, fileptr)) != -1) {
    	   if(l==0)
    	   {
    		   bool negative=false;
    	 	   for (int b = 0; b < read; ++b)
    	 	   {
    	 		   if(buffer[b]=='\n')
    	 			   break; //Just in case...

    	 		   value*=10;
    	 		   switch(buffer[b])
    	 		   {
    	 		   case '-':
    	 			   if(b==0)
    	 			   {
    	 				   negative=true;
    	 			   }
    	 			   else
    	 			   {
    	 				   return false;
    	 			   }
    	 			   break;

    	 		   case '0':
    	 			   if(value==0)
    	 			   {
    	 				 return false;
    	 			   }

    	 			   break;

    	 		   case '1':
    	 		   case '2':
    	 		   case '3':
    	 		   case '4':
    	 		   case '5':
    	 		   case '6':
    	 		   case '7':
    	 		   case '8':
    	 		   case '9':
    	 			   value+=buffer[b]-48;
    	 			   break;
    	 		   default: return false;

    	 		   }
    	       }
    	 	   if(negative)
    	 		   value*=-1;
    	 	   return true;
    	   }
    	   else
    	   {
    		   return false;
    	   }
       }


	   fscanf(fileptr,"%[^\n]",buffer);
	   return true;
}

