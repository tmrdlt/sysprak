CC       = /usr/bin/gcc
# compiling flags here
CFLAGS   = -std=c99 -Wall -I. -Wcpp -Wextra -Wpedantic -Werror
LINKER   = gcc -o
# linking flags here
LFLAGS   = -Wall -I. -lm
# standard make target
TARGET   = client
# subdirectories
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin
# variables
# -g flag (REQUIRED)
GAMEID   = 123gameid1234
# -p flag (OPTIONAL)
PLAYER   = 1
# -f flag (OPTIONAL)
CONFIG   = client.conf

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(LFLAGS) $(OBJECTS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

play: $(BINDIR)/$(TARGET)
			./$< -g $(GAMEID) -p$(PLAYER) -f$(CONFIG)

.PHONY: clean
clean:
	@$(rm) $(OBJECTS) $(BINDIR)/$(TARGET)$(GAMEID)
	@echo "Cleanup complete!"
