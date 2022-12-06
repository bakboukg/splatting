//
//  kd_tree.cpp
//  
//
//  Created by Ghada Bakbouk on 10/7/21.
//

#include "kd_tree.h"

#include <iostream>




kd_tree::kd_tree(){
    root = nullptr;
    current = nullptr;
    size = 0;
}

std::shared_ptr<kd_node> kd_tree::insert_root(kd_node & n, std::shared_ptr<kd_node> start, int depth){
    
    std::shared_ptr<kd_node> g (new kd_node(n.get_pos(), n.get_power()));
    
    if (start == nullptr){
        //cout << "assign root" << endl;
        return g;
    }
    
    int axis = depth % 2;
    
   
    int c = (*start).compare(*g, axis);
    if (c == -1 ){
            start->set_left(insert_root(n, start->get_left(), depth+1));
        }
    else if (c == 1 ){
            start->set_right( insert_root(n, start->get_right(), depth+1));
        }
    else if (c==0){
        //std::cout << "Equals" << std::endl;
        //do nothing, the comparison function will have taken care of it.
    }
    else{
        std::cout << "ERROR ROOT" << std::endl;
        //std::cout << start->get_pos() << std::endl;
        return nullptr;
    }
    return start;

}

void kd_tree::insert(kd_node & n, std::shared_ptr<kd_node> start){
    //if (search(n.get_pos(),root,0)==-1){
        root = insert_root(n, root,0);
        size++;
    //}
    
}

std::shared_ptr<kd_node> kd_tree::get_root(){
    return root;
}

std::shared_ptr<kd_node> kd_tree::get_current(){
    return current;
}

int kd_tree::get_size(){
       return size;
}

void kd_tree::print(std::shared_ptr<kd_node> start){
    
    if (start == nullptr){
       // cout << "this" << endl;
        return;
        
    }
    start->kd_node::print();
    //cout << "To the left of " << start->get_pos () << " you get : ";
    print(start->get_left());
    //cout << "To the right of " << start->get_pos () << " you get : ";
    print(start->get_right());
}

float kd_tree::search(Vec2D x, std::shared_ptr<kd_node> start, int axis, float dist){
    if (start == nullptr){
        return -1;
    }
    Vec2D cur = (*start).get_pos();
    Vec2D dif = cur - x;
    if (length(dif) < dist){
       // std::cout << "here" << std::endl;
        return(*start).get_power();
    }
     if (axis == 0){
        if (real(x) < real(cur) ){
          //  cout << "less than " << cur << endl;
            return search(x, start->get_left(), 1, dist);
        }
         if (real(x) >= real(cur) ){
            // cout << "more than " << cur << endl;
             return search(x, start->get_right(), 1, dist);
         }
    }
    if (axis == 1){
        if (imag(x) < imag(cur) ){
            //cout << "less than " << cur << endl;
            return search(x, start->get_left(), 0, dist);
        }
        if (imag(x) >= imag(cur) ){
            //cout << "more than " << cur << endl;
            return search(x, start->get_right(), 0, dist);
        }
    }
    std::cout << "ERROR" << std::endl;
    return -2;
}


void kd_tree::traverse(Vec2D x, std::shared_ptr<kd_node> start, float r, std::vector<float> &a, std::vector<float>& radii){
    if (start == nullptr){
        //cout << "end" << endl;
        return;
    }
    Vec2D cur = (*start).get_pos();
    Vec2D dif = cur - x;
    float radius = length(dif);
    if (radius < r){// (abs(r-radius)>(1.0/128.0)) && ){//{//&& radius > EPSILON){
        //cout << "here" << endl;
        a.push_back(start->get_power());
        radii.push_back(radius);
    }
    traverse(x, start->get_left(), r, a, radii);
    traverse(x, start->get_right(), r, a, radii);
}


void kd_tree::build_pdf(std::shared_ptr<kd_node> start, int& maximum){
    if (start == nullptr){
        //cout << "end" << endl;
        return;
    }
    Vec2D cur = (*start).get_pos();
    float r = 2.0/128.0;
    std::vector<float> a;
    std::vector<float> radii;
    traverse(cur, get_root(), r, a, radii);
    if (a.size() < 1 ){
        std::cout << "ZERO" << std::endl;
    }
    int pdf = a.size();
    start -> set_pdf(pdf);
    if (maximum < pdf){
        maximum =pdf;
    }
    build_pdf(start->get_left(), maximum);
    build_pdf(start->get_right(), maximum);
}

