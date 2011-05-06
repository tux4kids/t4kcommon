/*
   t4k_replacements.c

   Replacements for functions not present locally on a system.

   Copyright 2007, 2008, 2009, 2010.
Authors: David Bruce, Aviral Dasgupta, Hatari Project
Project email: <tuxmath-devel@lists.sourceforge.net>
Project website: http://tux4kids.alioth.debian.org

t4k_replacements.c is part of the t4k_common library.

t4k_common is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

t4k_common is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "config.h"

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* TODO Is this required at all? */
#ifdef WIN32
#   undef DATADIR
#   include <windows.h>
#endif /* WIN32 */

/* Prefer strcoll, but use strcmp otherwise, for string comparison */
#ifdef HAVE_STRCOLL
#   define _str_Compare(s1, s2) strcoll((s1), (s2))
#else
#   define _str_Compare(s1, s2) strcmp((s1), (s2))
#endif

/* replacement for alphasort; works on Windows too! */
#ifndef HAVE_ALPHASORT
int alphasort(const struct dirent **d1, const struct dirent **d2) {
    return _str_Compare((*d1)->d_name, (*d2)->d_name);
}
#endif //!HAVE_ALPHASORT

#ifndef HAVE_SCANDIR
#   ifdef WIN32
int scandir(const char *dirname, struct dirent ***namelist, int (*sdfilter)(const struct dirent *), int (*dcomp)(const struct dirent**, const struct dirent**))
{
    int len;
    char *findIn, *d;
    WIN32_FIND_DATA find;
    HANDLE h;
    int nDir = 0, NDir = 0;
    struct dirent **dir = 0, *selectDir;
    unsigned long ret;

    len    = strlen(dirname);
    findIn = (char *)malloc(len+5);
    strcpy(findIn, dirname);
    printf("scandir : findIn orign=%s\n", findIn);
    for (d = findIn; *d; d++)
	if (*d=='/')
	    *d='\\';
    if ((len==0))
    {
	strcpy(findIn, ".\\*");
    }
    if ((len==1)&& (d[-1]=='.'))
    {
	strcpy(findIn, ".\\*");
    }
    if ((len>0) && (d[-1]=='\\'))
    {
	*d++ = '*';
	*d = 0;
    }
    if ((len>1) && (d[-1]=='.') && (d[-2]=='\\'))
    {
	d[-1] = '*';
    }
    if ((len>1) && (d[-2]!='\\') && (d[-1]!='*'))
    {
	*d++ = '\\';
	*d++ = '*';
	*d = 0;
    }

    printf("scandir : findIn processed=%s\n", findIn);
    if ((h=FindFirstFile(findIn, &find))==INVALID_HANDLE_VALUE)
    {
	printf("scandir : FindFirstFile error\n");
	ret = GetLastError();
	if (ret != ERROR_NO_MORE_FILES)
	{
	    // TODO: return some error code
	}
	*namelist = dir;
	return nDir;
    }
    do
    {
	printf("scandir : findFile=%s\n", find.cFileName);
	selectDir=(struct dirent*)malloc(sizeof(struct dirent)+strlen(find.cFileName));
	strcpy(selectDir->d_name, find.cFileName);
	if (!sdfilter || (*sdfilter)(selectDir))
	{
	    if (nDir==NDir)
	    {
		struct dirent **tempDir = (struct dirent **)calloc(sizeof(struct dirent*), NDir+33);
		if (NDir)
		    memcpy(tempDir, dir, sizeof(struct dirent*)*NDir);
		if (dir)
		    free(dir);
		dir = tempDir;
		NDir += 32;
	    }
	    dir[nDir] = selectDir;
	    nDir++;
	    dir[nDir] = 0;
	}
	else
	{
	    free(selectDir);
	}
    }
    while (FindNextFile(h, &find));
    ret = GetLastError();
    if (ret != ERROR_NO_MORE_FILES)
    {
	// TODO: return some error code
    }
    FindClose(h);

    free (findIn);

    if (dcomp)
	qsort (dir, nDir, sizeof(*dir),dcomp);

    *namelist = dir;
    return nDir;
}
#   elif defined(__BEOS__) || defined(__HAIKU__) || (defined(__sun) && defined(__SVR4))

