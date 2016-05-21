//#include "stdafx.h"

#include "vFileName.h"

vFileName::vFileName()
{
}

vFileName::vFileName(vString st)
{
	m_FullPath	= st;
	m_FileExt	= GetFileExt( m_FullPath);
	m_FileName	= GetFileName( m_FullPath);
	m_FilePath	= GetPath( m_FullPath );
	m_FileType	= m_FileExt.Right( m_FileExt.GetLength()-1);

	FindNum();		
}

vString vFileName::GetPath( vString path)
{
	vString ret;

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

vString vFileName::GetPath()
{
	return m_FilePath;
}

vString vFileName::GetFileName( vString path)
{
	vString ret;
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

void vFileName::FindNum()
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

vString vFileName::NO(int no,int order)
{
	vString buf;

	if ( order==0 )	buf.Format("%d",no);
	else
	{
		vString tmp;
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

vString vFileName::GetFileName()
{
	return m_FileName;
}

vString vFileName::GetName()	{ return m_NoNum; }
vString vFileName::GetNum()		{ return m_Num; }

vString vFileName::GetFileExt( vString path )
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
	vString ret;
	ret	= buf;

	return ret;
}

vString vFileName::GetFileExt()
{
	return m_FileExt;
}

