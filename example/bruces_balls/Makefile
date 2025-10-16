TARGET = bruces_balls.elf
OBJS = bruces_balls.o   

CFLAGS += -std=gnu23 -O3 \
          -fomit-frame-pointer -flto -fbuiltin -ffast-math -ffp-contract=fast -mfsrra -mfsca \
          -fmerge-all-constants -funroll-loops -fno-PIC -fipa-pta  \
          -ftree-vectorize -I$(KOS_BASE)/utils  


all: rm-elf $(TARGET)
include $(KOS_BASE)/Makefile.rules

clean: rm-elf
	-rm -f $(OBJS)

rm-elf:
	-rm -f $(TARGET)  

$(TARGET): $(OBJS)
	kos-cc -o $(TARGET) $(OBJS) -lsh4zam -lkosutils -lm \
           -Wl,--gc-sections -Wl,--strip-all

run: $(TARGET)
	$(KOS_LOADER) $(TARGET)

dist: $(TARGET)
	-rm -f $(OBJS)  
	$(KOS_STRIP) $(TARGET)