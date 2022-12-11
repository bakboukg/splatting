#include <algorithm>
#include <functional>
#include <random>
#include <fstream>
#include <math.h>
#include <cstring>
//#include "boundary.h"
#include "particles_nanoflann.h"
#include <time.h>
#include <eigen3/Eigen/Dense>
#include "buffer.h"
using namespace std;

void world_to_pixel(Vec2D p, int& i, int& j){
    float x = real(p);
    float y = imag(p);
    i = floor(x*128.0);
    j = floor(y*128.0);
}

void pixel_to_world(int i, int j, Vec2D& p){
    float x = (float)i / 128.0;
    float y = (float)j / 128.0;
    p = Vec2D(x,y);
}

float updated_weight(float r, float w){
    float v = 1.0/(M_PI*r*r);
    return w/v;
}

float Green(float r, float R){
    return(log (R/(r+EPSILON))/(2.*M_PI));
}

void local_splat(buffer& image, Vec2D point_source, Vec2D origin_point, float r, float base_weight){
    int pixel_r = floor(r*128.0);//min((int)floor(r*128.0), (int)10);
    //cout << pixel_r << endl;
    int orig_i, orig_j;
    world_to_pixel(origin_point, orig_i, orig_j);
    int i, j;
    world_to_pixel(point_source, i, j);
        //cout << i  << "   " << j << endl;
        //float w = image.get_col_pixel(i, j);
    //float up_w = updated_weight(r, base_weight);
    for (int n = i - pixel_r; n <= i + pixel_r; n++){
        for (int m = j - pixel_r; m<= j + pixel_r; m++){
            if (abs(n-orig_i)<3 && abs(m - orig_j)<3){
                Vec2D temp;
                pixel_to_world(n, m, temp);
                //cout << temp << endl;
                float l = length(temp - point_source);
                if (l < r){
                    float up_w = Green(l, r);
                    //cout << up_w << endl;
                    image.insert_to_pois_pixel(n,m, up_w);
                }
            }
        }
    }
    //cout << up_w << endl;
    //return up_w;
}

void splat(buffer& image, buffer particles, Vec2D point_source, Vec2D origin_point, vector<Segment> segs, float base_weight, int track){
    float terminate = random(0.0, 1.0);
    if (track > 10){
        return;
    }
    /*if (terminate > 0.7){
        return;
    }*/
    int i, j;
    world_to_pixel(point_source, i, j);
    if (i < 0 || i > 127 || j < 0 || j > 127){
        return;
    }
    Vec2D closest_point = closestPointBoundary(point_source,segs);
    float r = length(closest_point - point_source);
    if ( r < EPSILON){
        return;
    }
    
   /* if (base_weight == 0.0){
        base_weight = 1.0;
    }*/
    //cout << base_weight << endl;
    if( base_weight < 10*EPSILON){
        return;
    }
    local_splat(image,point_source,origin_point, r, base_weight);
    track++;
    for (int h =0; h <1; h++){
        float theta = random(0., 2.*M_PI);
        //cout << theta << endl;
        point_source = point_source + Vec2D( (r*cos(theta)), (r*sin(theta)) );
        world_to_pixel(point_source, i, j);
        base_weight = image.get_pois_pixel(i,j);

        splat(image, particles, point_source,origin_point, segs, base_weight, track);
    }
}

vector<Segment> scene2 = {
   {{ Vec2D(0.5, 0.1), Vec2D(0.9, 0.5) }},
   {{ Vec2D(0.5, 0.9), Vec2D(0.1, 0.5) }},
   {{ Vec2D(0.1, 0.5), Vec2D(0.5, 0.1) }},
   {{ Vec2D(0.5, 0.9), Vec2D(0.9, 0.5) }}
};

vector<Segment> scene1 = {
   {{ Vec2D(0.5, 0.1), Vec2D(0.9, 0.5) }}
};

vector<Segment> scene = {
{{ Vec2D(0.5, 0.1), Vec2D(0.9, 0.5) }},
{{ Vec2D(0.5, 0.9), Vec2D(0.1, 0.5) }},
{{ Vec2D(0.1, 0.5), Vec2D(0.5, 0.1) }},
{{ Vec2D(0.5, 0.33333333), Vec2D(0.5, 0.6666666) }},
{{ Vec2D(0.33333333, 0.5), Vec2D(0.6666666, 0.5) }}
};

