include Config
all:

# ************* #
# Real targets. #
# ************* #

# Interface headers.
FACE_INCS = $(wildcard bbunit/*.h)
FACE_OBJS = $(patsubst bbunit/%.h, build/%.h.o, $(FACE_INCS))

$(FACE_OBJS): build/%.h.o: bbunit/%.h $(FACE_INCS)
	@mkdir -p build
	echo -e "#include <$<>\nint main(void) { return 0; }\n" | \
	$(CC) $(ALL_HFLAGS) -c -xc - -I. -o $@

# Main library.
MAIN_SRCS = engine.c report.c
MAIN_OBJS = $(patsubst %.c, build/%.o, $(MAIN_SRCS))
MAIN_LIB  = build/libbbunit.a

$(MAIN_OBJS): build/%.o: %.c $(FACE_INCS)
	@mkdir -p build
	$(CC) $(ALL_CFLAGS) -c $< -I. -o $@

$(MAIN_LIB): $(MAIN_OBJS)
	ar rcs $@ $^

# Demo program.
DEMO_SRC  = demo.c
DEMO_BIN  = build/demo

$(DEMO_BIN): $(DEMO_SRC) $(MAIN_LIB)
	$(CC) $(ALL_HFLAGS) $(ALL_LDFLAGS) $< -I. -Lbuild -lbbunit -o $@

# ************** #
# Phony targets. #
# ************** #

.PHONY: all face demo install clean dist

all: $(MAIN_LIB)
face: $(FACE_OBJS)
demo: $(DEMO_BIN)

install: all
	install -m644 $(FACE_INCS) -Dt $(DESTDIR)$(INCDIR)/bbunit
	install -m644 $(MAIN_LIB) -Dt $(DESTDIR)$(LIBDIR)
	install -m644 $(DEMO_SRC) -Dt $(DESTDIR)$(SHAREDIR)/bbunit
clean:
	rm -rf build
dist:
	@mkdir -p build
	tar -zcf build/$(TARNAME).tar.gz --transform="s/^\./$(TARNAME)/" \
		--exclude="./build" --exclude="./.*" .
