/* gentone.c
 * 
 * gentone is a Command-line program for writing audio wav files 
 * of pure tones and synth waveforms.
 *
 * Compile: gcc gentone.c waveform.c savewav.c -o gentone
 * by Fernando Garcia de la Cruz (fergarciadlc), 2020
 * Last update: 20-July-2020
 */
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<string.h>

double *waveform(double,double,double,char *, double, double);
  int   savewav(double,double,int,char *,double *);
 
double *x; //signal

int main (int argc, char **argv) {
  
  int fflag = 0;
  int tflag = 0;
  int sflag = 0;
  int bflag = 0;
  int Nflag = 0;
  int wflag = 0;
  int aflag = 0;
  int pflag = 0;
  int help = 0;
  char *userfilename = NULL;
  char *userwaveform = NULL;
  double frequency = 0;
  double duration = 0;
  double amplitude = 0;
  double Fs = 0;
  double phase = 0;
  int bits = 0;
  int index;
  int option;

  static char usage[] = "usage: %s -f <frequency (Hz)> [OPTIONS]\n";

  if (argc < 2) {
    fprintf(stderr, "%s error: You must provide tone frequency\n", argv[0]);
    fprintf(stderr, usage, argv[0]);
    fprintf(stderr, "\nType %s -h to see a list of all options.\n", argv[0]);
    exit(1);
  }

  while ((option = getopt(argc, argv, "f:t:a:s:b:N:w:d:r:h")) != -1) {
    switch (option) {
      case 'f':
        fflag = 1;
        frequency = atof(optarg);
        break;
      case 't':
        tflag = 1;
        duration = atof(optarg);
        break;
       case 'a':
        aflag = 1;
        amplitude = atof(optarg);
        break;
      case 's':
        sflag = 1;
        Fs = atof(optarg);
        break;
      case 'b':
        bflag = 1;
        bits = atof(optarg);
        break;
       case 'h':
       	help = 1;
       	break;
       case 'N':
       	Nflag = 1;
       	userfilename = optarg;
       	break;
       case 'w':
       	wflag = 1;
       	userwaveform = optarg;
       	break;
       case 'r':
       	pflag = 1;
       	phase = atof(optarg);
       	break;
       case 'd':
       	pflag = 1;
       	phase = atof(optarg)*(3.14159265/180);
       	break;
      case '?':
          fprintf(stderr, usage, argv[0]);
          fprintf(stderr, "\nType %s -h to get help.\n", argv[0]);
          exit(1);
          break;
      default:
        abort ();
      }
    }
    if (help){
    	printf("%s - 2020 by Fernando Garcia de la Cruz\n\n", argv[0]);
    	printf("Tone wav generator\n");
    	printf(usage,argv[0]);
    	printf("\n");
    	printf("-f 	Frequency of the signal in Hertz (Mandatory argument)\n");
    	printf("\nOPTIONS:\n");
    	printf("-t	Time of the signal in seconds (default: 1 s)\n");
    	printf("-a	Amplitude of the signal (0-1) (default: 1)\n");
    	printf("-s 	Sampling frequency in Hertz   (default: 44100 Hz)\n");
    	printf("-b 	Bit depth, only 16 or 8       (default: 16 bits)\n");
    	printf("-w 	Waveform:   <sine>   (default)\n");
    	printf("   	           <square> \n");
    	printf("   	          <triangle> \n");
    	printf("   	          <sawtooth> \n");
    	printf("-d 	Phase Shift in degrees        (default: 0)\n");
    	printf("-r 	Phase Shift in radians        (default: 0)\n");
    	printf("-N 	filename (default: \"[freq]Hz--waveform--date--time.wav\")\n");
    	exit(1);
    	}
    if (fflag == 0) { // -f is mandatory 	
      fprintf(stderr, "%s: missing -f option\n", argv[0]);
      fprintf(stderr, usage, argv[0]);
      exit(1);
    } else if (frequency <= 0){
    fprintf(stderr, "%s syntax error: Frequency value must be a number greater than 0\n", argv[0]);
    exit(1);
  }

  if (tflag == 0) duration = 1; //Default 1 second
  if (tflag == 1) {
    if (duration <= 0){
    	fprintf(stderr, "%s syntax error: Time value must be a number greater than 0\n", argv[0]);
      	exit(1);
    }
  }

  if (aflag == 0) amplitude = 1;
  if (aflag){
  	if (amplitude <= 0 || amplitude > 1){
  		fprintf(stderr, "%s syntax error: Amplitude value must be a number between 0 and 1\n", argv[0]);
      	exit(1);
  	}
  }

  if (sflag == 0) Fs=44100; // Default = 44.1kHz
  if (sflag == 1){
    if (Fs <= 0){
    	fprintf(stderr, "%s syntax error: Sampling Frequency (Fs) value must be a number greater than 0\n", argv[0]);
      	exit(1);
    }
  }
  if (2*frequency >= Fs){ // Checking sampling theorem
      printf("\nWARNING: Nyquist-Shannon sampling theorem:\n%.2f Hz sampling frequency is less than 2x(%.2f Hz)\n\n",Fs,frequency);
    }

  if (bflag == 0) bits = 16; // Default 16 bits
  if (bflag == 1){
    if (bits != 8 && bits != 16){
    	fprintf(stderr, "%s syntax error: Bitdepth must be 8 or 16 bits \n", argv[0]);
      	exit(1);
    }
  }

  for (index = optind; index < argc; index++){
  	printf ("Non-option argument %s\n", argv[index]);
  	exit(1);
  }

  char bufferwaveform[10];
  if (wflag == 0) strcpy(bufferwaveform,"sine"); // default sine wave
  if (wflag){
  	strcpy(bufferwaveform,userwaveform);
  }

  // format default date-filename
  time_t rawtime;
  struct tm *info;
  char buffer[80], filename[50], bufferuserfile[50];
  time(&rawtime);
  info = localtime( &rawtime );
  strftime(buffer,80,"--%d-%b-%Y--%H-%M-%S", info);
  // filename
  if (Nflag){
  	strcpy(bufferuserfile,userfilename);
  	sprintf(filename,"%s.wav",bufferuserfile);
  } else{
  	sprintf(filename,"%.2fHz--%s%s.wav",frequency, bufferwaveform, buffer);
  }

  x = waveform(frequency,duration*Fs,Fs,bufferwaveform, amplitude, phase);
  if (x==0){ //check for errors
  	fprintf(stderr, "%s syntax error: invalid waveform \n", argv[0]);
  	fprintf(stderr, "\nType %s -h to get help.\n", argv[0]);
    exit(1);
  }
// savewav(duration in samples, sampling frequency, bits, filename, signal)
  savewav(duration*Fs, Fs, bits, filename, x);
  free(x);

  printf("File: '%s' generated.\n", filename);
  printf("\nValues:\n");
  printf("  Frequency = %.2f Hz\n", frequency);
  printf("  Duration  = %.2f s\n", duration);
  printf("  Amplitude = %.2f \n", amplitude);
  printf("  Waveform = '%s'\n", bufferwaveform);
  printf("  Phase Shift = %.4f rad \n", phase);
  printf("  Fs = %.2f Hz\n", Fs);
  printf("  Bit depth = %d bits\n", bits);
  return(0);
}
