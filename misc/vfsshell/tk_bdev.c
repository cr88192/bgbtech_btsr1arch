FILE *tkbdev_sdimage[256];

char *tkspi_sdimg_name;
int tkspi_sdimg_sz;
int tkspi_id_bdev;

int TKSPI_SetInitSizeString(char *str)
{
	char *sfx;
	int mult;
	
	if(!str)
		return(-1);
	
	sfx=str+strlen(str)-1;
	mult=1;
	
	if((*sfx=='k') || (*sfx=='K'))
		mult=2;
	if((*sfx=='m') || (*sfx=='M'))
		mult=2048;
	if((*sfx=='g') || (*sfx=='G'))
		mult=2048*1024;
	
	tkspi_sdimg_sz=tk_atoi(str)*mult;
	return(0);
}

int TKSPI_InitDevice()
{
	char tbuf[512];
	int i;

	if(!tkspi_sdimg_name)
	{
		tkspi_sdimg_name="sdimage.img";
	}
	
	tkbdev_sdimage[1]=fopen(tkspi_sdimg_name, "r+b");
	if(!tkbdev_sdimage[1])
	{
		if(tkspi_sdimg_sz>0)
		{
			tkbdev_sdimage[1]=fopen(tkspi_sdimg_name, "w+b");
			memset(tbuf, 0, 512);
			for(i=0; i<tkspi_sdimg_sz; i++)
			{
				fwrite(tbuf, 1, 512, tkbdev_sdimage[1]);
			}
		}else
		{
			printf("TKSPI_InitDevice: Fail open %s\n", tkspi_sdimg_name);
		}
	}else
	{
		fseek(tkbdev_sdimage[1], 0, SEEK_END);
		tkspi_sdimg_sz=ftell(tkbdev_sdimage[1])>>9;
	}

	tkspi_id_bdev=1;
	return(0);
}

int TKBDEV_ReadSectors(int bdev, void *buf, s64 lba, int lbn)
{
	if((bdev<=0) || (bdev>=256))
	{
		memset(buf, 0, lbn*512);
		return(-1);
	}
	
	if(!tkbdev_sdimage[bdev])
	{
		memset(buf, 0, lbn*512);
		return(-1);
	}

	fseek(tkbdev_sdimage[bdev], lba*512, SEEK_SET);
	fread(buf, 512, lbn, tkbdev_sdimage[bdev]);
	return(0);
}

int TKBDEV_WriteSectors(int bdev, void *buf, s64 lba, int lbn)
{
	if((bdev<=0) || (bdev>=256))
	{
		return(-1);
	}

	fseek(tkbdev_sdimage[bdev], lba*512, SEEK_SET);
	fwrite(buf, 512, lbn, tkbdev_sdimage[bdev]);
	return(0);
}
