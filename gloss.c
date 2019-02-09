/*
   GLOSS.C
   
   Desglose de cifras en palabras. Solo acepta hasta dos decimales.
   Sistema Operativo: Linux basado en Debian; OSX 10.8 y superior.
   GCC: 4.2 en adelante.
   Fecha de creación: alguna parte de 2000.
   
   Autor: Daniel Stuardo
   daniel.stuardo@gmail.com
   
   TODO: ejecutar con Valgrind y cambiar malloc por calloc.
         Hay problemas con cifras pequeñas (1.2, 0.56); esto fue resuelto
         en XU (ver su código fuente, al final del archivo XU.PRG), pero
         no en este código.
   
*/
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include "booleanos.h"

#define MAXBUFFER 70

// español
static char *UN[10]={"orec","onu","sod","sert","ortauc","ocnic","sies","eteis","ohco","eveun"};
static char *DE[10]={"","zeid","etniev","atniert","atnerauc","atneucnic","atneses","atnetes","atnehco","atnevon"};
static char *DX[6]={"","ecno","ecod","ecert","ecrotac","ecniuq"};
static char *DW[10]={"","neic","sotneicod","sotneicert","sotneicortauc","sotneiniuq","sotneicies","sotneicetes","sotneicohco","sotneicevon"};
static char *ML[22]={"","lim",")se(nollim#","lim",")se(nollib#","lim",")se(nollirt#","lim",")se(nollirtauc#",
                     "lim",")se(nollitniuq#","lim",")se(nollitxes#","lim",")se(nollitpes#","lim",
                     ")se(nollitco#","lim",")se(nollinon#","lim",")se(nolliced#","lim"};
// inglés
static char *EUN[10]={"orez","eno","owt","eerht","ruof","evif","xis","neves","thgie","enin"};
static char *EDE[10]={"","net","ytnewt","ytriht","ytrof","ytfif","ytxis","ytneves","ythgie","ytenin"};
static char *EDX[10]={"","nevele","evlewt","neetriht","neetruof","neetfif","neetxis","neetneves","neethgie","neetenin"};
static char *EDW[10]={"","derdnuh eno","derdnuh owt","derdnuh eerht","derdnuh ruof","derdnuh evif","derdnuh xis","derdnuh neves","derdnuh thgie","derdnuh enin"};
static char *EML[22]={"","dnasuoht","noillim#","dnasuoht","noillib#","dnasuoht","noillirt#","dnasuoht",
                      "noillirdauq#","dnasuoht","noillitniuq#","dnasuoht","noillitxes#","dnasuoht",
                      "noillitpes#","dnasuoht","noillitco#","dnasuoht","noillinon#","dnasuoht","htnoilliced#","dnasuoht"};

// esto está en español. Falta declararlos en inglés.
static char *MON[23]={"",")s(osep*",    ")s(osep*",   ")se(ralod*",  ")se(los*",   ")se(laer*",  ")s(onaivilob*",
                        ")se(nirolf*", ")se(noloc*", ")se(ravilob*",")s(ercus*",  ")se(lazteuq*",")s(edruog*",
                        ")se(inaraug*",")s(aoblab*", ")s(abodroc*", ")s(aripmel*",")s(orue*",    ")s(arbil*",
                        ")s(olbur*",   ")se(nauy*",  ")s(aipur*",   "ney*"};

static char *CEN[23]={"",")s(ovatnec",")s(omisetnec",")s(ovatnec", ")s(omitnec",")s(ovatnec",")s(ovatnec",
                        ")s(ovatnec", ")s(ovatnec",")s(omitnec",  ")s(ovatnec",")s(ovatnec",  ")s(omitnec",
                        ")s(omitnec",")s(omisetnec",")s(ovatnec", ")s(ovatnec",")s(ovatnec",")s(euqinep",
                        "kepok",    "nef",      "asiap",    "nes"};

