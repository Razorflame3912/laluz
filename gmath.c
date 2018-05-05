#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  color a = calculate_ambient(alight, areflect);
  color d = calculate_diffuse(light, dreflect, normal);
  color s = calculate_specular(light, sreflect, view, normal);
  i.red = a.red + d.red + s.red;
  i.green = a.green + d.green + s.green;
  i.blue = a.blue + d.blue + s.blue;
  limit_color(&i);
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * areflect[RED];
  a.green = alight.green * areflect[GREEN];
  a.blue = alight.blue * areflect[BLUE];
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  normalize(light[LOCATION]);
  normalize(normal);
  d.red = light[COLOR][RED] * dreflect[RED]*dot_product(light[LOCATION],normal);
  d.green = light[COLOR][GREEN] * dreflect[GREEN]*dot_product(light[LOCATION],normal);
  d.blue = light[COLOR][BLUE] * dreflect[BLUE]*dot_product(light[LOCATION],normal);
  limit_color(&d);
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  double r[3];
  double l[3];
  r[0] = normal[0];
  r[1] = normal[1];
  r[2] = normal[2];
  l[0] = light[LOCATION][0];
  l[1] = light[LOCATION][1];
  l[2] = light[LOCATION][2];
  normalize(light[LOCATION]);
  normalize(view);
  normalize(normal);
  if(dot_product(light[LOCATION],normal) < 0){
    color s;
    s.red = 0;
    s.green = 0;
    s.blue = 0;
    return s;
  }
  double power = 5;
  double redinit  = light[COLOR][RED] * sreflect[RED];
  double greeninit  = light[COLOR][GREEN] * sreflect[GREEN];
  double blueinit  = light[COLOR][BLUE] * sreflect[BLUE];
  double calc = 2 * dot_product(normal,light[LOCATION]);
  r[0] *= calc;
  r[1] *= calc;
  r[2] *= calc;
  r[0] -= l[0];
  r[1] -= l[1];
  r[2] -= l[2];
  double whole = dot_product(r,view);
  
	       
  if(whole < 0){
    calc = 0;
  }
  whole = pow(whole, power);

  color s;
  s.red = redinit * whole;
  s.green = greeninit * whole;
  s.blue = blueinit * whole;
  limit_color(&s);
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  if(c[0].red > 255){
    c[0].red = 255;
  }
  if(c[0].green > 255){
    c[0].green = 255;
  }
  if(c[0].blue > 255){
    c[0].blue = 255;
  }
  if(c[0].red < 0){
    c[0].red = 0;
  }
  if(c[0].green < 0){
    c[0].green = 0;
  }
  if(c[0].blue < 0){
    c[0].blue = 0;
  }
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double magnitude = sqrt((vector[0]*vector[0])+
			  (vector[1]*vector[1])+
			  (vector[2]*vector[2]));

  vector[0] = vector[0]/magnitude;
  vector[1] = vector[1]/magnitude;
  vector[2] = vector[2]/magnitude;
}

//Return the dot product of a . b
double dot_product( double *a, double *b ) {
  return (a[0]*b[0]) + (a[1]*b[1]) + (a[2]*b[2]);
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
