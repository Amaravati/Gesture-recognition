double gc_Weights_Quad1[3][3] = {{-0.0052414, 0.0022281, 0.0024968}, {0.0022281, -0.0046258, 0.0034622}, {0.0024968, 0.0034622, -0.014636}};

double gc_Weights_Quad2[3][3] = {-0.019958, 0.0075453, 0.011175, 0.0075453, -0.010766, 0.00078831, 0.011175, 0.00078831, -0.023769};

double gc_Weights_Quad3[3][3] = {-0.0043858, 0.0048427, -0.001611, 0.0048427, -0.012106, 0.0054752, -0.001611, 0.0054752, -0.010414};

double gc_Weights_Quad4[3][3] = {-0.0027651, -0.00049796, 0.0021466, -0.00049796, -0.0058226, 0.0030379, 0.0021466, 0.0030379, -0.0052363};

double gc_Weights_Quad5[3][3] = {-0.019062, 0.0053905, -0.0029344, 0.0053905, -0.015255, -0.0026117, -0.0029344, -0.0026117, -0.0063135};


double gc_Weights_Lin1[1][3] = { -0.46483, 0.37479, -0.25334};

double gc_Weights_Lin2[1][3] = { 1.0885, -0.070671, 0.26327};

double gc_Weights_Lin3[1][3] =  { 1.484, -3.4354, 1.9394};

double gc_Weights_Lin4[1][3] = { -0.49478, 0.00094804, 0.42916};

double gc_Weights_Lin5[1][3] = { 1.3513, -0.62878, -0.41373};


double gc_Biases[5] = {-24.0986, -47.3964, -259.7487, -33.5603, -49.2274};


char gc_Labels[5] = {'+', 'O', 'N', 'X', 'Z'};


double* gc_Weights_Quad[5] = {gc_Weights_Quad1, gc_Weights_Quad2, gc_Weights_Quad3, gc_Weights_Quad4, gc_Weights_Quad5};

double* gc_Weights_Lin[5] = {gc_Weights_Lin1, gc_Weights_Lin2, gc_Weights_Lin3, gc_Weights_Lin4, gc_Weights_Lin5};


int gc_N =5;