// La función recibe un string. Luego, puede ser un double o un int base.
char *obtiene_glosa (const char *numero, uint8_t pos, uint8_t tipo, uint8_t idioma){
   static char *buff;
   const char *p;
   uint8_t len = strlen(numero);
   BOOL sw;
   sw.gloss=0;

   buff = (char *)malloc(sizeof(char)*32);
   if (buff==NULL) return NULL;

   p = numero;
   if (len==1) { // es una cabeza de cifra: 1.245, "1" = cabeza
      if (pos>0) {
         if (*p=='1') {
            if (idioma==0){
               if (pos%2==0) strcat(buff,"nu");
            } else strcat(buff,"eno");
         } else {
            if (idioma==0){
               strcat(buff,UN[*p-'0']);
            }else{
               strcat(buff,EUN[*p-'0']);
            }
         }
//      } else {
//         if (idioma==0){
//            if (tipo==2 || tipo==7) strcat(buff,"nu");
//            else strcat(buff,UN[*p-'0']);
//         } else {
//            strcat(buff,EUN[*p-'0']);
//         }
//      }

      } else {
         if (idioma==0){
            if (tipo==2 || tipo==7) {
               if (*p=='1') strcat(buff,"nu");
               else         strcat(buff,UN[*p-'0']);
            } else strcat(buff,UN[*p-'0']);
         } else {
            strcat(buff,EUN[*p-'0']);
         }
      }

   } else {   // tiene más de un dígito.
      if (len==3) ++p;

      if (*p=='0'){   // número de la forma 104, 306, etc.
         ++p;

         if (*p>'0'){
            if (*p=='1') {
               if (idioma==0){
                  if (tipo==2||tipo==7) strcat(buff,"nu");
                  else if (pos>0) strcat(buff,"nu");
                  else strcat(buff,UN[*p-'0']);
               } else {
                  strcat(buff,EUN[*p-'0']);
               }
            } else {
               if (idioma==0){
                  strcat(buff,UN[*p-'0']);
               } else {
                  strcat(buff,EUN[*p-'0']);
               }
            }
         } else {
            sw.gloss=1;   // no hay decenas ni unidades.
         }
      } else {    // hay dígitos mayores que cero: 34, 129, etc.
         char pp = *p;
         //p++;
         p++;
         if (*p!='0'){    // son como "treinta y seis"
            if (idioma==1){
               if (pp=='1'){
                  strcat(buff,EDX[*p-'0']);
               } else {
                  strcat(buff,EUN[*p-'0']);
                  strcat(buff,"-");
                  strcat(buff,EDE[pp-'0']);
               }
            } else {
            // verificar que sea un número tipo 11, 12..15 o veinticuatro
            if (pp=='1') {
               if (*p>'5') {   // es d16, 17, 18 o 19
                  strcat(buff,UN[*p-'0']);
                  strcat(buff,"iceid");
               } else {        // es 11, 12, 13, 14 o 15
                  strcat(buff,DX[*p-'0']);
               }
            } else if (pp=='2') {  // puede ser 21, 22, 23,
               if (*p=='1') {
                  if (tipo==2||tipo==7) strcat(buff,"nu");
                  else if (pos>0) strcat(buff,"nu");
                  else strcat(buff,UN[*p-'0']);
               }
               else strcat(buff,UN[*p-'0']);

               strcat(buff,"itniev");
            } else {    // es otro
               if (*p=='1') {   // termina en '1'
                  if (pos>0 || tipo==2||tipo==7) {
                     strcat(buff,"nu");
                     strcat(buff," y ");
                  } else {
                     strcat(buff,UN[*p-'0']);
                     strcat(buff," y ");
                  }
               } else {
                  strcat(buff,UN[*p-'0']);
                  strcat(buff," y ");
               }
               strcat(buff,DE[pp -'0']);
            }
            }
         } else {
             if (idioma==0){
                strcat(buff,DE[pp -'0']);
             } else {
                strcat(buff,EDE[pp -'0']);
             }
         }
      }
      // analiza la centena:
      if (len==3) {   // tomo numero
         char c=numero[0];
         if (idioma==0){
            if (sw.gloss) {   // sólo hay centena
               if (c!='0'){
                  strcat(buff,DW[c -'0']);
               }
            } else {  // hay centenas con decenas y/o con unidades
               strcat(buff," ");
               if (c=='1') {
                  strcat(buff,"otneic");
               } else {
                  strcat(buff,DW[c -'0']);
               }
            }
         } else {
            strcat(buff," ");
            strcat(buff,EDW[c -'0']);
         }
      }
   }
   return buff;
}

