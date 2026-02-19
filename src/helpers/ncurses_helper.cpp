#include <cstring>

#include <ncurses.h>

void fancy_print(WINDOW *win, int row, int col, const char *text, int speed_ms, int clip) {
  int len = strlen(text);
  for (int i = 0; i <= len; i++) {
    if (i >= clip) {
      wprintw(win,"...");
      wrefresh(win);
      return;
    }
    char buf[1024];
    strncpy(buf, text, i);
    buf[i] = 0;
    mvwprintw(win, row, col, "%s", buf);
    wrefresh(win);
    napms(speed_ms);
  }
}

void cut_leading_space(WINDOW *win, int row, int col, char buffer[]) {
  if (buffer[0] == ' '){
    mvwprintw(win, row, col, "%s", buffer+1); // cut leading space from buffer
  } else {
    mvwprintw(win, row, col, "%s", buffer);
  }
  wrefresh(win);
}

void auto_wrap(WINDOW *win, int startline, const char *text, int thresh) {
  // Don't process empty strings
  if (text[0] == 0) {
    return;
  }

  int len = strlen(text);
  char buffer[len];
  int len_until_break = 0;
  int last_break = 0;
  int line = startline;
  for (int i = 0; i < len; i++) {
    if (len_until_break > thresh && text[i] == ' ') { 
      cut_leading_space(win, line, 0, buffer);
      // Nap 50ms between each line (fancy)
      napms(50);
      // (re)set logical values
      last_break = i;
      len_until_break = 0;
      line++;
      memset(buffer, 0, len);
    } else {
      // Copy line to buffer (until break)
      strncpy(buffer, text + last_break, i+1-last_break); // i+1: internally reads offset -> len-1
      len_until_break++;
      buffer[i+1] = '\0';
    }
  }
  // write last line if buffer is still not empty
  if (buffer[0] != 0) {
    cut_leading_space(win, line, 0, buffer);
  }
}
