/*
open mode:
  r		rb		//read
  w		wb		//write (create, binary)
  r+	r+b		//read/write (exists)
  w+	w+b		//read/write (create/truncate)
Text modes will be equivalent to the binary modes ('b' or 't' being ignored).

rename mode:
  r				//rename
  l				//link
  s				//symlink (normal)
  S				//symlink (force absolute)
With 'l', assume a local/relative link.
  Fails if the target is not in the same filesystem.
With 's', use a local link if possible.
  Use an absolute link if target is not in the same filesystem.
  May only link to VFS paths.
With 'S', force an absolute link.

*/

// byte *tk_ird_imgbuf=NULL;

char kerninit[256];

void *tk_handle_arr[256];

TK_FILE *tk_vf_freelist=NULL;
TK_MOUNT *tk_mnt_freelist=NULL;

TK_FILE_VT *tk_fsty_root=NULL;
TK_MOUNT *tk_vf_mount=NULL;

int tk_vf_register(TK_FILE_VT *fsty)
{
	fsty->next=tk_fsty_root;
	tk_fsty_root=fsty;
}

int tk_vf_addmount(TK_MOUNT *mnt)
{
	TK_MOUNT *cur, *prv;
	
	if(!mnt)
		return(-1);
	if(!mnt->vt)
		{ __debugbreak(); }
	
	if(!mnt->vt->fsname)
		{ __debugbreak(); }
//	if(mnt->vt->next)
//		{ __debugbreak(); }
		
	mnt->magic1=0x1234ABCD;
	
	mnt->szSrc=0;
	if(mnt->src)
		{ mnt->szSrc=strlen(mnt->src); }
	
	cur=tk_vf_mount; prv=NULL;
	while(cur)
	{
		if(mnt->szSrc>=cur->szSrc)
			break;
		prv=cur;
		cur=cur->next;
	}
	
	if(prv)
	{
		mnt->next=cur;
		prv->next=mnt;
	}else
	{
		mnt->next=tk_vf_mount;
		tk_vf_mount=mnt;
	}
}

TK_MOUNT *tk_vf_nextmount(TK_MOUNT *mnt)
{
	return(mnt->next);
}

TK_MOUNT *tk_vf_firstmount(void)
{
	return(tk_vf_mount);
}

int tk_fat_init();
int tk_mount_sdfat();

int tk_sysc_init();
int tk_mount_sysc();

int tk_wad4_init();


static int tk_vf_init=0;

int TK_FindFreeObjHandle(TKPE_TaskInfo *task)
{
	int i;
	
	for(i=1; i<256; i++)
	{
		if(!tk_handle_arr[i])
			return(i);
	}
	return(-1);
}

int TK_FreeObjHandle(TKPE_TaskInfo *task, int ix)
{
	tk_handle_arr[ix]=NULL;
	return(0);
}

int TK_LookupHandleForPtr(TKPE_TaskInfo *task, void *ptr)
{
	int i;
	
	if(!ptr)
		return(0);
	
	for(i=1; i<256; i++)
	{
		if(tk_handle_arr[i]==ptr)
			return(i);
	}
	return(-1);
}

int TK_GetHandleForPtr(TKPE_TaskInfo *task, void *ptr)
{
	int i;
	i=TK_LookupHandleForPtr(task, ptr);
	if(i>=0)
		return(i);
	i=TK_FindFreeObjHandle(task);
	if(i>0)
	{
		tk_handle_arr[i]=ptr;
		return(i);
	}
	return(-1);
}

void *TK_GetPtrForHandle(TKPE_TaskInfo *task, int ix)
{
	return(tk_handle_arr[ix]);
}

/* Check if a given user (usr) can access a given target (tgt).
 * Returns 1 if true, 0 if false, or negative on error.
 */
int TK_CheckUserAccess(TK_USERINFO *usr, TK_USERINFO *tgt)
{
	int usrm, tgtm;

	if(!usr)		// NULL means root
		return(1);
	if(!tgt)
	{
		// NULL means root-only
		if(usr->uid || usr->gid)
			return(0);
		return(1);
	}
	
	usrm=(usr->mode)&7;
	if(!usrm)
		return(0);

	tgtm=tgt->mode;
	if(usr->gid==tgt->gid)
	{
		if(usr->uid==tgt->uid)
		{
			if(((tgtm>>6)&usrm)==usrm)
				return(1);
			return(0);
		}
		if(((tgtm>>3)&usrm)==usrm)
			return(1);
		return(0);
	}
	if(((tgtm>>0)&usrm)==usrm)
		return(1);
	return(0);
}