float solve( Vec2D x0, vector<Segment> segments, function<float(Vec2D)> g, buffer& particles){
    const float eps = EPSILON;//0.01; // stopping tolerance
   const int nWalks = 100; // number of Monte Carlo samples
   const int maxSteps = 16; // maximum walk length
    Vec2D SPOINT;
   float sum = 0.;
   for( int i = 0; i < nWalks; i++ ) {
      Vec2D x = x0;
      float R;
      int steps = 0;
      do {
          
          SPOINT = closestPointBoundary(x,segments);
          R = length(x-SPOINT);
         float theta = random( 0., 2.*M_PI );
         x = x + Vec2D( (R*cos(theta)), (R*sin(theta)) ); //picking a new point within the ball
         steps++;
      }
      while( R > eps && steps < maxSteps );

      sum += g(SPOINT);
   }
    float result = sum/nWalks;
   return result; // Monte Carlo estimate
}

void fill_particles(buffer& particles, int n, vector<Segment> segments, function<float(Vec2D)> g){
    for (int i = 0; i < n; i++){
        float x = random(0., 1.);
        float y = random(0., 1.);
        Vec2D point(x, y);
        float u = solve(point, segments, g, particles);
        int n, m;
        world_to_pixel(point, n, m);
        particles.insert_to_col_pixel_particle(n, m, u);
    }
    
}

void fill_particles2(buffer& particles, vector<Segment> segments, function<float(Vec2D)> g){
    int s = 128;
    for (int i = 0; i < 128; i++){
        for (int j = 0; j < 128; j++){
            float x = random(0.0, 1.0);
            if ( x > 0.5)
            {
                Vec2D point((float)i/(float)s, (float)j/(float)s);
                float u = solve(point, segments, g, particles);
                int n, m;
                world_to_pixel(point, n, m);
                particles.insert_to_col_pixel_particle(n, m, u);
            }
        
    }
    }
    
}

float checker( Vec2D x ) {
   const float s = 6.;
   return fmod( floor(s*real(x)) + floor(s*imag(x)), 2. );
}

void set_value(buffer& image, buffer particles, buffer map, int i, int j){
    float sum = 0.0;
    float weights = 0.0;
    for (int n =-3; n < 3; n++){
        for (int m = -3; m < 3; m++){
            sum+= particles.get_col_pixel(i+n,j+m)*map.get_col_pixel(i+n,j+m);
            //cout << map.get_pois_pixel(n,m) << endl;
           // weights+=map.get_pois_pixel(i+n,j+m);
        }
    }
    
    /* for (int n =0; n < 128; n++){
        for (int m = 0; m < 128; m++){
            sum+= particles.get_col_pixel(n,m)*map.get_pois_pixel(n,m);
            //cout << map.get_pois_pixel(n,m) << endl;
            weights+=map.get_pois_pixel(n,m);
        }
    }*/
    //if (weights > EPSILON){
      //  sum /= weights;
    //}
    image.insert_to_col_pixel_adjusted(i, j, sum);
    
}

int main( int argc, char** argv ) {
    srand( time(NULL) );
   
    buffer particles;
    fill_particles2(particles, scene1, checker);
    
    
    buffer image;
    int s = 128;
    for(int i =0; i < 128; i++){
        for (int j =0; j < 128; j++){
            buffer map;
            Vec2D point_source( (float)i/(float)s, (float)j/(float)s);
            int track = 0;
            for (int x = 0; x < 20; x++){
              splat(map, particles, point_source, point_source, scene1, 1.0, track);
                map.add_path();
                map.clear_pois();
            }
            map.normalize();
    
            set_value(image, particles, map, i, j);
        }
    }
    
      
    
    string file_name = "comp_test_7x7_p5.pfm";
    ofstream out(file_name.c_str());
    image.write_to_pfm(out);
    out.close();
    
    
    
    
    
    
    
    return 0;
}

