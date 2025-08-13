#include "main.h"

int maximum(a, b, c)
{
   return ((a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c));
}
int minimum(a, b, c)
{
   return ((a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c));
}

int main()
{
   SEP

   printf("%i\n", maximum(1, 3, 2));
   printf("%i\n", minimum(1, 3, 2));

   SEP return 0;
}