int tk_vfile_init()
{
	if(tk_vf_init==1)return(0);
	tk_vf_init=1;
	
	tk_puts("tk_vfile_init\n");
	
#ifndef __TK_CLIB_ONLY__
	if(tk_iskernel())
	{
		tk_devfs_init();
		tk_bdspi_init();
		tk_bdvfb_init();
		tk_bdram_init();
	
//		tk_ird_init();
		tk_fat_init();
		tk_wad4_init();

		tk_mount_wad4temp(NULL);
		tk_mount_sdfat("boot");

//		tk_mount_sdfat(NULL);
		tk_mount_devfs();
		
		tk_wad4_mount("boot/tkusrimg.wd4", "usr", "wad4", NULL, NULL);

		TK_Env_SetCwd("/boot");
	}else
	{
		tk_sysc_init();	
		tk_mount_sysc();
	}
#else
	tk_sysc_init();	
	tk_mount_sysc();
#endif

	tk_puts("tk_vfile_init: OK\n");

	return(1);
}

TK_FILE *tk_alloc_file()
{
	TK_FILE *tmp, *tmpa;
	int i;
	
	tmp=tk_vf_freelist;
	if(tmp)
	{
		tk_vf_freelist=tmp->udata0;
		memset(tmp, 0, sizeof(TK_FILE));
		return(tmp);
	}
	
	tmpa=tk_malloc(256*sizeof(TK_FILE));
	
	tmp=tmpa;
	for(i=0; i<256; i++)
	{
		tmp->udata0=tk_vf_freelist;
		tk_vf_freelist=tmp;
		tmp++;
	}

	tmp=tk_vf_freelist;
	if(tmp)
	{
		tk_vf_freelist=tmp->udata0;
		memset(tmp, 0, sizeof(TK_FILE));
		return(tmp);
	}
	
	__debugbreak();
	return(NULL);
	
//	tmp=tk_malloc(sizeof(TK_FILE));
//	memset(tmp, 0, sizeof(TK_FILE));
//	return(tmp);
}

TK_DIR *tk_alloc_dir()
{
	return((TK_DIR *)tk_alloc_file());
}

void tk_free_file(TK_FILE *tmp)
{
	tmp->udata0=tk_vf_freelist;
	tk_vf_freelist=tmp;
}

void tk_free_dir(TK_DIR *tmp)
{
	tk_free_file((TK_FILE *)tmp);
//	tmp->udata0=tk_vf_freelist;
//	tk_vf_freelist=tmp;
}


TK_MOUNT *tk_alloc_mount()
{
	TK_MOUNT *tmp;
	
	tmp=tk_mnt_freelist;
	if(tmp)
	{
		tk_mnt_freelist=tmp->udata0;
		memset(tmp, 0, sizeof(TK_MOUNT));
		return(tmp);
	}
	
	tmp=tk_malloc(sizeof(TK_MOUNT));
	memset(tmp, 0, sizeof(TK_MOUNT));
	return(tmp);
}

void tk_free_mount(TK_MOUNT *tmp)
{
	tmp->udata0=tk_mnt_freelist;
	tk_mnt_freelist=tmp;
}

int TK_VF_FlagsFromModeString(char *mode)
{
	int fl;
	
	fl=0;
	
	if((mode[0]=='0') && (mode[1]=='x'))
	{
		return(tk_atoi(mode));
	}
	
	if(mode[0]=='r')
	{
		if(mode[1]='+')
		{
			fl|=TKVF_O_RDWR;
		}else
		{
			fl|=TKVF_O_RDONLY;
		}
	}else
		if(mode[0]=='w')
	{
		if(mode[1]='+')
		{
			fl|=TKVF_O_RDWR|TKVF_O_TRUNC|TKVF_O_CREAT;
		}else
		{
			fl|=TKVF_O_WRONLY|TKVF_O_TRUNC|TKVF_O_CREAT;
		}
	}else
		if(mode[0]=='a')
	{
		if(mode[1]='+')
		{
			fl|=TKVF_O_RDWR|TKVF_O_CREAT;
		}else
		{
			fl|=TKVF_O_WRONLY|TKVF_O_CREAT;
		}
	}
	
	return(fl);
}