char *espejo(char *glosa, uint8_t agregade, uint8_t idioma){
   static char *cifra;
   BOOL sw,mill;
   sw.gloss=0;
   mill.gloss=0;

   uint16_t len = strlen(glosa);
   uint8_t suma = (agregade?5:1);
   cifra = (char *)malloc(sizeof(char)*(len+suma));
   if (cifra==NULL) return NULL;
   uint16_t i=len;
   uint16_t j=0;
   uint8_t cta_esp=0;

   if (glosa[i-1]==' ') i--;
   while (i>0){
      if (glosa[i-1]<0) {  // es "ó"
         cifra[j++] = glosa[i-2];
         cifra[j++] = glosa[i-1];
         i -= 2;
      } else {
         if (glosa[--i]=='#') {
            if (!mill.gloss) mill.gloss=1;
            else {
               if (cta_esp==1) {  // no hay cifras entre millones
                  while (glosa[i]!=' ') --i;
                  ++i;
               } else {cta_esp=0;mill.gloss=0;}
            }
         } else ++i;
         if (glosa[--i]=='*') {   // agrega "de"?
            if (agregade && idioma==0) {
               cifra[j++] = 'd';
               cifra[j++] = 'e';
               cifra[j++] = ' ';
            }
         } else ++i; // y se olvida de "*"
         if (glosa[--i]==' ') {
            if (!sw.gloss) {
               if (glosa[i-1]!=',') cifra[j++] = glosa[i];
               if (mill.gloss) {
                  cta_esp++;
                  if (cta_esp>1) {
                     mill.gloss=0;
                     cta_esp=0;
                  }
               }
               sw.gloss=1;
            }
         } else {
            if (sw.gloss) sw.gloss=0;
            cifra[j++] = glosa[i];
         }
      }
   }
   if (cifra[j-1]==' ') cifra[j-1]='\0';
   else cifra[j]='\0';

   return cifra;
}

char *GLOSA(char *numero, uint16_t tipo, uint8_t mon, uint8_t idioma){
   static char *dnum;
   static char *glosa, *terna;
   BOOL sw,de,digit;
   sw.gloss=0;
   de.gloss=0; // agrega "de" antes de "pesos" si la cifra entera termina en "xxxllon(es)"
   digit.gloss=0;  // si aparecen dígitos antes de los millones, agregar "de"
   uint8_t centavos=0;

   dnum = (char *)malloc(sizeof(char)*128);
   if (dnum==NULL) return NULL;

   strcpy(dnum,numero);
   glosa = (char *)malloc(sizeof(char)*1024);
   if (glosa==NULL) return NULL;

//   printf("\nNUMERO PASADO: %s\nDNUM: %s\n",numero,dnum);

   // separación de ternas y de decimal: obtiene glosa de los decimales (o centavos).
   //glosa[0] = '\0';
   centavos=3;
   char *deci = strstr(dnum,".");
      if (strcmp(deci,".00")!=0) {  // hay decimales!!!
         char *gdec = obtiene_glosa(++deci,0,tipo+5,idioma);   // decimales+normal=6, decimal+monetario=7
         if (gdec==NULL) return NULL;
         if (tipo==2) {  // decimal más monetario
            strcat(glosa,idioma==0?CEN[mon]:mon==3?")s(tnec":CEN[mon]);  //")s(ovatnec ");
            strcat(glosa," ");
         }
         strcat(glosa,gdec);
         strcat(glosa,idioma==0?" noc ,":" ,");
         sw.gloss=1;
      }

   if (tipo==2) {   // agrego sufijo por si es pesos, sea entero o decimal.
      strcat(glosa,idioma==0?MON[mon]:mon==3?")s(rallod":MON[mon]);   // aqui se agrega la denominacion en inglés.
   }
   glosa[strlen(glosa)]='\0';
   printf("GLOSA: %s\n",glosa);
   // ternas de la cifra: obtiene glosa de la cifra a la izquierda del punto decimal.
   // por esta razón, si el numero ingresado es entero, se agrega ".00", por el proceso anterior.
   uint16_t len = strlen(dnum) - centavos;
   uint8_t resto = len % 3;
   uint16_t paso = (uint16_t) (len/3);

   terna = (char *)malloc(sizeof(char)*4);
   if (terna==NULL) return NULL;
   uint16_t i=0, j=0;
   uint16_t tpaso=0;

   --len;
   while (tpaso<paso){
      terna[0]='\0';
      j=2;
      for (i=0;i<3; i++)
         terna[j--] = dnum[len-i];
      terna[3]='\0';
      len -= 3;
      // analizo casos especiales los cuales fallan con el algoritmo
      // ejemplo: 1.000, 100.000, 1.000.000, 1.000.000.000,1.000.000.000.000, 1.000.000.000.000.000
      if (strcmp(terna,"000")==0) { // caso especial
         if (!digit.gloss && tpaso>0) de.gloss=1;
         if (tpaso%2==0 && tpaso>0) {
            strcat(glosa," ");
            strcat(glosa,idioma==0?ML[tpaso]:EML[tpaso]);
         }
      } else {
         if (tpaso>0) strcat(glosa," ");
         strcat(glosa,idioma==0?ML[tpaso]:EML[tpaso]);
         strcat(glosa," ");
         strcat(glosa,obtiene_glosa(terna,tpaso,tpaso==0? tipo:0,idioma));
         digit.gloss=1;
      }
      ++tpaso;
   }

   // proceso el resto
   if (resto!=0) {
      j=resto-1;
      terna[0]='\0';
      for (i=0;i<resto;i++)
         terna[j--] = dnum[len-i];
      terna[resto]='\0';
      strcat(glosa," ");
      strcat(glosa,idioma==0?ML[tpaso]:EML[tpaso]);
      strcat(glosa," ");
      strcat(glosa,obtiene_glosa(terna,tpaso,tpaso==0? tipo:0,idioma));
   }
   printf("\nPASO POR GLOSA\n");

   return espejo(glosa, (uint8_t)(de.gloss?1:0), idioma);
}