#undef DIRSIZ

#define DIRSIZ(dp)                                          \
    ((sizeof(struct dirent) - sizeof(dp)->d_name) +     \
     (((dp)->d_reclen + 1 + 3) &~ 3))

#if defined(__sun) && defined(__SVR4)
# define dirfd(d) ((d)->dd_fd)
#elif defined(__BEOS__)
#ifndef (__HAIKU__)
#else
# define dirfd(d) ((d)->fd)
#endif
#endif

int scandir(const char *dirname, struct dirent ***namelist, int (*sdfilter)(const struct dirent *), int (*dcomp)(const struct dirent**, const struct dirent**))
{
    struct dirent *d, *p, **names;
    struct stat stb;
    size_t nitems;
    size_t arraysz;
    DIR *dirp;

    if ((dirp = opendir(dirname)) == NULL)
	return(-1);

    if (fstat(dirfd(dirp), &stb) < 0)
	return(-1);

    /*
     * estimate the array size by taking the size of the directory file
     * and dividing it by a multiple of the minimum size entry.
     */
    arraysz = (stb.st_size / 24);

    names = (struct dirent **)malloc(arraysz * sizeof(struct dirent *));
    if (names == NULL)
	return(-1);

    nitems = 0;

    while ((d = readdir(dirp)) != NULL)
    {

	if (sdfilter != NULL && !(*sdfilter)(d))
	    continue;       /* just selected names */

	/*
	 * Make a minimum size copy of the data
	 */

	p = (struct dirent *)malloc(DIRSIZ(d));
	if (p == NULL)
	    return(-1);

	p->d_ino = d->d_ino;
	p->d_reclen = d->d_reclen;
	/*p->d_namlen = d->d_namlen;*/
	memcpy(p->d_name, d->d_name, p->d_reclen + 1);

	/*
	 * Check to make sure the array has space left and
	 * realloc the maximum size.
	 */

	if (++nitems >= arraysz)
	{

	    if (fstat(dirfd(dirp), &stb) < 0)
		return(-1);     /* just might have grown */

	    arraysz = stb.st_size / 12;

	    names = (struct dirent **)realloc((char *)names, arraysz * sizeof(struct dirent *));
	    if (names == NULL)
		return(-1);
	}

	names[nitems-1] = p;
    }

    closedir(dirp);

    if (nitems && dcomp != NULL)
	qsort(names, nitems, sizeof(struct dirent *), dcomp);

    *namelist = names;

    return nitems;
}
#   else // Display a "fatal" error and exit on other platforms
#   error t4kcommon : scandir not implemented for your platform. Report at tuxmath-devel@lists.sourceforge.net
/* NOTE Removed in favour of compile time preprocessor error macro. *//*
									 int scandir(const char *dirname, struct dirent ***namelist, int (*sdfilter)(struct dirent *), int (*dcomp)(const void *, const void *)) {
									 fprintf(stderr, "scandir (t4kcommon) fatal: unimplemented on your platform.\n");
									 exit(EXIT_SUCCESS);
									 }
									 */
#   endif // Platform selection.
#endif //!HAVE_SCANDIR


/* At this point, we should always have alphasort and scandir, whether 
 * in platform or using our replacements above. Therefore, we can simply
 * define T4K_alphasort() and T4K_scandir() as wrappers:
 */
int T4K_alphasort(const struct dirent **d1, const struct dirent **d2)
{
    return alphasort(d1, d2);
}

int T4K_scandir(const char *dirname, struct dirent ***namelist, int (*sdfilter)(const struct dirent *), int (*dcomp)(const struct dirent**, const struct dirent**))
{
    return scandir(dirname, namelist, sdfilter, dcomp);
}

