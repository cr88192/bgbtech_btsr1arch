/*
Copyright (C) 2015 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifdef __linux__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <malloc.h>
#include <errno.h>
// #include <linux/videodev.h>
#include <linux/videodev2.h>
 
typedef unsigned char byte;
 
typedef struct {
	int  fd;
	char device[0xFF];
	char name[0xFF];
	int width;
	int height;
	int depth;

	int cMap;
	int nMap;
	byte *mmap[16];
}v4l_device;

typedef struct {
	byte *data;

	int width;
	int height;
	int depth;
	int size;
}v4l_videobuffer;

static int v4l_xioctl(int fh, int request, void *arg)
{
	int r;

	r=ioctl(fh, request, arg);
	while((r==(-1)) && (errno==EINTR))
		{ r=ioctl(fh, request, arg); }

	return r;
}

int v4l_open(char *devname, v4l_device *dev)
{
	struct v4l2_format fmt;
	struct v4l2_requestbuffers rqb;
	struct v4l2_buffer buf;
	int i;

	dev->fd=open(devname, O_RDWR);
	if(dev->fd<=0)
	{
		perror(devname);
		return -1;	
	}

	strcpy(dev->device, devname);

	dev->width=640;
	dev->height=480;

	fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width=640;
	fmt.fmt.pix.height=480;
	fmt.fmt.pix.pixelformat=V4L2_PIX_FMT_YUYV;
	fmt.fmt.pix.field=V4L2_FIELD_INTERLACED;
	if(v4l_xioctl(dev->fd, VIDIOC_S_FMT, &fmt)<0)
	{
		printf("VIDIOC_S_FMT\n");
		perror(devname);
		return -1;	
	}

	memset(&rqb, 0, sizeof(struct v4l2_requestbuffers));
	rqb.count=4;
	rqb.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	rqb.memory=V4L2_MEMORY_MMAP;

	if(v4l_xioctl(dev->fd, VIDIOC_REQBUFS, &rqb)<0)
	{
		printf("VIDIOC_REQBUFS\n");
		perror(devname);
		return -1;	
	}

	dev->nMap=rqb.count;

	for(i=0; i<dev->nMap; i++)
	{
		memset(&buf, 0, sizeof(struct v4l2_buffer));
		buf.index=i;
		buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory=V4L2_MEMORY_MMAP;

		if(v4l_xioctl(dev->fd, VIDIOC_QUERYBUF, &buf)<0)
		{
			printf("VIDIOC_QUERYBUF\n");
			perror(devname);
			return -1;	
		}

		dev->mmap[i]=(byte *)mmap(0, buf.length,
			PROT_READ|PROT_WRITE, MAP_SHARED, dev->fd, buf.m.offset);	

		if(dev->mmap[i]==MAP_FAILED)
		{
			printf("MMAP Fail\n");
			perror(devname);
			dev->mmap[i]=NULL;
			return -1;	
		}
	}

	return 1;
}

int v4l_trynextframe(v4l_device *dev)
{
	enum v4l2_buf_type type;
	struct v4l2_buffer buf0, buf1;
	int i0, i1;
	int i;

	i0=dev->cMap;
	i1=(dev->cMap+1)%dev->nMap;
	
	buf0.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf0.memory=V4L2_MEMORY_MMAP;
	buf0.index=i0;
	i=ioctl(dev->fd, VIDIOC_QUERYBUF, &buf0);
	if(i<0)
	{
		printf("v4l_trynextframe: ");
		perror("VIDIOC_QUERYBUF");
		return(-1);
	}

	buf1.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf1.memory=V4L2_MEMORY_MMAP;
	buf1.index=i1;
	i=ioctl(dev->fd, VIDIOC_QUERYBUF, &buf1);
	if(i<0)
	{
		printf("v4l_trynextframe VIDIOC_QUERYBUF: ");
		perror("VIDIOC_QUERYBUF");
		return(-1);
	}

	if((buf0.flags&V4L2_BUF_FLAG_DONE) &&
		(buf1.flags&V4L2_BUF_FLAG_DONE))
	{
//		memset(&buf, 0, sizeof(struct v4l2_buffer));
		buf1.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf1.memory=V4L2_MEMORY_MMAP;
		buf1.index=i0;
		ioctl(dev->fd, VIDIOC_QBUF, &buf1);
		
		dev->cMap=i1;
		return(1);
	}
	
	return(0);
}

int v4l_capture(v4l_device *dev, v4l_videobuffer *vbuffer)
{
	enum v4l2_buf_type type;
	struct v4l2_buffer buf;
	int i;

	for(i=0; i<dev->nMap; i++)
	{
		memset(&buf, 0, sizeof(struct v4l2_buffer));
		buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory=V4L2_MEMORY_MMAP;
		buf.index=i;
		if(v4l_xioctl(dev->fd, VIDIOC_QBUF, &buf)<0)
		{
			perror("VIDIOC_QBUF");
			return -1;
		}
	}

	type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(v4l_xioctl(dev->fd, VIDIOC_STREAMON, &type)<0)
	{
		perror("VIDIOC_STREAMON");
		return -1;
	}
	
	vbuffer->data=dev->mmap;

	return 1;
}

void v4l_close(v4l_device *dev)
{
	munmap(dev->mmap, dev->width*dev->height*dev->depth);
	close(dev->fd);
}

v4l_videobuffer *v4l_create(int width, int height, int depth)
{
	v4l_videobuffer *buffer;

	buffer=(v4l_videobuffer *)malloc(sizeof(v4l_videobuffer));
	buffer->width=width;
	buffer->height=height;
	buffer->depth=depth;
	buffer->size=width*height*depth;
	buffer->data=(byte *)malloc(buffer->size);

	return buffer;
}

v4l_device *v4l_alloc_device(void)
{
	v4l_device *dev;
	dev=(v4l_device *)malloc(sizeof(v4l_device));
	memset(dev, 0, sizeof(v4l_device));
	return dev;
}


v4l_device *btic1g_cam0_dev;
v4l_videobuffer *btic1g_cam0_buf;

byte *BTIC1G_GetCameraFBuf()
{
	if(btic1g_cam0_dev)
		return(btic1g_cam0_dev->mmap[btic1g_cam0_dev->cMap]);
	return(NULL);
}

int BTIC1G_NextCameraFrame()
{
	if(btic1g_cam0_dev)
		return(v4l_trynextframe(btic1g_cam0_dev));
	return(NULL);
}

void BTIC1G_InitCamera()
{
	static int init=0;
	int i, j, k;
	
	if(init)
		return;
	init=1;

	printf("BTIC1G_InitCamera\n");

	btic1g_cam0_dev=v4l_alloc_device();
	i=v4l_open("/dev/video0", btic1g_cam0_dev);
	if(i<0)
	{
		printf("BTIC1G_InitCamera: Failed\n");
		free(btic1g_cam0_dev);
		btic1g_cam0_dev=NULL;
		return;
	}
		
	printf("%dx%d\n", btic1g_cam0_dev->width, btic1g_cam0_dev->height);
	
	btic1g_cam0_buf=v4l_create(
		btic1g_cam0_dev->width,
		btic1g_cam0_dev->height,
		2);
	i=v4l_capture(
		btic1g_cam0_dev,
		btic1g_cam0_buf);

	if(i<0)
	{
		printf("BTIC1G_InitCamera: Failed 2\n");
		v4l_close(btic1g_cam0_dev);
		free(btic1g_cam0_dev);
		btic1g_cam0_dev=NULL;
		return;
	}

	printf("BTIC1G_InitCamera: OK\n");
}

void BTIC1G_DeinitCamera()
{
	if(btic1g_cam0_dev)
	{
		v4l_close(btic1g_cam0_dev);
		free(btic1g_cam0_dev);
		btic1g_cam0_dev=NULL;
	}
}

#else

byte *BTIC1G_GetCameraFBuf()
{
	return(NULL);
}

int BTIC1G_NextCameraFrame()
{
	return(0);
}

void BTIC1G_InitCamera()
{
}

void BTIC1G_DeinitCamera()
{
}

#endif
