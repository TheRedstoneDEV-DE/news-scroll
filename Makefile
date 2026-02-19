PKGS = "ncursesw libcurl jsoncpp"

CXX = "i686-unknown-linux-gnu-g++"

OPTIM ?= -march=pentium3 -mtune=pentium3

CFLAGS += $(shell pkgconf --cflags $(PKGS)) 
LDLIBS += $(shell pkgconf --libs $(PKGS))

%.o: %.cpp
	@echo -e "[\033[32mBUILD\033[0m] \033[94m$<\033[0m"
	$(CXX) $(CFLAGS) -c $< -o $@

executable: src/helpers/news_helper.o src/helpers/ncurses_helper.o src/helpers/layout.o src/main.o
	@echo -e "[\033[33mLINK\033[0m] \033[94mnews_scroll\033[0m"
	$(CXX) $^ $(LDLIBS) -o news_scroll
	@echo -e "[\033[32mDONE\033[0m] \033[94mBuild Succsessful!\033[0m"

cross-pentium3: CFLAGS += -march=pentium3 -mtune=pentium3 -O3
cross-pentium3: executable
	@echo -e "\033[32mBuilt Pentium 3 optimized binary!\033[0m"

optim: CFLAGS += -O3
optim: executable
	@echo -e "\033[32mBuilt optimized binary!\033[0m"

clean:
	rm src/*.o src/helpers/*.o news_scroll

test: executable
	./news_scroll
