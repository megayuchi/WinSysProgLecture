

/* this ALWAYS GENERATED file contains the RPC server stubs */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for RPC_Stub.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=ARM64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_ARM64)


#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/

#include <string.h>
#include "RPC_Stub_h.h"

#define TYPE_FORMAT_STRING_SIZE   67                                
#define PROC_FORMAT_STRING_SIZE   133                               
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

static const RPC_SYNTAX_IDENTIFIER  _NDR64_RpcTransferSyntax_1_0 = 
{{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}};

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

/* Standard interface: RPC_StubInteface, ver. 1.0,
   GUID={0xf56cd7ef,0x28ad,0x47d6,{0x82,0x1b,0xf0,0xe9,0x99,0x59,0x38,0xcd}} */


extern const MIDL_SERVER_INFO RPC_StubInteface_ServerInfo;

extern const RPC_DISPATCH_TABLE RPC_StubInteface_v1_0_DispatchTable;

static const RPC_SERVER_INTERFACE RPC_StubInteface___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0xf56cd7ef,0x28ad,0x47d6,{0x82,0x1b,0xf0,0xe9,0x99,0x59,0x38,0xcd}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    (RPC_DISPATCH_TABLE*)&RPC_StubInteface_v1_0_DispatchTable,
    0,
    0,
    0,
    &RPC_StubInteface_ServerInfo,
    0x06000000
    };
RPC_IF_HANDLE RPC_StubInteface_v1_0_s_ifspec = (RPC_IF_HANDLE)& RPC_StubInteface___RpcServerInterface;
#ifdef __cplusplus
namespace {
#endif

extern const MIDL_STUB_DESC RPC_StubInteface_StubDesc;
#ifdef __cplusplus
}
#endif


#if !defined(__RPC_ARM64__)
#error  Invalid build platform for this stub.
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
/*  8 */	NdrFcShort( 0x30 ),	/* ARM64 Stack size/offset = 48 */
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x0 ),	/* ARM64 Stack size/offset = 0 */
/* 14 */	NdrFcShort( 0x10 ),	/* 16 */
/* 16 */	NdrFcShort( 0x8 ),	/* 8 */
/* 18 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 20 */	0x10,		/* 16 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 22 */	NdrFcShort( 0x1 ),	/* 1 */
/* 24 */	NdrFcShort( 0x1 ),	/* 1 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */
/* 28 */	NdrFcShort( 0x5 ),	/* 5 */
/* 30 */	0x5,		/* 5 */
			0x80,		/* 128 */
/* 32 */	0x81,		/* 129 */
			0x82,		/* 130 */
/* 34 */	0x83,		/* 131 */
			0x84,		/* 132 */

	/* Parameter pOutBuffer */

/* 36 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 38 */	NdrFcShort( 0x8 ),	/* ARM64 Stack size/offset = 8 */
/* 40 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter out_buffer_count */

/* 42 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 44 */	NdrFcShort( 0x10 ),	/* ARM64 Stack size/offset = 16 */
/* 46 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pInBuffer */

/* 48 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 50 */	NdrFcShort( 0x18 ),	/* ARM64 Stack size/offset = 24 */
/* 52 */	NdrFcShort( 0x16 ),	/* Type Offset=22 */

	/* Parameter in_chr_count */

/* 54 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 56 */	NdrFcShort( 0x20 ),	/* ARM64 Stack size/offset = 32 */
/* 58 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 60 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 62 */	NdrFcShort( 0x28 ),	/* ARM64 Stack size/offset = 40 */
/* 64 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RPC_MemCpy */

/* 66 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 68 */	NdrFcLong( 0x0 ),	/* 0 */
/* 72 */	NdrFcShort( 0x1 ),	/* 1 */
/* 74 */	NdrFcShort( 0x30 ),	/* ARM64 Stack size/offset = 48 */
/* 76 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 78 */	NdrFcShort( 0x0 ),	/* ARM64 Stack size/offset = 0 */
/* 80 */	NdrFcShort( 0x10 ),	/* 16 */
/* 82 */	NdrFcShort( 0x8 ),	/* 8 */
/* 84 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 86 */	0x10,		/* 16 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 88 */	NdrFcShort( 0x1 ),	/* 1 */
/* 90 */	NdrFcShort( 0x1 ),	/* 1 */
/* 92 */	NdrFcShort( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x5 ),	/* 5 */
/* 96 */	0x5,		/* 5 */
			0x80,		/* 128 */
/* 98 */	0x81,		/* 129 */
			0x82,		/* 130 */
