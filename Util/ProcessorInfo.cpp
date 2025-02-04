#include "pch.h"
#include <Windows.h>
#include "ProcessorInfo.h"
#include "HybridDetect.h"


typedef BOOL(WINAPI *LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);
DWORD CountSetBits(ULONG_PTR bitMask);

BOOL GetPhysicalCoreCount(DWORD* pdwOutPhysicalCoreCount, DWORD* pdwOutLogicalCoreCount)
{
	BOOL	bResult = FALSE;

	{
		LPFN_GLPI glpi;

		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION pBuffer = nullptr;
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = nullptr;
		DWORD returnLength = 0;
		DWORD logicalProcessorCount = 0;
		DWORD numaNodeCount = 0;
		DWORD processorCoreCount = 0;
		DWORD processorL1CacheCount = 0;
		DWORD processorL2CacheCount = 0;
		DWORD processorL3CacheCount = 0;
		DWORD processorPackageCount = 0;
		DWORD byteOffset = 0;
		PCACHE_DESCRIPTOR Cache;



		glpi = (LPFN_GLPI)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "GetLogicalProcessorInformation");
		if (nullptr == glpi)
		{
			SYSTEM_INFO systemInfo;
			GetSystemInfo(&systemInfo);
			*pdwOutPhysicalCoreCount = systemInfo.dwNumberOfProcessors;
			*pdwOutLogicalCoreCount = systemInfo.dwNumberOfProcessors;

			OutputDebugStringW(L"\nGetLogicalProcessorInformation is not supported.\n");
			goto lb_return;
		}

		BOOL done = FALSE;
		while (!done)
		{
			DWORD rc = glpi(pBuffer, &returnLength);

			if (FALSE == rc)
			{
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					if (pBuffer)
						free(pBuffer);

					pBuffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);
				}
				else
				{
					break;
				}
			}
			else
			{
				done = TRUE;
			}
		}

		ptr = pBuffer;

		while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
		{
			switch (ptr->Relationship)
			{
				case RelationNumaNode:
					// Non-NUMA systems report a single record of this type.
					numaNodeCount++;
					break;

				case RelationProcessorCore:
					processorCoreCount++;

					// A hyperthreaded core supplies more than one logical processor.
					logicalProcessorCount += CountSetBits(ptr->ProcessorMask);
					break;

				case RelationCache:
					// Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache. 
					Cache = &ptr->Cache;
					if (Cache->Level == 1)
					{
						processorL1CacheCount++;
					}
					else if (Cache->Level == 2)
					{
						processorL2CacheCount++;
					}
					else if (Cache->Level == 3)
					{
						processorL3CacheCount++;
					}
					break;

				case RelationProcessorPackage:
					// Logical processors share a physical package.
					processorPackageCount++;
					break;

				default:
					OutputDebugStringW(L"\nError: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n");
					break;
			}
			byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
			ptr++;
		}
		*pdwOutPhysicalCoreCount = processorCoreCount;
		*pdwOutLogicalCoreCount = logicalProcessorCount;
		//numaNodeCount;
		//processorPackageCount;
		//processorCoreCount;
		//logicalProcessorCount;
		//processorL1CacheCount;
		//processorL2CacheCount;
		//processorL3CacheCount

		free(pBuffer);

		bResult = TRUE;
	}
lb_return:
	return bResult;
}

// Helper function to count set bits in the processor mask.
DWORD CountSetBits(ULONG_PTR bitMask)
{
	DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
	DWORD bitSetCount = 0;
	ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
	DWORD i;

	for (i = 0; i <= LSHIFT; ++i)
	{
		bitSetCount += ((bitMask & bitTest) ? 1 : 0);
		bitTest /= 2;
	}

	return bitSetCount;
}
BOOL IsHybridCpuSupported()
{
	BOOL bResult = FALSE;
#if defined(_M_ARM64) || defined(_M_ARM64EC)
	bResult = TRUE;
#else
	PROCESSOR_INFO	info = {};
	GetProcessorInfo(info);

	if (info.hybrid)
	{
		bResult = TRUE;
	}
#endif
	return bResult;
}