int tk_fmount(char *devfn, char *mntfn, char *fsty, char *mode, char **opts)
{
	TK_FILE_VT *cur;
	TK_MOUNT *mnt;
	
	cur=tk_fsty_root;
	while(cur)
	{
		if(!strcmp(cur->fsname, fsty))
		{
			if(cur->mount)
			{
				mnt=cur->mount(devfn, mntfn, fsty, mode, opts);
				if(mnt)
					return(0);
			}
		}
		cur=cur->next;
	}
	return(-1);
}

int tk_unlink2(TK_USERINFO *usri, char *name)
{
	TK_MOUNT *mnt;
	TK_FILE *fd;
	char *s1;
	int i;

	tk_vfile_init();

	if((name[0]=='.') && (name[1]=='/'))
		name+=2;
	if(*name=='/')
		name++;

//	fd=tk_ird_fopen(NULL, name, mode);
//	if(fd)return(fd);

	mnt=tk_vf_mount;
	while(mnt)
	{
		s1=name;
		if(mnt->src && (mnt->szSrc>0))
		{
			if(strncmp(mnt->src, name, mnt->szSrc))
			{
				mnt=mnt->next;
				continue;
			}
			s1+=mnt->szSrc;
			if(*s1 && (*s1!='/') && (*(s1-1)!=':'))
			{
				mnt=mnt->next;
				continue;
			}
			if(*s1=='/')
				s1++;
		}
	
//		fd=mnt->vt->fopen(mnt, name, mode);
		if(mnt->vt->unlink)
		{
			i=mnt->vt->unlink(mnt, usri, s1);
			if(i>=0)
				return(i);
		}
		mnt=mnt->next;
	}

	tk_printf("tk_unlink: Fail %s\n", name);
	return(-1);
}

int tk_unlink(char *name)
{
	return(tk_unlink2(NULL, name));
}

int tk_rmdir2(TK_USERINFO *usri, char *name)
{
	TK_MOUNT *mnt;
	TK_FILE *fd;
	char *s1;
	int i;

	tk_vfile_init();

	if((name[0]=='.') && (name[1]=='/'))
		name+=2;
	if(*name=='/')
		name++;

//	fd=tk_ird_fopen(NULL, name, mode);
//	if(fd)return(fd);

	mnt=tk_vf_mount;
	while(mnt)
	{
		s1=name;
		if(mnt->src && (mnt->szSrc>0))
		{
			if(strncmp(mnt->src, name, mnt->szSrc))
			{
				mnt=mnt->next;
				continue;
			}
			s1+=mnt->szSrc;
			if(*s1 && (*s1!='/') && (*(s1-1)!=':'))
			{
				mnt=mnt->next;
				continue;
			}
			if(*s1=='/')
				s1++;
		}
	
//		fd=mnt->vt->fopen(mnt, name, mode);
		if(mnt->vt->rmdir)
		{
			i=mnt->vt->rmdir(mnt, usri, s1);
			if(i>=0)
				return(i);
		}
		mnt=mnt->next;
	}

	tk_printf("tk_rmdir: Fail %s\n", name);
	return(-1);
}

int tk_rmdir(char *name)
{
	return(tk_rmdir2(NULL, name));
}

int tk_mkdir2(TK_USERINFO *usri, char *name, char *mode)
{
	TK_MOUNT *mnt;
	TK_FILE *fd;
	char *s1;
	int i;

	tk_vfile_init();

	if((name[0]=='.') && (name[1]=='/'))
		name+=2;
	if(*name=='/')
		name++;

//	fd=tk_ird_fopen(NULL, name, mode);
//	if(fd)return(fd);

	mnt=tk_vf_mount;
	while(mnt)
	{
		s1=name;
		if(mnt->src && (mnt->szSrc>0))
		{
			if(strncmp(mnt->src, name, mnt->szSrc))
			{
				mnt=mnt->next;
				continue;
			}
			s1+=mnt->szSrc;
			if(*s1 && (*s1!='/') && (*(s1-1)!=':'))
			{
				mnt=mnt->next;
				continue;
			}
			if(*s1=='/')
				s1++;
		}
	
		if(mnt->vt->mkdir)
		{
			i=mnt->vt->mkdir(mnt, usri, s1, mode);
			if(i>=0)
				return(i);
		}
		mnt=mnt->next;
	}

	tk_printf("tk_mkdir: Fail %s\n", name);
	return(-1);
}

