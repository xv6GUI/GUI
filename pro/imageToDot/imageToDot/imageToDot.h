#ifndef IMAGETODOT_H
#define IMAGETODOT_H

typedef struct _pic_data pic_data;
struct _pic_data
{
		int width, height;				/* 尺寸 */
		int bit_depth;						/* 位深 */
		int flag;								/* 一个标志，表示是否有alpha通道 */

		unsigned char **rgba;		/* 图片数组 */
};

#define PNG_BYTES_TO_CHECK 4
#define HAVE_ALPHA 1
#define NO_ALPHA 0

static int detect_png(const char *filepath, pic_data *out, FILE* outfile);

#endif