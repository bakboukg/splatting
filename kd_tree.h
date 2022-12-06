//
//  kd_tree.hpp
//  
//
//  Created by Ghada Bakbouk on 10/7/21.
//

#ifndef kd_tree_h
#define kd_tree_h

#include <cstdlib>
#include <vector>
#include <memory>
#include "kd_node.h"
#include "kd_node.cpp"
#include "boundary.h"



class kd_tree{
private:
    std::shared_ptr<kd_node> root;
    std::shared_ptr<kd_node> current;
    int size;
    
public:
    kd_tree();
    
    std::shared_ptr<kd_node> insert_root(kd_node & n, std::shared_ptr<kd_node> start, int depth);
    
    void insert(kd_node & n, std::shared_ptr<kd_node> start);

    std::shared_ptr<kd_node> get_root();
    
    std::shared_ptr<kd_node> get_current();
    
    int get_size();
    
    void print(std::shared_ptr<kd_node> start);
    
    float search(Vec2D x, std::shared_ptr<kd_node> start, int axis, float dist);
    
    void traverse(Vec2D x, std::shared_ptr<kd_node> start, float r, std::vector<float> &a, std::vector<float> &radii);
    
    void build_pdf(std::shared_ptr<kd_node> start, int& maximum);
    
    void traverse_with_pdf(Vec2D x, std::shared_ptr<kd_node> start, float r, std::vector<float> &a, std::vector<float>& pdfs);
    
    //float GreensFunctionPDF12(Vec2D x, std::vector<Segment> segments);
    
    //void build_pdf_Greens(std::shared_ptr<kd_node> start, std::vector<Segment> segments);
    
    void traverse2(Vec2D x, std::shared_ptr<kd_node> start, float r, std::vector<float> &a);
    
    void traverse_points(Vec2D x, std::shared_ptr<kd_node> start, float r, std::vector<kd_node_lite> &a);
    
    void k_closest(Vec2D x, std::shared_ptr<kd_node> start, float r, int k, std::vector<kd_node_lite> &b );
    
    void float_closest(Vec2D x, std::shared_ptr<kd_node> start, float r, float k, std::vector<kd_node_lite> &b);
};
#endif /* kd_tree_hpp */