int tk_mkdir(char *name, char *mode)
{
	return(tk_mkdir2(NULL, name, mode));
}

int tk_rename2(TK_USERINFO *usri, char *oldname, char *newname, char *mode)
{
	char tb[512];
	TK_MOUNT *mnt;
	TK_FILE *fd;
	char *oldname0, *newname0;
	char *s1, *t1, *tn1;
	int islink, isuri;
	int i;

	tk_vfile_init();

	oldname0=oldname;
	newname0=newname;

	if((oldname[0]=='.') && (oldname[1]=='/'))
		oldname+=2;
	if(*oldname=='/')
		oldname++;

	if((newname[0]=='.') && (newname[1]=='/'))
		newname+=2;
	if(*newname=='/')
		newname++;

	islink=0;
	if(mode)
	{
		i=*mode;
		if((i=='l') || (i=='L'))
			islink=1;
		if(i=='s')
			islink=2;
		if(i=='S')
			islink=3;
	}

	isuri=0;
	s1=oldname0;
	while(*s1 && (*s1>='a') && (*s1<='z'))
		s1++;
	if(s1==':')
		isuri=1;

//	fd=tk_ird_fopen(NULL, name, mode);
//	if(fd)return(fd);

	mnt=tk_vf_mount;
	while(mnt)
	{
		s1=oldname;
		t1=newname;
		if(mnt->src && (mnt->szSrc>0))
		{
			if(strncmp(mnt->src, newname, mnt->szSrc))
			{
				tk_printf("tk_rename: skip %s, newname mismatch\n", mnt->src);
				mnt=mnt->next;
				continue;
			}
		
			if(strncmp(mnt->src, oldname, mnt->szSrc) && !(islink&2))
			{
				tk_printf("tk_rename: skip %s, oldname mismatch\n", mnt->src);
				mnt=mnt->next;
				continue;
			}

			s1+=mnt->szSrc;
			t1+=mnt->szSrc;

			if(islink==3)
			{
//				if(!isuri && (oldname0[0]!='/'))
//					{ mnt=mnt->next; continue; }
				s1=oldname0;
			}else if(islink==2)
			{
				if(strncmp(mnt->src, oldname, mnt->szSrc) || isuri)
				{
					if(!isuri && (oldname0[0]!='/'))
						{ mnt=mnt->next; continue; }
					s1=oldname0;
				}else
				{
					while(*s1=='/')
						s1++;
					tb[0]=':';
					strcpy(tb+1, s1);
					tn1=tk_rstrdup(tb);
					
					s1=tn1;
				}
			}else
			{
				if(*s1 && (*s1!='/') && (*(s1-1)!=':'))
				{
					tk_printf("tk_rename: skip %s, oldname no slash\n",
						mnt->src);
					mnt=mnt->next;
					continue;
				}
				if(*s1=='/')
					s1++;
			}

			if(*t1 && (*t1!='/') && (*(t1-1)!=':'))
			{
				tk_printf("tk_rename: skip %s, newname no slash\n",
					mnt->src);
				mnt=mnt->next;
				continue;
			}
			if(*t1=='/')
				t1++;
		}else
		{
			if(islink&2)
				{ s1=oldname0; }
		}
	
		if(mnt->vt->rename)
		{
			i=mnt->vt->rename(mnt, usri, s1, t1, mode);
			if(i>=0)
				return(i);
		}
		mnt=mnt->next;
	}

	tk_printf("tk_rename: Fail old=%s new=%s mode=%s\n",
		oldname0, newname0, mode);
	return(-1);
}

int tk_rename(char *oldname, char *newname, char *mode)
{
	return(tk_rename2(NULL, oldname, newname, mode));
}

int tk_fcopy2(TK_USERINFO *usri, char *oldname, char *newname)
{
	char tb[1024];
	TK_FILE *oldfd, *newfd;
	int n;
	
	oldfd=tk_fopen2(usri, oldname, "rb");

	if(!oldfd)
	{
//		tk_printf("fail open %s\n", oldname);
		return(-1);
	}

	newfd=tk_fopen2(usri, newname, "wb");
	
	if(!newfd)
	{
//		tk_printf("fail open %s\n", newname);
		tk_fclose(oldfd);
		return(-1);
	}

//	while(!tk_feof(oldfd))
	while(1)
	{
		n=tk_fread(tb, 1, 1024, oldfd);
		if(n<1024)
		{
			if(n<=0)
				break;
			tk_fwrite(tb, 1, n, newfd);
			break;
		}
		tk_fwrite(tb, 1, 1024, newfd);
	}

	tk_fclose(oldfd);
	tk_fclose(newfd);
	return(1);
}

