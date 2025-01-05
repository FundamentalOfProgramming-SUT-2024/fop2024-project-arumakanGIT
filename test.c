#include <ncurses.h>
#include <signal.h>
#include <unistd.h>

// هندلر برای سیگنال SIGWINCH
void handle_winch(int sig)
{
    // به‌روزرسانی ابعاد ترمینال
    endwin();
    refresh();
    clear();

    // گرفتن اندازه جدید ترمینال
    int new_rows, new_cols;
    getmaxyx(stdscr, new_rows, new_cols);

    // چاپ اندازه جدید در ترمینال
    mvprintw(0, 0, "Terminal resized: %d rows, %d cols", new_rows, new_cols);
    refresh();
}

int main()
{
    // راه‌اندازی ncurses
    initscr();
    noecho();
    cbreak();

    // ثبت هندلر سیگنال SIGWINCH
    signal(SIGWINCH, handle_winch);

    // چاپ پیام اولیه
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    mvprintw(0, 0, "Terminal size: %d rows, %d cols", rows, cols);
    mvprintw(1, 0, "Resize the terminal and watch for updates...");
    refresh();

    // حلقه برای خواندن ورودی
    while (getch() != 'q') {
        // برنامه اجرا می‌شود تا زمانی که کاربر کلید 'q' را بزند.
    }

    // پاکسازی و خروج
    endwin();
    return 0;
}