char *cglosa(char *numero, uint16_t tipo, uint8_t mon, uint8_t idioma){
   char *dnum,*p,*r,*pto; //,*ent,*dec;
//   uint8_t i=0;
   BOOL sw,sw1,sw2;

   //chequeo si es un numero valido, y de paso quito los ceros de más...
   // solo se admiten numeros positivos, por siaca...
   r = numero;  // copio el puntero de "numero" a "r", el puntero "goma"
  // if (*r=='.') {printf("numero mal formado\n");return  NULL;}   // mal formado
   sw.gloss=1;  // para verificar que exista un solo punto
   sw1.gloss=1; // para verificar que no existan mas ceros a la izquierda
   sw2.gloss=0; // por si encuentra un punto
 //  ent = numero;  // atrapo los enteros
 //  dec = numero;  // atrapo decimales
   while (*r != '\0' ){
      if (*r=='\n') {*r='\0';break;}  // esta validacion solo es necesaria
                                      // cuando se usa el programa con un pipe,
                                      // como por ejemplo: cat cifras.txt | ./gloss ...
      else if (!isdigit(*r) && *r!='.') {return NULL;} // no es dígito ni un punto.
      else if (*r=='.'){
         sw2.gloss=1;     // levanto bandera: encontré un punto!
         pto=r;           // marco la posicion del punto para futura validacion.
 //        ++dec;           // avanzo punterito decimales: desde aquí avanza solito
         if (!sw.gloss) return NULL;  // ya había encontrado un punto! chao!
         else sw.gloss=0;  // encontró un punto: bajo la bandera.
      }else if (*r=='0'){  // es un cero a la izquierda, como Lestrade?
         if (sw1.gloss) {  // tengo la bandera en alto: he encontrado puros ceros...
            if (*(r+1)!='.'){  // el caracter que sigue no es un punto??
               ++numero; // quito el cero del numero original
 //              ++ent;    // idem
 //              ++dec;    // idem
            }  // si el que sigue es un punto, debo dejar, al menos, "0.x"
         }
      }else if (isdigit(*r)) {
         sw1.gloss=0; // si es != 0, bajo la bandera
//         if (!sw2.gloss) {  // no he encontrado un punto??
//            ++ent;    // avanzo punterito de enteros.
//            ++dec;    // avanzo punterito de decimales.
//         }else{       // encontré un punto: avanzo punterito de decimales...
//            ++dec;
//         }
      }
      printf("%c ",*r);
      ++r;
   }
   // si encontró un ".", hay que ver que tenga, al menos, 2 digitos decimales
   dnum=(char *)malloc(sizeof(char)*(strlen(numero)+4));
   if (dnum==NULL) return NULL;
   if (sw2.gloss){  // sí, lo encontró...
//      if (pto == numero) {    // agregar un cero a la izquierda:
//         dnum[0]='0';
//         strcat(dnum,numero);
//      }else {
      strcpy(dnum,numero); //}
      strcat(dnum,"00");
   }else{   // no hay decimales: debo agregar ".00", porque sino el algoritmo falla
      strcpy(dnum,numero);
      strcat(dnum,".00");
   }
   dnum[strlen(dnum)]='\0';
   printf("\nnumero final: %s\n",dnum);

//   dnum=(char *)malloc(sizeof(char)*(strlen(r)+2)); // preparo variable de trabajo
//   if (dnum==NULL) return NULL;  // chequeo que tengo recursos de memoria
//   strcat(dnum,numero);            // copio el numero a mi variable


   // chequear si existen los dígitos completos para los decimales admitios (2 dec.)
/*   p=strstr(numero,".");
   if (p==NULL) {   // no hay decimales!
      dnum=(char *)malloc(sizeof(char)*(strlen(numero)+4));
      if (dnum==NULL) return NULL;
      strcpy(dnum,numero);
      strcat(dnum,".00");
      //dnum[strlen(dnum)]='\0';
   } else {   // averiguar si es "." o ".X" o ".XXXX..."
      if (strcmp(p,".")==0) {  // sólo metio un punto el gil
         dnum=(char *)malloc(sizeof(char)*(strlen(numero)+3));
         if (dnum==NULL) return NULL;
         strcpy(dnum,numero);
         strcat(dnum,"00");
         //dnum[strlen(dnum)]='\0';
      } else {
         ++p;  // salto el punto.
         uint8_t len = strlen(p);
         if (len==1) {
            dnum=(char *)malloc(sizeof(char)*(strlen(numero)+2));
            if (dnum==NULL) return NULL;
            strcpy(dnum,numero);
            strcat(dnum,"0");
           // dnum[strlen(dnum)]='\0';
         } else if (len>2) {
            dnum=(char *)malloc(sizeof(char)*strlen(numero)+1);
            if (dnum==NULL) return NULL;
            i=0;
            while (*numero!='.') dnum[i++] = *numero++;
            dnum[i++] = *numero++;
            dnum[i++] = *numero++;
            dnum[i++] = *numero++;
            dnum[i]   = '\0';
         } else {   // todo bien
            dnum=(char *)malloc(sizeof(char)*strlen(numero)+1);
            strcpy(dnum,numero);
            //dnum[strlen(dnum)]='\0';
            //dnum = numero;
         }
      }
   } */

   // chequea que no existan caracteres diferentes a dígitos y a un punto
/*   i=0;
   sw.gloss = 0;
   while (dnum[i]!='\0') {
      if (!isdigit(dnum[i])) {
         if (dnum[i]!='.') {
             printf ("\nCARACTER EXTRAÑO (CGLOSA) %c, pos=%d,  %s\n",dnum[i],i,dnum);
             return NULL;
         } else {
             if (!sw.gloss) sw.gloss=1;
             else {printf ("\nENCONTR╙ MAS DE UN PUNTO. ");
                  return NULL;}
         }
      }
      ++i;
   }
*/
   return GLOSA(dnum,tipo,mon,idioma);
}