int tk_fcopy(char *oldname, char *newname)
{
	return(tk_fcopy2(NULL, oldname, newname));
}

int tk_fstat2(TK_USERINFO *usri, char *name, TK_FSTAT *st)
{
	TK_MOUNT *mnt;
	TK_FILE *fd;
	char *s1;
	int i;

	tk_vfile_init();

	if((name[0]=='.') && (name[1]=='/'))
		name+=2;
	if(*name=='/')
		name++;

	mnt=tk_vf_mount;
	while(mnt)
	{
		s1=name;
		if(mnt->src && (mnt->szSrc>0))
		{
			if(strncmp(mnt->src, name, mnt->szSrc))
			{
				mnt=mnt->next;
				continue;
			}
			s1+=mnt->szSrc;
			if(*s1 && (*s1!='/') && (*(s1-1)!=':'))
			{
				mnt=mnt->next;
				continue;
			}
			if(*s1=='/')
				s1++;
		}
	
//		fd=mnt->vt->fopen(mnt, name, mode);
		if(mnt->vt->fstat)
		{
			i=mnt->vt->fstat(mnt, usri, s1, st);
			if(i>=0)
				return(i);
		}
		mnt=mnt->next;
	}

	tk_printf("tk_fstat: Fail %s\n", name);
	return(-1);
}

int tk_fstat(char *name, TK_FSTAT *st)
{
	return(tk_fstat2(NULL, name, st));
}



int tk_fsctl2(TK_USERINFO *usri, char *name, int cmd, void *ptr)
{
	TK_MOUNT *mnt;
	TK_FILE *fd;
	char *s1;
	int i;

	tk_vfile_init();

	if((name[0]=='.') && (name[1]=='/'))
		name+=2;
	if(*name=='/')
		name++;

	mnt=tk_vf_mount;
	while(mnt)
	{
		s1=name;
		if(mnt->src && (mnt->szSrc>0))
		{
			if(strncmp(mnt->src, name, mnt->szSrc))
			{
				mnt=mnt->next;
				continue;
			}
			s1+=mnt->szSrc;
			if(*s1 && (*s1!='/') && (*(s1-1)!=':'))
			{
				mnt=mnt->next;
				continue;
			}
			if(*s1=='/')
				s1++;
		}
	
		if(mnt->vt->fsctl)
		{
			i=mnt->vt->fsctl(mnt, usri, s1, cmd, ptr);
			if(i>=0)
				return(i);
		}
		mnt=mnt->next;
	}

	tk_printf("tk_fsctl: Fail %s\n", name);
	return(-1);
}

int tk_fsctl(char *name, int cmd, void *ptr)
{
	return(tk_fsctl2(NULL, name, cmd, ptr));
}

#ifndef __TK_CLIB_ONLY__
// #include "tk_vf_ird.c"
#include "tk_vf_fat.c"
#endif

#include "tk_vf_sysc.c"

static int tk_open_reclim=0;

TK_FILE *tk_fopen2(TK_USERINFO *usri, char *name, char *mode)
{
	TK_MOUNT *mnt;
	TK_FILE *fd;
	char *s1;

	if(tk_open_reclim>=8)
		return(NULL);

	tk_vfile_init();

	if((name[0]=='.') && (name[1]=='/'))
		name+=2;
	if(*name=='/')
		name++;

//	fd=tk_ird_fopen(NULL, name, mode);
//	if(fd)return(fd);

	tk_open_reclim++;

	mnt=tk_vf_mount;
	while(mnt)
	{
		if(mnt->magic1!=0x1234ABCD)
			{ __debugbreak(); }

		s1=name;
		if(mnt->src && (mnt->szSrc>0))
		{
			if(strncmp(mnt->src, name, mnt->szSrc))
			{
				mnt=mnt->next;
				continue;
			}
			s1+=mnt->szSrc;
			if(*s1 && (*s1!='/') && (*(s1-1)!=':'))
			{
				mnt=mnt->next;
				continue;
			}
			if(*s1=='/')
				s1++;
		}
		
		if(!mnt->vt)
			{ __debugbreak(); }
	
		if(mnt->vt->fopen)
		{
	//		fd=mnt->vt->fopen(mnt, name, mode);
			fd=mnt->vt->fopen(mnt, usri, s1, mode);
			if(fd)
			{
				tk_open_reclim--;
				return(fd);
			}
		}
		mnt=mnt->next;
	}

//	tk_printf("tk_fopen: Fail %s\n", name);

	tk_open_reclim--;
	return(NULL);
}

