#include <string>

#include <unistd.h>
#include <ncurses.h>
#include <json/json.h>

#include "ncurses_helper.hpp"

struct Layout {
  WINDOW *headlinewin;
  WINDOW *newswin;
  WINDOW *newstext;
  WINDOW *statuswin;
  WINDOW *statustext;
};

void init_status(WINDOW *win) {
  wattrset(win, A_NORMAL);
  mvwaddstr(win, 0, 0, "Source:");
  mvwaddstr(win, 4, 0, "Date:");
  mvwaddstr(win, 16, 0, "Progress:"); 
}

Layout init_layout() {
  Layout l;
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  // -> Headline
  l.headlinewin = newwin(3, 80, 0, 0);
  box(l.headlinewin, 0, 0);
  wattrset(l.headlinewin, COLOR_PAIR(1));
  mvwprintw(l.headlinewin, 1, 1, "Please wait...");
  wrefresh(l.headlinewin);
  // -> Newswindow with overflow protection
  l.newswin = newwin(22, 60, 3, 0);
  box(l.newswin, 0, 0);
  l.newstext = subwin(l.newswin, 20, 56, 4, 2);
  wattrset(l.newstext, A_NORMAL);
  mvwaddstr(l.newstext, 0, 0, "Loading Content...");
  wrefresh(l.newswin);
  // -> Statuswindow with overflow protection
  l.statuswin = newwin(22, 20, 3, 60);
  box(l.statuswin, 0, 0);
  l.statustext = subwin(l.statuswin, 20, 18, 4, 61);
  init_status(l.statustext);
  wrefresh(l.statuswin);
  return l;
}

void display_json(Layout layout, std::string news){
  Json::Value root;
  Json::Reader reader;
  reader.parse(news.c_str(), news.c_str() + news.length(), root);
  for (int index = 0; index < root.size(); index++){
    // write status
    werase(layout.statustext);
    init_status(layout.statustext);
    mvwprintw(layout.statustext, 1, 0, "%s", root[index]["account"].get("username", "Parse Error").asCString());
    std::string created_at = root[index].get("created_at", "ParseError-ParseErr-").asCString();
    mvwprintw(layout.statustext, 5, 0, "%s", created_at.substr(0, 10).c_str());
    mvwprintw(layout.statustext, 6, 0, "%s", created_at.substr(11,8).c_str());
    wmove(layout.statustext, 17, 0);
    for (int i = 0; i < root.size(); i++) {
      if (i == index) {
        wattr_on(layout.statustext, COLOR_PAIR(2), NULL);
        wprintw(layout.statustext, "*");
        wattr_off(layout.statustext, COLOR_PAIR(2), NULL);
      } else { 
        wprintw(layout.statustext, "*");
      }
      wnoutrefresh(layout.statustext);
      wrefresh(layout.statustext);
      napms(20);
    } 
    // write headline
    werase(layout.headlinewin);
    box(layout.headlinewin, 0, 0);
    fancy_print(layout.headlinewin, 1, 1, root[index]["card"].get("title", "Parse Error").asCString(), 50, 76);
    
    // write content
    werase(layout.newstext);
    auto_wrap(layout.newstext, 0, root[index]["card"].get("description", "Parse Error").asCString(), 44);
    wrefresh(layout.newswin); 

    sleep(10);
  }
}
