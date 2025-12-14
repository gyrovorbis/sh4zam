TARGET = pvr_dma.elf
OBJS = pvr_dma.o romdisk.o
KOS_ROMDISK_DIR = romdisk

# Optimization flags
KOS_CFLAGS += -std=gnu2x -O3 \
              -fomit-frame-pointer -ffast-math -ffp-contract=fast \
              -fmerge-all-constants -funroll-loops \
              -ftree-vectorize

all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean: rm-elf
	-rm -f $(OBJS)

rm-elf:
	-rm -f $(TARGET) romdisk.*

$(TARGET): $(OBJS)
	kos-cc -o $(TARGET) $(OBJS) -lpng -ljpeg -lkmg -lz -lkosutils -lm -lsh4zam

run: $(TARGET)
	$(KOS_LOADER) $(TARGET)

dist: $(TARGET)
	-rm -f $(OBJS) romdisk.img
	$(KOS_STRIP) $(TARGET)
