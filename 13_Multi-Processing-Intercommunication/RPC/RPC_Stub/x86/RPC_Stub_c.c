

/* this ALWAYS GENERATED file contains the RPC client stubs */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for RPC_Stub.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0628 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#include <string.h>

#include "RPC_Stub_h.h"

#define TYPE_FORMAT_STRING_SIZE   67                                
#define PROC_FORMAT_STRING_SIZE   117                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _RPC_Stub_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } RPC_Stub_MIDL_TYPE_FORMAT_STRING;

typedef struct _RPC_Stub_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } RPC_Stub_MIDL_PROC_FORMAT_STRING;

typedef struct _RPC_Stub_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } RPC_Stub_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax_2_0 = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};

#if defined(_CONTROL_FLOW_GUARD_XFG)
#define XFG_TRAMPOLINES(ObjectType)\
NDR_SHAREABLE unsigned long ObjectType ## _UserSize_XFG(unsigned long * pFlags, unsigned long Offset, void * pObject)\
{\
return  ObjectType ## _UserSize(pFlags, Offset, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserMarshal_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserMarshal(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserUnmarshal_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserUnmarshal(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE void ObjectType ## _UserFree_XFG(unsigned long * pFlags, void * pObject)\
{\
ObjectType ## _UserFree(pFlags, (ObjectType *)pObject);\
}
#define XFG_TRAMPOLINES64(ObjectType)\
NDR_SHAREABLE unsigned long ObjectType ## _UserSize64_XFG(unsigned long * pFlags, unsigned long Offset, void * pObject)\
{\
return  ObjectType ## _UserSize64(pFlags, Offset, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserMarshal64_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserMarshal64(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserUnmarshal64_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserUnmarshal64(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE void ObjectType ## _UserFree64_XFG(unsigned long * pFlags, void * pObject)\
{\
ObjectType ## _UserFree64(pFlags, (ObjectType *)pObject);\
}
#define XFG_BIND_TRAMPOLINES(HandleType, ObjectType)\
static void* ObjectType ## _bind_XFG(HandleType pObject)\
{\
return ObjectType ## _bind((ObjectType) pObject);\
}\
static void ObjectType ## _unbind_XFG(HandleType pObject, handle_t ServerHandle)\
{\
ObjectType ## _unbind((ObjectType) pObject, ServerHandle);\
}
#define XFG_TRAMPOLINE_FPTR(Function) Function ## _XFG
#define XFG_TRAMPOLINE_FPTR_DEPENDENT_SYMBOL(Symbol) Symbol ## _XFG
#else
#define XFG_TRAMPOLINES(ObjectType)
#define XFG_TRAMPOLINES64(ObjectType)
#define XFG_BIND_TRAMPOLINES(HandleType, ObjectType)
#define XFG_TRAMPOLINE_FPTR(Function) Function
#define XFG_TRAMPOLINE_FPTR_DEPENDENT_SYMBOL(Symbol) Symbol
#endif


extern const RPC_Stub_MIDL_TYPE_FORMAT_STRING RPC_Stub__MIDL_TypeFormatString;
extern const RPC_Stub_MIDL_PROC_FORMAT_STRING RPC_Stub__MIDL_ProcFormatString;
extern const RPC_Stub_MIDL_EXPR_FORMAT_STRING RPC_Stub__MIDL_ExprFormatString;

#define GENERIC_BINDING_TABLE_SIZE   0            


/* Standard interface: RPC_StubInteface, ver. 1.0,
   GUID={0xf56cd7ef,0x28ad,0x47d6,{0x82,0x1b,0xf0,0xe9,0x99,0x59,0x38,0xcd}} */



static const RPC_CLIENT_INTERFACE RPC_StubInteface___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0xf56cd7ef,0x28ad,0x47d6,{0x82,0x1b,0xf0,0xe9,0x99,0x59,0x38,0xcd}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0,
    0x00000000
    };
RPC_IF_HANDLE RPC_StubInteface_v1_0_c_ifspec = (RPC_IF_HANDLE)& RPC_StubInteface___RpcClientInterface;
#ifdef __cplusplus
namespace {
#endif

extern const MIDL_STUB_DESC RPC_StubInteface_StubDesc;
#ifdef __cplusplus
}
#endif

static RPC_BINDING_HANDLE RPC_StubInteface__MIDL_AutoBindHandle;


unsigned int RPC_ConvertToUpperCase( 
    /* [in] */ handle_t IDL_handle,
    /* [size_is][out] */ wchar_t *pOutBuffer,
    /* [in] */ unsigned int out_buffer_count,
    /* [size_is][in] */ const wchar_t *pInBuffer,
    /* [in] */ unsigned int in_chr_count)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&RPC_StubInteface_StubDesc,
                  (PFORMAT_STRING) &RPC_Stub__MIDL_ProcFormatString.Format[0],
                  ( unsigned char * )&IDL_handle);
    return ( unsigned int  )_RetVal.Simple;
    
}


