MKDIR      ?= mkdir -p
CPP        ?= cpp
CC         ?= cc

ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer -Wno-format-security
CFLAGS    += -std=c11 -Wall -pedantic
CPPFLAGS  += -MMD -I.
LDFLAGS   += -lm -lpthread -ldl

ifeq ($(DEBUG),0)
  CFLAGS += -O3 -DNDEBUG
else
  CFLAGS += -ggdb -O0
endif

Q ?= @

EXECUTABLE = pagerank
OBJDIR = obj
SRCDIR = src

all: $(EXECUTABLE)

SOURCES   = $(wildcard $(SRCDIR)/*.c)
OBJFILES  = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# rebuild everything if the Makefile changes
$(OBJFILES): Makefile

# include dependency files:
-include $(OBJFILES:.o=.d)

.PHONY: all clean tests

clean:
	$(Q)rm -f $(EXECUTABLE)
	$(Q)rm -rf $(OBJDIR)

$(EXECUTABLE): $(OBJFILES)
	$(Q)echo Linking $@
	$(Q)$(CC) $(LDFLAGS) $(ASAN_FLAGS) -o $@ $^

obj/.dir:
	$(Q)$(MKDIR) $(@D)
	$(Q)date >$@

obj/%.o: $(SRCDIR)/%.c | obj/.dir
	$(Q)echo Compiling $<
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) $(ASAN_FLAGS) -c -o $@ $<

tests: $(EXECUTABLE)
	tests/run-tests.py $(EXECUTABLE)
