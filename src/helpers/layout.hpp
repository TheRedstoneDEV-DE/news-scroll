#ifndef LAYOUT_HPP

#include <string>

struct Layout {
  WINDOW *headlinewin;
  WINDOW *newswin;
  WINDOW *newstext;
  WINDOW *statuswin;
  WINDOW *statustext;
};

Layout init_layout();
void display_json(Layout layout, std::string news_json);

#endif 
