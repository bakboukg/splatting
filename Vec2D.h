//
//  Vec2D.h
//  
//
//  Created by Ghada Bakbouk on 10/7/21.
//

#ifndef Vec2D_h
#define Vec2D_h

#include <complex>
#include <array>

// use std::complex to implement 2D vectors
using Vec2D = std::complex<float>;

static const float EPSILON = 10e-6;

float dot(Vec2D u, Vec2D v) { return real(conj(u)*v); }

float length( Vec2D u ) { return sqrt( norm(u) ); }

// a segment is just a pair of points
using Segment = std::array<Vec2D,2>;

// returns a random value in the range [rMin,rMax]
float random( float rMin, float rMax ) {
   const float rRandMax = 1./(float)RAND_MAX;
   float u = rRandMax*(float)rand();
   return u*(rMax-rMin) + rMin;
}

struct kd_node_lite{
    Vec2D pos;
    float power;
    float dist;
};

struct color{
    float r;
    float g;
    float b;
};

#endif /* Vec2D_h */