TK_FILE *tk_fopen(char *name, char *mode)
{
	return(tk_fopen2(NULL, name, mode));
}

int tk_fread(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fread)
		return(fd->vt->fread(buf, sz1, sz2, fd));
	return(-1);
}

int tk_fwrite(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fwrite)
		return(fd->vt->fwrite(buf, sz1, sz2, fd));
	return(-1);
}

s64 tk_fseek(TK_FILE *fd, s64 ofs, int rel)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fseek)
		return(fd->vt->fseek(fd, ofs, rel));
	return(-1);
}

s64 tk_ftell(TK_FILE *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->ftell)
		return(fd->vt->ftell(fd));
	return(-1);
}

int tk_fclose(TK_FILE *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fclose)
		return(fd->vt->fclose(fd));
	return(-1);
}

int tk_fgetc(TK_FILE *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fgetc)
		return(fd->vt->fgetc(fd));
	return(-1);
}

int tk_fputc(int ch, TK_FILE *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fputc)
		return(fd->vt->fputc(ch, fd));
	return(-1);
}

int tk_fputs(char *str, TK_FILE *fd)
{
	char *s;
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fputc)
	{
		s=str;
		while(*s)
			fd->vt->fputc(*s++, fd);
		return(0);
	}
	return(-1);
}

int tk_fgets(char *str, int lim, TK_FILE *fd)
{
	char *s, *se;
	int c;
	
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fgetc)
	{
		s=str; se=str+lim;
		while(s<se)
		{
			c=fd->vt->fgetc(fd);
			if(c=='\n')
				break;
			*s++=c;
		}
		*s=0;
		return(0);
	}
	return(-1);
}

int tk_fioctl(TK_FILE *fd, int cmd, void *ptr)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fioctl)
		return(fd->vt->fioctl(fd, cmd, ptr));
	return(-1);
}

int tk_fsend(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->fsend)
		return(fd->vt->fsend(fd, cmd,
			msgbuf, szmsg, flag,
			sockaddr, szsockaddr));
	return(-1);
}

int tk_frecv(TK_FILE *fd, int cmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->frecv)
		return(fd->vt->frecv(fd, cmd,
			msgbuf, szmsg, flag,
			sockaddr, szsockaddr));
	return(-1);
}

int TK_InitUserInfoForTask(TKPE_TaskInfo *task, TK_USERINFO *usri)
{
	usri->uid=task->uid;
	usri->gid=task->gid;
	usri->mode=0;
}

int tk_hfmount(TKPE_TaskInfo *task,
	char *devfn, char *mntfn, char *fsty,
	char *mode, char **opts)
{
//	TK_USERINFO tacc;
//	TK_InitUserInfoForTask(task, &tacc);

	if(task->uid || task->gid)
	{
		return(-1);
	}

	return(tk_fmount(devfn, mntfn, fsty, mode, opts));
}

int tk_hfstat(TKPE_TaskInfo *task, char *name, TK_FSTAT *st)
{
	TK_USERINFO tacc;
	TK_InitUserInfoForTask(task, &tacc);
	return(tk_fstat2(&tacc, name, st));
}

int tk_hfsctl(TKPE_TaskInfo *task, char *name, int cmd, void *ptr)
{
	TK_USERINFO tacc;
	TK_InitUserInfoForTask(task, &tacc);
	return(tk_fsctl2(&tacc, name, cmd, ptr));
}

int tk_hunlink(TKPE_TaskInfo *task, char *name)
{
	TK_USERINFO tacc;
	TK_InitUserInfoForTask(task, &tacc);
	return(tk_unlink2(&tacc, name));
}

int tk_hrmdir(TKPE_TaskInfo *task, char *name)
{
	TK_USERINFO tacc;
	TK_InitUserInfoForTask(task, &tacc);
	return(tk_rmdir2(&tacc, name));
}

