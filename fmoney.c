/*
   FMONEY.C
   
   Formateo de números a monedas.
   Sistema Operativo: Linux basado en Debian; OSX 10.8 y superior.
   GCC: 4.2 en adelante.
   Fecha de creación: alguna parte de 2000.
   
   Autor: Daniel Stuardo
   daniel.stuardo@gmail.com
   
   TODO: ejecutar con Valgrind y cambiar malloc por calloc.
   
*/
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/************************************************
  s = fmoney(2340293234, "$","blanco", 2, 20);
      $   2,340,293,234.00 lindo, no???
      O no????

***********************************************/

// functions:
char *fmoney (double numero, char *tipo, char *cblanc, uint16_t decimales, uint16_t pad){
   char *buf,*cnum;
   const char * d, * s;
   uint16_t isize=0, ipart=0, iresto=0, tpad=0, swSign=0;

   buf = (char *)malloc (sizeof(char *) * 32);
   if (buf==NULL) return NULL;

   if( numero < 0 ) {
       swSign=1;
       numero *= -1;
   }

   uint16_t size = sprintf(buf,"%.*lf",decimales, numero);

 ///  printf("\n**** [%s],len=%lu,size=%d ****\n",buf,strlen(buf),size);

   if (decimales>0) {
      d = strstr(buf,".");
      if (d!=NULL) {
         isize = d - buf;  // obtengo parte entera.
         d++;   // tendrá el decimal
      } else isize = size;
   } else isize = size;

   iresto = isize % 3;   // cuantos son los dígitos sobran
   ipart = (uint16_t) isize / 3; // cuántas particiones debo efectuar; ipart-1=núm de sep
   tpad = isize+(ipart-1)+decimales+2+strlen(tipo)+swSign;

   uint16_t blancos=0;

   if (pad < tpad) {
       pad = tpad;
   } else {
       blancos = pad - tpad;
       if (isize%2!=0) ++blancos;
       if (decimales==0) ++blancos; // por el punto decimal
   }

   cnum = (char *)malloc(sizeof(char *)*(pad+1));
   if (cnum==NULL) return NULL;

   strcpy(cnum,tipo);
   uint16_t i;

   for (i=0;i<blancos;i++) strncat(cnum,cblanc,strlen(cblanc));

   if( swSign ) strcat(cnum,"-");

   s = buf;

   // agrego el resto, de existir
   if (iresto>0) {
      strncat(cnum,s,iresto);
      strcat(cnum,",");
      s += iresto;  // avanza el puntero
   }
   // agrego la parte entera
   for (i=0;i<ipart; i++) {
       strncat(cnum,s,3);
       if (i<ipart-1) strcat(cnum,",");
       s += 3;
   }
   if (decimales>0) {
      strcat(cnum,".");
      strncat(cnum,d,strlen(d));
   }
   cnum[strlen(cnum)]='\0';
   free(buf);

   return cnum;
}

// algorithm:
int main(){

   char *s;

   puts("439879889");
   s = fmoney (-439879889, "US$"," ", 1, 22);
   puts (s);
   free(s);

   puts("256899074.45");
   s = fmoney (256899074.45, "$"," ", 1, 22);
   puts (s);
   free(s);

   puts("1327");
   s = fmoney (1327, "$","-", 1, 22);
   puts (s);
   free(s);

   puts("-13278908.897");
   s = fmoney (-13278908.897, "Ch$",".", 1, 22);
   puts (s);
   free(s);

   puts("-13278908.897");
   s = fmoney (-13278908.897, "Ch$",".", 3, 22);
   puts (s);
   free(s);

   puts("-13278908.897");
   s = fmoney (-13278908.897, "Ch$",".", 0, 22);
   puts (s);
   free(s);

   puts("13278908.897");
   s = fmoney (13278908.897, "Ch$",".", 0, 22);
   puts (s);
   free(s);

   puts("89370133");
   s = fmoney (-89370133, "",".", 1, 22);
   puts (s);
   free(s);

   puts("89370133");
   s = fmoney (89370133, "US$","", 0, 22);
   puts (s);
   free(s);

   puts("89370133");
   s = fmoney (8933, "US$"," ", 0, 22);
   puts (s);
   free(s);

  return 0;
}
