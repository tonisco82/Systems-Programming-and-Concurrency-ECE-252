paster2: lab_png.h lab_png.c crc.h crc.c zutil.h zutil.c catpng.h catpng.c main_2proc.h main_2proc.c buffer.h buffer.c paster2.c
	gcc -std=c99 -o paster2 lab_png.c crc.c zutil.c catpng.c buffer.c main_2proc.c paster2.c -lz -Wall -lcurl -pthread

clean:
	rm -f paster2 output* *.png
	./clean_ipcs.sh
