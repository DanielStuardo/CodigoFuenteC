#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *upper(char *linea, uint16_t size){
   char *buffer,*r;
   uint16_t i;
   r = linea;
   buffer=(char *)malloc(sizeof(char)*(size+1));
   if (buffer==NULL) return NULL;

   for (i=0;i<size;i++) {buffer[i] = toupper(*r);r++;} 
   buffer[i]='\0';

   return buffer; 
}

char *lower(char *linea, uint16_t size){
   char *buffer,*r;
   uint16_t i;
   r = linea;
   buffer=(char *)malloc(sizeof(char)*(size+1));
   if (buffer==NULL) return NULL;

   for (i=0;i<size;i++) {buffer[i] = tolower(*r);r++;} 
   buffer[i]='\0';

   return buffer; 
}

// calcula longitud de cadena con caracteres extendidos
uint16_t len(char *linea) {
   uint16_t lenght=0;

   while (*linea) {
      if (*linea<0) linea++;
      lenght++;
      linea++;
   }
   return lenght;
}


int main(){

   char *t="Nadie sabe para quién trabaja, sólo maría!!!";
   puts(upper(t,strlen(t)));
   puts(lower(t,strlen(t)));
   printf("[%s], LEN=%d\n",t,len(t));
   printf("[%s], LEN=%d\n","123456789",len("123456789"));
   printf("[%s], LEN=%d\n","Medio weón salió",len("Medio weón salió"));

   return 0;
}
