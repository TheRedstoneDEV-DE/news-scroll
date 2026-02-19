#include <iostream>
#include <regex>
#include <cstring>

#include <ncurses.h>

#include "helpers/news_helper.hpp"
#include "helpers/ncurses_helper.hpp"
#include "helpers/layout.hpp"

struct Credentials {
  char *list_url;
  char *login_token;
};

void error(std::string message){
  std::cout << "\033[31mSyntaxError\033[0m: " << message << std::endl;
  std::cout << "Correct usage:" << std::endl;
  std::cout << "./news_scroll --url <list-url> --token <mastodon-application-token>" << std::endl; 
  exit(1);
}

Credentials parse_arguments(char* argv[]){
  Credentials creds;
  if (strcmp(argv[1],"--url") == 0 && strcmp(argv[3],"--token") == 0) {
    creds.list_url = argv[2];
    creds.login_token = argv[4];
  } else if (strcmp(argv[3],"--url") == 0 && strcmp(argv[1],"--token") == 0) {
    creds.list_url = argv[4];
    creds.login_token = argv[2];
  } else {
    error("Wrong arguments!");
  }
  return creds;
}

int main(int argc, char* argv[]) {
  if (argc != 5) {
    error("Wrong argument count!");
    return 1;
  }

  Credentials creds = parse_arguments(argv);
  if (creds.login_token == "" || creds.list_url == "") {
    return 1;
  }

  setlocale(LC_ALL, ""); // auto-detect locale 
  initscr();
  start_color();
  Layout layout = init_layout();
  while (true) {
    display_json(layout, 
      get_news(creds.list_url, creds.login_token)
    );
  }
  attroff(COLOR_PAIR(1));
  endwin();
  echo();
  return 0;
}
