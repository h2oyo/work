#pragma once
#include "sfwl.h"

inline void drawGraph(unsigned count, const float *x, const float *y, const unsigned *colors, const float *adj)
{
    for (unsigned i = 0; i < count-1; ++i)
        for (unsigned j = i + 1; j < count; ++j)
            if (adj[i*count + j] != 0)
                sfwl::drawLineGradient(x[i], y[i], x[j], y[j], colors[i], colors[j]);
            
    for (unsigned i = 0; i < count; ++i)
        sfwl::drawBox(x[i], y[i], 8, colors[i]);
}