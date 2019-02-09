#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "booleanos.h"

// REVISAR POR DESBORDES DE CADENA:
uint16_t at(char *sub, char *linea, uint16_t pos) {
   uint16_t l=strlen(linea);

   if (l<=pos) return 0;

   char *t,*s;

   s = linea + pos;    // ajusto la posición de búsqueda: innovación
   if (pos>0) s++;     // ajusto "s" por si parte desde el medio
   t = strstr(s, sub);
   if (t==NULL) return 0;
   l = (t - linea) + 1 ;

   // ajusto para caracteres especiales:
   register i;
   s = linea;          // cuento desde el principio
   for (i=0; i<l; i++) {
       if (*s<0) {s++;l--;}
       s++;
   }

   return l;
}

char *substr(char *linea, uint16_t inicio, uint16_t cuenta)
{
   uint16_t l = strlen(linea);
   // ajusto inicio: resto uno
   --inicio;
   if (l<inicio) return "";   // OJO con esto: el que reciba, debe poner ""

   char *buf;
   buf = (char *)malloc(cuenta+1);
   if (buf == NULL) return NULL;

   // evaluar si hay caracteres especiales antes del inicio
   register i;
   char *t;
   t = linea;
   for (i=0; i<inicio; i++) {
       if (*t<0) {inicio++;t++;}
       t++;
   }
   // evaluar si los hay entre el substring
   t = linea+inicio;
   for (i=0; i<cuenta; i++) {
       if (*t<0) {cuenta++;t++;if(*t=='\0') break;}
       t++;
       if(!*t) break;
   }
   strncpy(buf, linea+inicio, cuenta);
   buf[cuenta] = '\0';

   return buf;
}

char *strrep(char *car, uint16_t size){
   char *buf;
   buf = (char *)malloc(sizeof(char) * (size + 1));
   if (buf == NULL) return NULL;

   register i;
   for (i=0;i<size;i++) buf[i]=*car;
   buf[i] = '\0';
   return buf;
}

// inserte en linea, car, en la posición pos
char *strins(char *linea, char *car, uint16_t desde){
   char *t,*s,*buf;
   register i;
   BOOL sw;
   uint16_t size;
   uint16_t lsize = strlen(linea);
   uint16_t csize = strlen(car);

   if (desde>0) desde--;   // ajuste C
   sw.str = 0;
   // calculo el espacio
   if (lsize < desde) {
      size = lsize + (desde - lsize + csize);   // caso extraordinario
      desde = desde - lsize;
      sw.str = 1;
   } else size = lsize + csize;   // caso normal

   buf = (char *)malloc(sizeof(char) * (size + 1));
   if (buf == NULL) return NULL;

   if (sw.str) {
      strcpy(buf,linea);
      t = linea;
      for (i=0; i<lsize; i++) {
          if (*t<0) {desde++;t++;}
          t++;
      }
      strcat(buf,strrep(" ",desde));
      strcat(buf,car);
   } else {
      t = linea;
      if (desde>0){
         for (i=0; i<desde; i++) {
             if (*t<0) t++;
             t++;
         }
         size = t - linea;
         strncpy(buf,linea,size);
         buf[size]='\0';
      }
      strcat(buf,car);
      strcat(buf,t);
   }
   return buf;
}

// en la invocaci{on debo verificar que desde < hasta
char *strcut (char *linea, uint16_t desde, uint16_t hasta){
   char *t,*s,*buf;
   uint16_t l;
   l = strlen(linea);
   if (l<desde) return linea;
   if (l-desde<hasta) hasta = 0;

   buf = (char *)malloc(sizeof(char) * (l - (hasta-desde) + 1));
   if (buf == NULL) return NULL;

   register i;
   t = linea;
   for (i=0; i<desde; i++) {
       if (*t<0) t++;
       t++;
   }

   l = (t - linea)-1;

   strncpy(buf,linea,l);
   buf[l]='\0';
   t--;
   // busco resto de linea no borrable:
   if (hasta>0) {
      for (i=0;i<hasta ;i++) {
         if (*t<0) t++;
         t++;
      }
      strcat(buf,t);
   }

   return buf;
}


int main(){
   char *part, *str = "María teníA un corderíto";
   char *part2;
   int j=0,i=strlen(str);

   part = substr(str, 1, 5);
   if (part==NULL) {printf ("ERROR: no hay memoria para SUBSTR\n");return 1;}
   puts(part); // -> Test one
   part2 = substr(str, 7, 5); //strlen(str));
   puts(part2);
   puts(substr(str,16,strlen(str)));
   uint16_t pos = at("un",str,0);
   printf (" Pos = %d\n",pos);
   puts(substr(str,pos,strlen(str)));
   printf (" POS = %d\n",at("un",str,24));
   puts(substr("Bananero sapbeeee",at("sap","Bananero sapbeeee",0),strlen("Bananero sapbeeee")));
   printf("substr [%s]\n",substr("Escueto",3,7)); 
   printf("substr [%s]\n",substr("Escueto",10,7)); 
   printf("[%s]\n",strcut(str,7,6));
   printf("[%s]\n",strcut("Nadie sabe para quien trabaja",12,5));
   printf("[%s]\n",strcut("Escueto",10,2));   // asignaciones freak
   printf("[%s]\n",strcut("Escuetola",6,2));   // asignaciones freak
   printf("[%s]\n",strcut("Escueto",5,10));   // asignaciones freak
   printf("[%s]\n",strcut("María sapbeee",7,30));   // asignaciones freak
   printf("[%s]\n",strcut("Mario tenía misterio",7,6));
   printf("[%s]\n",strcut("Mario tenía misterio",at("tenía","Mario tenía misterio",0),6));
   printf("[%s]\n[%s]\n","Esta cadena será intervenida",strins("Esta cadena será intervenida","malamente ",18));
   printf("[%s]\n",strrep(":",20)); 
   printf("[%s]\n[%s]\n","Esta cadena es...",strins("Esta cadena es...","genial!!!",30));
   printf("[%s]\n[%s]\n","Esta cádena es...",strins("Esta cádená es...","genial!!!",30));
   printf("[%s]\n[%s]\n","es un bribón",strins("es un bribón","Ernesto ",0));
   printf("[%s]\n[%s]\n","es un bribón",strins("es un bribón","          ",3));
   return 0;
}
