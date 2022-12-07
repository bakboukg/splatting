//
//  particles_nanoflann.h
//  
//
//  Created by Ghada Bakbouk on 5/27/22.
//

#ifndef particles_nanoflann_h
#define particles_nanoflann_h
#include "boundary.h"
#include "utils.h"
#include "nanoflann.hpp"
//#include <nanoflann.hpp>
#include <set>
//#include ""

using my_kd_tree_t = nanoflann::KDTreeSingleIndexAdaptor<
   nanoflann::L2_Simple_Adaptor<float, PointCloud2D<float>>,
   PointCloud2D<float>, 2 /* dim */
   >;

float gather (Vec2D x0, std::vector<Segment> segments, PointCloud2D<float> cloud, std::function<float(Vec2D)> g){
    Vec2D p = closestPointBoundary(x0, segments);
    //float r = length(p-x0);
    float r = 1.0/128.0;
    //std::vector<float> a;
    //std::vector<float> radii;
    //std::cout << r << std::endl;
    float query_pt[2] = {real(x0), imag(x0)};
    //using my_kd_tree_t = nanoflann::KDTreeSingleIndexAdaptor<
    //nanoflann::L2_Simple_Adaptor<float, PointCloud<float>>,
    //PointCloud<float>, 3 /* dim */
    //>;
     my_kd_tree_t index(2 /*dim*/, cloud, {10 /* max leaf */});
        std::vector<std::pair<unsigned int, float>> indices_dists;
        nanoflann::SearchParams params;
        unsigned int nMatches = index.radiusSearch(
                                                         &query_pt[0], (r*r), indices_dists, params);
    //std::cout << nMatches << std::endl;
    
    float sum = 0.0;
    if(nMatches == 1){
        return get_power(cloud, indices_dists[0].first);
     }
    if(nMatches < 1){
        return 0.0;
    }
    int track = 0;
   
    for (int i=0; i<nMatches; i++){
        
        /*float x_0, y_0;
        get_coordinates(cloud, i, x_0, y_0);
        Vec2D c0(x_0, y_0);
        float r0 = length(x0-c0);
        //std::cout << r0 << std::endl;
        if (r0 > r){
            //std::cout << "WTF" << std::endl;
        }
        else{*/
            sum = sum + get_power(cloud, indices_dists[i].first);
           // track++;
      //  }
        
        
    }
     //std::cout  << std::endl  << std::endl  << std::endl;
    //nMatches = nMatches - track;
    if(nMatches > 0){
        return sum/(nMatches);
    }
    //std::cout << track << std::endl;
    return 0.0;
    
}

float gather2 (Vec2D x0, std::vector<Segment> segments, PointCloud2D<float> cloud, std::function<float(Vec2D)> g){
    Vec2D p = closestPointBoundary(x0, segments);
    float r = length(p-x0);
    //std::vector<float> a;
    //std::vector<float> radii;
    float query_pt[2] = {real(x0), imag(x0)};
    //using my_kd_tree_t = nanoflann::KDTreeSingleIndexAdaptor<
    //nanoflann::L2_Simple_Adaptor<float, PointCloud<float>>,
    //PointCloud<float>, 3 /* dim */
    //>;
     my_kd_tree_t index(2 /*dim*/, cloud, {10 /* max leaf */});
    std::vector<std::pair<unsigned int, float>> indices_dists;
    nanoflann::SearchParams params;
    const unsigned int nMatches = index.radiusSearch(
               &query_pt[0], r, indices_dists, params);
   // nanoflann::RadiusResultSet<float, unsigned int> resultSet(r, indices_dists);

    //index.findNeighbors(resultSet, query_pt, nanoflann::SearchParams());
    //std::cout << nMatches << std::endl;
    
    float sum = 0.0;
   // unsigned int nMatches = indices_dists.size();
    if(nMatches == 1){
        return get_power(cloud, indices_dists[0].first);
     }
    if(nMatches < 1){
        return 0.0;
    }
   
    int track = 0;
    for (int i=0; i<nMatches; i++){
            sum = sum + get_power(cloud, indices_dists[i].first);
        
    }
    if(nMatches > 0){
        return sum/nMatches;
    }
    return 0.0;
    
}

 unsigned int find_pdf(Vec2D x, float r, PointCloud2D<float> cloud){
    float query_pt[2] = {real(x), imag(x)};
    my_kd_tree_t index(2 /*dim*/, cloud, {10 /* max leaf */});
    nanoflann::SearchParams params;
    //params.sorted = false;
    std::vector<std::pair<unsigned int, float>> indices_dists;
    return(index.radiusSearch(&query_pt[0], r*r,indices_dists, params));
}

void build_pdf(PointCloud2D<float>& cloud){
    for (int i=0; i < get_size(cloud); i++){
        float x, y;
        get_coordinates(cloud, i, x, y);
        Vec2D p(x, y);
        int pdf = find_pdf(p, 2.0/128.0, cloud);
        set_pdf(cloud, i, pdf);
    }
   
}

