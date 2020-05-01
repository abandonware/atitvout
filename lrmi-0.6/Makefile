CFLAGS = -g -Wall

sources = lrmi.c
objects = lrmi.o
pic_objects = pic/lrmi.o
all = liblrmi.a liblrmi.so vbetest

%.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

pic/%.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -fPIC -o $@ $<

all: $(all)

liblrmi.a: $(objects)
	$(AR) -rs $@ $^

liblrmi.so: $(pic_objects)
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -shared -o $@ $^

vbetest: vbetest.c liblrmi.a
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(objects) $(pic_objects) $(all) core

.PHONY: distclean
distclean: clean
	rm -f .depend

.PHONY: depend
depend: $(sources)
	-$(CC) -M $(CPPFLAGS) $^ >.depend
