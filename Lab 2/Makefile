paster: lab_png.h lab_png.c crc.h crc.c zutil.h zutil.c catpng.h catpng.c main_write_header_cb.h main_write_header_cb.c paster.c
	gcc -std=c99 -o paster lab_png.c crc.c zutil.c catpng.c main_write_header_cb.c paster.c -lz -Wall -lcurl -pthread

clean:
	rm -f paster output* *.png
