#include <string.h>
#include <stdio.h>
#include <ncurses.h>

void RegisterMenu(WINDOW *win)
{
    int scrX, scrY;
    int choice = 0;    // انتخاب اولیه
    int highlight = 0; // گزینه های برجسته
    char *menu_items[] = {
        "Option 1",
        "Option 2",
        "Option 3",
        "Exit",
        NULL};

    int num_items = 4;         // تعداد گزینه‌ها
    getmaxyx(win, scrY, scrX); // گرفتن ابعاد صفحه

    // ایجاد پنجره جدید برای منو
    WINDOW *menuWin = newwin(scrY, scrX, 0, 0);
    keypad(menuWin, TRUE); // فعال‌سازی کلیدهای ویژه مثل بالا و پایین
    refresh();             // به‌روزرسانی صفحه

    while (1)
    {
        // پاک کردن پنجره قبلی
        werase(menuWin);

        // نمایش گزینه‌ها
        for (int i = 0; i < num_items; i++)
        {
            if (i == highlight)
            {
                // برجسته کردن گزینه انتخابی
                attron(A_REVERSE); // رنگ معکوس (پس‌زمینه و متن)
                mvwprintw(menuWin, i + 2, scrX / 2 - strlen(menu_items[i]) / 2, menu_items[i]);
                attroff(A_REVERSE);
            }
            else
            {
                mvwprintw(menuWin, i + 2, scrX / 2 - strlen(menu_items[i]) / 2, menu_items[i]);
            }
        }

        wrefresh(menuWin); // به‌روزرسانی پنجره منو

        // دریافت ورودی کاربر
        int ch = wgetch(menuWin);

        switch (ch)
        {
        case KEY_UP:
            // حرکت به گزینه بالا
            if (highlight > 0)
                highlight--;
            break;
        case KEY_DOWN:
            // حرکت به گزینه پایین
            if (highlight < num_items - 1)
                highlight++;
            break;
        case 10: // Enter key
            // اگر کاربر Enter را فشار دهد
            if (highlight == num_items - 1)
            {
                delwin(menuWin); // خروج از منو
                return;          // پایان تابع
            }
            break;
        default:
            break;
        }
    }
}

int main()
{
    // شروع ncurses
    initscr();
    noecho();             // جلوگیری از نمایش ورودی
    cbreak();             // فعال‌سازی حالت cbreak
    keypad(stdscr, TRUE); // فعال‌سازی کلیدهای خاص

    // فراخوانی تابع منو
    RegisterMenu(stdscr);

    // پایان ncurses
    endwin();
    return 0;
}