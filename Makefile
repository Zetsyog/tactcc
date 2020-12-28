CC   ?= gcc
YACC ?= yacc
LEX  ?= lex

CFLAGS    ?= -Wall -Wextra -g
LDLIBS    ?= -lfl
YACCFLAGS ?= -v

INCLUDE_PATH = ./include
TEST_DIR = test
TARGET   = scalpa
SRCDIR   = src
OBJDIR   = build
BINDIR   = bin

LEX_HDR_NAME = grammar.h

YSOURCES := $(wildcard $(SRCDIR)/**/*.y)
LSOURCES := $(wildcard $(SRCDIR)/**/*.l)
YBUILD   := $(YSOURCES:$(SRCDIR)/%.y=$(OBJDIR)/%.yacc.c)
LBUILD   := $(LSOURCES:$(SRCDIR)/%.l=$(OBJDIR)/%.lex.c)
SOURCES  := $(wildcard $(SRCDIR)/**/*.c) $(wildcard $(SRCDIR)/*.c)

INCLUDES := $(wildcard $(INCLUDE_PATH)/**/*.h) $(wildcard $(INCLUDE_PATH)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
OBJECTS2 := $(YBUILD:$(OBJDIR)/%.c=$(OBJDIR)/%.o) $(LBUILD:$(OBJDIR)/%.c=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS2) $(OBJECTS)
	@echo --- Linking ---
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS) -I$(INCLUDE_PATH)

$(OBJECTS2): $(OBJDIR)/%.o : $(OBJDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS) -I$(INCLUDE_PATH)

$(YBUILD): $(YSOURCES)
	@echo --- Compiling yacc ---
	mkdir -p $(dir $@)
	$(YACC) $(YACCFLAGS) -o $@ --defines=$(INCLUDE_PATH)/$(LEX_HDR_NAME) $<

$(LBUILD): $(LSOURCES)
	@echo --- Compiling lex ---
	mkdir -p $(dir $@)
	$(LEX) -o $@ $<

.PHONY: clean doc test
clean:
	rm -rf $(OBJDIR)/*
	find $(TEST_DIR)/ -name "*.tmp" -type f -delete
	rm $(INCLUDE_PATH)/$(LEX_HDR_NAME)
	rm -f $(BINDIR)/$(TARGET)

test: $(BINDIR)/$(TARGET)
	cd test; sh test.sh

doc:
	doxygen doxygen.conf