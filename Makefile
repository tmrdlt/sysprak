CC       = /usr/bin/gcc
# compiling flags here
CFLAGS   = -std=c99 -Wall -I. -Wcpp -Wextra -Wpedantic
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
GAMEID   = 123gameid1234 	# -g flag (REQUIRED)
PLAYER   = 1							# -p flag (OPTIONAL)
CONFIG   = client.conf		# -f flag (OPTIONAL)

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

play: client
			./$< -g $(GAMEID)

.PHONY: clean
clean:
	@$(rm) $(OBJECTS) $(BINDIR)/$(TARGET)
	@echo "Cleanup complete!"
