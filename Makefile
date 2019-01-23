INCDIR=include
SRCDIR=src
LIBDIR=lib
EXAMPLEDIR=examples
PANELDIR=$(SRCDIR)/panel

CC=gcc
CPP=g++
LD=ld
LN=ln
AR=ar
RANLIB=ranlib
CFLAGS=-fPIC -fno-stack-protector -fno-pie -no-pie
CFLAGS+=-g -O0	# debug version
#CFLAGS+=-O2	# release version
LDFLAGS=-shared
INCLUDES=-I/usr/include -I$(INCDIR) -I$(PANELDIR)
LIBS=-L/usr/lib -L/usr/X11R6/lib/ -lGL -lGLU -lX11 -lXext -lm
LIB_OBJS=$(LIBDIR)/buffer.o       \
     $(LIBDIR)/color.o            \
     $(LIBDIR)/device.o           \
     $(LIBDIR)/grpos.o            \
     $(LIBDIR)/igl.o              \
     $(LIBDIR)/light.o            \
     $(LIBDIR)/matrix.o           \
     $(LIBDIR)/menu.o             \
     $(LIBDIR)/object.o           \
     $(LIBDIR)/text.o             \
     $(LIBDIR)/texture.o          \
     $(LIBDIR)/vertex.o           \
     $(LIBDIR)/window.o

EXAMPLE_OBJS=$(EXAMPLEDIR)/demo1.o    \
             $(EXAMPLEDIR)/demo2.o    \
             $(EXAMPLEDIR)/demo3.o    \
             $(EXAMPLEDIR)/demo4.o    \
             $(EXAMPLEDIR)/demo5.o

PANEL_OBJS=$(PANELDIR)/panel.o	\
	$(PANELDIR)/button.o	\
	$(PANELDIR)/list.o	\
	$(PANELDIR)/user.o	\
	$(PANELDIR)/mslider.o	\
	$(PANELDIR)/slider.o	\
	$(PANELDIR)/script.o	\
	$(PANELDIR)/typein.o	\
	$(PANELDIR)/puck.o	\
	$(PANELDIR)/D.app/colormod.o	\
	$(PANELDIR)/D.app/fade.o	\
	$(PANELDIR)/D.app/ep.o

#
# targets
#
SHARED=$(LIBDIR)/libIGL.so.0.1.8
STATIC=$(LIBDIR)/libIGL.a
EXAMPLES=$(EXAMPLEDIR)/demo1 $(EXAMPLEDIR)/demo2 $(EXAMPLEDIR)/demo3 $(EXAMPLEDIR)/demo4 $(EXAMPLEDIR)/demo5
EP=ep

all: $(EP) $(SHARED) $(STATIC) $(EXAMPLES)
shared: $(SHARED)
static: $(STATIC)
examples: $(EXAMPLES)

$(PANEL_OBJS): $(PANELDIR)/%.o: $(PANELDIR)/%.c $(INCDIR)/*.h
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(LIB_OBJS): $(LIBDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/*.h
	@echo Compiling $<...
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(EP): $(PANEL_OBJS) $(LIB_OBJS)
	$(CC) $(LIBS) $(LIB_OBJS) $(PANEL_OBJS) -o $@ -g

$(SHARED): $(LIB_OBJS)
	@echo Creating shared library $@...
	@$(LD) $(LDFLAGS) $(LIBS) $(LIB_OBJS) -o $@
	@echo DONE

$(STATIC): $(LIB_OBJS)
	@echo Creating static library $@...
	@$(AR) cru $@ $(LIB_OBJS)
	@$(RANLIB) $@
	@echo DONE

$(EXAMPLE_OBJS): $(EXAMPLEDIR)/%.o: $(EXAMPLEDIR)/%.c $(LIB_OBJS)
	@echo Compiling $<...
	@$(CC) $(INCLUDES) -o $@ $< $(LIBS) -L./lib -lIGL -e igl_X11main -lGL

$(EXAMPLES): %: %.o $(SHARED) $(STATIC)
	@echo Linking $(@F)...
	#@$(LD) $@.o -o $@ -L./lib -lIGL -e igl_X11main -lGL -lGLU -lX11 -lc -lm
	@$(LD) $@.o -o $@ -L./lib -lIGL -lGL -lGLU -lX11 -lc -lm

clean:
	@echo Cleaning up...
	@rm -f $(LIB_OBJS) $(SHARED) $(STATIC)
	@rm -f $(EXAMPLE_OBJS) $(EXAMPLES)
	@rm -f $(PANEL_OBJS)
	@rm $(EP)
	@echo DONE

