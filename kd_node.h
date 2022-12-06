//
//  kd_node.h
//  
//
//  Created by Ghada Bakbouk on 10/7/21.
//
#ifndef kd_node_h
#define kd_node_h

#include <cstdlib>
#include "Vec2D.h"

class kd_node{
    
private:
    Vec2D node;
    float power;
    std::shared_ptr<kd_node> left;
    std::shared_ptr<kd_node> right;
    float clamp = 1.0;
    int counter = 1;
    float pdf = 1.0;
    
public:
    kd_node();
    
    kd_node(Vec2D p,  std::shared_ptr<kd_node> l,  std::shared_ptr<kd_node> r, float pr);
    
    kd_node(Vec2D p, float pr);
    
    int compare(const kd_node p, int axis);
        
    void adjust_intensity(const kd_node p);
    
    void print();
    
    std::shared_ptr<kd_node> get_left();
    
    std::shared_ptr<kd_node> get_right();
    
    Vec2D get_pos();
    
    float get_power();
    
    float get_pdf();
    
    void set_left(std::shared_ptr<kd_node> l);
    
    void set_right(std::shared_ptr<kd_node> r);
    
    void set_pdf(float num);
    
    void operator=(const kd_node &x);
    
    
};

#endif /* kd_node_h */
