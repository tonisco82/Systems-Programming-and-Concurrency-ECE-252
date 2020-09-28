#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "lab_png.h"
#include "crc.h"
#include "zutil.h"

unsigned long expected_CRC(struct chunk* buf) {
	U8* crc_buf = malloc(buf->length + 4);
	memcpy(crc_buf, &buf->type, 4);
	memcpy(crc_buf + 4, buf->p_data, buf->length);

	unsigned long result = crc(crc_buf, 4 + buf->length);

	free(crc_buf);

	return result;
}

void write_chunk(struct chunk* buf, FILE* fp) {
	if (fp == NULL) {
		return;
	}
	U32 net_length = htonl(buf->length);
	fwrite(&net_length, 4, 1, fp);
	fwrite(&buf->type[0], 1, 1, fp);
	fwrite(&buf->type[1], 1, 1, fp);
	fwrite(&buf->type[2], 1, 1, fp);
	fwrite(&buf->type[3], 1, 1, fp);
	fwrite(buf->p_data, buf->length, 1, fp);
	U32 net_crc = htonl(buf->crc);
	fwrite(&net_crc, 4, 1, fp);
}

int main(int argc, char** argv) {
	if (argc == 1) {
		return -1;
	}
	/*assumed all arguments are valid pngs*/

	FILE* sample = fopen(argv[1], "r");
	if (sample == NULL) {
		return -1;
	}

	/*Init array of IDAT chunks*/
	struct chunk** IDAT_arr = malloc((argc-1) * sizeof(struct chunk*));

	/*read header*/
	U8 header[8];
	fread(header, 8, 1, sample);
	/*init IHDR and IHDR_data (independent)*/
	struct chunk* new_IHDR = malloc(sizeof(struct chunk));
	get_chunk(new_IHDR, sample, 0);
	struct data_IHDR* new_IHDR_data = malloc(sizeof(struct data_IHDR));
	get_png_data_IHDR(new_IHDR_data, sample, 0, 0);
	/*init IDAT*/
	struct chunk* new_IDAT = malloc(sizeof(struct chunk));
	get_chunk(new_IDAT, sample, 1);
	IDAT_arr[0] = new_IDAT;
	/*read IEND data*/
	struct chunk* new_IEND = malloc(sizeof(struct chunk));
	get_chunk(new_IEND, sample, 2);

	/*cycle from image 2 to N*/
	for (int i = 2; i < argc; ++i) {
		FILE* png_inst = fopen(argv[i], "r");
		if (png_inst == NULL) {
			/*add freeing operations here*/
			return -1;
		}

		/*read image height*/
		fseek(png_inst, 20, SEEK_SET);
		U32 inst_height;
		fread(&inst_height, 4, 1, png_inst);
		new_IHDR_data->height += ntohl(inst_height);

		/*read IDAT data*/
		struct chunk* IDAT_inst = malloc(sizeof(struct chunk));
		get_chunk(IDAT_inst, png_inst, 1);
		IDAT_arr[i - 1] = IDAT_inst;

		fclose(png_inst);
	}

	/*Here, we save the final height value calculated and put it into the appropiate location in IHDR data*/
	U32 final_height = htonl(new_IHDR_data->height);
	memcpy(new_IHDR->p_data + 4, &final_height, 4);

	/*compute expected approximate IDAT Length*/
	for (int i = 1; i < argc - 1; ++i) {
		new_IDAT->length += IDAT_arr[i]->length;
	}

	/*Concatenate IDAT data*/
	U8* inflated_data = malloc(30* new_IDAT->length);
	U64 buffer_index = 0;
	for (int i = 0; i < argc - 1; ++i) {
		U64 len_inf = 0;
		U64 src_length = IDAT_arr[i]->length;
		int ret = mem_inf(inflated_data + buffer_index, &len_inf, IDAT_arr[i]->p_data, src_length);
		if (ret) {	/*failure*/
			/*clean up*/
			return ret;
		}
		buffer_index += len_inf;
	}
	free(new_IDAT->p_data);
	new_IDAT->p_data = malloc(2 * new_IDAT->length);
	U64 len_def = 0;
	int ret = mem_def(new_IDAT->p_data, &len_def, inflated_data, buffer_index, Z_DEFAULT_COMPRESSION);
	if (ret) {
		/*clean up*/
		return ret;
	}
	new_IDAT->length = len_def;

	/*Compute new CRC Values here*/
	new_IHDR->crc = expected_CRC(new_IHDR);
	new_IDAT->crc = expected_CRC(new_IDAT);

	FILE* merged = fopen("all.png", "w");
	fwrite(header, 1, 8, merged);

	write_chunk(new_IHDR, merged);
	write_chunk(new_IDAT, merged);
	write_chunk(new_IEND, merged);

	free(inflated_data);
	free(new_IHDR_data);
	free(new_IHDR->p_data);
	free(new_IHDR);
	/*free all IDAT data*/
	for (int i = 0; i < argc - 1; ++i) {
		free(IDAT_arr[i]->p_data);
		free(IDAT_arr[i]);
	}
	free(IDAT_arr);
	free(new_IEND);
	fclose(sample);
	fclose(merged);
	return 0;
}

/*ALGORITHM TO GENERATE NEW IDAT DATA
	Get compressed pixel data from each png, inflate the data, and store as new data
	Take new data and create IDAT chunk by compressing using mem_def
*/

/*write data to merged png in order
        HEADER: CAN COPY FROM THE FIRST FILE
        IHDR:
                LENGTH IS 13
                TYPE CAN COPY
                DATA: ALL SAME, EXCEPT HEIGHT IS THE SUM OF THE HEIGHTS OF THE CONSTITUENTS
                CRC: COMPUTED BASED ON NEW TYPE AND DATA
        IDAT: Use the above algorithm to generate this field
        IEND: CAN COPY FROM FIRST FILE
*/
