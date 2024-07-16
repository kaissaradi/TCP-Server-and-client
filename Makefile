# Makefile
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm

SRCDIR = src
INCDIR = include
OBJDIR = obj

OBJECTS = $(OBJDIR)/cipher.o $(OBJDIR)/keygen.o $(OBJDIR)/otc_d.o $(OBJDIR)/otp.o

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

all: otp otc_d keygen

otp: $(OBJDIR)/otp.o $(OBJDIR)/cipher.o
	$(CC) $(CFLAGS) $^ -o $@

otc_d: $(OBJDIR)/otc_d.o $(OBJDIR)/cipher.o
	$(CC) $(CFLAGS) $^ -o $@

keygen: $(OBJDIR)/keygen.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OBJDIR) otp otc_d keygen

.PHONY: all clean
