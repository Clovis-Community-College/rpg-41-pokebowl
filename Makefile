CCFLAGS = -lcurl -Wfatal-errors -Wno-sign-compare -g -D_GLIBCXX_DEBUG -fsanitize=undefined -fsanitize=address -std=c++26

# BRIDGES import:
# grab libs folder in BRIDGES and cpy into project directory,
# then replace the empty BRIDGES flag with `-llibs/include`

BRIDGES = 

SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc=.o)
DEPS = $(OBJS:.o=.d)

a.out: $(OBJS)
	@clear

	@ # Equivalent to: g++ {flags} a.o b.o ... -o a.out
	@g++ $(CCFLAGS) $(BRIDGES) $^ -o $@

	@ # msg wont show if LINKING fail
	@echo Done linking
	@echo -e '\t$(OBJS)'
	@echo to 'a.out'!

%.o: %.cc %.h
	@ # -MMD -MP auto-magically consider all @ #include-s when
	@ # compiling .o; this way we dont have to do
	@ # tons of a.cc a.h b.h c.h d.h, etc. manually.

	@ # Assume "@ #include a.h" "@ #include b.h" are in foo.h,
	@ # the below is similar to:
	@ #
	@ # foo.o: foo.cc foo.h a.h b.h 
	@ # 	g++ {flags} -c foo.cc -o foo.o
	@ #
	
	@g++ $(CCFLAGS) $(BRIDGES) -MMD -MP -c $< -o $@

	@ # msg wont show if a .cc fails to compile
	@echo Done compiling '$<'!

-include $(DEPS)

clean:
	@ # C++ related files
	@rm -f *.o *.d a.out

	@ # Junk temp files
	@rm -f *.swap *.save* *.orig

	@echo Done clean-up!
