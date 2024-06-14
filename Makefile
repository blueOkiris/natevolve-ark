# Build system for Natevolve Ark library

# Settings

## Project

PROJNAME :=		natevolve
OBJNAME :=		lib$(PROJNAME).a
SRC :=			$(wildcard src/*.cpp)
OBJS :=			$(subst src/,obj/,$(subst .cpp,.o,$(SRC)))
HFILES :=		$(wildcard include/*.hpp)

## Test

ifeq ($(OS), Windows_NT)
TESTOBJ :=		test.exe
else
TESTOBJ :=		test.bin
endif
TESTSRC :=		$(wildcard test/*.cpp)
TESTOBJS :=		$(subst test/,test/obj/,$(subst .cpp,.o,$(TESTSRC)))

## Compiler

CPPC :=			g++
CPPFLAGS :=		-std=c++17 -O2 -Wall -Werror -Iinclude
LD :=			g++
LDFLAGS :=		-L. -l$(PROJNAME)

# Targets

## Helper

.PHONY: all
all: $(OBJNAME)

.PHONY: test
test: $(TESTOBJ)

.PHONY: clean
clean:
	rm -rf obj/
	rm -rf test/obj/
	rm -rf $(OBJNAME)
	rm -rf $(TESTOBJ)

## Main

obj/%.o: src/%.cpp $(HFILES)
ifeq ($(OS), Windows_NT)
	-mkdir obj
else
	mkdir -p obj
endif
	$(CPPC) -o $@ $(CPPFLAGS) -c $<

$(OBJNAME): $(OBJS)
	ar rcs $@ $(OBJS)

test/obj/%.o: test/%.cpp $(HFILES)
ifeq ($(OS), Windows_NT)
	-mkdir test\obj
else
	mkdir -p test/obj
endif
	$(CPPC) -o $@ $(CPPFLAGS) -c $<

$(TESTOBJ): $(OBJNAME) $(TESTOBJS)
	$(LD) -o $@ $(TESTOBJS) $(LDFLAGS)

