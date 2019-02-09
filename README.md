# src
Código fuente de funciones de proceso de fechas, horas y cadenas en C.

La mayoría de estas rutinas han sido mejoradas e incorporadas en XU (XU.PRG, sección #pragma DUMP). Para más detalles, lo invito a revisarlas.

DATETIME.C

Tiene un conjunto de funciones para:
 - descomponer fecha en numeros.
 - componer numeros en fechas (string).
 - sumar fechas.
 - calcular horas.
 - etcétera.

FMONEY.C

Formatea números a formato moneda.

FSATURA.C

Toma un string y reemplaza campos con los campos de un string de datos. Dicho string de datos puede ser un archivo de datos.
El resultado puede ser almacenado en un archivo.

GLOSS.C

Desglose de cifras en palabras. Solo acepta hasta dos decimales.

STRTRAN.C

Reemplaza uno o más subcadenas de una cadena mayor.
unsigned char *strtran(source, find, replace, ignore, limit)
donde:

* source: string que será modificado.
* find: string a buscar.
* replace: string que reemplazará al string encontrado.
* ignore: número de matches a ignorar antes de reemplazar. 0=no ignore nada.
* limit: número de reemplazos a realizar. 0=reemplace hasta el final.

SUBSTR.C

Grupo de funciones para tratamiento de strings. Entre ellas están:
 - SUBSTR()  extrae una porción de texto desde un string.
 - AT()      devuelve la posición donde inicia un substring dentro de un string.
 - STRREP()  replica un caracter, "n" veces. En XU se puede ver una versión mejorada,
             donde se puede replicar un string.
 - STRINS()  inserta un substring dentro de otro string.
 - STRCUT()  corta una porción de un string.

TOKEN.C

Proceso básico de tokens.

TRIM.C

Funciones para quitar espacios y otros caracteres desde un string.
- RTRIM()    quita espacios desde la derecha del string.
- LTRIM()    quita espacios desde la izquierda del string.
- TRIM()     quita espacios desde ambos lados del string.

UPPER.C

Funciones para convertir un string a mayúsculas y minñusculas.
- UPPER()    convierte un string a mayúsculas.
- LOWER()    convierte un string a minúsculas.
- LEN()      obtiene la longitud de un string.

PAD.C

Aplica un pad de relleno a un string, hacia la izquierda, derecha o bien deja el string centrado. Es como la función inversa de TRIM.

Saludos, Mr. Dalien!
