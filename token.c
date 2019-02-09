#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
   todas las posibilidades: t=token, s=string
      STSTST  = 3
      STSTS   = 3
      TSTSTS  = 3
      TSTSTST = 3
*/

char *strtoken(char *linea, char *token, uint16_t indice){
   uint16_t ntok = 0,l;
   char *s;
   char *t;
   char *buf;

   if (indice<=0) return NULL;

   s = linea;
   while ((t=strstr(s,token))!=NULL) {
       if (t-s>1) {
          ntok++;  // salta caracteres repetidos
          if (ntok==indice) {
              l = t - s;
              buf = (char *) malloc (sizeof(char)*(l+1));
              if (buf==NULL) return NULL;
              strncpy(buf,s,l);
              buf[l]='\0'; 
              break;
          }
       }
       ++t;
       s = t;
       if (*t == '\0') break; 
   }  
   // si llego aquí, el indice es mayor que los tokens o es una estructura especial:
   if (ntok==0) {
      if (s==linea || t==NULL) return NULL;
   } else {
      if (t==NULL || *t == '\0') {   // es el último???
         if (indice-ntok == 1 && s!=t){
            buf = (char *) malloc (sizeof(char)*(strlen(s)+1));
            if (buf==NULL) return NULL;
            strcpy(buf,s);  
         } else return NULL;
      }
   } 
   
   return buf;
}

uint16_t strgett(char *linea, char *token){
   uint16_t ntok = 0;
   char *s;
   char *t;

   s = linea;
   while ((t=strstr(s,token))!=NULL) {
       if (t-s>1) ntok++;  // salta caracteres repetidos
       ++t;
       s = t;
       if (!*t) return ntok;
   }  
   if (s==linea) return 0;
   if (*s) ++ntok;  // queda uno más... *s!='\0'   

   return ntok;
}

int main(){

   char *t;
   uint16_t ndx=0;

   printf("Tokens = %d\n",(ndx=strgett("Esta línea será tokenizada burdamente"," ")));
   printf("Tokens = %d\n",(ndx=strgett(" Esta línea será tokenizada burdamente"," ")));
   printf("Tokens = %d\n",(ndx=strgett("Esta línea será tokenizada burdamente       "," ")));
   printf("Tokens = %d\n",(ndx=strgett("     Esta      línea   será tokenizada burdamente     "," ")));
   printf("Tokens = %d\n",(ndx=strgett("          "," ")));

   // ANTES DE PROCESAR UNA LINEA, ES BUENO PASARLA POR UN TRIM...

   ndx=1;
   puts("1");
   while ((t=strtoken("Esta línea será tokenizada burdamente     "," ",ndx++))!=NULL) {
      puts (t);
      free(t);
   }

   ndx=1;
   puts("2");
   while ((t=strtoken("     Esta      línea   será tokenizada burdamente    "," ",ndx++))!=NULL) {
      puts (t);
      free(t);
   }
   puts("3");
   ndx=1;
   while ((t=strtoken("Esta línea será tokenizada burdamente"," ",ndx++))!=NULL) {
      puts (t);
      free(t);
   }

   // procedimiento: 1) obtengo token. Si se asigna a misma variable, usar un puntero auxiliar:
   //     $l, $token, $indexToken /.TOKEN /MOV  

   return 0;
}