int tk_hmkdir(TKPE_TaskInfo *task, char *name, char *mode)
{
	TK_USERINFO tacc;
	TK_InitUserInfoForTask(task, &tacc);
	return(tk_mkdir2(&tacc, name, mode));
}

int tk_hrename(TKPE_TaskInfo *task,
	char *oldname, char *newname, char *mode)
{
	TK_USERINFO tacc;
	TK_InitUserInfoForTask(task, &tacc);
	return(tk_rename2(&tacc, oldname, newname, mode));
}


int tk_hfopen(TKPE_TaskInfo *task, char *name, char *mode)
{
	TK_USERINFO tacc;
	FILE *fd;
	int hdl;

	TK_InitUserInfoForTask(task, &tacc);
	if(mode[0]=='r')
	{
		tacc.mode|=TKFAT_EMODE_ACC_RO;
		if(mode[1]=='+')
			{ tacc.mode|=TKFAT_EMODE_ACC_WO; }
	}else if((mode[0]=='w') || (mode[0]=='a'))
	{
		tacc.mode|=TKFAT_EMODE_ACC_WO;
		if(mode[1]=='+')
			{ tacc.mode|=TKFAT_EMODE_ACC_RO; }
	}

//	fd=tk_fopen(name, mode);
	fd=tk_fopen2(&tacc, name, mode);
	if(!fd)
	{
//		tk_printf("tk_hfopen: failed open %s\n", name);
		return(0);
	}
	hdl=TK_GetHandleForPtr(task, fd);
//	tk_printf("tk_hfopen: open %s h=%d\n", name, hdl);
	return(hdl);
}

int tk_hread(TKPE_TaskInfo *task, int iHdl, void *pBuf, int szBuf)
{
	TK_USERINFO tacc;
	TK_FILE *fd;
	int i, sde;

	TK_InitUserInfoForTask(task, &tacc);

	fd=TK_GetPtrForHandle(task, iHdl);
	if(!fd)
		return(-1);

	return(tk_fread(pBuf, 1, szBuf, fd));
}

int tk_hwrite(TKPE_TaskInfo *task, int iHdl, void *pBuf, int szBuf)
{
	TK_USERINFO tacc;
	TK_FILE *fd;
	int i, j, sde;

	TK_InitUserInfoForTask(task, &tacc);

	fd=TK_GetPtrForHandle(task, iHdl);
	if(!fd)
		return(-1);

	return(tk_fwrite(pBuf, 1, szBuf, fd));
}

s64 tk_hseek(TKPE_TaskInfo *task, int iHdl, s64 lOffs, int iMod)
{
	TK_USERINFO tacc;
	TK_FILE *fd;
	int i, sde;

	TK_InitUserInfoForTask(task, &tacc);

	fd=TK_GetPtrForHandle(task, iHdl);
	if(!fd)
		return(-1);

	if(!lOffs)
	{
		if(iMod==1)
		{
			return(tk_ftell(fd));
		}
	}

	return(tk_fseek(fd, lOffs, iMod));
}

int tk_hclose(TKPE_TaskInfo *task, int iHdl)
{
	TK_USERINFO tacc;
	TK_FILE *fd;
	int i, sde;

	TK_InitUserInfoForTask(task, &tacc);

	fd=TK_GetPtrForHandle(task, iHdl);
	if(!fd)
		return(-1);
	return(tk_fclose(fd));
}

s64 tk_hioctl(TKPE_TaskInfo *task, int iHdl, int iCmd, void *ptr)
{
	TK_USERINFO tacc;
	TK_FILE *fd;
	int i, sde;

	TK_InitUserInfoForTask(task, &tacc);

	fd=TK_GetPtrForHandle(task, iHdl);
	if(!fd)
		return(-1);
	return(tk_fioctl(fd, iCmd, ptr));
}

s64 tk_hsend(TKPE_TaskInfo *task, int iHdl, int iCmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr)
{
	TK_USERINFO tacc;
	FILE *fd;
	int i, sde;

	TK_InitUserInfoForTask(task, &tacc);

	fd=TK_GetPtrForHandle(task, iHdl);
	if(!fd)
		return(-1);
	return(tk_fsend(fd, iCmd,
		msgbuf, szmsg, flag,
		sockaddr, szsockaddr));
}

