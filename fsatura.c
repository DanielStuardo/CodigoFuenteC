/*
   FSATURA.C
   
   Reemplaza un string con campos de datos.
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
#include <ctype.h>
////#include "ftokens.h"

#define MAXBUFFER  512

/*
  s = fsaturate("hugo,paco,luis", "," ,"$0 es hermano de $1, pero es primo de $2");
      hugo es hermano de paco, pero es primo de luis.
  satura un string con elementos de otro string
*/

uint16_t ftokens(const char *linea, const char *buscar, uint16_t lb) {
   const char *t,*r; // son solo punteros apuntando a la cadena s.

   uint16_t n=0;

   r = linea;  // rescato primera posición
   t = strstr(r,buscar);
   while (t!=NULL) {
      r = t + lb;
      ++n;
      t = strstr(r,buscar);
   }

   return n;
}

char *strpad (const char *linea, uint16_t size, uint16_t sizel, uint16_t codeFun){
   char *t,*buffer;
   const char *s, *r;
   uint16_t tsize,l1,l2,ts, lsizel;
   int i,p,q;

  // acoto el string a padear, por ambos lados (evito llamar a TRIM).
   r = linea;
   while ((*r=='\t' || *r=='\r' || *r=='\n' || *r==' ') && *r) r++;
   if (!*r) return (char *)linea;  // no hay texto, sólo puras weás!

   s = linea + (sizel-1);
   while ((*s=='\t' || *s=='\r' || *s=='\n' || *s==' ') && s!=linea) s--;
   if (s==linea) return (char *)linea;  // no hay texto, sólo puras weás!

   tsize = s - r + 1;  // longitud del texto.
   if (tsize>size) return (char *)linea;

   lsizel = sizel - strlen(linea);

   l1 = ( (size - tsize) + lsizel ) / 2;
   l2 = l1;

   if (l1+l2+tsize < size+lsizel) l2++;   // por si el pad es impar
   ts = l1+l2+tsize;
// asigno espacio (size) para nueva cadena:
   buffer = (char *) calloc(ts+1,sizeof(char));
   //t = (char *) malloc((ts+1) * sizeof(char));
   if (buffer==NULL) return NULL;    // todo termina si no hay memoria.
   t = buffer;
   switch( codeFun ){
      case 0: { // centrado
        p=0;
        while (l1--){
          //strcat(t," ");
          t[p++] = ' ';
        }
        //strncat(t,r,tsize);
        q=0;
        while (tsize--){
             t[p++] = r[q++];
        }
        while (l2--){
         // strcat(t," ");
           t[p++] = ' ';
        }
        t[p] = '\0';
        break;
      }
      case 1: {  // Left
        p=0;
        //strncat(t,r,tsize);
        q=0;
        while (tsize--){
             t[p++] = r[q++];
        }
        i = l1 + l2;
        while (i--){
          //strcat(t," ");
          t[p++] = ' ';
        }
        t[p] = '\0';
        break;
      }
      case 2: {    // Right
        i = l1 + l2;
        p = 0;
        while (i--){
          //strcat(t," ");
          t[p++] = ' ';
        }
        //strncat(t,r,tsize);
        q=0;
        while (tsize--){
             t[p++] = r[q++];
        }
        t[p] = '\0';
        break;
      }
   }
   return buffer;
}

