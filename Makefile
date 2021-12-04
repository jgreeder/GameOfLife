CXX=g++ --std=gnu++20
CXXFLAGS=-Iobjs/ -O3 -Wall -Wextra

APP_NAME=game_of_life
OBJDIR=objs

default: $(APP_NAME)

.PHONY: dirs clean

dirs:
	/bin/mkdir -p $(OBJDIR)/

clean:
	/bin/rm -rf $(OBJDIR) $(APP_NAME)

OBJS=$(OBJDIR)/main.o $(OBJDIR)/game_of_life.o

$(APP_NAME): dirs $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) -lpthread

$(OBJDIR)/%.o: %.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@
