#pragma once


enum HYBRID_CORE_TYPE
{
	HYBRID_CORE_TYPE_PCORE,
	HYBRID_CORE_TYPE_ECORE,
	HYBRID_CORE_TYPE_ALL,
	HYBRID_CORE_TYPE_COUNT
};
enum HT_CORE_TYPE
{
	HT_PHYSICAL_CORE,
	HT_LOGICAL_CORE,
	HT_CORE_ANY,
	HT_CORE_TYPE_COUNT
};


BOOL GetPhysicalCoreCount(DWORD* pdwOutPhysicalCoreCount, DWORD* pdwOutLogicalCoreCount);
DWORD GetPCoresProcessorMask(UINT64* pi64OutMask);
ULONG GetHybridCoreCpuIDSet(HANDLE hProcess, ULONG* pulOutIDList, DWORD_PTR* pOutCoreMaskList, DWORD dwMaxBufferCount, HYBRID_CORE_TYPE hybridCoreType, HT_CORE_TYPE htCoreType);
BOOL IsHybridCpuSupported();
