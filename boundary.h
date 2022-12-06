//
//  boundary.h
//  
//
//  Created by Ghada Bakbouk on 10/7/21.
//

#ifndef boundary_h
#define boundary_h

#include <vector>
#include <iostream>
#include "Vec2D.h"
#include "kd_tree.cpp"

// returns the point on segment s closest to x
Vec2D closestPoint( Vec2D x, Segment s ) {
   Vec2D u = s[1]-s[0];
   float t = std::clamp(dot(x-s[0],u)/dot(u,u),0.f,1.f);
   return (1-t)*s[0] + t*s[1];
}

Vec2D closestPointBoundary( Vec2D x, std::vector<Segment> segments) {
  float dist = RAND_MAX;
  Vec2D best_point;
  for( auto s : segments ) {
   Vec2D p = closestPoint( x, s );
   float temp_dist = length(x - p);
   if (temp_dist < dist){
       dist = temp_dist;
       best_point = p;
   }
  }
    return best_point;
}

Vec2D farthestPointBoundary( Vec2D x, std::vector<Segment> segments) {
  float dist = 0;
  Vec2D best_point;
  for( auto s : segments ) {
   Vec2D p = closestPoint( x, s );
   float temp_dist = length(x - p);
   if (temp_dist > dist){
       dist = temp_dist;
       best_point = p;
   }
  }
    return best_point;
}



bool onBoundary(Vec2D p, std::vector<Segment> segments){ //mostly works!
    for( auto s : segments ){
        Vec2D v1 = s[1] - s[0];
        float l1 = length (v1);
        v1 = v1 / l1;
        Vec2D v2 = p - s[0];
        float l2 = length (v2);
        v2 = v2 / l2;
        float d = dot(v1, v2);
        float dif = abs(d - 1.0);
        if ((dif <= EPSILON) && ((l2 - l1) < EPSILON)){
            return true;
        }
    }
    return false;
}

float onBoundaryNum(Vec2D p, std::vector<Segment> segments){ //mostly works!
    for( auto s : segments ){
        Vec2D v1 = s[1] - s[0];
        float l1 = length (v1);
        v1 = v1 / l1;
        Vec2D v2 = p - s[0];
        float l2 = length (v2);
        v2 = v2 / l2;
        float d = dot(v1, v2);
        float dif = abs(d - 1.0);
        if ((dif <= EPSILON) && ((l2 - l1) < EPSILON)){
            return 1.0;
        }
    }
    return 0.5;
}

bool inLine (float m, Vec2D x, Vec2D x0){
    Vec2D v = x - x0;
    float dif = imag(v) - (m*real(v));
    //cout << dif << endl;
    if (abs(dif) < EPSILON){
        return true;
    }
    return false;
}

float onBoundary2(Vec2D p, std::vector<Segment> segments){
    std::vector<float> slopes;
    for( auto s : segments ){
        Vec2D v = s[1] - s[0];
        float m = imag(v)/real(v);
        slopes.push_back(m);
    }
    
    for (int i=0; i<slopes.size(); i++ ){
        if (inLine(slopes[i], p, segments[i][0])){
            std::cout << "here" << std::endl;
            return true;
        }
    }
    return false;
}

float onBoundary2Num(Vec2D p, std::vector<Segment> segments){
    std::vector<float> slopes;
    for( auto s : segments ){
        Vec2D v = s[1] - s[0];
        float m = imag(v)/real(v);
        slopes.push_back(m);
        //cout << m << endl;
    }
    
    
    for (int i=0; i<slopes.size(); i++ ){
        if (inLine(slopes[i], p, segments[i][0])){
            return 0.0;
        }
    }
    return 0.5;
}




bool inSegment (Vec2D x, Segment s){
    Vec2D v1 = s[1] - s[0];
    float l1 = length (v1);
    v1 = v1 / l1;
    Vec2D v2 = x - s[0];
    float l2 = length (v2);
    v2 = v2 / l2;
    float d = dot(v1, v2);
    float dif = abs(d - 1.0);
    if ((dif <= EPSILON) && ((l2 - l1) < EPSILON)){
        return true;
    }
    return false;
}

