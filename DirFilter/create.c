#include "callbackRoutines.h"

FLT_PREOP_CALLBACK_STATUS
PreCreate(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID *CompletionContext
)
{
	ULONG createOptions;
	FLT_PREOP_CALLBACK_STATUS retVal = FLT_PREOP_SUCCESS_NO_CALLBACK;

	PAGED_CODE();

	try
	{
		//
		//���̼��
		//

		PCHAR procName = GetProcessName();
		if (!IsMonitoredProcess(procName))
		{
			leave;
		}


		//
		//��ҳ�����ڴ��������ʱ���Ǿ���д��
		//

		if (Data->Iopb->OperationFlags&SL_OPEN_PAGING_FILE)
		{
			//DbgPrint("\tIgnoring paging file open\n");
			leave;
		}

		//
		// �򿪾�
		//

		if (Data->Iopb->TargetFileObject->Flags&FO_VOLUME_OPEN)
		{
			//DbgPrint("\tIgnoring volume open\n");
			leave;
		}

		//
		//���ļ���
		//

		if (Data->Iopb->OperationFlags&SL_OPEN_TARGET_DIRECTORY)
		{
			leave;
		}


		//
		//�ļ�������Ϊ,ʵ��д���ļ���
		//

		createOptions = Data->Iopb->Parameters.Create.Options >> 24;
		/*if (createOptions != FILE_SUPERSEDE&&
			createOptions != FILE_OVERWRITE&&
			createOptions != FILE_OVERWRITE_IF&&
			createOptions != FILE_CREATE&&
			createOptions != FILE_OPEN_IF)
		{
			leave;
		}*/


		retVal = FLT_PREOP_SUCCESS_WITH_CALLBACK;

	}
	finally
	{
		

	}

	return retVal;
}


FLT_POSTOP_CALLBACK_STATUS
PostCreate(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__inout_opt PVOID CompletionContext,
	__in FLT_POST_OPERATION_FLAGS Flags
)
{
	NTSTATUS status;
	PSTREAM_CONTEXT pStreamCtx = NULL;
	BOOLEAN bNewCreated = FALSE;

	ULONG retVal;
	ULONG createOptions = Data->Iopb->Parameters.Create.Options >> 24;

	try
	{
		if (!NT_SUCCESS(Data->IoStatus.Status))
		{
			leave;
		}


		//
		// ���Դ򿪣�flag�ļ�������ɹ���˵���ļ��Ѽ���,ʧ����鿴�Ƿ�Ϊ�����ļ�
		//

		retVal = CreateOrOpenFileWithFlag(Data, FltObjects, FILE_OPEN);
		if (retVal < RET_CREATE_OPEN_FAIL)
		{
			leave;
		}

		//
		// ������ ��ȡ������
		//

		status = Ctx_FindOrCreateStreamContext(Data, FltObjects, TRUE, &pStreamCtx, &bNewCreated);
		if (!NT_SUCCESS(status))
		{
			leave;
		}

		if (!bNewCreated)
		{
			leave;
		}

		DbgPrint("\tContext Created\n");

		//
		// �򿪣�flag�ļ��ɹ���˵���ļ��Ѿ�����
		//

		if (retVal == RET_CREATE_OPEN_SUCCEED)
		{

			pStreamCtx->bHasWrittenData = FALSE;
			pStreamCtx->bIsThereAFlag = TRUE;
			pStreamCtx->bNeedDecrypt = TRUE;
			pStreamCtx->bNeedEncrypt = TRUE;

		}

		//
		//�򿪣�flag�ļ�ʧ�ܣ�������Ҫ�����Ƿ�Ϊ�½��ļ���������ǣ�˵��ֻ����ͨ�ļ��Ź�
		//

		if (retVal == RET_CREATE_OPEN_FAIL)
		{
			if (createOptions != FILE_SUPERSEDE&&
				createOptions != FILE_OVERWRITE&&
				createOptions != FILE_OVERWRITE_IF&&
				createOptions != FILE_CREATE&&
				createOptions != FILE_OPEN_IF)
			{
				leave;
			}

			pStreamCtx->bHasWrittenData = FALSE;
			pStreamCtx->bIsThereAFlag = FALSE;
			pStreamCtx->bNeedDecrypt = TRUE;
			pStreamCtx->bNeedEncrypt = TRUE;

		}


		DbgPrint("\tPostCreate Ended!\n");

	}
	finally
	{
		if (pStreamCtx)
		{
			FltReleaseContext(pStreamCtx);
		}
	}

	return FLT_POSTOP_FINISHED_PROCESSING;
}