/* 100 */	0x83,		/* 131 */
			0x84,		/* 132 */

	/* Parameter pDest */

/* 102 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 104 */	NdrFcShort( 0x8 ),	/* ARM64 Stack size/offset = 8 */
/* 106 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter dest_size */

/* 108 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 110 */	NdrFcShort( 0x10 ),	/* ARM64 Stack size/offset = 16 */
/* 112 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pSrc */

/* 114 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 116 */	NdrFcShort( 0x18 ),	/* ARM64 Stack size/offset = 24 */
/* 118 */	NdrFcShort( 0x36 ),	/* Type Offset=54 */

	/* Parameter src_size */

/* 120 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 122 */	NdrFcShort( 0x20 ),	/* ARM64 Stack size/offset = 32 */
/* 124 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 126 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 128 */	NdrFcShort( 0x28 ),	/* ARM64 Stack size/offset = 40 */
/* 130 */	0x8,		/* FC_LONG */
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
/* 12 */	NdrFcShort( 0x10 ),	/* ARM64 Stack size/offset = 16 */
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
/* 28 */	NdrFcShort( 0x20 ),	/* ARM64 Stack size/offset = 32 */
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
/* 44 */	NdrFcShort( 0x10 ),	/* ARM64 Stack size/offset = 16 */
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
/* 60 */	NdrFcShort( 0x20 ),	/* ARM64 Stack size/offset = 32 */
/* 62 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 64 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */

			0x0
        }
    };

static const unsigned short RPC_StubInteface_FormatStringOffsetTable[] =
    {
    0,
    66
    };


static const RPC_DISPATCH_FUNCTION RPC_StubInteface_table[] =
    {
    NdrServerCall2,
    NdrServerCall2,
    0
    };
static const RPC_DISPATCH_TABLE RPC_StubInteface_v1_0_DispatchTable = 
    {
    2,
    (RPC_DISPATCH_FUNCTION*)RPC_StubInteface_table
    };


#endif /* defined(_M_ARM64) */



/* this ALWAYS GENERATED file contains the RPC server stubs */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for RPC_Stub.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=ARM64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_ARM64)




#if !defined(__RPC_ARM64__)
#error  Invalid build platform for this stub.
#endif


#include "ndr64types.h"
#include "pshpack8.h"
#ifdef __cplusplus
namespace {
#endif


typedef 
NDR64_FORMAT_CHAR
__midl_frag25_t;
extern const __midl_frag25_t __midl_frag25;

typedef 
NDR64_FORMAT_CHAR
__midl_frag23_t;
extern const __midl_frag23_t __midl_frag23;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_VAR frag2;
}
__midl_frag22_t;
extern const __midl_frag22_t __midl_frag22;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct _NDR64_ARRAY_ELEMENT_INFO frag2;
}
__midl_frag21_t;
extern const __midl_frag21_t __midl_frag21;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag20_t;
extern const __midl_frag20_t __midl_frag20;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_VAR frag2;
}
__midl_frag17_t;
extern const __midl_frag17_t __midl_frag17;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct _NDR64_ARRAY_ELEMENT_INFO frag2;
}
__midl_frag16_t;
extern const __midl_frag16_t __midl_frag16;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag15_t;
extern const __midl_frag15_t __midl_frag15;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct /* ARM Parameter Layout */
    {
        NDR64_UINT16 frag1;
        NDR64_UINT8 frag2;
        NDR64_UINT8 frag3[ 5 ];
    } frag8;
}
__midl_frag14_t;
extern const __midl_frag14_t __midl_frag14;

typedef 
NDR64_FORMAT_CHAR
__midl_frag11_t;
extern const __midl_frag11_t __midl_frag11;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct _NDR64_ARRAY_ELEMENT_INFO frag2;
}
__midl_frag9_t;
extern const __midl_frag9_t __midl_frag9;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag8_t;
extern const __midl_frag8_t __midl_frag8;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct _NDR64_ARRAY_ELEMENT_INFO frag2;
}
__midl_frag4_t;
extern const __midl_frag4_t __midl_frag4;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag3_t;
extern const __midl_frag3_t __midl_frag3;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct /* ARM Parameter Layout */
    {
        NDR64_UINT16 frag1;
        NDR64_UINT8 frag2;
        NDR64_UINT8 frag3[ 5 ];
    } frag8;
}
__midl_frag2_t;
extern const __midl_frag2_t __midl_frag2;

typedef 
NDR64_FORMAT_UINT32
__midl_frag1_t;
extern const __midl_frag1_t __midl_frag1;

