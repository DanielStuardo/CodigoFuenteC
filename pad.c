#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *ESP;

// calcula longitud de cadena con caracteres extendidos
uint16_t len(const char *linea) {
   uint16_t lenght=0;

   while (*linea) {
      if (*linea<0) linea++;
      lenght++;
      linea++;
   }
   return lenght;
}

char *strpad (const char *linea, uint16_t size, uint16_t sizel, uint16_t codeFun){
   char *t;
   const char *s, *r;
   uint16_t tsize,l1,l2,ts, lsizel;
   int i;
   
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
   t = (char *) calloc((ts+1),sizeof(char));
   if (t==NULL) return NULL;    // todo termina si no hay memoria.

   switch( codeFun ){
      case 0: { // centrado
        while (l1--){
      	  strcat(t," ");
        }
        strncat(t,r,tsize);
        while (l2--){
      	  strcat(t," ");
        }
        break;
      }
      case 1: {  // Left
        strncat(t,r,tsize);
        i = l1 + l2;
        while (i--){
      	  strcat(t," ");
        }
        break;
      }
      case 2: {    // Right
        i = l1 + l2;
        while (i--){
      	  strcat(t," ");
        }
        strncat(t,r,tsize);
        break;
      }
   }
   return t;   
}

char *strpadc (const char *linea, uint16_t size, uint16_t sizel, uint16_t codeFun){
   char *t;
   const char *s, *r;
   uint16_t tsize,l1,l2,ts, lsizel;
   int i;
   
  // acoto el string a padear, por ambos lados (evito llamar a TRIM).
   r = linea;
   while ((*r=='\t' || *r=='\r' || *r=='\n' || *r==' ') && *r) r++; 
   if (!*r) return NULL;  // no hay texto, sólo puras weás! 

   s = linea + (sizel-1);
   while ((*s=='\t' || *s=='\r' || *s=='\n' || *s==' ') && s!=linea) s--;     
   if (s==linea) return NULL;  // no hay texto, sólo puras weás!

   tsize = s - r + 1;  // longitud del texto.
   if (tsize>size) return NULL;

   lsizel = sizel - len(linea);

   l1 = ( (size - tsize) + lsizel ) / 2;
   l2 = l1;

   if (l1+l2+tsize < size+lsizel) l2++;   // por si el pad es impar
   ts = l1+l2+tsize;
// asigno espacio (size) para nueva cadena:
 //  t = (char *) malloc(sizeof(char)*(ts+1));
   t = (char *) calloc((ts+1),sizeof(char));
   if (t==NULL) return NULL;    // todo termina si no hay memoria.

   while (l1--){
   	  strcat(t," ");
   }
//   strncpy(t,ESP,l1);
//   t[l1+1]='\0';

   strncat(t,r,tsize);
//   t[l1+tsize+1]='\0';
   while (l2--){
   	  strcat(t," ");
   }   
//   strncat(t,ESP,l2);
//   t[ts+1]='\0';

   return t;   
}

char *strpadr (const char *linea, uint16_t size, uint16_t sizel){
   char *s;
   const char *r;
   int i;
   uint16_t tsize=0,l;
 
   if (size<=sizel) return (char*)linea;  // si el pad es menor o igual a la long actual, queda igual
   if (size<=0 || sizel<=0) return NULL;   // parámetros malos. retorna NULL
   
   // recorro la linea para llegar al final con el puntero
   r = linea + (sizel-1);
   while ((*r=='\t' || *r=='\r' || *r=='\n' || *r==' ') && r!=linea) r--;     
   if (r==linea) return NULL; 

   tsize = r - linea + 1;
   l = size - tsize + (sizel - len(linea));
   
// asigno espacio (size) para nueva cadena:
  // s = (char *) malloc(sizeof(char)*(l+tsize+1));
   s = (char *) calloc((l+tsize+1),sizeof(char));
   if (s==NULL) return NULL;    // todo termina si no hay memoria.
   
   while (l--){
   	  strcat(s," ");
   } 
//   strncpy(s,ESP,l);
//   s[l]='\0';

   strncat(s,linea,tsize);
//   s[l+tsize+1] = '\0';

   return s;   
}

