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
		if (createOptions != FILE_SUPERSEDE&&
			createOptions != FILE_OVERWRITE&&
			createOptions != FILE_OVERWRITE_IF&&
			createOptions != FILE_CREATE&&
			createOptions != FILE_OPEN_IF)
		{
			leave;
		}

		


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
	KIRQL irql = KeGetCurrentIrql();

	//DbgPrint("\tIRQL: %d\n", irql);

	if (AddFileFlag(Data, FltObjects))
	{
		DbgPrint("\tFile Flag Added!\n");
	}

	return FLT_POSTOP_FINISHED_PROCESSING;
}