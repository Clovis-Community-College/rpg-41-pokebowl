BRIDGES = -Ilibs/include
CCFLAGS = $(BRIDGES) -lncurses -lcurl -Wno-sign-compare -fsanitize=undefined -fsanitize=address -std=c++26 -O3 -w
SRCS := $(filter-out $(wildcard _*.cc), $(wildcard *.cc))
OBJS = $(SRCS:.cc=.o)
DEPS = $(OBJS:.o=.d)

MOVE_UP    := \033[A
CLEAR_LINE := \033[2K

# shell env for the a.out prompt, else it fails to run a.out
SHELL := /bin/bash
BRIDGES_URL = https://bridgesuncc.github.io/sw/bridges-cxx-3.4.3-x86_64-linux-gnu.tgz

all: a.out

libs:
	@echo "Downloading BRIDGES..."
	@curl -L $(BRIDGES_URL) -o bridges.tgz
	@tar -xzf bridges.tgz
	@mv bridges-cxx-*-x86_64-linux-gnu libs
	@rm bridges.tgz

	@echo -ne "BRIDGES installed!"
	@echo

a.out: $(OBJS) | libs
	@echo -ne "Linking '$@'..."
	@echo

	@ # Equivalent to: g++ {flags} a.o b.o ... -o a.out
	@g++ $(CCFLAGS) $(OBJS) -o $@

	@echo -ne "$(CLEAR_LINE)"

	@ # msg wont show if LINKING fail
	@echo Done linking
	@echo -e '\t$(OBJS)'
	@echo to '$@'!
	
	@ # prompt to run a.out convieniently
	@read -p "Run a.out too? [y/n]: " ans; \
	if [ "$$ans" = "y" ]; then ./$@; fi
	
	@echo -ne "$(MOVE_UP)$(CLEAR_LINE)"
	
%.o: %.cc
	@ # -MMD -MP auto-magically consider all @ #include-s when
	@ # compiling .o; this way we dont have to do
	@ # tons of a.cc a.h b.h c.h d.h, etc. manually.

	@ # Assume "@ #include a.h" "@ #include b.h" are in foo.h,
	@ # the below is similar to:
	@ #
	@ # foo.o: foo.cc foo.h a.h b.h 
	@ # 	g++ {flags} -c foo.cc -o foo.o
	@ #
	
	@echo -ne "Compiling '$<'...\n"
	@g++ $(CCFLAGS) -MMD -MP -c $< -o $@

	@echo -ne "$(MOVE_UP)$(CLEAR_LINE)"

	@ # msg wont show if a .cc fails to compile
#	@echo Done compiling '$<'!

-include $(DEPS)

clean:
	@ # C++ related files
	@rm -f *.o a.out

	@ # Junk temp files
	@rm -f .*.swp *.save* *.orig

	@echo Done cleaning-up!
