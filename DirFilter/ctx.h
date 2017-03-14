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


#define STREAM_CONTEXT_SIZE sizeof(STREAM_CONTEXT)
#define STREAM_CONTEXT_TAG	'SCTA'


NTSTATUS
Ctx_FindOrCreateStreamContext(
	_In_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_In_ BOOLEAN CreateIfNotFound,
	_Out_ PSTREAM_CONTEXT *StreamContext,
	_Out_ BOOLEAN *ContextCreated
);



#endif