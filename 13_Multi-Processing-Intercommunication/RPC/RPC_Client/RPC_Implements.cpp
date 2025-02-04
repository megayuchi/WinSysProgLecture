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

void __RPC_FAR * __RPC_USER midl_user_allocate(size_t cBytes)
{
	return((void __RPC_FAR *) malloc(cBytes));
}
void __RPC_USER midl_user_free(void __RPC_FAR * p)
{
	free(p);
}