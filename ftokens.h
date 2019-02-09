// sólo busca tokens para contarlos para size:
uint16_t ftokens(unsigned char *linea, const char *buscar, uint16_t lb) {
   char *t,*r; // son solo punteros apuntando a la cadena s.

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