/*float kd_tree::GreensFunctionPDF12(Vec2D x, std::vector<Segment> segments){
    float constant = 1.0/(2.0*M_PI);
    float sum = 0.0;
    for( auto s : segments ) {
        Vec2D p = closestPoint( x, s );
        float dist = length(p-x);
        sum+= constant*log(1.0/dist);
        
    }
    return sum/segments.size();
}

void kd_tree::build_pdf_Greens(std::shared_ptr<kd_node> start, std::vector<Segment> segments){
    if (start == nullptr){
        //cout << "end" << endl;
        return;
    }
    Vec2D cur = (*start).get_pos();
    float constant = 1.0/(2.0*M_PI);
    float sum = 0.0;
    for( auto s : segments ) {
        Vec2D p = closestPoint( cur, s );
        float dist = length(p-cur);
        sum+= constant*log(1.0/dist);
        
    }
    float pdf = sum/segments.size();
    
   // float pdf = GreensFunctionPDF12(cur, segments);
    
    start -> set_pdf(pdf);
   
    build_pdf_Greens(start->get_left(), segments);
    build_pdf_Greens(start->get_right(), segments);
}*/

void kd_tree::traverse_with_pdf(Vec2D x, std::shared_ptr<kd_node> start, float r, std::vector<float> &a, std::vector<float>& pdfs){
    if (start == nullptr){
        //cout << "end" << endl;
        return;
    }
    Vec2D cur = (*start).get_pos();
    Vec2D dif = cur - x;
    float radius = length(dif);
    if (radius < r){// (abs(r-radius)>(1.0/128.0)) && ){//{//&& radius > EPSILON){
        //cout << "here" << endl;
        a.push_back(start->get_power());
        pdfs.push_back(start->get_pdf());
    }
    traverse_with_pdf(x, start->get_left(), r, a, pdfs);
    traverse_with_pdf(x, start->get_right(), r, a, pdfs);
}



void kd_tree::traverse_points(Vec2D x, std::shared_ptr<kd_node> start, float r, std::vector<kd_node_lite> &a){
    if (start == nullptr){
        //cout << "end" << endl;
        return;
    }
    kd_node_lite p;
    p.pos = (*start).get_pos();
    Vec2D dif = p.pos - x;
    p.dist = length(dif);
    p.power =  (*start).get_power();
    if (p.dist <= r){// && p.dist > EPSILON ){
        //cout << "here" << endl;
        a.push_back(p);
    }
    traverse_points(x, start->get_left(), r, a);
    traverse_points(x, start->get_right(), r, a);
}

void kd_tree::k_closest(Vec2D x, std::shared_ptr<kd_node> start, float r, int k, std::vector<kd_node_lite> &b){
    
    std::vector<kd_node_lite> a;
    traverse_points(x, start, r, a);
    auto comp_func = [](kd_node_lite x1, kd_node_lite x2)
    {
        return x1.dist < x2.dist;
    };
    std::sort(a.begin(), a.end(), comp_func);
    int l = a.size();
    int n = std::min(k, l);
    for (int i = 0; i < n; i++){
        b.push_back(a[i]);
        //radii.push_back(a[i].dist);
    }
}



void kd_tree::float_closest(Vec2D x, std::shared_ptr<kd_node> start, float r, float k, std::vector<kd_node_lite> &b){
    
    std::vector<kd_node_lite> a;
    traverse_points(x, start, r, a);
    auto comp_func = [](kd_node_lite x1, kd_node_lite x2)
    {
        return x1.dist < x2.dist;
    };
    std::sort(a.begin(), a.end(), comp_func);
    int l = a.size();
    int n = std::floor(k*l);
    for (int i = 0; i < n; i++){
        b.push_back(a[i]);
        //radii.push_back(a[i].dist);
    }
    

    
}

void kd_tree::traverse2(Vec2D x, std::shared_ptr<kd_node> start, float r, std::vector<float> &a){
    if (start == nullptr){
        //cout << "end" << endl;
        return;
    }
    Vec2D cur = (*start).get_pos();
    Vec2D dif = cur - x;
    std::vector<float> radii;
    if (length(dif) <= r){
        //cout << "here" << endl;
        traverse(x, start, r, a, radii);
    }
    //if (a.size() < num){
        traverse2(x, start->get_left(), r+(1.0/128.0), a);
        traverse2(x, start->get_right(), r+(1.0/128.0), a);
    //}
}
