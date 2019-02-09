/*
   DATETIME.C
   
   Grupo de funciones para procesar fechas y horas.
   Sistema Operativo: Linux basado en Debian; OSX 10.8 y superior.
   GCC: 4.2 en adelante.
   Fecha de creación: alguna parte de 2000.
   
   Autor: Daniel Stuardo
   daniel.stuardo@gmail.com
   
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

// TODO: ( OK ) funcion para descomponer fecha en numeros. dejar en stack y /DATDIFF recupera de ahí.
// TODO: ( OK ) funcion para componer numeros en fechas (string)
// TODO: ( OK ) sumar fechas: se puede hacer con datediff. 
// TODO: ( OK ) restar horas. Ejemplo: difftime ("23:12:30","04:12:30") --> 5,0
// TODO: ( OK ) timeadd("23:12:30","05:30:00",&dias,&h,&m,&s) --> 0 dias, 04 h, 42 m, 30 s 
// TODO: ( OK ) timelock ("23:12:30",5,30,0) --> "04:42:30" 
// TODO: resta timesub ("23:12:30",3,0,0,&dias) --> "20:12:30" dia=0
// TODO: ( OK ) llevar segundos a horas
// TODO: ( OK ) llevar horas a segundos
// TODO: lleva un número decimal a horas

int tabla[2][13] = {
    {0,31,28,31,30,31,30,31,31,30,31,30,31},
    {0,31,29,31,30,31,30,31,31,30,31,30,31}
};

static char *nombredias[8] = {"","Domingo","Lunes","Martes","Miércoles","Jueves","Viernes","Sábado"};

// determina si es bisiesto:
short isleap (int year)
{
  return (year%4==0) && ( (year%100!=0) || (year%400==0) );
}

// valida si la fecha esta correcta
short vdate(short day, short month, int year)
{
  int leap;

  if ( (day<=0) || (month<=0 || year<0) ) return 0;

  leap=isleap(year); 
  return (day<=tabla[leap][month]);
}

short isrange(int num, int r1, int r2, char *mod){
   char der=mod[1], izq=mod[0];

   if (izq=='[') --r1;
   if (der==']') ++r2;
   return num>r1 && num<r2?1:0;
}

// puede funcionar así: $num /ISRANGE [0,23] 
short vtime(int hor, int min, int sec){
  return (!isrange(hor,0,23,"[]") || !isrange(min,0,59,"[]") || !isrange(sec,0,59,"[]"))?0:1;
}

void dateint(char *fecha, int *dia, int *mes, int *ano) {
   int i;
   *dia=0;
   *mes=0;
   *ano=0;

   for (i=0;fecha[i]!='/'; i++) *dia = 10 * (*dia) + (fecha[i] - '0');
   for (i=3;fecha[i]!='/'; i++) *mes = 10 * (*mes) + (fecha[i] - '0');
   for (i=6;fecha[i]!='\0'; i++) *ano = 10 * (*ano) + (fecha[i] - '0');

   if (!vdate(*dia,*mes,*ano)) {printf("ERROR dateint");*dia=0; *mes=0; *ano=0;}
}

char *intxtime(int hor, int min, int sec){

   char *buffer;
   buffer = (char *)malloc(sizeof(char)*9);
   if (buffer==NULL) {printf ("ERROR inttime");return "";}

   sprintf (buffer,"%d:%02d:%02d",hor,min,sec);
   return buffer;
}

char *inttime(int hor, int min, int sec){

   if (!vtime(hor,min,sec)) {printf("ERROR inttime");return "";}

   char *buffer;
   buffer = (char *)malloc(sizeof(char)*9);
   if (buffer==NULL) {printf ("ERROR inttime");return "";}

   sprintf (buffer,"%02d:%02d:%02d",hor,min,sec);
   return buffer;
}

char *intdate(short dia, short mes, int ano){

   if (!vdate(dia,mes,ano)) {printf("ERROR intdate");return "";}

   char *buffer;
   buffer = (char *)malloc(sizeof(char)*11);
   if (buffer==NULL) {printf ("ERROR intdate");return "";}

   sprintf (buffer,"%02d/%02d/%04d",dia, mes, ano);
   return buffer;
}

void timeint(char *tiempo, int *hor, int *min, int *sec, char *tipo) {
   int i,j,k;
   *hor=0;
   *min=0;
   *sec=0;

   for (i=0;tiempo[i]!=':'; i++) *hor = 10 * (*hor) + (tiempo[i] - '0');
   for (j=i+1;tiempo[j]!=':'; j++) *min = 10 * (*min) + (tiempo[j] - '0');
   for (k=j+1;tiempo[k]!='\0'; k++) *sec = 10 * (*sec) + (tiempo[k] - '0');

   if (toupper(tipo[0])=='E') {   // solicito hora real y no sumandos
      if (!vtime(*hor,*min,*sec)) {printf("ERROR timeint");*hor=0; *min=0; *sec=0;}
   }
}

int tround(float num){

   int n=(int)num;

   float f = ((num-(int)num)*10);
   if ((int)f>=5) ++n;
   return n;
}

float time2num(char *hora) {
   int hor,min,sec;
   timeint(hora,&hor,&min,&sec,"X");
  
// primero: normalizo la hora a su equivalente real. 
   //int csec = (int)(sec/60);   //tround((float)((float)sec/60));
   float csec = (sec/60);
   sec %= 60;
   //min += csec;
   min += tround(csec); 
   //int cmin = (int)(min/60);
   float cmin = (min/60); 
   min %= 60;
   //hor += cmin;
   hor += tround(cmin);
// luego, obtengo su versión numérica:
   sec = (float)(tround ( ((float)sec*100)/60 ) );
   min = (float)(tround ( ((float)min*100)/60 ) );
   
   return (float)( hor+(float)( (((float)min*100)+(float)sec)/10000) );  
}

// 7,25 = 25/100 = x/60; 25*60/100 = 15 min; 7:15
char *num2time(float num){
   int inum=(int)num;
   float imin=(float)((num - inum)*100);
   int fnum=(int) imin;
   int min = tround((float)(((float)fnum * 60)/100));
   float isec=(float)((imin - (int)imin) * 100);
   int sec = tround((float) ((float)isec * 60 / 100));
   
   return intxtime(num,min,sec); 
} 

char *sec2time(unsigned int sec){
   char *buffer;
   buffer = (char *)malloc(sizeof(char)*9);
   if (buffer==NULL) {printf ("ERROR sec2time");return "";}

   unsigned int hor=sec/3600;
   sec%=3600;
   unsigned int min=sec/60;
   sec%=60;
   sprintf (buffer,"%02d:%02d:%02d",hor,min,sec);
   
   return buffer;
}

unsigned int time2sec(char *tiempo){
   unsigned int hor, min, sec;

   timeint (tiempo, &hor, &min, &sec,"X");
   sec=hor*60*60+min*60+sec;
   return sec;
}

// resto dos horas y el resultado es en segundos, min u horas
float timediff(char *horaf, char *horai, char *tipo){

   int horf, hori, minf, mini, secf, seci;
   timeint(horaf, &horf, &minf, &secf,"E"); // E= deben ser horas reales
   timeint(horai, &hori, &mini, &seci,"E"); //    idem...
   if (horf<hori) horf += 24; 

   float resp = (float) (horf*60*60+minf*60+secf)-(float) (hori*60*60+mini*60+seci);

   char c=toupper(tipo[0]);
   if (c=='M') resp /= 60;
   else if (c=='H') resp = (resp/60)/60; 
   return resp;
}

// días y horas, minutos y segs equivalentes al tiempo pasado por parametro. ej: 360 seg = 0 dias, 6 min.
void timeday (char *hora2, int *dias, int *h, int *m, int *s){
   int chor, cmin, csec; // hora origen
   
   csec = time2sec(hora2);  // transformo la hora a sumar a segundos y sumo
   cmin = (int)(csec/60);   // cantidad de minutos equivalentes a csec...
   *s = csec%60;             // de donde quedan segundos remanentes.
   chor = (int)(cmin/60);   // mismo tratamiento
   *m = cmin%60;             // minutos remanentes
   *dias = (int)(chor/24);   // total días transcurridos 
   *h = chor%24;             // horas remanentes
}

// resto horas, min o seg a la hora dada.
char *timesub(char *hora, int hor, int min, int sec){
   unsigned int chor, cmin, csec;

   unsigned int secs = (hor % 24)*60*60+min*60+sec;
   int secr, sect=time2sec(hora);
   if (secs>sect) secr = (int) (secs-sect);
   else secr = (int) (sect-secs);
   timeint(sec2time(secr),&chor,&cmin,&csec,"X");
   chor %= 24;

   return inttime(chor,cmin,csec); 
}


// suma a la hora indicada, una cantidad ilimitada de horas, minutos y segundos. El resultado se expresa en
// la hora final. Ejemplo: si a las 22 hrs le sumamos 360 minutos, queda las 4 de la mañana. No indica nada
// respecto a los días transcurridos.
char *timeadd (char *hora, int hor, int min, int sec){ 

   unsigned int chor, cmin, csec;

   unsigned int secs = (hor % 24)*60*60+min*60+sec;
   unsigned int secr = time2sec(hora)+secs;
   timeint(sec2time(secr),&chor,&cmin,&csec,"X");
   chor %= 24;

   return inttime(chor,cmin,csec); 
}

void dateadd(int *df, int *mf, int *yf, int dias) {
   
   if (!vdate(*df,*mf,*yf) || dias<0) printf ("ERROR dateadd"); 
   int i=1,td;

   td=tabla[isleap(*yf)][*mf];
   while (i<=dias) {
      if (*df==td) {      // analizar problemas...
         ++(*mf);          // cae el mes anterior
         if (*mf==13) {   // era enero...
            *mf=1;      // cae el año anterior
            ++(*yf);
         }
         td=tabla[isleap(*yf)][*mf];
         *df=1;  //tabla[isleap(*yf)][*mf];   // obtengo total de días del mes anterior
      } else {
         ++(*df);   // sólo resta un día miserable 
      }
      ++i;
   }
}

void datesub(int *df, int *mf, int *yf, int dias) {
   
   if (!vdate(*df,*mf,*yf) || dias<0) printf ("ERROR datesub"); 
   int i=1;

   while (i<=dias) {
      if (*df==1) {      // analizar problemas...
         --(*mf);          // cae el mes anterior
         if (*mf==0) {   // era enero...
            *mf=12;      // cae el año anterior
            --(*yf);
         }
         *df=tabla[isleap(*yf)][*mf];   // obtengo total de días del mes anterior
      } else {
         --(*df);   // sólo resta un día miserable 
      }
      ++i;
   }
}


unsigned int datediff(int df, int mf, int yf, int di, int mi, int yi, char *tipo) {

   char ctipo=toupper(tipo[0]);
   int rdias;

   // validar fechas antes que cualquier cosa...
   if (!vdate(di,mi,yi) || !vdate(df,mf,yf)) {printf ("ERROR datediff");return 0;}

   // veo si es la misma fecha:
   if (di==df && mi==mf && yi==yf) return 0;

   // veo si el orden de las fechas está cambiado, pues, no falta el genius:
   if (yi>yf) {  // intercambio, pues, asumo que la fecha está al revés!!
      int td=di, tm=mi, ty=yi;
      di=df; mi=mf; yi=yf;
      df=td; mf=tm; yf=ty;  // listo. 
   } else if (yi==yf) {   // supongamos que metió mal la fecha, dentro del mismo año
      if (mi==mf) {       // mismo año, mismo mes...
         if (di>df) {     // ahueonao, días cambiados!!!
            int td=di;
            di=df;
            df=td;
         }
      } else if (mi>mf) {  // ahueonao, mismo año, meses cambiados... Se asume que los días también
         int td=di, tm=mi; // porque sólo un bruto galardonado pondría el día correcto y el mes cambiado.
         di=df; mi=mf;     // y a esas personas hay que hacerlas sufrir... ñacañacañaca!!
         df=td; mf=tm;   // listo. 
      }
   }
   
   // obtengo días faltantes para terminar el primer mes.
   int diasi = tabla[isleap(yi)][mi]-di+1;  // días en extremo inicial, contando el mismo día inicial

   if (yi==yf) {
      if (mi==mf) {
         rdias = (df-di)+1;
         if (ctipo=='D') return rdias;   // quiere los días
         else if (ctipo=='H') return rdias*24;   // horas
         else if (ctipo=='M') return rdias*1440;
         else if (ctipo=='S') return rdias*86400;
         return rdias;   // quiere los días por defecto
      } else if (mf-mi == 1) {
         rdias = df+diasi;
         if (ctipo=='D') return rdias;   // quiere los días
         else if (ctipo=='H') return rdias*24;   // horas
         else if (ctipo=='M') return rdias*1440;
         else if (ctipo=='S') return rdias*86400;
         return rdias;   // quiere los días por defecto
      }
   }

   // debo restar 1 al dia final, porque recién empieza a las 0 horas y aún no ha terminado
   if (df==1) {      // analizar problemas...
      --mf;          // cae el mes anterior
      if (mf==0) {   // era enero...
         mf=12;      // cae el año anterior
         --yf;
      }
      df=tabla[isleap(yf)][mf];   // obtengo total de días del mes anterior
   } else {
      --df;   // sólo resta un día miserable 
   }

   if (di==df && mi==mf && yi==yf) {  // sólo hay un dia de diferencia
      if (ctipo=='D') return 1;   // quiere los días
      else if (ctipo=='H') return 24;   // horas
      else if (ctipo=='M') return 1440;
      else if (ctipo=='S') return 86400;
      return rdias;   // quiere los días por defecto
   }

   // los días son diferentes. ahora preparo el cálculo, mijo
   rdias = df+diasi;  // los días extremos

   // como ya obtuve los días del primer mes, no lo cuento, así es que ajusto el mes.
   int tmes = mi+(mi==12?-11:1);  // ajusta por si es diciembre
   int tano = yi+(mi==12?1:0);    // ajusta el año, por lo mismo

   // sumo días de los meses desde el inicio hasta el final, menos 1.
   while (tano<=yf) {
      if (tano==yf) {
         if (tmes==mf) break;
      }
      rdias += tabla[isleap(tano)][tmes++];
      if (tmes==13) {++tano; tmes=1;}
   }
   // acomodo el resultado a lo que quiera la caserita o el mandado:
      
   if (ctipo=='D') return rdias;   // quiere los días
   else if (ctipo=='H') return rdias*24;   // horas
   else if (ctipo=='M') return rdias*1440;
   else if (ctipo=='S') return rdias*86400;
   return rdias;   // por defecto, retorna días.
}

unsigned int cdatediff(char *fechafin, char *fechaini, char *tipo){

  int diaf, mesf, anof;
  int diai, mesi, anoi;

   dateint(fechafin, &diaf, &mesf, &anof);
   dateint(fechaini, &diai, &mesi, &anoi);
   return datediff(diai,mesi,anoi,diaf,mesf,anof,tipo);
}

// retorna el día del año a partir del mes y el año
int doy (int day, int month, int year) {
   int i, leap;
   leap=isleap(year);
   for (i=1;i<month; i++) day += tabla[leap][i];
   return day;
}

int adoy (){
   time_t hora;
   struct tm *tiempo;  

   hora = time(NULL);
   tiempo = localtime(&hora);
 
   return tiempo->tm_yday+1;
}

// obtiene mes y día a partir del día del año
void mday (int year, int yearday, int *pmonth, int *pday){
   int i,leap;
   leap=isleap(year);
   for (i=1; yearday>tabla[leap][i]; i++) yearday -= tabla[leap][i];
   *pmonth = i;
   *pday = yearday;
}

char *cmonth(int month){
   static char *nombre[] = {
        "","Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre",
        "Octubre","Noviembre","Diciembre"};
   return (month<1 || month>12)? nombre[0]:nombre[month];  
}

// algoritmo que calcula el numero del día de la semana, desde 1=domingo hasta 7=sabado, de cualquier fecha
int zeller(int day, int month, int year) {
   int h, k, j;

   if (month<=2){
      month +=12;
      year -=1;
   } else month -=2;

   k=year%100;
   j=year/100;

   h = ((700+((26 * month -2) / 10)+day+k+(k/4)+((j/4)+5*j)) % 7);
   
   return h+1;
}

int nday (int day, int month, int year) {
   return vdate(day,month,year)?zeller(day,month,year):0;
}

char *cday(int day, int month, int year) {
   return vdate(day,month,year)?nombredias[zeller(day,month,year)]:nombredias[0];
}

char *acday() {
   time_t hora;
   struct tm *tiempo;  

   hora = time(NULL);
   tiempo = localtime(&hora);
 
   return nombredias[tiempo->tm_wday+1];
}

int anday(){  // obtiene el día de la semana de la fecha actual
   time_t hora;
   struct tm *tiempo;  

   hora = time(NULL);
   tiempo = localtime(&hora);
 
   return tiempo->tm_wday+1;
}

void gettime(int *hor, int *min, int *sec) {
   time_t hora;
   struct tm *tiempo;

   hora = time(NULL);
   tiempo = localtime(&hora);

   *hor=tiempo->tm_hour; *min=tiempo->tm_min; *sec=tiempo->tm_sec;
}

void getdate(int *dia, int *mes, int *ano) {
   time_t hora;
   struct tm *tiempo;

   hora = time(NULL);
   tiempo = localtime(&hora);

   *dia=tiempo->tm_mday; *mes=tiempo->tm_mon+1; *ano=tiempo->tm_year+1900;
}

// Obtiene la hora actual y la devuelve como un string "hh:mm:ss"
unsigned char *cgettime() {
   time_t hora;
   struct tm *tiempo;
   char *fecha;

   hora = time(NULL);
   tiempo = localtime(&hora);

   fecha = (char *)malloc(sizeof(char)*9);
   sprintf (fecha,"%02d:%02d:%02d",tiempo->tm_hour,tiempo->tm_min,tiempo->tm_sec);
   return fecha;
}

// Obtiene la fecha actual (local) y la devuelve como un string "dd/mm/aaaa" 
unsigned char *cgetdate() {
   time_t hora;
   struct tm *tiempo;
   char *fecha;

   hora = time(NULL);
   tiempo = localtime(&hora);

   fecha = (char *)malloc(sizeof(char)*11);
   sprintf (fecha,"%02d/%02d/%4d",tiempo->tm_mday,tiempo->tm_mon+1,tiempo->tm_year+1900);
   return fecha;

}

void main(void){
 
   printf ("día del año de 8-10-2012: %d\n", doy(8,10,2012));
   int m;
   int d;
   mday(2012,doy(8,10,2012),&m,&d);
   printf ("mes y día del día %d del año: mes=%d, dia=%d\n",doy(8,10,2012),m,d);
   printf ("Nombre del mes %d, %s\n",m,cmonth(m));
   printf ("Nombre del mes 14, %s\n",cmonth(14));

   printf ("Es valida esta fecha? 23/11/-1901 %s\n",vdate(23,11,-1901)?"Si":"no");
   printf ("Es valida esta fecha? 34/8/2013 %s\n",vdate(34,8,2013)?"Si":"no");
   printf ("Es valida esta fecha? 29/2/2000 %s\n",vdate(29,2,2000)?"Si":"no");
   printf ("Es valida esta fecha? 29/2/2001 %s\n",vdate(29,2,2001)?"Si":"no");

   printf ("Número de día de la semana actual: %d\n",anday());
   printf ("Nombre de día de la semana actual: %s\n", acday());
   printf ("Número de dia del año fecha actual: %d\n",adoy());
   printf ("Número de dia del año fecha actual: %d\n",doy(10,10,2012));
   printf ("Número de dia del año 31/12/2012: %d\n",doy(31,12,2012));
   printf ("Número de dia del año 31/12/2011: %d\n",doy(31,12,2011));

   printf ("Número de día de la semana 9/10/2012: %d\n", nday(9,10,2012));
   printf ("Nombre de día de la semana 9/10/2012: %s\n", cday(9,10,2012));

   printf ("Número de día de la semana 19/9/2004: %d\n", nday(19,9,2004));
   printf ("Nombre de día de la semana 19/9/2004: %s\n", cday(19,9,2004));

   printf ("Número de día de la semana 30/2/2004: %d\n", nday(30,2,2004));
   printf ("Nombre de día de la semana 30/2/2004: %s\n", cday(30,2,2004));
   printf ("CGETTIME: %s\n",cgettime());
   printf ("CGETDATE : %s\n",cgetdate());
   
   int hor, min, sec;
   gettime(&hor,&min,&sec);
   printf ("GETTIME: %d : %d : %d\n",hor,min,sec);
   int dia=0, mes=0, ano=0;
   getdate(&dia,&mes,&ano);
   printf ("GETDATE: %d : %d : %d\n",dia,mes,ano);

   getchar();

   // resta de fechas: resultado en días(D), horas(H), minutos(m) o
   // segundos(s), o,
   
   printf ("DATEDIFF: 5/3/2010, 10/12/2012  = %d\n",datediff (10,12,2012,5,3,2010,"D"));
   printf ("DATEDIFF: 5/3/2011, 5/3/2012    = %d\n",datediff (5,3,2012,5,3,2011,"D"));
   printf ("DATEDIFF: 1/10/2012, 31/10/2012 = %d\n",datediff (31,10,2012,1,10,2012,"D"));
   printf ("DATEDIFF: 1/4/2011, 30/4/2011 = %d\n",datediff (1,4,2011,30,4,2011,"D"));
   printf ("DATEDIFF: 28/8/2010, 28/8/2011 = %d\n",datediff (28,8,2011,28,8,2010,"M"));
   printf ("DATEDIFF: 27/9/2012, 9/10/2012 = %d, horas:%s\n",datediff (9,10,2012,27,9,2012,"D"),
                                                    sec2time(datediff (9,10,2012,27,9,2012,"S")));
   printf ("DATEDIFF: 27/2/2012, 9/3/2012 = %d\n",datediff (9,3,2012,27,2,2012,"D"));
   printf ("DATEDIFF: 27/2/2011, 9/3/2011 = %d\n",datediff (9,3,2011,27,2,2011,"D"));
   printf ("DATEDIFF: 27/2/2011, 29/2/2012 = %d, %d hrs, %d min, %d segs; horas: %s\n",
                                                   datediff (27,2,2011,29,2,2012,"D"),
                                                   datediff (27,2,2011,29,2,2012,"H"),
                                                   datediff (27,2,2011,29,2,2012,"M"),
                                                   datediff (27,2,2011,29,2,2012,"S"),
                                                   sec2time(datediff (27,2,2011,29,2,2012,"S")));
   
   printf ("CDATEDIFF: 27/2/2011, 29/2/2012 = %d días\n",cdatediff("29/2/2012","27/2/2011","D"));
   int tdia=0, tmes=0, tano=0;
   char fecha[11];
   strcpy(fecha, cgetdate());
   dateint(fecha,&dia,&mes,&ano);
   printf ("DATEINT: \"%s\" = %d/%d/%d\n",fecha,dia,mes,ano);
   tdia=dia; tmes=mes; tano=ano;
   datesub(&dia,&mes,&ano,12);
   printf ("DATESUB: [%s] - 12 días = %d/%d/%d\n", fecha, dia, mes,ano);

   tdia=dia; tmes=mes; tano=ano;
   datesub(&dia,&mes,&ano,365);
   printf ("DATESUB: 28/9/2012 - 365 días = %d/%d/%d\n", dia, mes,ano);

   dia=28; mes=9; ano=2010;
   datesub(&dia,&mes,&ano,365);
   printf ("DATESUB: 28/9/2010 - 365 días = %d/%d/%d\n", dia, mes,ano);

   dia=28; mes=9; ano=2012;
   dateadd(&dia,&mes,&ano,15);
   printf ("DATEADD: 28/9/2012 + 15 días = %d/%d/%d\n", dia, mes,ano);
   dia=13; mes=10; ano=2012;
   dateadd(&dia,&mes,&ano,365);
   printf ("DATEADD: 13/10/2012 + 365 días = %d/%d/%d\n", dia, mes,ano);

   dia=13; mes=10; ano=2010;
   dateadd(&dia,&mes,&ano,365);
   printf ("DATEADD: 13/10/2010 + 365 días = %d/%d/%d\n", dia, mes,ano);

   getchar();
   
   char tiempo[9];
   strcpy(tiempo, cgettime());
   timeint(tiempo,&hor,&min,&sec,"E");  // "E" se trata de una hora real.
   printf ("TIMEINT: \"%s\" = %d:%d:%d\n",tiempo, hor,min,sec);

   strcpy(fecha,cgetdate());
   dateint(fecha,&dia,&mes,&ano);
   printf ("DATEINT: \"%s\" = %d/%d/%d\n",fecha, dia,mes,ano);

   printf ("INTDATE: %d / %d / %d --> \"%s\"\n",dia,mes,ano,intdate(dia,mes,ano));
   printf ("INTTIME: %d : %d : %d --> \"%s\"\n",hor,min,sec,inttime(hor,min,sec));
   strcpy(tiempo,"23:12:30");

   printf ("TIMEADD: \"%s\" + 00:15:00 = %s\n",tiempo,timeadd(tiempo,0,15,0)); 
   printf ("TIMEADD: \"%s\" + 12:00:00 = %s\n",tiempo,timeadd(tiempo,12,0,0));
   printf ("TIMEADD: \"%s\" + 00:300:00 = %s\n",tiempo,timeadd(tiempo,0,300,0));
   printf ("TIMEADD: \"%s\" + 14:300:340 = %s\n",tiempo,timeadd(tiempo,14,300,340));
   printf ("TIMEADD: \"%s\" + 250:0:0 = %s\n",tiempo,timeadd(tiempo,250,0,0));
   printf ("TIMESUB: \"%s\" - 00:300:0 = %s\n",tiempo,timesub(tiempo,0,300,0));
   printf ("TIMESUB: \"%s\" - 00:15:00 = %s\n",tiempo,timesub(tiempo,0,15,0)); 
   printf ("TIMESUB: \"%s\" - 12:00:00 = %s\n",tiempo,timesub(tiempo,12,0,0));
   printf ("TIMESUB: \"%s\" - 14:300:340 = %s\n",tiempo,timesub(tiempo,14,300,340));
   printf ("TIMESUB: \"%s\" - 19:5:40 = %s\n",tiempo,timesub(tiempo,19,5,40));
  // Observaciones: 
   printf ("TIMESUB: \"%s\" - 250:0:0 = %s\n",tiempo,timesub(tiempo,250,0,0));
   printf ("TIMESUB: \"%s\" - 23:50:40 = %s\n",tiempo,timesub(tiempo,23,50,40));

   timeday ("8808:00:00", &dia, &hor, &min, &sec);
   printf ("TIMEDAY: 8808:00:00 = %d dias, %d hs, %d m, %d s\n",dia,hor, min,sec);   
   timeday ("00:00:59", &dia, &hor, &min, &sec);
   printf ("TIMEDAY: 00:00:59 = %d dias, %d hs, %d m, %d s\n",dia,hor, min,sec);  
   timeday ("00:525623:00", &dia, &hor, &min, &sec);
   printf ("TIMEDAY: 00:525623:00 = %d dias, %d hs, %d m, %d s\n",dia,hor, min,sec);  
   timeday ("00:00:360", &dia, &hor, &min, &sec);
   printf ("TIMEDAY: 00:00:360 = %d dias, %d hs, %d m, %d s\n",dia,hor, min,sec);  

   timeday ("27:15:07", &dia, &hor, &min, &sec);
   printf ("TIMEDAY: 27:15:07 = %d dias, %d hs, %d m, %d s\n",dia,hor, min,sec);  
   timeday ("317:00:00", &dia, &hor, &min, &sec);
   printf ("TIMEDAY: 317:00:00 = %d dias, %d hs, %d m, %d s\n",dia,hor, min,sec);  

   getchar();

   printf ("Convensión TIMEDIFF: resta hacia atrás\n");
   printf ("TIMEDIFF: 17:00:00-00:15:00 = %f horas\n",timediff("17:00:00","00:15:00","H"));    
   printf ("TIMEDIFF: 17:00:00-00:15:00 = %f minutos\n",timediff("17:00:00","00:15:00","M"));    
   printf ("TIMEDIFF: 17:00:00-00:15:00 = %f segundos\n",timediff("17:00:00","00:15:00","S"));    
   printf ("TIMEDIFF: 17:00:00-16:15:00 = %f minutos\n",timediff("17:00:00","16:15:00","M"));    
   printf ("TIMEDIFF: 04:00:00-23:00:00 = %f horas\n",timediff("4:00:00","23:00:00","H"));
   printf ("\nTIMEDIFF: 00:15:00-17:00:00 = %f horas\n",timediff("00:15:00","17:00:00","H"));    
   printf ("TIMEDIFF: 00:15:00-17:00:00 = %f minutos\n",timediff("00:15:00","17:00:00","M"));    
   printf ("TIMEDIFF: 00:15:00-17:00:00 = %f segundos\n",timediff("00:15:00","17:00:00","S"));    
   printf ("TIMEDIFF: 16:15:00-17:00:00  = %f minutos\n",timediff("16:15:00","17:00:00","M"));    
   printf ("TIMEDIFF: 23:00:00-04:00:00 = %f horas\n",timediff("23:00:00","4:00:00","H"));
   printf ("\nNUM2TIME: %f = %s\n", 7.23, num2time(7.23));
   printf ("NUM2TIME: %f = %s\n", 2.8345, num2time(2.8345));    
   printf ("NUM2TIME: %f = %s\n", 5.9746, num2time(5.9746));
   printf ("NUM2TIME: %f = %s\n", 16.75, num2time(16.75));    
   printf ("NUM2TIME: %f = %s\n", 46.7521, num2time(46.7521));    
   printf ("\nTIME2NUM: %s = %f\n","27:15:07",time2num("27:15:07"));
   printf ("TIME2NUM: %s = %f\n","5:58:28",time2num("5:58:28"));
   printf ("TIME2NUM: %s = %f\n","250:00:00",time2num("250:00:00"));
   printf ("TIME2NUM: %s = %f\n","16:45:00",time2num("16:45:00"));
   printf ("TIME2NUM: %s = %f\n","2:50:27",time2num("2:50:27"));
}


