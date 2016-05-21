#ifndef __vFileName__
#define __vFileName__

#include "vString.h"

typedef struct {
		vString		name;
		int			type;
}  FILETYPE;


class vFileName
{
public:
		vFileName();
		vFileName(vString);
		//~vFileName();

public:
		vString GetPath();
		vString GetFileName();
		vString	GetFileExt();
		vString GetFileType();
		vString GetNoExtName();
		


		vString GetPath(vString);
		vString GetFileName(vString);
		vString	GetFileExt(vString);


public:
		vString m_FullPath;
		vString	m_FileName;
		vString m_FileExt;
		vString m_FilePath;
		vString m_FileType;




};


#endif 