char *fsaturate( const char *tokens, const char *tok, const char *linea){
   char *buffer;
   const char *t, *s;
   char **ltoken;     // lista de tokens
   uint16_t lentok = strlen(tok);
   uint16_t i=0,l, sizei;

   // cuántos tokens tengo?
   uint16_t ntok = ftokens(tokens,tok,lentok);
   if (ntok==0) return NULL;   // no hay tokens
   //ltoken = (char**)malloc( sizeof(char *)*(ntok+2) );
   ltoken = (char**)calloc( ntok+1, sizeof(char *) );
   // a ntok se suma 1, pues acá necesito los tokens, no los separadores, como en strtran.

   if (ltoken==NULL) return NULL;   // no hay memoria

   // obtengo tokens y los guardo en un arreglo para reemplazar después.
   t = tokens;
   while ((s = strstr(t,tok))!=NULL) {
       l = s - t;
       char *temp;
       ltoken[i] = (char*)calloc( (l+1), sizeof(char *) );
       if (ltoken[i]==NULL) {     // limpiar la memoria que ya fue asignada
           //free(ltoken);
           for (l=0;l<=i;++l) free(ltoken[l]);
           free(ltoken);
           return NULL;     // no hay memoria para el arreglo
       }
       temp = ltoken[i];
       //strncpy(ltoken[i], t, l);
       strncpy(temp, t, l);
       t = ++s;
       ++i;
   }

   if (t!=NULL) {           // queda un token más: lo capturo.
       char *temp;
       ltoken[i] = (char*)calloc( (strlen(t)+1), sizeof(char *) );
       if (ltoken[i]==NULL) {     // limpiar la memoria que ya fue asignada
           ///free(ltoken);
           for (l=0;l<=i;++l) free(ltoken[l]);
           free(ltoken);
           return NULL;     // no hay memoria para el arreglo
       }
       temp = ltoken[i];
       //strcpy(ltoken[i], t);
       strcpy(temp, t);
       sizei = i;
   } else sizei = i-1;

   // asigno espacio a la línea objetiva
   ///buf = (char *) malloc (sizeof(char *)*MAXBUFFER); // MAXBUFFER lo asgno por flag "512"
   buffer = (char *) calloc (MAXBUFFER,sizeof(char *));
   if (buffer==NULL) {         // limpiar la memoria que ya fue asignada
       //free(ltoken);
       for (l=0;l<=i;++l) free(ltoken[l]);
       free(ltoken);
       free(buffer);
       return NULL;         // no hay memoria para el arreglo
   }
   // Reemplazo los tokens en la línea objetiva
   uint16_t ncampo=0;
   uint16_t lc=0;
   const char *c;
   char *buf = buffer;


  // strcpy(buf,"");
   t = linea;
   while ((s = strstr(t,"$"))!=NULL) {
       l = s - t;
       strncat(buf,t,l);         // rescato porción de linea sin tocar, antes del token
       c = s+1;                  // desde el supuesto dígito en adelante
       while (isdigit(*c)) c++;  // obtengo el número del campo
       lc = c - s;               // longitud del número
       if (lc>1) {               // es un campo. 1= no es un campo
          char *cindice,*cind;        // para guardar los índices
          cindice = (char *) malloc (sizeof(char *)*(lc+1));
          if (cindice==NULL) {     // limpiar la memoria que ya fue asignada
              ///free(ltoken);
              for (l=0;l<=i;++l) free(ltoken[l]);
              free(ltoken);
              free(buffer);
              return NULL;         // no hay memoria para el arreglo
          }
          cind = cindice;
          strncpy(cind,s+1,lc);   // preparo el indice del arreglo para obtener token
          ncampo = atoi(cind);
          if (ncampo<=sizei) {     // es un campo válido??
             // aniadir aqui el pad, si lo hay
             char * campo = ltoken[ncampo];
             if(*c==':'){  // aqui hay un pad
                c++;  // me lo salto
                char *cPadding;
                const char *cPadToken;        // para guardar los índices
                int w=1;

                while(isdigit(*c)) {c++;++w;}
                lc = c - ( s+w);
                cPadding = (char *) calloc (lc+1,sizeof(char *));
                char * cPad = cPadding;
                strncpy(cPad,(s+w),lc);
                if(toupper(*c) == 'C'){  //0
                   cPadToken=strpad(campo,atoi(cPad),strlen(campo),0);
                }else if(toupper(*c) == 'L'){  //1
                   cPadToken=strpad(campo,atoi(cPad),strlen(campo),1);
                }else if(toupper(*c) == 'R'){  //2
                   cPadToken=strpad(campo,atoi(cPad),strlen(campo),2);
                }else{   // error
                   return NULL;
                }
                strcat(buf,cPadToken);
                c++;
                free(cPadding);

             }else{
                strcat(buf,campo);
             }
          }
          t = c;              // siempre: por si o por no, el campo será eliminado igual
          free(cindice);
       } else {
          strncat(buf,s,1);   // rescato "$" que no es un campo
          t = ++s;            // avanzo un espacio y continúo el proceso
       }
   }
   // rescato lo último que no ha sido rescatado
   strcat (buf,t);
//   buf[strlen(buf)+1]='\0';

   // libero memoria

   for (l=0;l<=i;++l) free(ltoken[l]);
   free(ltoken);

   // deguerbo resultado
   return buffer;
}

