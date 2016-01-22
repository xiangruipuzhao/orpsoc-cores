CFLAGS= -I. -Wall

.PHONY: default all clean

default: $(TARGET)
all: clean default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	@$(CROSS_COMPILE) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	@$(CROSS_COMPILE) $(OBJECTS) -Wall -o $@


clean:
	-rm -f *.o *.~
	-rm -f $(TARGET)
