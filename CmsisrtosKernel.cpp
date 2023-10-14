/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

/* **************************************************************************************
 * Include
 */

//---------------------------------------------------------------------------------------
#define USING_CMSIS_COMPILER
#include "arm.h"
#include "CmsisrtosKernel.h"

//---------------------------------------------------------------------------------------
#include "rtos_rtx5/CmsisrtosThread.h"
#include "rtos_rtx5/rtx/rtx_os.h"
#include "rtos_rtx5/rtx/cmsis_os2.h"

/* **************************************************************************************
 * Using
 */
using cmsisrtos::CmsisrtosKernel;
using mframe::lang::Pointers;
using mframe::lang::Class;

//---------------------------------------------------------------------------------------

/* **************************************************************************************
 * Legacy
 */


extern "C" __WEAK __NO_RETURN void osRtxIdleThread(void* argument) {
  (void)argument;

  for (;;) {
  }
}

// OS Error Callback function
extern "C" __WEAK uint32_t osRtxErrorNotify(uint32_t code, void* object_id) {
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
  for (;;) {
  }
  // return 0U;
}

/* **************************************************************************************
 * Construct Method
 */

//---------------------------------------------------------------------------------------
CmsisrtosKernel::CmsisrtosKernel(void) {
  this->mReboot = nullptr;
  this->mLockStack = 0;
  return;
}

//---------------------------------------------------------------------------------------
CmsisrtosKernel::~CmsisrtosKernel(void) {
  return;
}

/* **************************************************************************************
 * Operator Method
 */

/* **************************************************************************************
 * Public Method <Override> - mframe::lang::Kernel
 */

//---------------------------------------------------------------------------------------
bool CmsisrtosKernel::initialize(void) {
  return (osKernelInitialize() == osOK);
}

//---------------------------------------------------------------------------------------
void CmsisrtosKernel::start(void) {
  osKernelStart();
}

//---------------------------------------------------------------------------------------
int CmsisrtosKernel::systemLock(void) {
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

//---------------------------------------------------------------------------------------
int CmsisrtosKernel::systemUnlock(void) {
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

//---------------------------------------------------------------------------------------
uint32_t CmsisrtosKernel::getTickCount(void) {
  return osKernelGetTickCount();
}

//---------------------------------------------------------------------------------------
uint32_t CmsisrtosKernel::getTickFreq(void) {
  return osKernelGetSysTimerCount();
}

//---------------------------------------------------------------------------------------
bool CmsisrtosKernel::systemDelay(uint32_t milliseconds) const {
  if (milliseconds <= 0)
    return false;

  return (osDelay(static_cast<uint32_t>(milliseconds)) == osOK);
}

//---------------------------------------------------------------------------------------
bool CmsisrtosKernel::systemWait(uint32_t timeout) const {
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

//---------------------------------------------------------------------------------------
void CmsisrtosKernel::reboot(void) {
  this->mReboot();
}

//---------------------------------------------------------------------------------------
mframe::sys::Thread* CmsisrtosKernel::allocThread(mframe::func::Runnable& task, mframe::lang::Data& stackMemory) {
  return new cmsisrtos::CmsisrtosThread(task, stackMemory);
}

//---------------------------------------------------------------------------------------
mframe::sys::Thread* CmsisrtosKernel::allocThread(mframe::func::Runnable& task, int stackSize) {
  return new cmsisrtos::CmsisrtosThread(task, stackSize);
}

//---------------------------------------------------------------------------------------
mframe::sys::Thread* CmsisrtosKernel::getCurrentThread(void) {
  uint32_t* id = Pointers::pointCast(osThreadGetId(), Class<uint32_t>::cast());

  if (id == nullptr)
    return nullptr;

  id = Pointers::pointShift(id, -8);
  mframe::sys::Thread* result = Pointers::pointCast(*id, Class<mframe::sys::Thread>::cast());

  if (result == nullptr)
    return nullptr;

  return result;
}

//---------------------------------------------------------------------------------------
mframe::sys::Timer* CmsisrtosKernel::allocTimer(void) {
  return nullptr;
}

//---------------------------------------------------------------------------------------
bool CmsisrtosKernel::systemYield(void) {
  return (osThreadYield() == osOK);
}

/* **************************************************************************************
 * Public Method
 */

/* **************************************************************************************
 * Protected Method
 */

/* **************************************************************************************
 * Private Method
 */

/* **************************************************************************************
 * Static Variable
 */

/* **************************************************************************************
 * Static Method
 */

/* **************************************************************************************
 * End of file
 */
