IDIR = include
ODIR = obj
SDIR = src
JDIR = java

CC = gcc
CFLAGS = -Wall -fPIC -I$(IDIR)
LIB = $(JDIR)/libmylibrary.so

JINCLUDES = -I/usr/lib/jvm/default-java/ -I/usr/lib/jvm/default-java/include/ -I/usr/lib/jvm/default-java/include/linux/
#JINCLUDES = -I/usr/lib/jvm/java-11-openjdk-amd64/ -I/usr/lib/jvm/java-11-openjdk-amd64/include/ -I/usr/lib/jvm/java-11-openjdk-amd64/include/linux/

_DEPS = getSchedule.h bstree.h instance.h list.h olist.h schedule.h utilities.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = getSchedule.o bstree.o instance.o list.o olist.o schedule.o utilities.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: $(OBJ) $(ODIR)/getSchedule.o
	$(CC) -shared -o $(LIB) $^

$(ODIR)/getSchedule.o : $(SDIR)/getSchedule.c $(DEPS)
	$(CC) $(CFLAGS) $(JINCLUDES) -c -o $@ $<

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(LIB)
