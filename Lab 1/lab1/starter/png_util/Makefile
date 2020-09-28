pnginfo: lab_png.h crc.h lab_png.c crc.c pnginfo.c
	gcc -std=c99 lab_png.c crc.c pnginfo.c -o pnginfo

findpng: lab_png.h crc.h lab_png.c crc.c findpng.c
	gcc -std=c99 lab_png.c crc.c findpng.c -o findpng

catpng: lab_png.h crc.h zutil.h lab_png.c crc.c zutil.c catpng.c
	gcc -std=c99 lab_png.c crc.c zutil.c catpng.c -o catpng -lz

run: catpng
	valgrind ./catpng ../images/cropped/pic_cropped_0.png ../images/cropped/pic_cropped_1.png ../images/cropped/pic_cropped_2.png ../images/cropped/pic_cropped_3.png ../images/cropped/pic_cropped_4.png
	valgrind ./catpng ../images/uweng_cropped/uweng_cropped_0.png ../images/uweng_cropped/uweng_cropped_1.png ../images/uweng_cropped/uweng_cropped_2.png ../images/uweng_cropped/uweng_cropped_3.png ../images/uweng_cropped/uweng_cropped_4.png ../images/uweng_cropped/uweng_cropped_5.png ../images/uweng_cropped/uweng_cropped_6.png

clean:
	rm -f catpng pnginfo findpng