static const __midl_frag25_t __midl_frag25 =
0x5    /* FC64_INT32 */;

static const __midl_frag23_t __midl_frag23 =
0x10    /* FC64_CHAR */;

static const __midl_frag22_t __midl_frag22 =
{ 
/*  */
    (NDR64_UINT32) 0 /* 0x0 */,
    { 
    /* struct _NDR64_EXPR_VAR */
        0x3,    /* FC_EXPR_VAR */
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 32 /* 0x20 */  /* Offset */
    }
};

static const __midl_frag21_t __midl_frag21 =
{ 
/* *char */
    { 
    /* *char */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 0 /* 0x0 */,
        { 
        /* *char */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 1 /* 0x1 */,
        &__midl_frag22
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 1 /* 0x1 */,
        &__midl_frag23
    }
};

static const __midl_frag20_t __midl_frag20 =
{ 
/* *char */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag21
};

static const __midl_frag17_t __midl_frag17 =
{ 
/*  */
    (NDR64_UINT32) 0 /* 0x0 */,
    { 
    /* struct _NDR64_EXPR_VAR */
        0x3,    /* FC_EXPR_VAR */
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 16 /* 0x10 */  /* Offset */
    }
};

static const __midl_frag16_t __midl_frag16 =
{ 
/* *char */
    { 
    /* *char */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 0 /* 0x0 */,
        { 
        /* *char */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 1 /* 0x1 */,
        &__midl_frag17
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 1 /* 0x1 */,
        &__midl_frag23
    }
};

static const __midl_frag15_t __midl_frag15 =
{ 
/* *char */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag16
};

static const __midl_frag14_t __midl_frag14 =
{ 
/* RPC_MemCpy */
    { 
    /* RPC_MemCpy */      /* procedure RPC_MemCpy */
        (NDR64_UINT32) 91095104 /* 0x56e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ServerCorrelation, ClientCorrelation, HasExtensions, HasArmParamLayout */
        (NDR64_UINT32) 48 /* 0x30 */ ,  /* Stack size */
        (NDR64_UINT32) 16 /* 0x10 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 5 /* 0x5 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x72,    /* FC64_BIND_PRIMITIVE */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 0 /* 0x0 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* pDest */      /* parameter pDest */
        &__midl_frag16,
        { 
        /* pDest */
            1,
            1,
            0,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [out], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* dest_size */      /* parameter dest_size */
        &__midl_frag25,
        { 
        /* dest_size */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* pSrc */      /* parameter pSrc */
        &__midl_frag21,
        { 
        /* pSrc */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* src_size */      /* parameter src_size */
        &__midl_frag25,
        { 
        /* src_size */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* int */      /* parameter int */
        &__midl_frag25,
        { 
        /* int */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    },
    { 
    /* RPC_MemCpy */      /* ARM register placement data */
        (NDR64_UINT16) 5 /* 0x5 */ ,  /* Number of Entries */
        (NDR64_UINT8) 5 /* 0x5 */ ,  /* Slots Used */
        { 
        /* RPC_MemCpy */      /* Placement data octets */
            (NDR64_UINT8) 0x80, 
            (NDR64_UINT8) 0x81, 
            (NDR64_UINT8) 0x82, 
            (NDR64_UINT8) 0x83, 
            (NDR64_UINT8) 0x84
        }
    }
};

static const __midl_frag11_t __midl_frag11 =
0x11    /* FC64_WCHAR */;

static const __midl_frag9_t __midl_frag9 =
{ 
/* *wchar_t */
    { 
    /* *wchar_t */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 1 /* 0x1 */,
        { 
        /* *wchar_t */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 2 /* 0x2 */,
        &__midl_frag22
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 2 /* 0x2 */,
        &__midl_frag11
    }
};

static const __midl_frag8_t __midl_frag8 =
{ 
/* *wchar_t */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag9
};

static const __midl_frag4_t __midl_frag4 =
{ 
/* *wchar_t */
    { 
    /* *wchar_t */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 1 /* 0x1 */,
        { 
        /* *wchar_t */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 2 /* 0x2 */,
        &__midl_frag17
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 2 /* 0x2 */,
        &__midl_frag11
    }
};

static const __midl_frag3_t __midl_frag3 =
{ 
/* *wchar_t */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag4
};

static const __midl_frag2_t __midl_frag2 =
{ 
/* RPC_ConvertToUpperCase */
    { 
    /* RPC_ConvertToUpperCase */      /* procedure RPC_ConvertToUpperCase */
        (NDR64_UINT32) 91095104 /* 0x56e0040 */,    /* explicit handle */ /* IsIntrepreted, ServerMustSize, ClientMustSize, HasReturn, ServerCorrelation, ClientCorrelation, HasExtensions, HasArmParamLayout */
        (NDR64_UINT32) 48 /* 0x30 */ ,  /* Stack size */
        (NDR64_UINT32) 16 /* 0x10 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 5 /* 0x5 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x72,    /* FC64_BIND_PRIMITIVE */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 0 /* 0x0 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* pOutBuffer */      /* parameter pOutBuffer */
        &__midl_frag4,
        { 
        /* pOutBuffer */
            1,
            1,
            0,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [out], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* out_buffer_count */      /* parameter out_buffer_count */
        &__midl_frag25,
        { 
        /* out_buffer_count */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* pInBuffer */      /* parameter pInBuffer */
        &__midl_frag9,
        { 
        /* pInBuffer */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* in_chr_count */      /* parameter in_chr_count */
        &__midl_frag25,
        { 
        /* in_chr_count */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* int */      /* parameter int */
        &__midl_frag25,
        { 
        /* int */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    },
    { 
    /* RPC_ConvertToUpperCase */      /* ARM register placement data */
        (NDR64_UINT16) 5 /* 0x5 */ ,  /* Number of Entries */
        (NDR64_UINT8) 5 /* 0x5 */ ,  /* Slots Used */
        { 
        /* RPC_ConvertToUpperCase */      /* Placement data octets */
            (NDR64_UINT8) 0x80, 
            (NDR64_UINT8) 0x81, 
            (NDR64_UINT8) 0x82, 
            (NDR64_UINT8) 0x83, 
            (NDR64_UINT8) 0x84
        }
    }
};

static const __midl_frag1_t __midl_frag1 =
(NDR64_UINT32) 0 /* 0x0 */;
#ifdef __cplusplus
}
#endif


#include "poppack.h"


static const FormatInfoRef RPC_StubInteface_Ndr64ProcTable[] =
    {
    &__midl_frag2,
    &__midl_frag14
    };


#ifdef __cplusplus
namespace {
#endif
static const MIDL_STUB_DESC RPC_StubInteface_StubDesc = 
    {
    (void *)& RPC_StubInteface___RpcServerInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    0,
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
    0x2000001, /* MIDL flag */
    0, /* cs routines */
    (void *)& RPC_StubInteface_ServerInfo,   /* proxy/server info */
    0
    };
#ifdef __cplusplus
}
#endif

static const RPC_DISPATCH_FUNCTION RPC_StubInteface_NDR64__table[] =
    {
    NdrServerCallAll,
    NdrServerCallAll,
    0
    };
static const RPC_DISPATCH_TABLE RPC_StubInteface_NDR64__v1_0_DispatchTable = 
    {
    2,
    (RPC_DISPATCH_FUNCTION*)RPC_StubInteface_NDR64__table
    };

static const MIDL_SYNTAX_INFO RPC_StubInteface_SyntaxInfo [  2 ] = 
    {
    {
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    (RPC_DISPATCH_TABLE*)&RPC_StubInteface_v1_0_DispatchTable,
    RPC_Stub__MIDL_ProcFormatString.Format,
    RPC_StubInteface_FormatStringOffsetTable,
    RPC_Stub__MIDL_TypeFormatString.Format,
    0,
    0,
    0
    }
    ,{
    {{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}},
    (RPC_DISPATCH_TABLE*)&RPC_StubInteface_NDR64__v1_0_DispatchTable,
    0 ,
    (unsigned short *) RPC_StubInteface_Ndr64ProcTable,
    0,
    0,
    0,
    0
    }
    };


static const SERVER_ROUTINE RPC_StubInteface_ServerRoutineTable[] = 
    {
    (SERVER_ROUTINE)RPC_ConvertToUpperCase,
    (SERVER_ROUTINE)RPC_MemCpy
    };

static const MIDL_SERVER_INFO RPC_StubInteface_ServerInfo = 
    {
    &RPC_StubInteface_StubDesc,
    RPC_StubInteface_ServerRoutineTable,
    RPC_Stub__MIDL_ProcFormatString.Format,
    (unsigned short *) RPC_StubInteface_FormatStringOffsetTable,
    0,
    (RPC_SYNTAX_IDENTIFIER*)&_NDR64_RpcTransferSyntax_1_0,
    2,
    (MIDL_SYNTAX_INFO*)RPC_StubInteface_SyntaxInfo
    };
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_ARM64) */