unsigned int RPC_MemCpy( 
    /* [in] */ handle_t IDL_handle,
    /* [size_is][out] */ unsigned char *pDest,
    /* [in] */ unsigned int dest_size,
    /* [size_is][in] */ const unsigned char *pSrc,
    /* [in] */ unsigned int src_size)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&RPC_StubInteface_StubDesc,
                  (PFORMAT_STRING) &RPC_Stub__MIDL_ProcFormatString.Format[58],
                  ( unsigned char * )&IDL_handle);
    return ( unsigned int  )_RetVal.Simple;
    
}


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif
#if !(TARGET_IS_NT60_OR_LATER)
#error You need Windows Vista or later to run this stub because it uses these features:
#error   compiled for Windows Vista.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const RPC_Stub_MIDL_PROC_FORMAT_STRING RPC_Stub__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure RPC_ConvertToUpperCase */

			0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 14 */	NdrFcShort( 0x10 ),	/* 16 */
/* 16 */	NdrFcShort( 0x8 ),	/* 8 */
/* 18 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 20 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 22 */	NdrFcShort( 0x1 ),	/* 1 */
/* 24 */	NdrFcShort( 0x1 ),	/* 1 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pOutBuffer */

/* 28 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 30 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 32 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter out_buffer_count */

/* 34 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 36 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 38 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pInBuffer */

/* 40 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 42 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 44 */	NdrFcShort( 0x16 ),	/* Type Offset=22 */

	/* Parameter in_chr_count */

/* 46 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 48 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 50 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 52 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 54 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 56 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RPC_MemCpy */

/* 58 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 60 */	NdrFcLong( 0x0 ),	/* 0 */
/* 64 */	NdrFcShort( 0x1 ),	/* 1 */
/* 66 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 68 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 70 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 72 */	NdrFcShort( 0x10 ),	/* 16 */
/* 74 */	NdrFcShort( 0x8 ),	/* 8 */
/* 76 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 78 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 80 */	NdrFcShort( 0x1 ),	/* 1 */
/* 82 */	NdrFcShort( 0x1 ),	/* 1 */
/* 84 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pDest */

/* 86 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 88 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 90 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter dest_size */

/* 92 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 94 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 96 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pSrc */

/* 98 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 100 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 102 */	NdrFcShort( 0x36 ),	/* Type Offset=54 */

	/* Parameter src_size */

/* 104 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 106 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 108 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 110 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 112 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 114 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const RPC_Stub_MIDL_TYPE_FORMAT_STRING RPC_Stub__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x0,	/* FC_RP */
/*  4 */	NdrFcShort( 0x2 ),	/* Offset= 2 (6) */
/*  6 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/*  8 */	NdrFcShort( 0x2 ),	/* 2 */
/* 10 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 12 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 14 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 16 */	0x5,		/* FC_WCHAR */
			0x5b,		/* FC_END */
/* 18 */	
			0x11, 0x0,	/* FC_RP */
/* 20 */	NdrFcShort( 0x2 ),	/* Offset= 2 (22) */
/* 22 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 24 */	NdrFcShort( 0x2 ),	/* 2 */
/* 26 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 28 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 30 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 32 */	0x5,		/* FC_WCHAR */
			0x5b,		/* FC_END */
/* 34 */	
			0x11, 0x0,	/* FC_RP */
/* 36 */	NdrFcShort( 0x2 ),	/* Offset= 2 (38) */
/* 38 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 40 */	NdrFcShort( 0x1 ),	/* 1 */
/* 42 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 44 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 46 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 48 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 50 */	
			0x11, 0x0,	/* FC_RP */
/* 52 */	NdrFcShort( 0x2 ),	/* Offset= 2 (54) */
/* 54 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 56 */	NdrFcShort( 0x1 ),	/* 1 */
/* 58 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 60 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 62 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 64 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */

			0x0
        }
    };

static const unsigned short RPC_StubInteface_FormatStringOffsetTable[] =
    {
    0,
    58
    };


#ifdef __cplusplus
namespace {
#endif
static const MIDL_STUB_DESC RPC_StubInteface_StubDesc = 
    {
    (void *)& RPC_StubInteface___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &RPC_StubInteface__MIDL_AutoBindHandle,
    0,
    0,
    0,
    0,
    RPC_Stub__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x60001, /* Ndr library version */
    0,
    0x8010274, /* MIDL Version 8.1.628 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#ifdef __cplusplus
}
#endif
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