ULONG GetHybridCoreCpuIDSet(HANDLE hProcess, ULONG* pulOutIDList, DWORD_PTR* pOutCoreMaskList, DWORD dwMaxBufferCount, HYBRID_CORE_TYPE hybridCoreType, HT_CORE_TYPE htCoreType)
{
	ULONG	ulCpuIdCount = 0;
	ULONG bufferSize = 0;
	// Get the Current Process Handle.
	//HANDLE curProc = GetCurrentProcess();

	DWORD	dwPhysicalCoreCount = 0;
	DWORD	dwLogicalCoreCount = 0;
	GetPhysicalCoreCount(&dwPhysicalCoreCount, &dwLogicalCoreCount);

	// Get total number (size) of elements in the data structure.
	GetSystemCpuSetInformation(nullptr, 0, &bufferSize, hProcess, 0);

	// Allocate data structures based on size returned from first call.
	uint8_t* pBuffer = (uint8_t*)malloc(bufferSize);
	memset(pBuffer, 0, bufferSize);


	// Get all of the CPUSet elements
	if (GetSystemCpuSetInformation((SYSTEM_CPU_SET_INFORMATION*)pBuffer, bufferSize, &bufferSize, hProcess, 0))
	{
		uint8_t* cpuSetPtr = pBuffer;

		for (ULONG cpuSetSize = 0; cpuSetSize < bufferSize; )
		{
			SYSTEM_CPU_SET_INFORMATION* nextCPUSet = (SYSTEM_CPU_SET_INFORMATION*)cpuSetPtr;

			if (nextCPUSet->Type == CPU_SET_INFORMATION_TYPE::CpuSetInformation)
			{
				// Store Logical Processor Information for Later Use.
				LOGICAL_PROCESSOR_INFO core = {};
				core.id = nextCPUSet->CpuSet.Id;
				core.group = nextCPUSet->CpuSet.Group;
				core.node = nextCPUSet->CpuSet.NumaNodeIndex;
				core.logicalProcessorIndex = nextCPUSet->CpuSet.LogicalProcessorIndex;
				core.coreIndex = nextCPUSet->CpuSet.CoreIndex;
				core.realTime = nextCPUSet->CpuSet.RealTime;
				core.parked = nextCPUSet->CpuSet.Parked;
				core.allocated = nextCPUSet->CpuSet.Allocated;
				core.allocatedToTargetProcess = nextCPUSet->CpuSet.AllocatedToTargetProcess;
				core.allocationTag = nextCPUSet->CpuSet.AllocationTag;
				core.efficiencyClass = nextCPUSet->CpuSet.EfficiencyClass;
				core.schedulingClass = nextCPUSet->CpuSet.SchedulingClass;

				DWORD_PTR CoreMask = (DWORD_PTR)1 << core.logicalProcessorIndex;
				if (HT_PHYSICAL_CORE == htCoreType)
				{
					// 논리 코어를 스킵
					if (core.logicalProcessorIndex != core.coreIndex)
						goto lb_next;
				}
				else if (HT_LOGICAL_CORE == htCoreType)
				{
					// 물리 코어를 스킵
					if (core.logicalProcessorIndex == core.coreIndex)
						goto lb_next;
				}
				if (HYBRID_CORE_TYPE_PCORE == hybridCoreType)
				{
					if (core.efficiencyClass > 0)
					{

						if (ulCpuIdCount < dwMaxBufferCount)
						{
							pulOutIDList[ulCpuIdCount] = core.id;
							pOutCoreMaskList[ulCpuIdCount] = CoreMask;
							ulCpuIdCount++;
						}
					}
				}
				else if (HYBRID_CORE_TYPE_ECORE == hybridCoreType)
				{
					if (!core.efficiencyClass)
					{
						if (ulCpuIdCount < dwMaxBufferCount)
						{
							pulOutIDList[ulCpuIdCount] = core.id;
							pOutCoreMaskList[ulCpuIdCount] = CoreMask;
							ulCpuIdCount++;
						}
					}
				}
				else if (HYBRID_CORE_TYPE_ALL == hybridCoreType)
				{
					if (ulCpuIdCount < dwMaxBufferCount)
					{
						pulOutIDList[ulCpuIdCount] = core.id;
						pOutCoreMaskList[ulCpuIdCount] = CoreMask;
						ulCpuIdCount++;
					}
				}
				else
				{
					__debugbreak();
				}
			}
		lb_next:
			cpuSetPtr += nextCPUSet->Size;
			cpuSetSize += nextCPUSet->Size;
		}
	}
	if (pBuffer)
	{
		free(pBuffer);
		pBuffer = nullptr;
	}
	return ulCpuIdCount;
}
DWORD GetPCoresProcessorMask(UINT64* pi64OutMask)
{
	///
	DWORD dwCoreCount = 0;

	UINT64 ui64Mask = 0;
	//SetThreadIdealProcessor;


	printf("\nProcessors\n"); int i = 0;

	for (EnumLogicalProcessorInformation enumInfo(RelationProcessorCore); auto pinfo = enumInfo.Current(); enumInfo.MoveNext())
	{
		for (UINT GroupIndex = 0; GroupIndex < pinfo->Processor.GroupCount; GroupIndex++)
		{
			printf("Group                    %d\n", pinfo->Processor.GroupMask[GroupIndex].Group);
			printf("Group Count:             %d\n", pinfo->Processor.GroupCount);
			printf("Node Number:             %d\n", (int)pinfo->NumaNode.NodeNumber);
			printf("Processor:               %d\n", i++);
			printf("Efficiency Class:        %d\n", (int)pinfo->Processor.EfficiencyClass);
			printf("Hyperthreaded:           %s\n", pinfo->Processor.Flags ? "Yes" : "No");
			printf("Processor Mask:          ");
			PrintMask(pinfo->Processor.GroupMask[GroupIndex].Mask);
			printf("\n\n");

			if (pinfo->Processor.EfficiencyClass > 0)
			{
				ui64Mask |= pinfo->Processor.GroupMask[GroupIndex].Mask;
			}
		}
	}
	*pi64OutMask = ui64Mask;
	return dwCoreCount;
}