#pragma once
#ifndef _CONTEXTFUNC_H
#define _CONTEXTFUNC_H

#include "common.h"


typedef struct _STREAM_CONTEXT
{
	BOOLEAN bNeedEncrypt;
	BOOLEAN bNeedDecrypt;
	BOOLEAN bIsThereAFlag;		//�����flag˵���ļ��Ѿ�����
	BOOLEAN bHasWrittenData;	//ȷ���Ƿ�������д���ļ�

}STREAM_CONTEXT, *PSTREAM_CONTEXT;




#endif