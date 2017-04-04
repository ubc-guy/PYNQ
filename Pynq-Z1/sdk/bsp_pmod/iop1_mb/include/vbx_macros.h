//VBXCOPYRIGHTTAG
/**
 * @file
 * @defgroup VBX_macros VBX Macros
 * @brief VBX macros
 *
 * @ingroup VBXapi
 */
/**@{*/

#ifndef __VBX_MACROS_H
#define __VBX_MACROS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vbx_extern.h"
#include "vbx_common.h"

#define VBX_REG_MXPCPU       16

#if VBX_USE_GLOBAL_MXP_PTR
#ifdef CATAPULTPS_SIMULATOR
vbx_mxp_t *VBX_GET_THIS_MXP();
void VBX_SET_THIS_MXP(vbx_mxp_t *POINTER);
#else
#define VBX_GET_THIS_MXP() (vbx_mxp_ptr)
	static inline void VBX_SET_THIS_MXP(vbx_mxp_t *POINTER){
		vbx_mxp_ptr = POINTER;
	}
#endif
#else
#define VBX_GET_THIS_MXP() \
	({ int __t__; vbx_get_reg( VBX_REG_MXPCPU, &__t__ ); (vbx_mxp_t*)__t__; })
#endif

//#define VBX_IS_VPTR(PTR)
//	({ vbx_void_t *__vptr__ = (vbx_void_t *)(PTR);
///           ( (VBX_SCRATCHPAD_ADDR<=__vptr__) && (__vptr__<VBX_SCRATCHPAD_END) );
//         })
//
//The alternative macros below only work if you can guarantee scratchpad
// start address is aligned to the scratchpad size.
//#define VPTR_MASK    (~(VECTOR_MEMORY_SIZE*1024-1))
//#define IS_VPTR(PTR) ( (((vbx_void_t *)PTR)&(VPTR_MASK))==(VBX_SCRATCHPAD_ADDR) )

#define VBX_PAD_UP(BYTES,ALIGNMENT) \
	(( ((size_t)(BYTES)) + (((size_t)(ALIGNMENT))-1)) & ~(((size_t)(ALIGNMENT))-1))

#define VBX_PAD_DN(BYTES,ALIGNMENT) \
	(((size_t)(BYTES))  &  ~(((size_t)(ALIGNMENT))-1))


#define VBX_IS_MISALIGNED(LENGTH,ALIGNMENT)	((((size_t)(LENGTH))&((size_t)(ALIGNMENT)-1))?1:0)
#define VBX_IS_ALIGNED(LENGTH,ALIGNMENT)	(!VBX_IS_MISALIGNED((LENGTH),(ALIGNMENT)))

// ---------------------------------

#define VBX_PADDING() (VBX_CPU_DCACHE_LINE_SIZE)

// ---------------------------------
#if VBX_SKIP_ALL_CHECKS == 1
#define VBX_DEBUG_FUNC1(fname,...) 	fname##_nodebug(__VA_ARGS__)
#define VBX_DEBUG_FUNC0(fname)	   fname##_nodebug()
#else
#define VBX_DEBUG_FUNC1(fname,...) 	fname##_debug(__LINE__,__FILE__,__VA_ARGS__)
#define VBX_DEBUG_FUNC0(fname)		fname##_debug(__LINE__,__FILE__)
#endif

/** Malloc in scratchpad.
 *
 * @param[in] amount -- number of bytes to allocate
 */
#define vbx_sp_malloc(amount)      ( VBX_DEBUG_FUNC1(vbx_sp_malloc,amount) )

/** Free entire scratchpad.
 *
 * Use @ref vbx_sp_push and @ref vbx_sp_pop for partial allocating/free
 */
#define vbx_sp_free()              do{ VBX_DEBUG_FUNC0( vbx_sp_free );   }while(0)


// ---------------------------------
#include <stdio.h>
#define VBX_PRINTF(...)	  \
	do{ \
		if( VBX_DEBUG_LEVEL ) { \
			printf( __VA_ARGS__ ); \
		} \
	}while(0)

	void VBX_FATAL(int , const char* , int);
#define VBX_EXIT(ERR)  \
	VBX_FATAL(__LINE__,__FILE__,ERR)

#define debug(var) printf("%s:%d  %s = %d \r\n",__FILE__,__LINE__,#var,(signed)(size_t)(var))
#define debugx(var) printf("%s:%d  %s = %08X \r\n",__FILE__,__LINE__,#var,(unsigned)(size_t)(var))
#define debugfxp(var,bits) printf("%s:%d  %s = %f \r\n",__FILE__,__LINE__,#var,(double)(var)/(1<<bits))
#define debugfxpw(var) debugfxp(var,VBX_GET_THIS_MXP()->fxp_word_frac_bits)
#ifdef __cplusplus
}
#endif

#endif //__VBX_MACROS_H
/**@}*/
