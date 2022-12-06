//
//  kd_node.cpp
//  
//
//  Created by Ghada Bakbouk on 10/7/21.
//
#include "kd_node.h"
#include <iostream>

kd_node::kd_node(){
       node = Vec2D(0,0);
       left = nullptr;
       right = nullptr;
       power = 0;
       counter = 1;
       pdf = 1.0;
    
   }

kd_node::kd_node(Vec2D p,  std::shared_ptr<kd_node> l,  std::shared_ptr<kd_node> r, float pr){
    node = p;
    left = l;
    right = r;
    power = std::min(pr, clamp);
     counter = 1;
}

kd_node::kd_node(Vec2D p, float pr){
    node = p;
    left = nullptr;
    right = nullptr;
    power = std::min(pr, clamp);
     counter = 1;
}

void kd_node::adjust_intensity(const kd_node p){
    //power = p.power;
   // std:: cout << counter << std::endl;
    
    power =  power*counter + p.power;
    counter++;
    power = power/counter;
    //node = Vec2D(((real(node)+real(p.node))/2.0), ((imag(node)+imag(p.node))/2.0) );
}

int kd_node::compare(const kd_node p, int axis){
    Vec2D v = p.node-node;
    if (length(v) == 0.0){
        adjust_intensity(p);
        return 0;
    }
    if (axis==0){
        if (real(p.node) < real(node)) return -1;
        if (real(p.node) >= real(node)) return 1;
        //return compare(p,1);
    }
    if (axis==1){
        if (imag(p.node) < imag(node)) return -1;
        if (imag(p.node) >= imag(node)) return 1;
        //return compare(p,0);
    }
    std::cout <<"error" <<std:: endl;
    return -2;
    
}



void kd_node::print(){
    std::cout << "(" << real(node) << "," << imag(node) << "), with p = " << power << std::endl;
}

std::shared_ptr<kd_node> kd_node::get_left(){
    return left;
}

std::shared_ptr<kd_node> kd_node::get_right(){
    return right;
}

Vec2D kd_node::get_pos(){
    return node;
}

float kd_node::get_power(){
    return power;
}

float kd_node::get_pdf(){
    return pdf;
}

void kd_node::set_left(std::shared_ptr<kd_node> l){
     left = l;
 }

void kd_node::set_right(std::shared_ptr<kd_node> r){
    right = r;
}

void kd_node::set_pdf(float num){
    //if (num < 0){
      //  std::cout << "ERROR IN PDF" << std::endl;
   // }
    //else{
         pdf = num;
    //}
   
}

void kd_node::operator=(const kd_node &x){
    node = x.node;
    left = x.left;
    right = x.right;
    power = x.power;
}
