all: ppramRead ppramClear

ppramRead: ppramRead.c ppram.h
	cc -o ppramRead ppramRead.c

ppramClear: ppramClear.c ppram.h
	cc -o ppramClear ppramClear.c

clean:
	rm -f ppramRead ppramClear
