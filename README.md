# gentone

Command-line program for writing audio wav files of pure tones.

## Installation

Use `gcc` to compile.

For UNIX systems type:

	git clone https://github.com/fergarciadlc/gentone.git
	cd gentone/
	sudo gcc gentone.c getsenx.c savewav.c -lm -w -o /usr/local/bin/gentone

For Windows: clone or download this repository, open a terminal in the directory and type:

	gcc gentone.c getsenx.c savewav.c -o gentone


Then, place the `gentone.exe` file in any location on your PATH (except for `C:\Windows\System32`).
## Usage
Generate a wav file of a pure tone by typing:

	gentone -f <(Hz)> [OPTIONS]

where:
	
	-f 	Frequency of the signal in Hertz (Mandatory argument)

### Options:
	-t	Time of the signal in seconds	(default: 1 s)
	-s 	Sampling frequency in Hertz 	(default: 44100 Hz)
	-b 	Bit depth, only 16 or 8			(default: 16 bits)
	-N 	filename   (default: "[frequency]Hz--date--time).wav")

Type `gentone -h` to see the usage and help.

## Example
Type: 

	gentone -f 220 -t 5
Output:
```
File: '220.00Hz--15-Jun-2020--15-19-12.wav' generated.

Values:
  Frequency = 220.00 Hz
  Duration  = 5.00 s
  Fs = 44100.00 Hz
  Bit depth = 16 bits
```
And it will generate a 5 seconds wav file of a 220 Hz pure tone with the rest of the default values.

### Espacial case: Aliasing
The program is aware of the **Nyquist-Shannon** [sampling theorem](https://en.wikipedia.org/wiki/Nyquist%E2%80%93Shannon_sampling_theorem) and whenever the signal's frequency is greater than the double of the sampling frequency (for instance: `gentone -f90 -s100`) it will show the following warning:
	
	WARNING: Nyquist-Shannon sampling theorem:
	100.00 Hz sampling frequency is less than 2x(90.00 Hz)
And then will generate the file that could be useful to show/explain the [Aliasing effect](https://en.wikipedia.org/wiki/Aliasing).

## License:
Copyright (c) 2020 Fernando García de la Cruz

Licensed under the [MIT License](LICENSE).