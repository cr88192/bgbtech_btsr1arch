#include <bgbmid.h>

int main(int argc, char *argv[])
{
	char tb[256];
	char *ifn, *ofn;

	BGBMID_Sample *head;
	byte *obuf;
	void *fd;
	short *buf;
	short *buf2, *buf3;
	int ch, rt, bits, len, flags, cmd, sz;
	double e, el, er;
	int ldb;
	int p0, p1, p2, p3, s0, s1;
	int i, j, k, n;

	ifn=NULL; ofn=NULL; flags=0; cmd=0;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-o"))
			{
				ofn=argv[i+1];
				i++;
				continue;
			}

			if(!strcmp(argv[i], "--splitstereo"))
			{
				flags|=BGBMID_BTAC_EFL_HALFMONO;
				continue;
			}

			if(!strcmp(argv[i], "--centermono"))
			{
				flags|=BGBMID_BTAC_EFL_CENTERMONO;
				continue;
			}

			if(!strcmp(argv[i], "--encode"))
				{ cmd=1; continue; }
			if(!strcmp(argv[i], "--decode"))
				{ cmd=2; continue; }

			if(!strcmp(argv[i], "--encode2"))
				{ cmd=3; continue; }
			if(!strcmp(argv[i], "--decode2"))
				{ cmd=4; continue; }
			if(!strcmp(argv[i], "--test2"))
				{ cmd=5; continue; }

			if(!strcmp(argv[i], "--blk256"))
			{
				flags|=BGBMID_BTAC_EBMODE_BLK256;
				continue;
			}

			if(!strcmp(argv[i], "--blk256lq"))
			{
				flags|=BGBMID_BTAC_EBMODE_BLK256LQ;
				continue;
			}

			if(!strcmp(argv[i], "--blk256hq"))
			{
				flags|=BGBMID_BTAC_EBMODE_BLK256HQ;
				continue;
			}

			continue;
		}
		
		if(!ifn)
		{
			ifn=argv[i];
			continue;
		}
		
		printf("%s: unexpected argument %s\n", argv[0], argv[i]);
	}

	if(cmd==2)
	{
		if(!ofn)
		{
			printf("%s: no output file\n", argv[0]);
			return(-1);
		}
	
		head=BGBMID_BTAC_LoadSample(ifn);
		if(!head)
		{
			printf("%s: load failed '%s'\n", argv[0], ifn);
			return(-1);
		}
		
		ch=head->chan;
		rt=head->rate;
		len=head->len;
		flags=head->flags;

		obuf=head->btac;

		buf2=bgbmid_malloc((len+256)*2*sizeof(short));
//		BGBMID_BTAC_DecodeStreamS16(obuf, buf2, len);
		BGBMID_BTAC_DecodeStreamS16(obuf, buf2, len, flags);
	
//		sprintf(tb, "%s_out.wav", argv[1]);
		BGBMID_StoreWAV(ofn, (byte *)buf2, ch, rt, 16, len);
		
		bgbmid_free(buf2);
	}

	if(cmd==1)
	{
		if(!ofn)
		{
			printf("%s: no output file\n", argv[0]);
			return(-1);
		}
	
		buf=BGBMID_LoadWAV16(ifn, &ch, &rt, &len);
		if(!buf)
		{
			printf("%s: load failed '%s'\n", argv[0], ifn);
			return(-1);
		}

		if(ch==2)
		{
			flags|=BGBMID_BTAC_EFL_STEREO;
		}

//		BGBMID_LoadWAV(argv[1], )

		head=BGBMID_BTAC_CreateSample(ch, rt, 16, len, flags);

		obuf=head->btac;

		BGBMID_BTAC_EncodeStreamS16(buf, obuf, len, flags);

//		sprintf(tb, "%s_dump0.wav", argv[1]);
//		BGBMID_StoreWAV(tb, (byte *)buf, ch, rt, 16, len);
	
//		sprintf(tb, "%s_dump.btac", argv[1]);
		BGBMID_BTAC_StoreSample(ofn, head);
	}

	if(cmd==4)
	{
		if(!ofn)
		{
			printf("%s: no output file\n", argv[0]);
			return(-1);
		}
	
		head=BGBMID_BTAC_LoadSample(ifn);
		if(!head)
		{
			printf("%s: load failed '%s'\n", argv[0], ifn);
			return(-1);
		}
		
		ch=head->chan;
		rt=head->rate;
		len=head->len;
		flags=head->flags;

//		obuf=head->btac;
		n=(len+(1<<head->blk_samples)-1)>>head->blk_samples;

		buf2=bgbmid_malloc((len+256)*2*sizeof(short));
		BTAC2_Sample_DecodeStream(head, buf2, 0, n, flags);
//		BGBMID_BTAC_DecodeStreamS16(obuf, buf2, len);
//		BGBMID_BTAC_DecodeStreamS16(obuf, buf2, len, flags);
	
//		sprintf(tb, "%s_out.wav", argv[1]);
		BGBMID_StoreWAV(ofn, (byte *)buf2, ch, rt, 16, len);
		
		bgbmid_free(buf2);
	}

	if(cmd==3)
	{
		if(!ofn)
		{
			printf("%s: no output file\n", argv[0]);
			return(-1);
		}
	
		buf=BGBMID_LoadWAV16(ifn, &ch, &rt, &len);
		if(!buf)
		{
			printf("%s: load failed '%s'\n", argv[0], ifn);
			return(-1);
		}

		if(ch==2)
		{
			flags|=BGBMID_BTAC_EFL_STEREO;
		}

		obuf=bgbmid_malloc(1<<24);
		sz=BTAC2_Encode(buf, ch, rt, len, flags, obuf, 1<<24);
		BGBMID_StoreFile(ofn, obuf, sz);
		bgbmid_free(obuf);
	}

	if(cmd==5)
	{
		sprintf(tb, "%s.wav", ifn);
		buf=BGBMID_LoadWAV16(tb, &ch, &rt, &len);
		if(!buf)
		{
			printf("%s: load failed '%s'\n", argv[0], ifn);
			return(-1);
		}

		if(ch==2)
		{
			flags|=BGBMID_BTAC_EFL_STEREO;
		}

//		obuf=bgbmid_malloc(1<<24);
		obuf=bgbmid_malloc(1<<26);
		sz=BTAC2_Encode(buf, ch, rt, len, flags, obuf, 1<<26);

		sprintf(tb, "%s_dump_bt2.btac", ifn);
		BGBMID_StoreFile(tb, obuf, sz);

		head=BGBMID_BTAC2_DecodeSample(obuf, sz);
		
		bgbmid_free(obuf);

		ch=head->chan;
		rt=head->rate;
		len=head->len;
		flags=head->flags;

//		obuf=head->btac;
		n=(len+(1<<head->blk_samples)-1)>>head->blk_samples;

		buf2=bgbmid_malloc((len+256)*2*sizeof(short));
		BTAC2_Sample_DecodeStream(head, buf2, 0, n, flags);
	
		sprintf(tb, "%s_out_bt2.wav", ifn);
		BGBMID_StoreWAV(tb, (byte *)buf2, ch, rt, 16, len);

		if(ch==2)
		{
			e=0; el=0; er=0;
			for(i=0; i<len; i++)
			{
				j=((buf2[i*2+0]+buf2[i*2+1])>>1)-
					((buf[i*2+0]+buf[i*2+1])>>1);
				j=(j<-32768)?(-32768):(j>32767)?(32767):j;
				e+=j*j;

				j=buf2[i*2+0]-buf[i*2+0];
				j=(j<-32768)?(-32768):(j>32767)?(32767):j;
				el+=j*j;
				buf2[i*2+0]=j;

				j=buf2[i*2+1]-buf[i*2+1];
				j=(j<-32768)?(-32768):(j>32767)?(32767):j;
				er+=j*j;
				buf2[i*2+1]=j;
			}

			printf("RMSE C=%f L=%f R=%f\n",
				sqrt(e/(ch*len)),
				sqrt(el/(ch*len)),
				sqrt(er/(ch*len)));
		}else
		{
			e=0;
			for(i=0; i<ch*len; i++)
			{
				j=buf2[i]-buf[i];
				j=(j<-32768)?(-32768):(j>32767)?(32767):j;
				e+=j*j;
				buf2[i]=j;
			}
			printf("RMSE=%f\n", sqrt(e/(ch*len)));
		}

		sprintf(tb, "%s_error_bt2.wav", ifn);
		BGBMID_StoreWAV(tb, (byte *)buf2, ch, rt, 16, len);
		
//		*(int *)-1=-1;
		
		bgbmid_free(buf2);
	}

	if(cmd==0)
	{
//		BGBMID_LoadWAV(argv[1], )

//		sprintf(tb, "%s.wav", argv[1]);
		sprintf(tb, "%s.wav", ifn);
//		buf=BGBMID_LoadWAV_44Mono16(tb, &len);
//		buf=BGBMID_LoadWAV_44Stereo16(tb, &len);
//		buf=BGBMID_LoadWAV_Stereo16(tb, &rt, &len);
//		flags|=BGBMID_BTAC_EFL_STEREO;

		buf=BGBMID_LoadWAV16(tb, &ch, &rt, &len);
		if(!buf)
		{
			printf("%s: load failed '%s'\n", argv[0], ifn);
			return(-1);
		}

		if(ch==2)
		{
			flags|=BGBMID_BTAC_EFL_STEREO;
		}

//		BGBMID_LoadWAV(argv[1], )

		head=BGBMID_BTAC_CreateSample(ch, rt, 16, len, flags);

#if 1
		n=(len+63)/64;
//		obuf=bgbmid_malloc(n*64+256);
		obuf=head->btac;

//		flags=BGBMID_BTAC_EFL_STEREO;
//		flags=BGBMID_BTAC_EFL_STEREO|BGBMID_BTAC_EFL_HALFMONO;

//		BGBMID_BTAC_Band0Stat64StreamS16(buf, obuf, len, flags);
//		BTAC2_TestFilterStreamS16(buf, obuf, len, flags);

//		BGBMID_BTAC_EncodeStreamS16(buf, obuf, len);
		BGBMID_BTAC_EncodeStreamS16(buf, obuf, len, flags);

		sprintf(tb, "%s_dump0.wav", ifn);
		BGBMID_StoreWAV(tb, (byte *)buf, ch, rt, 16, len);
	
		sprintf(tb, "%s_dump.btac", ifn);
//		fd=bgbtac_fopen(tb, "wb");
//		bgbtac_fwrite(obuf, n, 32, fd);
//		bgbtac_fclose(fd);
	
		BGBMID_BTAC_StoreSample(tb, head);
	
		buf2=bgbmid_malloc((len+256)*2*sizeof(short));
//		BGBMID_BTAC_DecodeStreamS16(obuf, buf2, len);
		BGBMID_BTAC_DecodeStreamS16(obuf, buf2, len, flags);
	
		sprintf(tb, "%s_out.wav", ifn);
		BGBMID_StoreWAV(tb, (byte *)buf2, ch, rt, 16, len);

		buf3=bgbmid_malloc((len+256)*2*sizeof(short));
//		if(flags&BGBMID_BTAC_EFL_STEREO)
		if(1)
		{
//			for(i=0; i<len*2; i++)
			for(i=0; i<len*ch; i++)
			{
				j=buf2[i]-buf[i];
				if(j<-32768)j=-32768;
				if(j>32767)j=32767;
//				j=bgbmid_clampi(j, -32768, 32767);
				buf3[i]=j;
			}

			sprintf(tb, "%s_error.wav", ifn);
			BGBMID_StoreWAV(tb, (byte *)buf3, ch, rt, 16, len);
		}
#endif

#if 1
		if(flags&BGBMID_BTAC_EFL_STEREO)
		{
			e=0; el=0; er=0; ldb=-1;
			for(i=0; i<len; i++)
			{
				j=buf[i*2+0]-buf2[i*2+0];
				el+=j*j;

				j=buf[i*2+1]-buf2[i*2+1];
				er+=j*j;
		
				j=(buf[i*2+0]+buf[i*2+1])>>1;
				k=(buf2[i*2+0]+buf2[i*2+1])>>1;
		
				p0=j; p1=k;
				
				j=j-k;
				e+=j*j;

#if 1
//				if(j>=8192)
				if(j>=16384)
				{						
					if(ldb!=(i/64))
					{
						printf("\n");
						
						ldb=i/64;

						k=obuf[(i/64)*32+0]&15;
//						printf("%d:%d E=%d(%d, %d) T=%d Bm,n=%d,%d\n",
//							i/64, i%64, j, p0, p1, k,
//							(s8)obuf[(i/64)*32+6], (s8)obuf[(i/64)*32+7]);

						s0=(s16)((obuf[(i/64)*32+6]<<8) |
							((obuf[(i/64)*32+1]&0x0F)<<4));
						s1=(s16)((obuf[(i/64)*32+7]<<8) |
							(obuf[(i/64)*32+1]&0xF0));

						printf("%d:%d E=%d(%d, %d) T=%d Bm,n=%d,%d\n",
							i/64, i%64, j, p0, p1, k,
							s0, s1);

						j=(s16)(obuf[ldb*32+2] | (obuf[ldb*32+3]<<8));
						k=(s16)(obuf[ldb*32+4] | (obuf[ldb*32+5]<<8));
						printf("\tM=%d, N=%d\n", j, k);

#if 0
						for(j=0; j<64; j++)
						{
							k=(buf[(ldb*64+j)*2+0]+buf[(ldb*64+j)*2+1])>>1;
							printf("%04X ", k&65535);
						}
						printf("\n");
#endif

					}else
					{
						printf("%d:%d E=%d (%d, %d)\n",
							i/64, i%64, j, p0, p1);
					}
				}
#endif
			}
			printf("RMSE: C=%f, L=%f, R=%f\n",
				sqrt(e/len),
				sqrt(el/len), sqrt(er/len));
		}else
		{
//			e=0;
//			for(i=0; i<len; i++)
//			{
//				j=buf[i]-buf2[i];
//				e+=j*j;
//			}

			e=0; ldb=-1;
			for(i=0; i<len; i++)
			{
//				j=buf[i]-buf2[i];
//				e+=j*j;

				j=buf[i];
				k=buf2[i];
		
				p0=j; p1=k;
				
				j=j-k;
				e+=j*j;

				if(j<0)j=-j;

#if 1
//				if(j>=8192)
				if(j>=16384)
				{						
					if(ldb!=(i/64))
					{
						printf("\n");
						
						ldb=i/64;

						k=obuf[(i/64)*32+0]&15;
//						printf("%d:%d E=%d(%d, %d) T=%d Bm,n=%d,%d\n",
//							i/64, i%64, j, p0, p1, k,
//							(s8)obuf[(i/64)*32+6], (s8)obuf[(i/64)*32+7]);

						s0=(s16)((obuf[(i/64)*32+6]<<8) |
							((obuf[(i/64)*32+1]&0x0F)<<4));
						s1=(s16)((obuf[(i/64)*32+7]<<8) |
							(obuf[(i/64)*32+1]&0xF0));

						printf("%d:%d E=%d(%d, %d) T=%d Bm,n=%d,%d\n",
							i/64, i%64, j, p0, p1, k,
							s0, s1);

						j=(s16)(obuf[ldb*32+2] | (obuf[ldb*32+3]<<8));
						k=(s16)(obuf[ldb*32+4] | (obuf[ldb*32+5]<<8));
						printf("\tM=%d, N=%d\n", j, k);

#if 0
						for(j=0; j<64; j++)
						{
							k=(buf[(ldb*64+j)*2+0]+buf[(ldb*64+j)*2+1])>>1;
							printf("%04X ", k&65535);
						}
						printf("\n");
#endif

					}else
					{
						printf("%d:%d E=%d (%d, %d)\n",
							i/64, i%64, j, p0, p1);
					}
				}
#endif
			}

			printf("RMSE = %f\n", sqrt(e/len));
		}
#endif
	}

	return(0);
}
