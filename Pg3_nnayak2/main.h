#include <stdio.h>
#include <iostream>
#include <fstream>
#include "scene.h"
#include <GL/glut.h>


static inline float _parseFloat(char*& token)
{
   token += strspn(token, " \t");
   float f = (float)atof(token);
   token += strcspn(token, " \t\r");
   return f;
}

static inline int _parseInt(char*& token)
{
   token += strspn(token, " \t");
   int f = (int)atoi(token);
   token += strcspn(token, " \t\r");
   return f;
}

static inline void _parseInt2(
   int& x, int& y,
   char*& token)
{
   x = _parseInt(token);
   y = _parseInt(token);
}

static inline void _parseFloat3(
   float& x, float& y, float& z,
   char*& token)
{
   x = _parseFloat(token);
   y = _parseFloat(token);
   z = _parseFloat(token);
}
