#ifndef UI_H
#define UI_H
#define MENU_SIZE 8
#define UP 1
#define DOWN 2
#define ENTER 3

extern char* menu[MENU_SIZE];

void displayMenu(int select);
void textcolor(int colorNum);

int getKey();
#endif // !UI_H