Segment normal (Vec2D x, std::vector<Segment> segments){
    Segment one;
    for( auto s : segments ){
        if (inSegment(x, s)){
            one = s;
            break;
        }
    }
    //float m = (imag(s[1]) - imag(s[0]))/(real(s[1]) - real(s[0]));
    Vec2D v = one[1] - one[0];
    if (real(v) == 0){
       Segment n = {{ x, x+Vec2D(0,1)}};
        return n;
    }
    float m = - (imag(v)/real(v));
    //cout << m << endl;
    Vec2D t(1, m);
    float l = length(t);
    t = t/l;
    Segment n = {{ x, x+t }};
    return n;
}

float normal_angle(Segment s){
    Vec2D v = s[1] - s[0];
    //std::cout << s[1] << "     " << s[0] << std::endl;
    if (abs(real(v)) < EPSILON){
        return 0.0;
    }
    if (abs(imag(v)) < EPSILON){
        return (M_PI/2.0);
    }
    float m = -1.0/(imag(v)/real(v));
    return (atan(m));
}

bool in_seg_slow(Vec2D x0, std::vector<Segment> segments, Segment& ans){
     for( auto s : segments ){
         Vec2D p = closestPoint(x0, s);
         if (length(x0-p) < EPSILON){
             ans = s;
             return true;
         }
     }
    return false;
}

bool in_cloud(Vec2D x0, std::vector<Segment> segments, Segment& ans){
     for( auto s : segments ){
         Vec2D p = closestPoint(x0, s);
         if (length(x0-p) < 0.05){
             ans = s;
             return true;
         }
     }
    return false;
}

void test_normal_angle(Vec2D x0, std::vector<Segment> segments, kd_tree& test){
    Segment s;
    bool na = in_seg_slow(x0, segments, s);
    if (na == false){
        return;
    }
    float theta = normal_angle(s);
    float r = random(0.0, 1.0);
    
    if (r > 0.5){
        theta = theta + M_PI;
    }
    std::cout << theta << std::endl;
    float step = 1.0/128.0;
    Vec2D x1 = x0;
    while (real(x1) <= 1 && real(x1) >= 0 && imag(x1) <= 1 && imag(x1) >= 0){
        x1 = x1 + Vec2D(step*cos(theta), step*sin(theta));
        kd_node n(x1, 1.0);
        test.insert(n, test.get_root());
        
    }
    
}

float GreensFunctionPDF(Vec2D x, std::vector<Segment> segments){
    float constant = 1.0/(2.0*M_PI);
    float sum = 0.0;
    for( auto s : segments ) {
        Vec2D p = closestPoint( x, s );
        float dist = length(p-x);
        sum+= constant*log(1.0/dist);
        
    }
    return sum/segments.size();
}

void build_pdf_Greens(std::shared_ptr<kd_node> start, std::vector<Segment> segments, int& maxi){
    if (start == nullptr){
        //cout << "end" << endl;
        return;
    }
    Vec2D cur = (*start).get_pos();
    /*float constant = 1.0/(2.0*M_PI);
    float sum = 0.0;
    for( auto s : segments ) {
        Vec2D p = closestPoint( cur, s );
        float dist = length(p-cur);
        sum+= constant*log(1.0/dist);
        
    }
    float pdf = sum/segments.size();*/
    
    float pdf = GreensFunctionPDF(cur, segments);
    if (pdf >  maxi){
        maxi = pdf;
    }
    
    start -> set_pdf(pdf);
   
    build_pdf_Greens(start->get_left(), segments, maxi);
    build_pdf_Greens(start->get_right(), segments,  maxi);
}

float light_ratio(std::vector<Segment> segments, std::function<float(Vec2D)> g, int s){
    int total = 0;
    int light = 0;
    for (int j = 0; j < s; j++){
        for (int i = 0; i < s; i++){
            Vec2D x0( (float)i/(float)s, (float)j/(float)s);
            Segment ans;
            if (onBoundary(x0, segments)){
                total++;
                if (g(x0) == 1){
                    light++;
                }
            }
            
        }
    }
    //std::cout << light << "      " << total;
    return (float)light/(float)total;
    
}


#endif /* boundary_h */
