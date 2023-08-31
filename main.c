#include <ncurses.h>
#include <png.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <locale.h>
int devnull;
uint32_t devnull_uint;
int main (int argc, char **argv){
    if (argc < 2){
        printf("Not enough arguments\n");
        return 1;
    }
    uint16_t n, i, j, k, x, y, idx;
    uint32_t frames = 6953;
    setlocale(LC_CTYPE, "");
    initscr();
    wchar_t braille[256];
    braille[0] = 0x2800;
    clear();

    for(i = 1; i < 256; i++){
        braille[i] = braille[i - 1] + 1;
        printw("%lc ", braille[i]);
    }
    printw("\nDo all characters above look fine? (press Space to continue)");
    refresh();
    if (getchar() != ' '){
        endwin();
        return 0;
    }
    png_structp	png_ptr;
    png_infop info_ptr;
    FILE * fp;
    uint32_t width;
    uint32_t height;
    uint16_t type, col;
    png_bytepp image;
    char *png_file = (char*)malloc(strlen(argv[1]) + 15*sizeof(char));
    strcpy(png_file, argv[1]);
    strcat(png_file, "/frame00000.png");
    uint16_t digit_idx = strlen(argv[1]) + 10;
    for (n = 1; n <= 6953; n++){
        png_file[digit_idx]++;
        for (idx = digit_idx; png_file[idx] > '9'; png_file[idx] = '0', png_file[idx - 1]++, idx--);
        fp = fopen (png_file, "rb");
        if (!fp){
            printf("Cannot read file named %s\n", png_file);
            endwin();
            return 1;
        }
        png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr){
            printf("Cannot read png structure from file named %s\n", png_file);
            endwin();
            return 1;
        }
        info_ptr = png_create_info_struct (png_ptr);
        if (!info_ptr){
            printf("Cannot read png information from file named %s\n", png_file);
            endwin();
            return 1;
        }
        fclose(fp);
    }
    png_file[digit_idx] = png_file[digit_idx - 1] = png_file[digit_idx - 2] = png_file[digit_idx - 3] = png_file[digit_idx - 4] = '0';
    char audio[1024];
    if (argc > 2){
        strcpy(audio, "ffplay -v 0 -nodisp -autoexit ");
        strcat(audio, argv[2]);
        strcat(audio, " &");
    }else{
        strcpy(audio, "ffplay -v 0 -nodisp -autoexit badappleaudio.mp3 &");
    }
    uint32_t delay = 23680;
    if (argc > 3){
        delay = atoi(argv[3]);
    }
    system(audio);

    for (n = 1; n <= 6953; n++){
        png_file[digit_idx]++;
        for (idx = digit_idx; png_file[idx] > '9'; png_file[idx] = '0', png_file[idx - 1]++, idx--);
        fp = fopen (png_file, "rb");
        png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        info_ptr = png_create_info_struct (png_ptr);
        png_init_io (png_ptr, fp);
        png_read_png (png_ptr, info_ptr, 0, 0);
        png_get_IHDR (png_ptr, info_ptr, &devnull_uint, &height, &devnull, &devnull, &devnull, &devnull, &devnull);
        image = png_get_rows (png_ptr, info_ptr);
        width = png_get_rowbytes(png_ptr, info_ptr);
        clear();
        for (i = 0; i + 15 < height; i += 16) {
            for (j = 0; j + 15 < width; j += 16) {
                type = 0;
                for (k = 0; k < 8; k++){
                    col = 0;
                    for (x = i + ((k >> 1) << 2); x < i + ((k >> 1) << 2) + 4; x++){
                        for (y = j + (k & 1)*8; y < j + (k & 1)*8 + 8; y++){
                            col += image[x][y];
                        }
                    }
                    type |= (1 << k)*(col > 4096);
                }
                printw("%lc", braille[type]);
            }
            printw("\n");

        }
        refresh();
        fclose(fp);
        usleep(delay);
    }
    endwin();
    return 0;
}
