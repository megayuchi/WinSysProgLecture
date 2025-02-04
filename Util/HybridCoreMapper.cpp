#include "pch.h"
#include <Windows.h>
#include "ProcessorInfo.h"
#include "HybridCoreMapper.h"

CHybridCoreMapper::CHybridCoreMapper()
{

}
void CHybridCoreMapper::Initialize()
{
	HANDLE hProcess = GetCurrentProcess();

	m_bHybridCpuSupported = ::IsHybridCpuSupported();
	//m_bHybridCpuSupported = FALSE;

	if (m_bHybridCpuSupported)
	{
		for (DWORD i = HYBRID_CORE_TYPE_PCORE; i < HYBRID_CORE_TYPE_COUNT; i++)
		{
			HYBRID_CORE_TYPE type = (HYBRID_CORE_TYPE)i;
			m_CoreInfoSet[type].ulCoreCount = GetHybridCoreCpuIDSet(hProcess, m_CoreInfoSet[type].CoreCpuIdList, m_CoreInfoSet[type].CoreMaskList, (DWORD)_countof(m_CoreInfoSet[type].CoreCpuIdList), type, HT_CORE_ANY);
		}
	}
	if (0 == m_CoreInfoSet[HYBRID_CORE_TYPE_PCORE].ulCoreCount || 0 == m_CoreInfoSet[HYBRID_CORE_TYPE_ECORE].ulCoreCount)
	{
		m_bHybridCpuSupported = FALSE;
	}
}
BOOL CHybridCoreMapper::SetThreadCoreType(HANDLE hThread, HYBRID_CORE_TYPE type) const
{
	BOOL bResult = FALSE;

	if (type >= HYBRID_CORE_TYPE_COUNT)
		__debugbreak();

	if (m_bHybridCpuSupported)
	{
		if (m_CoreInfoSet[type].ulCoreCount)
		{
			if (SetThreadSelectedCpuSets(hThread, m_CoreInfoSet[type].CoreCpuIdList, m_CoreInfoSet[type].ulCoreCount))
			{
				bResult = TRUE;
			}
		}
	}
	return bResult;
}
ULONG CHybridCoreMapper::GetCoreCount(HYBRID_CORE_TYPE type) const
{
	if (type >= HYBRID_CORE_TYPE_COUNT)
		__debugbreak();

	return m_CoreInfoSet[type].ulCoreCount;
}
const ULONG* CHybridCoreMapper::GetCpuIdList(HYBRID_CORE_TYPE type) const
{
	if (type >= HYBRID_CORE_TYPE_COUNT)
		__debugbreak();

	return m_CoreInfoSet[type].CoreCpuIdList;

}
CHybridCoreMapper::~CHybridCoreMapper()
{

}