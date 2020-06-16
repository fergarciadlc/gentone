//====================================================================================================
//NAME
//       savewav.c -generar archivo.wav de un vector x.
//USO
//       error = savewav(ndatos,nyquist,bits,namefile,x);
//REM
//       error = 0 si ok, 1 si error.
//      ndatos =  numero de datos.
//     nyquist = frecuencia de muestreo nyquist.
//        bits = bits por muestra (8 o 16).
//    namefile = nombre del archivo.
//           x = vector de datos [-1 +1].
//DATE
//       200812MPG. ver genwav.c, formato wav en red
//
//BY: 
//        Maximino Peña Guerrero
//Source: 
//      http://www.max.esimez.ipn.mx/funciones/savewav.c
//==========================================================================================
//
#include<string.h>
#include<stdio.h>

#define PCM 1
#define MONO 1

typedef struct
 {
   char       riff[4];
   int        tamano;
   char       wave[4];
   char       fmt[4];
   int        nbytescabeza;
   short int  formato;
   short int  monoestereo;
   int        nyquist;
   int        bytesporseg;
   short int  bytesporcaptura;
   short int  bitsporseg;
   char       data[4];
   int        ndatos;
} DESCRIPTOR;

static DESCRIPTOR fdw;
       FILE *fd;
       unsigned char y8;
       unsigned short int y16;

int savewav(double ndatos, double nyquist, int bits, char *file, double *x)
{
    unsigned int i;
    if ((fd=fopen(file, "wb"))==NULL)
       {
         printf("fopen():error en genwav()\n");
        }
     strncpy(fdw.riff,"RIFF",4);
     strncpy(fdw.wave,"WAVE",4);
     strncpy(fdw.fmt,"fmt ",4);
     fdw.nbytescabeza=16;
     fdw.formato = PCM;
     fdw.monoestereo = MONO;
     fdw.nyquist = (int)nyquist;
     fdw.bitsporseg = (short int)bits;
     fdw.bytesporseg = fdw.nyquist*fdw.monoestereo*fdw.bitsporseg/8;
     fdw.bytesporcaptura = fdw.monoestereo*fdw.bitsporseg/8;
     strncpy(fdw.data,"data",4);
     fdw.ndatos = (int)ndatos*fdw.monoestereo*fdw.bitsporseg/8;
     fdw.tamano = 4+(8+fdw.nbytescabeza) + (8+fdw.ndatos);
     
     fwrite(&fdw,sizeof(DESCRIPTOR),1,fd);
     
     for (i=0; i<(int)ndatos; i++)
      {
         if(fdw.bitsporseg==8)
           {
              y8=127+127*x[i];
                 fwrite(&y8,sizeof(y8),1,fd);
            }
         else if(fdw.bitsporseg==16)
            {
              y16=32767*x[i];
              fwrite(&y16,sizeof(y16),1,fd);
             }
          else
             {
                printf("savewav() error: no 16 u 8 bits\n");
                fclose(fd);
                return(1);
              }
       }

       fclose(fd);
       return(0);
}
