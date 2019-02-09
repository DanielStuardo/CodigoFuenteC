#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "booleanos.h"
#define MAXBUFFER 512

char *rtrim(char *linea, char *car, uint16_t size) { 

   size--;   // me salto el '\0'
   char *r, *t;
   uint16_t tsize;

   t = linea;
   t += size;
   while ((*t=='\n' || *t=='\t' || *t=='\r' || *t==*car) && t!=linea) t--;
   if (t==linea) return NULL;
   tsize = t - linea + 1;

   r = (char *) malloc(sizeof(char)*(tsize + 1));
   if (r==NULL) return NULL;
   strncpy(r,linea,tsize);
   r[tsize+1]='\0';
   
/*   while ((*t=='\n' || *t=='\t' || *t=='\r' || *t==*car) && size>=0){
      t--;
      size--;
   }*/   
/*   if (size>0) { 
      size++; 
      r = (char *) malloc(sizeof(char)*(size + 1));
      if (r==NULL) return NULL;
      strncpy(r,linea,size);
      r[size]='\0';
   } else {
      r = NULL;   
      //strcpy(r,"");
   } */
   return r;
}

char *ltrim(char *linea, char *car, uint16_t size) {

   char *t;
   t = linea;

   while ((*t==*car || *t=='\t' || *t=='\r' || *t=='\n') && *t) t++;
   if (!*t) return NULL;

   char *r;
   r = (char *) malloc(sizeof(char)*(size - (t - linea) + 1));
   if (r==NULL) return NULL;
   strcpy(r,t); 
   return r;
} 

char *trim(char *linea, char *car, uint16_t sizel) {
   char *t,*r,*s;
   uint16_t tsize;

   r = linea;
   while ((*r==*car || *r=='\t' || *r=='\r' || *r=='\n' || *r==' ') && *r) r++; 
   if (!*r) return NULL;  // no hay texto, sólo puras weás! 

   s = linea + (sizel-1);
   while ((*s==*car || *s=='\t' || *s=='\r' || *s=='\n' || *s==' ') && s!=linea) s--;     
   if (s==linea) return NULL;  // no hay texto, sólo puras weás!

   tsize = s - r + 1;  // longitud del texto.
   t = (char *) malloc(sizeof(char)*(tsize+1));
   if (t==NULL) return NULL;
   strncpy (t,r,tsize);
   t[tsize+1]='\0';

   return t;

/*   t = rtrim(linea,car,size);
   size = strlen(t);
   r = ltrim(t,car,size);
   s = (char *) malloc(sizeof(char)*(strlen(r)+1));
   strcpy(s,r); 
   return s;*/
}


int main(int argc, char *argv[]){
   
   if ( argc == 1 ) {
      system("clear");
      printf( "TRIM versión 1.0. Elimina caracteres laterales.\n");
      printf( "Programado por (c)Daniel L. Stuardo. 2012.\n");
      printf( "Uso: %s <str:cadena_fuente> -c<str:caracter a eliminar> -m<modo>\n", argv[0] );
      printf( "Ejemplos:\n");
      printf( "   trim \"      Esta linea tiene espacios laterales\" -c\" \" -ml\n");
      printf( "    ==> \"Esta linea tiene espacios laterales\"\n");
      printf( "   trim \"Esta linea tiene espacios laterales.......\" -c\".\" -mr\n");
      printf( "    ==> \"Esta linea tiene espacios laterales\"\n");
      printf( "   trim \"      Esta linea tiene espacios laterales       \" -c\" \" -ma\n");
      printf( "    ==> \"Esta linea tiene espacios laterales\"\n");
      printf( "\nImportante: \n1) cadenas con espacios, deben ir entre comillas.\n");
      printf( "2) Si omite -m elimina los caracteres de ambos lados.\n"); 
      printf( "3) Si omite -c elimina los espacios en blanco.\n");
      printf( "\nEs posible hacer \"pipe\", por ejemplo: \n");
      printf( "\tcat transacciones.txt | trim -c\" \" -ml > salida.txt\n");
      printf( "\nEste programa es para uso público. No cobre por él, no sea \"reptil\".");
      printf( "\nTodavía puede mejorar: escribir a daniel.stuardo@gmail.com\n");  
      return 1 ;

   } else {
    
      uint8_t idx=1;
      char *indice,*csource,*car;
      char modo; 
      char *id;
      BOOL swc,swm,sw; 
      swc.trim = 0;  // hay caracter
      swm.trim = 0;  // hay modo
      sw.trim = 0;   // hay linea de entrada. ausencia=stdin

      while (idx<argc) {
         id = strdup(argv[idx]);
         indice = id;
         if (*indice=='-') { // es un parámetro
            indice++;
            if (*indice=='c') { // buscar
                indice++;
                car = strdup(indice);
                swc.trim=1;
            } else if (*indice=='m') { // reemplazar
                indice++;
                modo = *indice;  // debe ser l, r o a
                swm.trim=1;
            }
         } else {   // es la cadena o está como la weas.
            csource = strdup(indice);
            sw.trim = 1;     // encontré cadena fuente
         }
         ++idx;
      }

      free(id);

      if (!swc.trim) strcpy(car," ");
      if (!swm.trim) modo = 'a';

      char *string;

     // considerar punteros a función para llamarlas
      uint16_t size=0;

      if (!sw.trim) {   // puede que haya entrada por flujo de stdin.

         csource=(char *)malloc(sizeof(char)*MAXBUFFER);  // acepta líneas de hasta 512 caracteres
         if (csource==NULL) {printf ("ERROR1: no hay memoria para TRIM\n"); return 1;}

         time_t comienzo, final;
         comienzo = time( NULL );
  
         if (modo=='l'){
            while (!feof(stdin)){ 
               fgets(csource,MAXBUFFER,stdin);
               size = strlen(csource);
               string = ltrim(csource,car,size);
               if (string==NULL) { 
                  printf ("ERROR2: no hay memoria para TRIM\n");
                  continue;
               }
               fputs(string,stdout);
               putchar('\n');
            }
         } else if (modo=='r') {
            while (!feof(stdin)){ 
               fgets(csource,MAXBUFFER,stdin);
               size = strlen(csource);
               string = rtrim(csource,car,size);
               if (string==NULL) { 
                  printf ("ERROR3: no hay memoria para TRIM\n");
                  continue;
               }
               fputs(string,stdout);
               putchar('\n');
            }
         } else {
            while (!feof(stdin)){ 
               fgets(csource,MAXBUFFER,stdin);
               size = strlen(csource);
               string = trim(csource,car,size);
               if (string==NULL) { 
                  printf ("ERROR4: no hay memoria para TRIM\n");
                  continue;
               }
               fputs(string,stdout);
               putchar('\n');
            }
         }

         final = time( NULL ); 
         printf ("Segundos: %f\n", difftime(final, comienzo));
  
      } else {
         if (modo=='l') {size=strlen(csource); string = ltrim(csource,car,size);}
         else if (modo=='r') {size=strlen(csource); string = rtrim(csource,car,size);}
         else {size=strlen(csource); string = trim(csource,car,size);}

         if (string==NULL) printf ("ERROR5: no hay memoria para TRIM o parámetro en blanco\n");
         else {
            puts(string); 
         }
      } 
      free(csource);
   }

   return 0;
}
