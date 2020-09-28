default: catpng.c crc.c crc.h findpng.c lab_png.c lab_png.h pnginfo.c zutil.c zutil.h
	gcc -std=c99 lab_png.c crc.c pnginfo.c -o pnginfo
	gcc -std=c99 lab_png.c crc.c findpng.c -o findpng
	gcc -std=c99 lab_png.c crc.c zutil.c catpng.c -o catpng -lz

pnginfo: lab_png.h crc.h lab_png.c crc.c pnginfo.c
	gcc -std=c99 lab_png.c crc.c pnginfo.c -o pnginfo

findpng: lab_png.h crc.h lab_png.c crc.c findpng.c
	gcc -std=c99 lab_png.c crc.c findpng.c -o findpng

catpng: lab_png.h crc.h zutil.h lab_png.c crc.c zutil.c catpng.c
	gcc -std=c99 lab_png.c crc.c zutil.c catpng.c -o catpng -lz

clean:
	rm -f pnginfo findpng catpng
