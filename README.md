# Bad Apple in terminal using Braille unicode characters

Just a quick C practice

## Dependencies

To build this program, you will need:

- ffmpeg (v2:6.0-8 or above)
- gcc (C89 or later)
- ncurses library (Linux/OSX)
- libpng
- All [6953 frames of Bad Apple!!](https://drive.google.com/file/d/1GPzS23quhUo8qS73kn_qPTMLgvgycYHe/view)
- An audio file of Bad Apple!!

## Building

Build using gcc:

```
gcc -o BadApple main.c -lpng -lncurses
```

## Usage

The arguments for the program are as follows:

```
./BadApple (directory to frames) [audio file] [delay time between two frames (in microseconds)]
```

- audio file without specification is defaulted to "./badappleaudio.mp3"
- delay time without specification is defaulted to 23680 (sweet spot for the program to be in sync with the audio - though this depends on everybody's machine, so it might not be for you)

## Known issues

- If your audio is longer than how the video will play, the audio will not stop unless you kill the `ffplay` process yourself 
