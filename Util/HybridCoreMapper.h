#pragma once

#include "ProcessorInfo.h"

static const DWORD MAX_CORE_COUNT = 256;

struct CORE_INFO_SET
{
	ULONG CoreCpuIdList[MAX_CORE_COUNT];
	DWORD_PTR	CoreMaskList[MAX_CORE_COUNT];
	ULONG ulCoreCount;
};

class CHybridCoreMapper
{

	BOOL m_bHybridCpuSupported = FALSE;
	
	CORE_INFO_SET	m_CoreInfoSet[HYBRID_CORE_TYPE_COUNT] = {};

public:
	void	Initialize();
	BOOL	SetThreadCoreType(HANDLE hThread, HYBRID_CORE_TYPE type) const;
	BOOL	IsHybridCpuSupported() const { return m_bHybridCpuSupported; };
	ULONG	GetCoreCount(HYBRID_CORE_TYPE type) const;
	const ULONG* GetCpuIdList(HYBRID_CORE_TYPE type) const;
	CHybridCoreMapper();
	~CHybridCoreMapper();
};
