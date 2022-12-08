//
//  buffer.h
//  
//
//  Created by Ghada Bakbouk on 9/29/22.
//

#ifndef buffer_h
#define buffer_h

#include <vector>
#include <fstream>
#include <iostream>
#include "Vec2D.h"


class buffer{
private:
    float pixel_color[128][128];
    float pois_norm[128][128];
    int counter = 0;
    
public:
buffer(){
    for (unsigned int i=0; i< 128; i++){
        for (unsigned int j=0; j<128; j++){
            pixel_color[i][j] = 0.0;
            pois_norm[i][j] = 0.0;
        }
    }
    std::cout << "Buffer defined" << std::endl;
}


  void clear_pois(){
    for (unsigned int i=0; i< 128; i++){
        for (unsigned int j=0; j<128; j++){
            pois_norm[i][j] = 0.0;
        }
    }

  }


  void add_path(){
    for (unsigned int i=0; i< 128; i++){
	for (unsigned int j=0; j<128; j++){
            pixel_color[i][j]+=pois_norm[i][j];
        }
    }

  }

  void normalize(){
    float sum = 0;
   for (unsigned int i=0; i< 128; i++){
     for (unsigned int j=0; j<128; j++){
       sum+= pixel_color[i][j];

     }
   }
   for (unsigned int i=0; i< 128; i++){
     for (unsigned int j=0; j<128; j++){
        pixel_color[i][j]/=sum;

     }
   }
  }
void insert_to_col_pixel(int i, int j, float col){
    //int i = x*128;
    //int j = y*128;
    if (i >=54 && i<74 && j>=54 && j<74){
      //     pixel_color[i][j] += (pixel_color[i][j]*counter + col)/(counter+1);
      //counter++;
      pixel_color[i][j]+=col;
    }
   
   
}
    
void insert_to_col_pixel_particle(int i, int j, float col){
    if (i >=0 && i<128 && j>=0 && j<128){
         pixel_color[i][j] = col;
    }
}
    
void insert_to_col_pixel_adjusted(int i, int j, float col){
        //int i = x*128;
        //int j = y*128;
        if (i >=0 && i<128 && j>=0 && j<128){
            pixel_color[i][j] = ((pixel_color[i][j]*pois_norm[i][j])+col)/(pois_norm[i][j] + 1.0);
            pois_norm[i][j] += 1.0;
           // std::cout << pois_norm[i][j] << std::endl;
        }
       
}

void insert_to_pois_pixel(int i, int j, float w){
   // int i = x*128;
    //int j = y*128;
    if (i >=54 && i<74 && j>=54 && j<74){
    pois_norm[i][j] += w;
    }
}

float get_pois_pixel (unsigned int i, unsigned int j){
    if (i >=0 && i<128 && j>=0 && j<128){
        return(pois_norm[i][j]);
    }
    else{
        return 0;
    }
}

float get_col_pixel (unsigned int i, unsigned int j){
     if (i >=0 && i<128 && j>=0 && j<128){
         return(pixel_color[i][j]);
     }
    else{
        return 0;
    }
}
    
void adjust_pixel(unsigned int i, unsigned int j){
    pixel_color[i][j]/=pois_norm[i][j];
    //std::cout << pois_norm[i][j] <<std::endl;
}
    
float pois_kernel_estimate(unsigned int i, unsigned int j){
    return 0.0;
}
    

    
void write_to_pfm(std::ofstream& file){
    int s = 128;
    file << "PF" << "\n";
    file << s << " ";
    file << s << "\n";
    file << "-1.000000" << "\n";
    color col;
    for( int j = 0; j < s; j++ )
    {
      // cerr << "row " << j+1 << " of " << s << endl;
       for( int i = 0; i < s; i++ )
       {
          // this->adjust_pixel(i,j);
           col.r = pixel_color[i][j];
           col.g = pixel_color[i][j];
           col.b = pixel_color[i][j];
           //::cout << col.b << std::endl;
           file.write(reinterpret_cast<char*>(&col), sizeof(col));
       }
    }
}

};

#endif /* buffer_h */