char *fLPsaturate( const char *tokens, int* pos, int nPos, const char *linea){
   char *buffer;
   const char *t, *s;
   char **ltoken;     // lista de tokens
   uint16_t i=0,j=0,k=0,l, sizei;

   ltoken = (char**)malloc((nPos+1) * sizeof(char **) );
   // a ntok se suma 1, pues acá necesito los tokens, no los separadores, como en strtran.

   if (ltoken==NULL) return NULL;   // no hay memoria

   // obtengo tokens y los guardo en un arreglo para reemplazar después.
   t = tokens;
   j = 0;
   const char *c;
   c = t;
   while (j<nPos){   //((s = strstr(t,tok))!=NULL) {
       k=0;
       while (k++<pos[j]) c++;
       l = c - t;
       char *temp;
       ltoken[i] = (char*)calloc( (l+1), sizeof(char) );
       if (ltoken[i]==NULL) {     // limpiar la memoria que ya fue asignada
           ///free(ltoken);
           for (l=0;l<=i;++l) free(ltoken[l]);
           free(ltoken);
           return NULL;     // no hay memoria para el arreglo
       }
       temp = ltoken[i];
       //strncpy(ltoken[i], t, l); // guardo con espacio y todo, porque con strpad se eliminan.
       strncpy(temp, t, l);
       t = c;
       ++i;
       ++j;
   }

   if (t!=NULL) {           // queda un token más: lo capturo.
       char *temp;
       ltoken[i] = (char*)calloc( (strlen(t)+1), sizeof(char) );
       if (ltoken[i]==NULL) {     // limpiar la memoria que ya fue asignada
           ///free(ltoken);
           for (l=0;l<=i;++l) free(ltoken[l]);
           free(ltoken);
           return NULL;     // no hay memoria para el arreglo
       }
       temp = ltoken[i];
       //strcpy(ltoken[i], t);
       strcpy(temp, t);
       sizei = i;
   } else sizei = i-1;

   // asigno espacio a la línea objetiva
   //buf = (char *) malloc (sizeof(char *)*MAXBUFFER); // MAXBUFFER lo asgno por flag "512"
   buffer = (char *) calloc (MAXBUFFER,sizeof(char));
   if (buffer==NULL) {         // limpiar la memoria que ya fue asignada
       //free(ltoken);
       for (l=0;l<=i;++l) free(ltoken[l]);
       free(ltoken);
       free(buffer);
       return NULL;         // no hay memoria para el arreglo
   }
   char *buf = buffer;

   // Reemplazo los tokens en la línea objetiva
   uint16_t ncampo=0;
   uint16_t lc=0;

  // strcpy(buf,"");
   t = linea;
   const char *cPadToken;        // para guardar los índices

   while ((s = strstr(t,"$"))!=NULL) {
       l = s - t;
       strncat(buf,t,l);         // rescato porción de linea sin tocar, antes del token
       c = s+1;                  // desde el supuesto dígito en adelante
       while (isdigit(*c)) c++;  // obtengo el número del campo
       lc = c - s;               // longitud del número
       if (lc>1) {               // es un campo. 1= no es un campo
          char *cindice, *cind;        // para guardar los índices
          cindice = (char *) malloc (sizeof(char )*(lc+1));
          //cindice = (char *) calloc (3,sizeof(char *));
          if (cindice==NULL) {     // limpiar la memoria que ya fue asignada
             //free(ltoken);
             for (l=0;l<=i;++l) free(ltoken[l]);
             free(ltoken);
             free(buffer);
             return NULL;         // no hay memoria para el arreglo
          }
          cind = cindice;
          strncpy(cind,s+1,lc);   // preparo el indice del arreglo para obtener token
          ncampo = atoi(cind);
          char * campo = ltoken[ncampo];
          if (ncampo<=sizei) {     // es un campo válido??
             // aniadir aqui el pad, si lo hay
             if(*c==':'){  // aqui hay un pad
                c++;  // me lo salto
                char *cPad,*cPadding;

                int w=1;

                //cPad = (char *) malloc (sizeof(char *)*5);
                while(isdigit(*c)) {c++;++w;}
                lc = c - ( s+w);
                cPadding = (char *) calloc (lc+1,sizeof(char));
                cPad = cPadding;
                strncpy(cPad,(s+w),lc);
                if(toupper(*c) == 'C'){  //0
                   cPadToken=strpad(campo,atoi((const char*)cPad),strlen(campo),0);
                }else if(toupper(*c) == 'L'){  //1
                   cPadToken=strpad(campo,atoi((const char*)cPad),strlen(campo),1);
                }else if(toupper(*c) == 'R'){  //2
                   cPadToken=strpad(campo,atoi((const char*)cPad),strlen(campo),2);
                }else{   // error
                   return NULL;
                }
                strcat(buf,cPadToken);
                c++;
                free(cPadding);
             }else{
                strcat(buf,campo);
             }
          }
          t = c;              // siempre: por si o por no, el campo será eliminado igual
          free(cindice);
       } else {
          strncat(buf,s,1);   // rescato "$" que no es un campo
          t = ++s;            // avanzo un espacio y continúo el proceso
       }
   }
   // rescato lo último que no ha sido rescatado
   strcat (buf,t);
 //  buf[strlen(buf)+1]='\0';

   // libero memoria

   for (l=0;l<=i;++l) free(ltoken[l]);
   free(ltoken);


   // deguerbo resultado
   return buffer;
}

