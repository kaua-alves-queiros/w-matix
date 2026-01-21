#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int width;
    int height;
    int* column_pos;
    char* pixels;
} MatrixRain;

MatrixRain create_rain(int w, int h) {
    MatrixRain r;
    r.width = w;
    r.height = h;
    r.pixels = (char*)calloc(w * h, sizeof(char));
    r.column_pos = (int*)malloc(w * sizeof(int));
    for (int i = 0; i < w; i++) r.column_pos[i] = rand() % h;
    return r;
}

void update_rain(MatrixRain r) {
    for (int j = 0; j < r.width; j++) {
        int head = r.column_pos[j];

        r.pixels[head * r.width + j] = (char)((rand() % 94) + 33);

        if (rand() % 10 > 7) {
            int tail = (head - 10 + r.height) % r.height;
            r.pixels[tail * r.width + j] = ' ';
        }

        r.column_pos[j] = (head + 1) % r.height;
    }
}

void render_rain(MatrixRain r) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(hConsole, coord);

    for (int i = 0; i < r.height; i++) {
        for (int j = 0; j < r.width; j++) {
            char c = r.pixels[i * r.width + j];
            if (c == ' ') {
                putchar(' ');
                continue;
            }

            if (i == r.column_pos[j] - 1)
                SetConsoleTextAttribute(hConsole, 15);
            else if (rand() % 5 == 0)
                SetConsoleTextAttribute(hConsole, 10);
            else
                SetConsoleTextAttribute(hConsole, 2);

            putchar(c);
        }
    }
}

int main() {
    srand((unsigned int)time(NULL));
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    CONSOLE_CURSOR_INFO info = { 100, FALSE };
    SetConsoleCursorInfo(hOut, &info);

    GetConsoleScreenBufferInfo(hOut, &csbi);
    int w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    MatrixRain rain = create_rain(w, h);

    while (1) {
        update_rain(rain);
        render_rain(rain);
        Sleep(1);
    }

    return 0;
}