/*
  GUIDO Library
  Copyright (C) 2006  Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#include <stdio.h>

enum { kNotUnicode, kUTF16BigEndian, kUTF16LittleEndian };

static int is_unicode (FILE * fd)
{
	int c1, c2;
	rewind (fd);
	c1 = getc( fd );
	c2 = getc( fd );
	if ((c1 == 0xff) && (c2 == 0xfe)) 
		return kUTF16LittleEndian;
	else if ((c1 == 0xfe) && (c2 == 0xff))
		return kUTF16BigEndian;	
	return kNotUnicode;
}

static long file_length (FILE * fd)
{
	if (fseek (fd, 0, SEEK_END) == -1) 
		return 0;
	return ftell(fd);
}

void convert_from_unicode (const char *filename)
{
	FILE *fd = fopen (filename,"rb");
	if (!fd) return;
	long n , len, i;
	unsigned char * content = 0;
	
	int ucode = is_unicode (fd);
	if (!ucode)  goto end;
	
	len = file_length(fd);
	if (!len) goto end;

	content = new unsigned char[len];
	if (!content) goto end;
	n = fread (content, len, 1, fd);
	if (n != len) goto free;
	fclose( fd );

	// write content back
	fd = fopen (filename,"wt");
	for (i = ((ucode==kUTF16LittleEndian) ? 2 : 3); i < len; i+=2 ) {
		fputc (content[i], fd);
	}
free:
	delete content;
end:
	fclose(fd);
	return;
}
