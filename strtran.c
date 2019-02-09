/*
   STRTRAN.C
   Reemplaza uno o más subcadenas de una cadena mayor.
   Autor: Daniel L. Stuaro. <<-----------------------------------+
   Fecha: Noviembre de 2012                                      |
                                                                 |
   unsigned char *strtran(source, find, replace, ignore, limit)  |
   donde:                                                        |
   * source: string que será modificado.                         |
   * find: string a buscar.                                      |
   * replace: string que reemplazará al string encontrado.       |
   * ignore: número de matches a ignorar antes de reemplazar.    |
             0=no ignore nada.                                   |
   * limit: número de reemplazos a realizar.                     |
             0=reemplace hasta el final.                         |
                                                                 |
   Eres libre de usar este pedazo de mierda, pero te pediré      |
   lo clásico: si te es útil, menciona al autor original >>------+

   Este código fue mejorado y se puede ver esta mejora en el
   código fuente de XU.

   Dudas o consultas, o si sabes que este código puede
   mejorar, avisa a daniel.stuardo@gmail.com
*/

#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdarg.h>
#include "ftokens.h"

#define MAXBUFFER 512

unsigned char *strtran(unsigned char *linea,
                       unsigned const char *buscar,
                       unsigned const char *reempl,
                       unsigned int ignore,
                       unsigned int limite ){
   uint16_t ntoken;
   uint16_t llinea=strlen(linea);
   uint16_t lbuscar=strlen(buscar);
   uint16_t lreempl=strlen(reempl);
   if ((ntoken = ftokens(linea,buscar,lbuscar))==0) return linea;  // para qué voy a calcular.
   if (limite==0) limite = ntoken;

   unsigned char *t;
   unsigned char *r;
   uint16_t l,i,j;
   size_t size;

   // cálculo del espacio preciso a reservar
   if (lbuscar>=lreempl) {
      size = llinea - ( (lbuscar-lreempl) * ntoken );
   } else  {
      size = llinea + ( (lreempl-lbuscar) * ntoken );
   }
   unsigned char *u;
   u = linea;

   r=(char *)malloc(sizeof(char)*size+2);  // MAXBUFFER
   if (r==NULL) return NULL;

   while (ntoken--){
      t = strstr(u,buscar);
      l = t - u;
      if (l>0)
        strncat(r,u,l);
      if (ignore > 0) {
         strcat (r,buscar);
         ignore--;
      } else {
         if (limite > 0){ // porque si limite==0, limite=ntoken
            strcat (r,reempl);
            limite--;
         } else strcat (r,buscar);
      }
      t += lbuscar;
      u = t;
   }
   strcat(r,u);

   return r;
}


int main(int argc, char *argv[]){

   char *linea="María tenía un corderito...\nLo que María no sabía era que el corderito era loba, y se llamaba María!";
   linea = strtran(linea, "María","Susana",0, 0);  // cambio todos "María" por "Susana"
   if (linea==NULL) {
      printf ("No hay memoria para STRTRAN\n");
      return 1;
   }
   // en estricto rigor, la validación anterior debería
   // hacerse cada vez que invocas la función.
   printf ("\nLINEA: %s\a\n",linea);

   printf ("[%s]\n",strtran(linea, "Susana","Macarena",0,0));
   linea = strtran(linea, "Susana","Macarena",0,0);

   printf ("\n[%s]\n",strtran(linea, "Macarena","María",0,0));
   printf ("[%s]\n",strtran(linea, "Macarena","Clotilde",1,0));
   printf ("[%s]\n",strtran(linea, "Macarena","Chabela",2,0));

   printf ("\n[%s]\n",strtran(linea, "Macarena","Juanita",0, 3));
   printf ("[%s]\n",strtran(linea, "Macarena","Juanita",0, 2));
   printf ("[%s]\n",strtran(linea, "Macarena","Juanita",0, 1));

   printf ("\n[%s]\n",strtran(linea, "Macarena","Jovita",1, 1));
   printf ("[%s]\n",strtran(linea, "Macarena","Jovita",1, 2));

   free(linea); //no olvides esto!

   return 0;
}