s64 tk_hrecv(TKPE_TaskInfo *task, int iHdl, int iCmd,
	void *msgbuf, int szmsg, int flag,
	void *sockaddr, int szsockaddr)
{
	TK_USERINFO tacc;
	FILE *fd;
	int i, sde;

	TK_InitUserInfoForTask(task, &tacc);

	fd=TK_GetPtrForHandle(task, iHdl);
	if(!fd)
		return(-1);
	return(tk_frecv(fd, iCmd,
		msgbuf, szmsg, flag,
		sockaddr, szsockaddr));
}

static int tk_opendir_reclim=0;

TK_DIR *tk_opendir2(TK_USERINFO *usri, char *name)
{
	TK_DIR *fda[64];
	TK_MOUNT *mnt;
	TK_DIR *fd;
	char *s1;
	int nfd;

	if(tk_opendir_reclim>=8)
		return(NULL);

	tk_vfile_init();

	if((name[0]=='.') && (name[1]=='/'))
		name+=2;
	if(*name=='/')
		name++;

	tk_opendir_reclim++;

	nfd=0;
	mnt=tk_vf_mount;
	while(mnt)
	{
		s1=name;
		if(mnt->src && (mnt->szSrc>0))
		{
//			tk_printf("tk_opendir: Src=%s\n", mnt->src);

			if(strncmp(mnt->src, name, mnt->szSrc))
			{
				mnt=mnt->next;
				continue;
			}
			s1+=mnt->szSrc;
			if(*s1 && (*s1!='/') && (*(s1-1)!=':'))
			{
				mnt=mnt->next;
				continue;
			}
			if(*s1=='/')
				s1++;
		}

//		tk_printf("tk_opendir: Check %s\n", mnt->src);
	
		if(mnt->vt->opendir)
		{
	//		fd=mnt->vt->opendir(mnt, name);
			fd=mnt->vt->opendir(mnt, usri, s1);
			if(fd)
			{
				fda[nfd++]=fd;
//				tk_opendir_reclim--;
//				return(fd);
			}
		}
		mnt=mnt->next;
	}
	
	if(nfd==1)
	{
		fd=fda[0];
		tk_opendir_reclim--;
		return(fd);
	}

	if(nfd>1)
	{
		fd=tk_multidir_create(fda, nfd);
//		fd=fda[0];
		tk_opendir_reclim--;
		return(fd);
	}

	tk_printf("tk_opendir: Fail %s\n", name);
	tk_opendir_reclim--;
	return(NULL);
}

TK_DIR *tk_opendir(char *name)
{
	return(tk_opendir2(NULL, name));
}

TK_DIRENT *tk_readdir(TK_DIR *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->readdir)
		return(fd->vt->readdir(fd));
	return(NULL);
}

int tk_closedir(TK_DIR *fd)
{
	if(!fd || !(fd->vt))
		{ __debugbreak(); }

	if(fd->vt->closedir)
		return(fd->vt->closedir(fd));
	return(-1);
}


int tk_hopendir(TKPE_TaskInfo *task, char *name)
{
	TK_USERINFO tacc;
	FILE *fd;

	TK_InitUserInfoForTask(task, &tacc);
	tacc.mode|=TKFAT_EMODE_ACC_RO;

//	fd=tk_opendir(name);
	fd=tk_opendir2(&tacc, name);
	return(TK_GetHandleForPtr(task, fd));
}

int tk_hreaddir(TKPE_TaskInfo *task, int iHdl, void *pDe, int szDe, int nDe)
{
	TK_DIRENT *de;
	void *de1;
	FILE *fd;
	int i, sde;

	fd=TK_GetPtrForHandle(task, iHdl);
	if(!fd)
		return(-1);
	
	sde=sizeof(TK_DIRENT);
	if(szDe<sde)sde=szDe;
	
	memset(pDe, 0, szDe*nDe);

	for(i=0; i<nDe; i++)
	{
		de=tk_readdir(fd);
		if(!de)
			break;
		de1=((byte *)pDe)+(i*szDe);
		memcpy(de1, de, sde);
	}
	return(i);

//	fd=tk_opendir(name);
//	return(TK_GetHandleForPtr(fd));
}

int tk_hclosedir(TKPE_TaskInfo *task, int iHdl)
{
	TK_DIRENT *de;
	void *de1;
	FILE *fd;
	int i, sde;

	fd=TK_GetPtrForHandle(task, iHdl);
	if(!fd)
		return(-1);
	return(tk_closedir(fd));
}
