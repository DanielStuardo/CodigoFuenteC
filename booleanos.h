
typedef struct _BOOL BOOL;

struct _BOOL {
   uint8_t strtran:1;
   uint8_t gloss:1;
   //uint8_t gltype:2;  // 00,01,10,11
   uint8_t trim:1;
   uint8_t str:1;
   // agregar otros booleans de otras funciones
};

