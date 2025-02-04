#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <Rpc.h>

#if defined(_M_ARM64EC) || defined(_M_ARM64)
	#include "../RPC_Stub/arm64/RPC_Stub_h.h"
#elif defined(_M_AMD64)
	#include "../RPC_Stub/x64/RPC_Stub_h.h"
#elif defined(_M_IX86)
	#include "../RPC_Stub/x86/RPC_Stub_h.h"
#endif


unsigned int RPC_ConvertToUpperCase( 
    /* [in] */ handle_t IDL_handle,
    /* [size_is][out] */ wchar_t *pOutBuffer,
    /* [in] */ unsigned int out_buffer_count,
    /* [size_is][in] */ const wchar_t *pInBuffer,
    /* [in] */ unsigned int in_chr_count)
{
	DWORD dwThreadId = GetCurrentThreadId();

	if (!in_chr_count)
		return 0;

	if (!out_buffer_count)
		return 0;

	if (in_chr_count >= out_buffer_count - 1)
		in_chr_count = out_buffer_count - 1;

	const WCHAR* whSrc = (const WCHAR*)pInBuffer;
	WCHAR* whDest = (WCHAR*)pOutBuffer;
    const int Offset = L'A' - L'a';
	for (DWORD i = 0; i < in_chr_count; i++)
	{
		WCHAR c = whSrc[i];
		if (c >= L'a' && c <= L'z')
		{
			c += Offset;
		}
		whDest[i] = c;
	}
	whDest[in_chr_count] = 0;
	wprintf_s(L"RPC_ConvertToUpperCase() called by other process. ThreadID:[%u], In Buffer's count:%u, Out buffer's count:%u, str:%s\n", dwThreadId, in_chr_count, out_buffer_count, pInBuffer);
	return in_chr_count;
}

unsigned int RPC_MemCpy(
	/* [in] */ handle_t IDL_handle,
	/* [size_is][out] */ unsigned char *pDest,
	/* [in] */ unsigned int dest_size,
	/* [size_is][in] */ const unsigned char *pSrc,
	/* [in] */ unsigned int src_size)
{
	DWORD dwThreadId = GetCurrentThreadId();

	unsigned int cpy_size = src_size;
	if (cpy_size > dest_size)
		cpy_size = dest_size;

	memcpy(pDest, pSrc, cpy_size);
	wprintf_s(L"RPC_MemCpy() called by other process. ThreadID:[%u], Source-Memory's size:%u, Destination-Memorty's size:%u\n", dwThreadId, src_size, dest_size);
	return cpy_size;
}

void __RPC_FAR * __RPC_USER midl_user_allocate(size_t cBytes)
{
    return((void __RPC_FAR *) malloc(cBytes));
}
void __RPC_USER midl_user_free(void __RPC_FAR * p)
{
    free(p);
}