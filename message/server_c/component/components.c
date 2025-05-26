#include "components.h"
#include <stdlib.h>
#include <string.h>

void DrawInputBox(char *text, int tick) {

    char *new_text = malloc(strlen(text) + 2);
    strcpy(new_text, text);

    // Blinking Cursor
    static int visible = 0;
    const int tickWait = 30;

    if (tick % tickWait == 0) {
        // flips between 0, 1
        visible = abs(visible - 1);
    }

    if (visible) {
        new_text[strlen(text)] = '|';

    } else {
        new_text[strlen(text)] = ' ';
    }

    new_text[strlen(text) + 1] = '\0';

    const int WIDTH = 700;
    const int HEIGHT = 100;

    DrawRectangle(GetScreenWidth() / 2 - WIDTH / 2, GetScreenHeight() - 125,
                  WIDTH, HEIGHT, WHITE);
    DrawText(new_text, 75, GetScreenHeight() - 90, 32, BLACK);
}