int main(){

   const char *tokens="Juanito Perez,5.334.234-K,Av. El Desguesadero S/N,Pedro Aguirre Cerda,<<fin>>,300.000";
   const char *tokens1="Juanito Perez       5.334.234-K Av. El Desguesadero S/N  Pedro Aguirre Cerda    300.000";
   const char *linea="Yo, $0:30L, RUT n° $1:15R, declaro bajo juramento $5 que soy inocente de los cargo que se me imputan.\nVivo en $2, comuna de $3:25C.\n\tY créanlo, porque soy $0 y gano $$5 !!$4";
   const char *linea2="$0:30L$1:15L$2:25L$3:20L$5:10R";
   const char *linea1="$0:30L$1:15L$2:25L$3:20L$4:10R";
   int pos[5]={20,12,25,20,10};
   char *s,*r,*t;

 //  printf("1) %s\n", fsaturate(tokens, ",", linea));
    s = fsaturate(tokens, ",", linea);
    puts(s);
   free(s);

 //  printf("2) %s\n", fsaturate(tokens, ",", linea2));
    r = fsaturate(tokens, ",", linea2);
    puts(r);
   free(r);

  // printf("3) %s\n", fLPsaturate(tokens1, pos, 5, linea1));
   t = fLPsaturate(tokens1, pos, 5, linea1);
   puts(t);
   free(t);
   return 0;
}
