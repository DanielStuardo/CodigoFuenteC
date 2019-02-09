#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
    s = fnumber("##.###.###-#","",numero);
      9.234.110-K
    el 2do parámetro es un caracter de relleno hacia la izquierda.
    "#" = caracter general
*/

char *fnumber(const char *formato, const char *car, const char *numero) {
   char *buffer;

   int16_t lf = strlen(formato);
   int16_t ln = strlen(numero); 

   buffer = (char *)malloc(sizeof(char)*(lf+1));
   if (buffer==NULL) return NULL;

   int16_t i=lf, k=lf; 

   char c; 
   --lf; --ln; --k;
   while (lf>=0 && ln>=0) {
      c = formato[lf];
      if (c=='#') buffer[k] = numero[ln--]; 
      else buffer[k] = c;
      --k;
      --lf;
   }
   while (lf>=0) {
      c = formato[lf];
      if (*car) {
         if (c=='#') buffer[k] = *car;
         else buffer[k] = c;
      } else {
         buffer[k] = ' ';
      }
      --lf;
      --k;
   }
   buffer[i]='\0';
  
   return buffer;
}

int main(){
   
   char *t="###.###.###-#";
   char *s="20144567K";
   char *r;

   r = fnumber(t,"",s);
   puts(s);
   puts(r);
   free(r);

   r = fnumber("###-###-###-###-###","0","1230FD090800");
   puts("1230989090800");
   puts(r);
   free(r);

   r = fnumber("##/##/####","","1012042018");
   puts("1012042018");
   puts(r);
   free(r);

   r = fnumber("$###.###.###,##","","12042018");
   puts("12042018");
   puts(r);
   free(r);

   r = fnumber("$###.###.###,##"," ","12042018");
   puts("12042018");
   puts(r);
   free(r);      
   return 0;
}
