BRIDGES = -Ilibs/include
CCFLAGS = -lcurl -Wfatal-errors -Wno-sign-compare -g -D_GLIBCXX_DEBUG -fsanitize=undefined -fsanitize=address -std=c++26
SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc=.o)
DEPS = $(OBJS:.o=.d)

# shell env for the a.out prompt, else it fails to run a.out
SHELL := /bin/bash

# BRIDGES import:
# copy 'libs' folder in BRIDGES, paste into project directory,
# then append-front $(BRIDGES) at CCFLAGS, line 2.

a.out: $(OBJS)
	@ # Equivalent to: g++ {flags} a.o b.o ... -o a.out
	@g++ $(CCFLAGS) $^ -o $@

	@ # msg wont show if LINKING fail
	@echo Done linking
	@echo -e '\t$(OBJS)'
	@echo to '$@'!
	
	@ # prompt to run a.out convieniently
	@read -p "Run a.out too? [y/n]: " ans; \
	if [ "$$ans" = "y" ]; then ./$@; fi
	
%.o: %.cc $(wildcard %.h)
	@ # -MMD -MP auto-magically consider all @ #include-s when
	@ # compiling .o; this way we dont have to do
	@ # tons of a.cc a.h b.h c.h d.h, etc. manually.

	@ # Assume "@ #include a.h" "@ #include b.h" are in foo.h,
	@ # the below is similar to:
	@ #
	@ # foo.o: foo.cc foo.h a.h b.h 
	@ # 	g++ {flags} -c foo.cc -o foo.o
	@ #
	
	@g++ $(CCFLAGS) -MMD -MP -c $< -o $@

	@ # msg wont show if a .cc fails to compile
	@echo Done compiling '$<'!

-include $(DEPS)

clean:
	@ # C++ related files
	@rm -f *.o *.d a.out

	@ # Junk temp files
	@rm -f .*.swp *.save* *.orig

	@echo Done cleaning-up!
