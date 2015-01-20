/*******************************************************************************
**  imageToDot.cpp
**  
**  Author: Yang Yanzhe    example.c
**  Last Revised Time: 2015/1/20
**  Function: Transfer image(jpg or png) into dot table which will be used in xv6
**
***********************************************************************************/

#include <iostream>
#include <png.h> 
#include <stdlib.h>
#include <sstream>
#include "imageToDot.h"
using namespace std;

#define PIC_NUM 6

int main()
{
	pic_data img;

	FILE *outfile;
	outfile = fopen("icon.h", "w");

	fprintf(outfile, "unsigned int icon[6][60][60] = {\n");
	for(int pp = 1; pp <= PIC_NUM; pp++)
	{
		stringstream ss;
		string str, path = "icon/";
		ss << pp;
		ss >> str;
		path += str + ".png";
	
		const char* filepath;
		filepath = path.c_str();

		fprintf(outfile, "	{\n");
		detect_png(filepath, &img, outfile);

		if(pp < PIC_NUM)						fprintf(outfile, "	},\n");
		else												fprintf(outfile, "	}\n");
	}
	
	fprintf(outfile, "};");
	fclose(outfile);
	return 0;
}

/* 5位red, 6位green, 5位blue,  */
unsigned int inline transferARGB(int red, int green, int blue, int alpha)
{
	return ((blue / 8)+((green / 4) <<5)+((red / 8)<<11) + (alpha << 16));
}

int inline transferRGB(int red, int green, int blue)
{
	return ((blue / 8)+((green / 4) <<5)+((red / 8)<<11));
}

int detect_png(const char *filepath, pic_data *out, FILE* outfile)
{
	unsigned char* p = (unsigned char*) malloc(sizeof(unsigned char) * 280);
	FILE *pic_fp;
	pic_fp = fopen(filepath, "rb");
		 
	if(pic_fp == NULL){
		printf("图片打开失败\n");
		return -1;
	}
 
	/* 初始化各种结构 */
	png_structp png_ptr;
	png_infop    info_ptr;
	char     buf[PNG_BYTES_TO_CHECK];
	int        temp;
 
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	info_ptr = png_create_info_struct(png_ptr);
 
	setjmp(png_jmpbuf(png_ptr));				// 这句很重要
 
	temp = fread(buf,1,PNG_BYTES_TO_CHECK,pic_fp);

	temp = png_sig_cmp((png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK);
 
	/*检测是否为png文件*/
	if (temp!=0) return 1;
 
	rewind(pic_fp);
	/*开始读文件*/
	png_init_io(png_ptr, pic_fp);
	// 读文件了
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

	int color_type,channels;
 
	/*获取宽度，高度，位深，颜色类型*/
	channels      = png_get_channels(png_ptr, info_ptr);			/*获取通道数*/
	out->bit_depth = png_get_bit_depth(png_ptr, info_ptr);	/* 获取位深 */
	color_type    = png_get_color_type(png_ptr, info_ptr);		/*颜色类型*/
 
	int i,j;
	int size, pos = 0;
	/* row_pointers里边就是rgba数据 */
	png_bytep* row_pointers;
	row_pointers = png_get_rows(png_ptr, info_ptr);
	out->width = png_get_image_width(png_ptr, info_ptr);
	out->height = png_get_image_height(png_ptr, info_ptr);
 
	size = out->width * out->height;		/* 计算图片的总像素点数量 */

	/*如果是RGB+alpha通道，或者RGB+其它字节*/ 
	if(channels == 4 || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		size *= (4*sizeof(unsigned char)); /* 每个像素点占4个字节内存 */
		out->flag = HAVE_ALPHA;				/* 标记 */
			   
		out->rgba = (unsigned char**) malloc(size);
			  
		/* 如果分配内存失败 */
		if(out->rgba == NULL)
		{
			fclose(pic_fp);
			puts("错误(png):无法分配足够的内存供存储数据!");
			return 1;
		}

		temp = (4 * out->width);					/* 每行有4 * out->width个字节 */
		for(i = 0; i < temp; i+=4)
		{
			fprintf(outfile, "		{");
			for(j = 0; j < out->height; j ++)
			{
				int red = row_pointers[j][i];
				int green = row_pointers[j][i+1];
				int blur = row_pointers[j][i+2];
				int alpha = row_pointers[j][i+3];

				fprintf(outfile, "%d", transferARGB(red, green, blur, alpha));

				if(j != out->height  -1)
					fprintf(outfile,  ", ");
				else if(i != out->width - 1)
					fprintf(outfile,  "},\n");
				else
					fprintf(outfile,  "}\n");

				++pos;
			}
		}
	}
	/* 如果是RGB通道 */
	else if(channels == 3 || color_type == PNG_COLOR_TYPE_RGB)
	{
		size *= (3*sizeof(unsigned char));		/* 每个像素点占3个字节内存 */
		out->flag = NO_ALPHA;						/* 标记 */
		out->rgba = (unsigned char**) malloc(size);

		/* 如果分配内存失败 */
		if(out->rgba == NULL)
		{
			fclose(pic_fp);
			puts("错误(jpg):无法分配足够的内存供存储数据!");
			return 1;
		}

		temp = (3 * out->width);			/* 每行有3 * out->width个字节 */
		for(i = 0; i < temp; i+=3)
		{
			fprintf(outfile, "		{");
			for(j = 0; j < out->height; j ++)		/* 一个字节一个字节的赋值 */
			{
				int red = row_pointers[j][i];
				int green = row_pointers[j][i+1];
				int blur = row_pointers[j][i+2];

				fprintf(outfile, "%d", transferRGB(red, green, blur));

				if(j != out->height  -1)
					fprintf(outfile,  ", ");
				else if(i != out->width - 1)
					fprintf(outfile,  "},\n");
				else
					fprintf(outfile,  "}\n");

				++pos;
			}
		}
	}
	else return 1;
 
	/* 撤销数据占用的内存 */
	png_destroy_read_struct(&png_ptr, &info_ptr, 0); 

	return 0;
}
