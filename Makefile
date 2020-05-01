VERSION=0.4
CFLAGS=-Wall -O2 -g -DVERSION=\"$(VERSION)\" -I./lrmi-0.6
OBJS=atitvout.o ati.o vbeinfo.o vbecall.o mach64.o radeon.o
TAR=atitvout-$(VERSION).tar.gz
BINARY=atitvout
PACKAGE=atitvout

all: $(BINARY)

$(BINARY): $(OBJS)
	make -C lrmi-0.6
	$(CC) $(OBJS) lrmi-0.6/liblrmi.a -o $(BINARY)

strip: $(BINARY)
	@du -h $(BINARY)
	strip -s $(BINARY)
	@du -h $(BINARY)

clean:
	rm -f *.o $(BINARY) *~ README index.html *.tar.gz test.out
	make -C lrmi-0.6 clean

install: strip
	install -g root -o root -m 555 $(BINARY) /usr/local/sbin

deinstall:
	rm -f /usr/local/sbin/$(BINARY)

README: README.in
	sed s/VERSION/$(VERSION)/ < README.in > README

index.html: README
	txt2html -t "$(PACKAGE) $(VERSION)" < README > index.html

html: index.html

$(TAR): clean README
	tar -C.. --exclude=$(PACKAGE)/$(TAR) -czvf $(TAR) $(PACKAGE)

tar: $(TAR)

web: $(TAR) README
	cp README $(TAR) ../../homepage/lennart/projects/$(PACKAGE)/ && $(MAKE) -C ../../homepage/lennart/projects/$(PACKAGE)

upload: web
	$(MAKE) -C ../../homepage/lennart upload

emacs:
	emacs Makefile README.in *.h *.c &

test: $(BINARY)
	./test.sh

.PHONY: test emacs upload web tar html deinstall install clean strip all
