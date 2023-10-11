/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */
#ifdef using_rtos_rtx5_rtx
#define using_rtos_rtx5_rtx_os
#define using_rtos_rtx5_rtx_lib
#define using_rtos_rtx5_rtx_evr
#define using_rtos_rtx5_rtx_def
#define using_rtos_rtx5_cmsis_os2
#endif

/* ****************************************************************************************
 * Include folder
 */

/* ****************************************************************************************
 * Include
 */
#ifdef using_rtos_rtx5_rtx_os
#undef using_rtos_rtx5_rtx_os
#include "./rtx_os.h"
#endif

#ifdef using_rtos_rtx5_rtx_lib
#undef using_rtos_rtx5_rtx_lib
#include "./rtx_lib.h"
#endif

#ifdef using_rtos_rtx5_rtx_evr
#undef using_rtos_rtx5_rtx_evr
#include "./rtx_evr.h"
#endif

#ifdef using_rtos_rtx5_rtx_def
#undef using_rtos_rtx5_rtx_evr
#include "./rtx_def.h"
#endif

#ifdef using_rtos_rtx5_cmsis_os2
#undef using_rtos_rtx5_cmsis_os2
#include "./cmsis_os2.h"
#endif

/* ****************************************************************************************
 * End of file
 */