// debe recibir la longitus de la línea desde afuera, para no repetir la invocación de la librería strlen...
char *strpadl (const char *linea, uint16_t size, uint16_t sizel){
   char *s;
   const char *r;
   int i,j,n;
 
   if (size<=sizel) return (char *)linea;  // si el pad es menor o igual a la long actual, queda igual
   if (size<=0 || sizel<=0) return NULL;   // parámetros malos. retorna NULL
   
// quito todos los espacios y caracteres raros antes del texto
   r = linea;
   while ((*r=='\t' || *r=='\r' || *r=='\n' || *r==' ') && *r) r++; 
   if (!*r) return NULL;  // no hay texto, sólo puras weás! 

// asigno espacio (size) para nueva cadena:
 //  s = (char *) malloc(sizeof(char *)*(size));
   s = (char *) calloc((size+1),sizeof(char));   
   if (s==NULL) return NULL;    // todo termina si no hay memoria.

// copio primera porción:
   ///strncat(s, r, sizel);
   strcat(s, r);
 //  s[sizel]='\0';
// copio espacios para finalizar el texto:
   i = sizel; //size-len(s);
   //i=sizel-1;
   while (i<size){
      s[i++] = ' ';
   } 
//   strncat(s,ESP,n);
   s[size]='\0';
   return s;   
}

void ConfigESP(uint16_t cantidad){
   int i;

   free(ESP);

   ESP = (char *)malloc(sizeof(char)*(cantidad+1));
   
   for (i=0;i<cantidad;i++) ESP[i]=' ';
   ESP[i]='\0';
}

int main(){

   char *t="Mensaje en una botella";
   char *s;

   // configura ESP para asignar espacios
   ConfigESP(100);

   puts(t);
   s = strpad(t,40,22,1);
   if (s!=NULL) {
      printf("[%s], LEN=%lu\n",s,strlen(s));
      free(s);
   } else puts("Algo falló...");

   printf("[%s]\n",strpad("   Estó es un bluf ",30,strlen("   Estó es un bluf "),0));

   printf("[%s]\n",strpad("   Esto es un bluf ",30,strlen("   Esto es un bluf "),0));
   printf("[%s]\n",strpad("   Estó es un blúf ",30,strlen("   Estó es un blúf "),1));
   printf("[%s]\n",strpad("   Estó es un blúf ",30,strlen("   Estó es un blúf "),2));
   
   printf("\n[%s]\n",strpad("   Esto es un bluf ",30,strlen("   Esto es un bluf "),0));
   printf("[%s]\n",strpad("   Estó es un blúf ",30,strlen("   Estó es un blúf "),1));
   printf("[%s]\n",strpad("   Estó es un blúf ",30,strlen("   Estó es un blúf "),2));
   printf("[%s]\n",strpad(t,30,len(t),0));
   printf("[%s]\n",strpad(t,30,strlen(t),1));
   printf("[%s]\n",strpad(t,30,strlen(t),2));
   printf("[%s]\n",strpad("              ",30,strlen("              "),0));
   printf("[%s]\n",strpad("              ",30,strlen("              "),1));
   printf("[%s]\n",strpad("              ",30,strlen("              "),2));
   printf("[%s]\n",strpad("",30,0,0));
   printf("[%s]\n",strpad("",30,0,1));
   printf("[%s]\n",strpad("",30,0,2));
   
/*   printf("[%s]\n",strpadl("   Estó es un bluf ",30,strlen("   Estó es un bluf ")));
   printf("[%s]\n",strpadl("              ",30,strlen("              ")));

   printf("[%s]\n",strpadr("   Esto es un bluf ",30,strlen("   Esto es un bluf ")));
   printf("[%s]\n",strpadr("   Estó es un blúf ",30,strlen("   Estó es un blúf ")));
   printf("[%s]\n",strpadr(t,30,strlen(t)));
   
   printf("\n[%s]\n",strpadc("   Esto es un bluf ",30,strlen("   Esto es un bluf ")));
   printf("[%s]\n",strpadc("   Estó es un blúf ",30,strlen("   Estó es un blúf ")));
   printf("[%s]\n",strpadc(t,30,len(t)));
   s = strpadc(t,10,22);
   if (s==NULL) printf("Última frase fue NULL\n");
   else {  puts(s);
           free(s);
   } */

/*   t = (char *)malloc(sizeof(char)*30);
   strcpy(t,"Este mensaje será alterado");

   puts(t);  // esta secuencia indica la asignación de una función str desed hacia la misma variable: s = f(s);
   s = f(t);  // s es un puntero temporal, de paso.
   free(t);
   puts(s);
   t = s;
   puts(t); */

   return 0;
}
