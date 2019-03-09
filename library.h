#ifndef TOOLDLL_LIBRARY_H
#define TOOLDLL_LIBRARY_H
extern "C" {
double *initial();
int feed(int toolNo,double load,double *p);
char *result(double *p);
};


#endif