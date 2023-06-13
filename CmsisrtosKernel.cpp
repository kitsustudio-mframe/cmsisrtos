/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

/* ****************************************************************************************
 * Include
 */
#include "./CmsisrtosKernel.h"

//-----------------------------------------------------------------------------------------
#define USING_CMSIS_COMPILER
#include "arm.h"
#undef USING_CMSIS_COMPILER

#define USING_CMSISRTOS_RTX_OS
#define USING_LEGACY
#include "rtos.h"
#undef USING_LEGACY
#undef USING_CMSISRTOS_RTX_OS

#include "./CmsisrtosThread.h"
#include "./rtx/cmsis_os2.h"


/* ****************************************************************************************
 * Macro
 */

/* ****************************************************************************************
 * Using
 */

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
using cmsisrtos::CmsisrtosKernel;


/* ****************************************************************************************
 * Legacy
 */
 
extern "C"{
  __WEAK __NO_RETURN void osRtxIdleThread (void *argument) {
    (void)argument;

    for (;;) {}
  }
   
  // OS Error Callback function
  __WEAK uint32_t osRtxErrorNotify (uint32_t code, void *object_id) {
    (void)object_id;

    switch (code) {
      case osRtxErrorStackOverflow:
        // Stack overflow detected for thread (thread_id=object_id)
        break;
      case osRtxErrorISRQueueOverflow:
        // ISR Queue overflow detected when inserting object (object_id)
        break;
      case osRtxErrorTimerQueueOverflow:
        // User Timer Callback Queue overflow detected for timer (timer_id=object_id)
        break;
      case osRtxErrorClibSpace:
        // Standard C/C++ library libspace not available: increase OS_THREAD_LIBSPACE_NUM
        break;
      case osRtxErrorClibMutex:
        // Standard C/C++ library mutex initialization failed.
      
      default:
        // Reserved
        break;
    }
    for (;;) {}
  //return 0U;
  }
}


/* ****************************************************************************************
 * Variable <Static>
 */

/* ****************************************************************************************
 * Construct Method
 */

/** ---------------------------------------------------------------------------------------
 *
 */
CmsisrtosKernel::CmsisrtosKernel(void (*reboot)(void)) {
  this->mReboot = reboot;
  this->mLockStack = 0;
  return;
}

/** ---------------------------------------------------------------------------------------
 *
 */
CmsisrtosKernel::~CmsisrtosKernel(void) {
  return;
}
/* ****************************************************************************************
 * Operator Method
 */

/* ****************************************************************************************
 * Public Method <Static>
 */

/* ****************************************************************************************
 * Public Method <Override> - mframe::lang::Kernel
 */

/** ---------------------------------------------------------------------------------------
 *
 */
bool CmsisrtosKernel::kernelInitialize(void) {
  return (osKernelInitialize() == osOK);
}

/** ---------------------------------------------------------------------------------------
 *
 */
void CmsisrtosKernel::kernelStart(void) {
  osKernelStart();
}

/** ---------------------------------------------------------------------------------------
 *
 */
int CmsisrtosKernel::kernelLock(void) {
  switch (osKernelGetState()) {
    //----------------------------------------
    case osKernelRunning:
      osKernelLock();
      this->mLockStack = 1;
      break;

    //----------------------------------------
    case osKernelLocked:
      ++this->mLockStack;
      break;

    //----------------------------------------
    case osKernelInactive:
      break;

    //----------------------------------------
    case osKernelReady:
      break;

    //----------------------------------------
    case osKernelSuspended:
      break;

    //----------------------------------------
    case osKernelError:
      break;

    //----------------------------------------
    case osKernelReserved:
      break;
  }

  return this->mLockStack;
}

/** ---------------------------------------------------------------------------------------
 *
 */
int CmsisrtosKernel::kernelUnlock(void) {
  switch (osKernelGetState()) {
    //----------------------------------------
    case osKernelRunning:
      break;

    //----------------------------------------
    case osKernelLocked:
      if (this->mLockStack == 1)
        osKernelUnlock();
      --this->mLockStack;
      break;

    //----------------------------------------
    case osKernelInactive:
      break;

    //----------------------------------------
    case osKernelReady:
      break;

    //----------------------------------------
    case osKernelSuspended:
      break;

    //----------------------------------------
    case osKernelError:
      break;

    //----------------------------------------
    case osKernelReserved:
      break;
  }

  return this->mLockStack;
}

/** ---------------------------------------------------------------------------------------
 *
 */
uint32_t CmsisrtosKernel::kernelGetTickCount(void) {
  return osKernelGetTickCount();
}

/** ---------------------------------------------------------------------------------------
 *
 */
uint32_t CmsisrtosKernel::kernelGetTickFreq(void) {
  return osKernelGetSysTimerCount();
}

/** ---------------------------------------------------------------------------------------
 *
 */
bool CmsisrtosKernel::kernelDelay(uint32_t milliseconds) const {
  if (milliseconds <= 0)
    return false;

  return (osDelay(static_cast<uint32_t>(milliseconds)) == osOK);
}

/** ---------------------------------------------------------------------------------------
 *
 */
bool CmsisrtosKernel::kernelWait(uint32_t timeout) const {
  osThreadFlagsClear(0x00000001U);

  if (timeout) {
    if (osThreadFlagsWait(0x00000001U, osFlagsWaitAny, timeout) == osFlagsErrorTimeout)
      return true;
  } else {
    osThreadFlagsWait(0x00000001U, osFlagsWaitAny, osWaitForever);
    return true;
  }

  return false;
}

/** ---------------------------------------------------------------------------------------
 *
 */
void CmsisrtosKernel::kernelReboot(void) {
  this->mReboot();
}

/** ---------------------------------------------------------------------------------------
 *
 */
mframe::lang::Thread* CmsisrtosKernel::kernelAllocThread(mframe::lang::Runnable& task, mframe::lang::Data& stackMemory) {
  return new cmsisrtos::CmsisrtosThread(task, stackMemory);
}

/** ---------------------------------------------------------------------------------------
 *
 */
mframe::lang::Thread* CmsisrtosKernel::kernelAllocThread(mframe::lang::Runnable& task, int stackSize) {
  return new cmsisrtos::CmsisrtosThread(task, stackSize);
}

/** ---------------------------------------------------------------------------------------
 *
 */
mframe::lang::Thread* CmsisrtosKernel::kernelGetCurrentThread(void) {
  uint32_t* id = Pointers::pointCast(osThreadGetId(), Class<uint32_t>::cast());

  if (id == nullptr)
    return nullptr;

  id = Pointers::pointShift(id, -8);
  mframe::lang::Thread* result = Pointers::pointCast(*id, Class<mframe::lang::Thread>::cast());

  if (result == nullptr)
    return nullptr;

  return result;
}

/** ---------------------------------------------------------------------------------------
 *
 */
bool CmsisrtosKernel::kenrelYield(void) {
  return (osThreadYield() == osOK);
}

/* ****************************************************************************************
 * Public Method
 */

/* ****************************************************************************************
 * Protected Method <Static>
 */

/* ****************************************************************************************
 * Protected Method <Override>
 */

/* ****************************************************************************************
 * Protected Method
 */

/* ****************************************************************************************
 * Private Method
 */

/* ****************************************************************************************
 * End of file
 */
