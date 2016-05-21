#include "stdafx.h"

#include "filename.h"

CFileName::CFileName()
{
}

CFileName::CFileName(CString st)
{
	m_FullPath	= st;
	m_FileExt	= GetFileExt( m_FullPath);
	m_FileName	= GetFileName( m_FullPath);
	m_FilePath	= GetPath( m_FullPath );
	m_FileType	= m_FileExt.Right( m_FileExt.GetLength()-1);

	FindNum();		
}

CString CFileName::GetPath( CString path)
{
	CString ret;

	int max=0;
	int i=0;
	char *buf = path.GetBuffer( path.GetLength() );
	
	while(1)
	{
		if ( buf[i]==0)	break;
		if ( buf[i]=='\\' || buf[i]=='/')	max	=i;
		i++;
	}

	ret	= buf;
	ret	= ret.Left(max);

	return ret;
}

CString CFileName::GetPath()
{
	return m_FilePath;
}

CString CFileName::GetFileName( CString path)
{
	CString ret;
	int max=0;
	int i=0;
	char *buf = path.GetBuffer( path.GetLength() );
	
	while(1)
	{
		if ( buf[i]==0)	break;
		if ( buf[i]=='\\' || buf[i]=='/')	max	=i+1;
		i++;
	}

	ret	= &buf[max];
	/*
	int dot = ret.Find('.');
	while(1)
	{
		int t = path.Find('.',dot+1);
		if ( t<0 )	break;
		else		dot	= t;
	}

	if ( dot>=0 )	
	ret	= ret.Left( dot );
	*/
	return ret;
}

void CFileName::FindNum()
{
	int max = m_FileName.GetLength();
	max-= m_FileExt.GetLength();
	int i,j;
	for (  i=max-1,j=max-1;i>=0 ;i--,j--)
	if ( m_FileName[i]>='0' && m_FileName[i]<='9' )	;
	else break;

	if ( j+1==max )	
	{
		m_NoNum	= m_FileName;
		m_Num.Empty();
	}
	else
	{
		m_NoNum	= m_FileName.Left(j+1);
		m_Num	= m_FileName.Mid(j+1,max-j-1);
	}
	if (m_Num.IsEmpty() )	nNum	= 0;
	else					nNum	= atoi(m_Num );
	
}

CString CFileName::NO(int no,int order)
{
	CString buf;

	if ( order==0 )	buf.Format("%d",no);
	else
	{
		CString tmp;
		buf.Format("%d",no);

		int nz = order-buf.GetLength();
		if ( nz<=0 )	return buf;
		else
		{
			tmp.Empty();
			for ( int i=0;i<nz;i++)	tmp	= tmp+"0";
			tmp	= tmp+"%d";
			buf.Format( tmp,no );
		}
	}
	return buf;
}

CString CFileName::GetFileName()
{
	return m_FileName;
}

CString CFileName::GetName()	{ return m_NoNum; }
CString CFileName::GetNum()		{ return m_Num; }

CString CFileName::GetFileExt( CString path )
{
	char *buf;
	buf	= path.GetBuffer( path.GetLength() );

	int s = path.Find('.');
	while(1)
	{
		int t = path.Find('.',s+1);
		if ( t<0 )	break;
		else		s	= t;
	}


	buf	= &buf[ s ];
	CString ret;
	ret	= buf;

	return ret;
}

CString CFileName::GetFileExt()
{
	return m_FileExt;
}


BOOL CFileName::Copy( CString s,CString t)
{
	HANDLE	hb;
	char	*buf;

	FILE *fp	= fopen( s,"rb");
	if ( fp==NULL )
	{
		AfxMessageBox( "Cannot read the file");
		return FALSE;
	}
	fseek(fp,0,SEEK_END);
	long size	= ftell(fp);

	hb	= ::GlobalAlloc(GHND,size);
	buf	= (char*)GlobalLock(hb);
	
	fseek(fp,0,SEEK_SET);
	fread( buf,size,1,fp);
	fclose(fp);

	fp	= fopen( t,"wb");
	if ( fp==NULL )
	{
		AfxMessageBox( "Cannot write the file");
		return FALSE;
	}
	fwrite( buf,size,1,fp);
	fclose(fp);

	return TRUE;
}

CString CFileName::GetFileType()
{
	return m_FileType;
}

// is this the sub folder of s?
BOOL CFileName::IsSubOf( CString s )
{
	if ( m_FullPath.Find(s)<0)	return FALSE;

	CFileName fn(s);
	if ( fn.GetPath()==GetPath() )	return FALSE;

	if ( m_FullPath[s.GetLength()]!='\\')	return FALSE;

	return TRUE;
}

CString CFileName::GetNoExtName()
{
	char *buf;
	CString path = m_FileName;
	buf	= (char*)path.operator LPCTSTR();
	int a	= path.Find("*.*");
	if ( a>=0)	
	{
		path	= path.Left( path.GetLength()-a);
		return path;
	}

	int s = path.Find('.');
	while(1)
	{
		int t = path.Find('.',s+1);
		if ( t<0 )	break;
		else		s	= t;
	}


	buf[ s ]	= 0;
	CString ret;
	ret	= buf;	
	return ret;
}




void CFileName::MakeFileNameWithDateTime(CString *rtn)
{
	char dateStr[9];
	char timeStr[9];

	_strdate( dateStr);
	_strtime(timeStr);
	CString tmp;
	tmp.Format("%s_%s",dateStr, timeStr);
	tmp.Replace(":", "");
	tmp.Replace("/", "");

	*rtn = tmp;
}


CString CFileName::GetFileNameS( CString path)
{
	CString ret;
	int max=0;
	int i=0;
	char *buf = path.GetBuffer( path.GetLength() );
	
	while(1)
	{
		if ( buf[i]==0)	break;
		if ( buf[i]=='\\' || buf[i]=='/')	max	=i+1;
		i++;
	}

	ret	= &buf[max];
	/*
	int dot = ret.Find('.');
	while(1)
	{
		int t = path.Find('.',dot+1);
		if ( t<0 )	break;
		else		dot	= t;
	}

	if ( dot>=0 )	
	ret	= ret.Left( dot );
	*/
	return ret;
}