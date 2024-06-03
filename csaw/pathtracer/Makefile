CSAW = ../../install/bin/csaw
GCC  = gcc

STDDIR = ../std/include
INCDIR = include
SRCDIR = src
OBJDIR = build

CSAW_FLAGS = -i $(INCDIR) -i $(STDDIR) -e $(OBJDIR)
GCC_FLAGS  =

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

TARGET = $(OBJDIR)/pathtracer.exe
SRCS   = $(call rwildcard, $(SRCDIR), *.csaw)
OBJS   = $(patsubst $(SRCDIR)/%.csaw, $(OBJDIR)/%.o, $(SRCS))

all: $(TARGET)

clean:
	@if exist $(OBJDIR) rd /s /q $(OBJDIR)

run: $(TARGET)
	@$(TARGET) out.ppm

$(TARGET): $(OBJS)
	$(GCC) -o $@ $^ $(GCC_FLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.csaw
	@if not exist $(@D) mkdir $(subst /,\,$(@D))
	$(CSAW) $(CSAW_FLAGS) -o $@ $<

.PHONY: all clean run