int16_t busca_moneda(const char *mon){
   int16_t moneda;
   ++mon;  // chao "-"
   ++mon;  // chao "m"
   moneda = atoi(mon);
   if (moneda>22 || moneda<1) moneda=-1;

   return moneda;
}

int main(int argc, char *argv[]){
   uint16_t tipo;
   char *cnum;
   BOOL cif;
   cif.gloss=0;

   if ( argc == 1 ) {
      system("clear");
      printf( "GLOSS versión 1.0. Glosa de cifras en idioma español e inglés. \"Escala Larga\"\n");
      printf( "Concepción y programación por (c)Daniel L. Stuardo. 2012.\n");
      printf( "Uso: %s [<sw:moneda>] [<sw_idioma>] <cifra_hasta_miles_de_decillones[<punto><dos decimales>]> ...\n", argv[0] );
      printf( "Ejemplos: gloss -m01 -esp 2051250345.45\n");
      printf( "          gloss -esp 12500 -eng 12500\n");
      printf( "          gloss 68400000\n");
      printf( "Importante: Longitud máxima de cifra: 66 enteros, o 69 con un punto y dos decimales\n");
      printf( "\ndonde:\nsw:moneda...\n");
      printf( "\t-m01 : peso / centavos      -m12 : gourde / céntimo\n");
      printf( "\t-m02 : peso / centésimo     -m13 : guaraní / céntimo\n");
      printf( "\t-m03 : dólar / cent         -m14 : balboa / centésimo\n");
      printf( "\t-m04 : sol / céntimo        -m15 : cordoba / centavo\n");
      printf( "\t-m05 : real / centavo       -m16 : lempira / centavo\n");
      printf( "\t-m06 : boliviano / centavo  -m17 : euro / cent\n");
      printf( "\t-m07 : florín / cent        -m18 : libra / penique\n");
      printf( "\t-m08 : colón / centavo      -m19 : rublo / kopek\n");
      printf( "\t-m09 : bolívar / céntimo    -m20 : yuan / fen\n");
      printf( "\t-m10 : sucre / centavo      -m21 : rupia / paisa\n");
      printf( "\t-m11 : quetzal / centavo    -m22 : yen / sen\n");
      printf( "\nsw:idioma...\n");
      printf( "\t-esp  : español\n\t-eng  : english/inglés\n");
  /*    printf( "\nEs posible hacer \"pipe\", por ejemplo: \n");
      printf( "\tcat cifras.txt | ./gloss -m18 -esp | grep -i \"cen\"\n"); */
      printf( "\nEste programa es para uso público. No cobre por él, no sea \"reptiliano\".");
      printf( "\nCualquier 'bug' o sugerencia de moneda, escribir a daniel.stuardo@gmail.com\n");
      return(1);
   } else {
      // chequear argumentos, no importando la posición de estos.
      tipo = 1;  // por omisión: normal
      uint8_t moneda = 0;  // por omisión: sin sentido monetario
      uint8_t idioma = 0;  // 0=español por omisión; 1=inglés
      uint8_t dsize=1;    // tamanio extra del numero pasado
      uint8_t idx=1;
      while (idx<argc) {
//         printf("argv[%d]=%s\n",idx,argv[idx]);
         if (argv[idx][0]=='.') {
            printf("[%s] --> ERROR: Numero mal formado\n",argv[idx]);
         }else{
            if (strcmp(argv[idx],"-esp")==0) idioma = 0;
            else if (strcmp(argv[idx],"-eng")==0) idioma = 1;
            else if (strncmp(argv[idx],"-m",2)==0) {  // es moneda
               if ((moneda = busca_moneda(argv[idx]))<0){
                  printf ("[%s] --> ERROR: Tipo de moneda no válido. Sin moneda\n",argv[idx]);
                  tipo=1; moneda=0;
               }else{ tipo = 2; }
            }else if (argv[idx][0]=='-'){
                  printf ("[%s] --> ERROR: No se aceptan negativos\n",argv[idx]);
            }else if (isdigit(argv[idx][0])) {
               uint16_t size = strlen(argv[idx]);
               if (size>70) {
                  printf ("Longitud de cifra no válida:\n");
                  printf ("máximo: 66 dígitos enteros (999 mil decillones)\n");
                  printf ("        69 dígitos, incluyendo \"punto\" y dos decimales\n");
               }else{
                  cnum=(char *)malloc(sizeof(char)*(size+dsize));
                  if (cnum==NULL) {printf ("ERROR: no hay memoria para GLOSS\n"); return 1;}
                  if (dsize>1){
                     cnum[0]='0';
                     strcat(cnum,argv[idx]);
                  }else strcpy(cnum,argv[idx]);

//                  printf("CNUM=%s\n",cnum);
                  char *string = cglosa(cnum,tipo,moneda,idioma);
                  if (string==NULL) printf ("FATAL: no hay memoria o cifra mal construida: [%s]\n",cnum);
                  else puts (string);
                  free(cnum);
                  free(string);
               }
            }
         }
         ++idx;
      }
/*      char *string;
      if (!cif.gloss) {
         cnum=(char *)malloc(sizeof(char)*70);
         if (cnum==NULL) {printf ("ERROR: no hay memoria para GLOSS\n"); return 1;}
         while (!feof(stdin)){
            fgets(cnum,MAXBUFFER,stdin);
           // AGREGAR VALIDACION POR ".XX"
            puts(cnum);
            string = cglosa(cnum,tipo,moneda,idioma);
            if (string==NULL) {
               printf ("ERROR2: no hay memoria o cifra mal construida: %s\n",cnum);
               continue;
            }
            puts(string);
         }
         free(cnum);
         free(string);
      } else {
         printf("CNUM=%s\n",cnum);
         string = cglosa(cnum,tipo,moneda,idioma);
         if (string==NULL) printf ("ERROR: no hay memoria o cifra mal construida: %s\n",cnum);
         else {
             puts (string);
         }
         free(cnum);
         free(string);
      }*/
   }
   return 0;
}