float gather_pdf(Vec2D x0, std::vector<Segment> segments, PointCloud2D<float> cloud, int& pdf){
    Vec2D p = closestPointBoundary(x0, segments);
    float r = length(p-x0);
    
    float query_pt[2] = {real(x0), imag(x0)};
    
    
    
    
    
    my_kd_tree_t index(2 /*dim*/, cloud, {10 /* max leaf */});
    std::vector<std::pair<unsigned int, float>> indices_dists;
    nanoflann::SearchParams params;
    const unsigned int nMatches = index.radiusSearch(
               &query_pt[0], r*r, indices_dists, params);
    
    
    
    
    
     //my_kd_tree_t index(2 /*dim*/, cloud, {1000 /* max leaf */});
    //std::vector<std::pair<unsigned int, float>> indices_dists;
    //nanoflann::RadiusResultSet<float, unsigned int> resultSet(r, indices_dists);
     //nanoflann::SearchParams params;
    //const unsigned int nMatches = index.radiusSearch(
    //&query_pt[0], r,indices_dists, params);
    
    float weight_sum = 0.0;
    float sum = 0.0;
    
    pdf = nMatches;
    for (int i=0; i<nMatches; i++){
        sum+= (1.0/get_pdf(cloud, indices_dists[i].first))*get_power(cloud, indices_dists[i].first);
        weight_sum+=1.0/get_pdf(cloud, indices_dists[i].first);
        
    }
    //std::cout << track << std::endl;
    if (nMatches==0){
        return 0.0;
    }
    return sum/weight_sum;//track;//maxi;
    
    
}

void Sample_point_cloud(std::vector<Segment> segments, std::function<float(Vec2D)> g, PointCloud2D<float>& cloud, int num){
    int c = 0;
    while (c < num){
        int t = segments.size();
        float rand = random(0.0, t);
        int index = floor(rand);
       // std::cout << index << std::endl;
        Segment s = segments[index];
        Vec2D seg_vec = s[1] - s[0];
        float l = length(seg_vec);
        seg_vec = Vec2D (real(seg_vec)/l, imag(seg_vec)/l);
        rand = random(0.0, l);
        Vec2D x = s[0] + rand*seg_vec;
        addToPointCloud(cloud, real(x), imag(x), g(x));
        c = get_size(cloud);
    }
    
    std::cout << "The cloud has " << c << " particles" << std::endl;
    
}

float simple_solve( Vec2D x0, std::vector<Segment> segments, std::function<float(Vec2D)> g){
    const float eps = EPSILON;//0.01; // stopping tolerance
   const int nWalks = 200; // number of Monte Carlo samples
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

void SampleFromSolution(std::vector<Segment> segments, std::function<float(Vec2D)> g, PointCloud2D<float>& cloud, int num){
    /*std::vector<Segment> scene_temp = {
       {{ Vec2D(0.4, 0.2), Vec2D(0.8, 0.6) }},
       {{ Vec2D(0.6, 0.8), Vec2D(0.2, 0.4) }},
       {{ Vec2D(0.2, 0.6), Vec2D(0.6, 0.2) }}
       //{{ Vec2D(0.6, 0.8), Vec2D(0.8, 0.6) }}
    };*/
    
    std::vector<Segment> scene_temp = {
       {{ Vec2D(0.5, 0.3), Vec2D(0.8, 0.6) }},
       {{ Vec2D(0.3, 0.5), Vec2D(0.6, 0.8) }},
       {{ Vec2D(0.6, 0.8), Vec2D(0.8, 0.6) }}
       //{{ Vec2D(0.6, 0.8), Vec2D(0.8, 0.6) }}
    };
    
    /*std::vector<Segment> scene_temp = {
       {{ Vec2D(0.5, 0.1), Vec2D(0.9, 0.5) }},
       {{ Vec2D(0.5, 0.9), Vec2D(0.1, 0.5) }},
       {{ Vec2D(0.1, 0.5), Vec2D(0.5, 0.1) }}
       //{{ Vec2D(0.5, 0.9), Vec2D(0.9, 0.5) }}
    };*/
    
    int c = 0;
    while (c < num){
        int t = scene_temp.size();
        float rand = random(0.0, t);
        int index = floor(rand);
       // std::cout << index << std::endl;
        Segment s = scene_temp[index];
        Vec2D seg_vec = s[1] - s[0];
        float l = length(seg_vec);
        seg_vec = Vec2D (real(seg_vec)/l, imag(seg_vec)/l);
        rand = random(0.0, l);
        Vec2D x = s[0] + rand*seg_vec;
        addToPointCloud(cloud, real(x), imag(x), simple_solve(x, segments, g));
        c = get_size(cloud);
    }
    
    
}

Vec2D closest_point_cloud(Vec2D x ,PointCloud2D<float> bound_cloud, float& power, float& dist){
     float query_pt[2] = {real(x), imag(x)};
    my_kd_tree_t index(2 /*dim*/, bound_cloud, {10 /* max leaf */});
    unsigned int num_results = 1;
    std::vector<unsigned int> ret_index(num_results);
    std::vector<float>    out_dist_sqr(num_results);
    num_results = index.knnSearch(
               &query_pt[0], num_results, &ret_index[0], &out_dist_sqr[0]);
    float x_ret , y_ret;
    get_coordinates(bound_cloud, ret_index[0], x_ret, y_ret);
    power = get_power(bound_cloud, ret_index[0]);
    dist = sqrt(out_dist_sqr[0]);
    return (Vec2D(x_ret, y_ret));
       
    
}

Vec2D move(float r, Vec2D x0, float theta){
    //float theta = random( 0., 2.*M_PI );
    //std::cout << theta << std::endl;
    float r1 = random(0, r);
    //float r1 = r/2;
    Vec2D x1 = x0 + Vec2D((r1*cos(theta)), (r1*sin(theta)));
    //std::cout << x1 - x0 << std::endl;
    return x1;
}

Vec2D sample_boundary(std::vector<Segment> segments, float& pdf){
    int seg_pick = floor(random(0, segments.size() ));
    Vec2D direction = segments[seg_pick][1] - segments[seg_pick][0];
    float l = length(direction);
    float angle = atan2(imag(direction),real(direction));
    Vec2D point = move(l,segments[seg_pick][0], angle);
    pdf = 1.0/(segments.size()*l);
    return point;
    
}

void backwards_method(PointCloud2D<float>& bound_cloud){
    
}


#endif /* particles_nanoflann_h */
