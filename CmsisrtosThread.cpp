/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

/* ****************************************************************************************
 * Include
 */
 
//-----------------------------------------------------------------------------------------
#include "CmsisrtosThread.h"

//-----------------------------------------------------------------------------------------

#include "rtos_rtx5/rtx/rtx_os.h"
#include "rtos_rtx5/rtx/cmsis_os2.h"

/* ****************************************************************************************
 * Macro
 */
#define CMSISRTOS_MEMORY_ALIGN 0x00000008

/* ****************************************************************************************
 * Using
 */
using cmsisrtos::CmsisrtosThread;

//-----------------------------------------------------------------------------------------
using mframe::lang::Data;
using mframe::lang::sys::ErrorCode;
using mframe::lang::func::Runnable;
using mframe::lang::sys::ThreadPriority;
using mframe::lang::System;
using mframe::lang::Pointers;
using mframe::lang::Class;

/* ****************************************************************************************
 * Variable <Static>
 */

/* ****************************************************************************************
 * Construct Method
 */

//-----------------------------------------------------------------------------------------
CmsisrtosThread::CmsisrtosThread(Runnable& task, int stackSize) : mStack(stackSize),
                                                                  mTask(task) {
  this->mThreadID = 0;
  return;
}

//-----------------------------------------------------------------------------------------
CmsisrtosThread::CmsisrtosThread(Runnable& task, Data& stackMemory) : mStack(stackMemory),
                                                                      mTask(task) {
  this->mThreadID = 0;
  return;
}

//-----------------------------------------------------------------------------------------
CmsisrtosThread::~CmsisrtosThread(void) {
  if (this->mThreadID != 0)
    System::error(this, ErrorCode::SYSTEM_ERROR);

  return;
}

/* ****************************************************************************************
 * Operator Method
 */

/* ****************************************************************************************
 * Public Method <Static>
 */

/* ****************************************************************************************
 * Public Method <Override> - mframe::lang::sys::Thread
 */

//-----------------------------------------------------------------------------------------
const char* CmsisrtosThread::getThreadName(void) const {
  osRtxThread_t* handler = reinterpret_cast<osRtxThread_t*>(this->mThreadID);
  if (handler != nullptr)
    return handler->name;

  return nullptr;
}

//-----------------------------------------------------------------------------------------
ThreadPriority CmsisrtosThread::getPriority(void) const {
  if (this->mThreadID == 0)
    return ThreadPriority::ERROR;

  osThreadId_t id = reinterpret_cast<osThreadId_t>(this->mThreadID);
  return static_cast<ThreadPriority>(osThreadGetPriority(id));
}

//-----------------------------------------------------------------------------------------
mframe::lang::sys::ThreadState CmsisrtosThread::getState(void) const {
  if (this->mThreadID == 0)
    return mframe::lang::sys::ThreadState::INACTIVE;

  osThreadId_t id = reinterpret_cast<osThreadId_t>(this->mThreadID);

  switch (osThreadGetState(id)) {
    case osThreadInactive:
      return mframe::lang::sys::ThreadState::INACTIVE;

    case osThreadReady:
      return mframe::lang::sys::ThreadState::READY;

    case osThreadRunning:
      return mframe::lang::sys::ThreadState::RUNNING;

    case osThreadBlocked:
      return mframe::lang::sys::ThreadState::BLOCKED;

    case osThreadTerminated:
      return mframe::lang::sys::ThreadState::TERMINATED;

    case osThreadError:
      return mframe::lang::sys::ThreadState::ERROR;

    case osThreadReserved:
      return mframe::lang::sys::ThreadState::ERROR;
  }

  return mframe::lang::sys::ThreadState::ERROR;
}

//-----------------------------------------------------------------------------------------
int CmsisrtosThread::getStackSize(void) const {
  osRtxThread_t* handler = reinterpret_cast<osRtxThread_t*>(this->mThreadID);

  if (handler != nullptr)
    return static_cast<int>(handler->stack_size);

  return 0;
}

//-----------------------------------------------------------------------------------------
bool CmsisrtosThread::start(const char* name, mframe::lang::sys::ThreadPriority priority) {
  osThreadAttr_t attr;

  if (this->mStack.length() < 256)
    return false;

  if (true) {
    uint32_t shift = this->mStack.getAddress() & 0x00000007;

    if (shift)
      shift -= 0x00000008;

    uint32_t* thread = this->mStack.pointer(shift, Class<uint32_t>::cast());
    Thread* thisThread = this;
    *thread = Pointers::castUnsigned(thisThread);

    shift += 8;

    attr.cb_mem = this->mStack.pointer(shift);
    attr.cb_size = sizeof(osRtxThread_t);

    if (attr.cb_size & 0x00000007)
      attr.cb_size = (attr.cb_size & 0x7FFFFFF8) + 0x00000008;

    shift += attr.cb_size;

    attr.stack_mem = this->mStack.pointer(shift);
    attr.stack_size = (this->mStack.lengthUnsigned() - shift);
    attr.stack_size &= 0x7FFFFFF8;
  }

  attr.name = name;
  attr.attr_bits = osThreadDetached;
  attr.priority = osPriorityNormal;
  attr.reserved = 0;

  osThreadId_t id = osThreadNew(CmsisrtosThread::entryPoint, this, &attr);

  if (id == nullptr)
    return false;

  this->mThreadID = reinterpret_cast<uint32_t>(id);
  return true;
}

//-----------------------------------------------------------------------------------------
void CmsisrtosThread::notify(void) {
  osThreadId_t id = reinterpret_cast<osThreadId_t>(this->mThreadID);
  osThreadFlagsSet(id, 0x00000001U);
}

//-----------------------------------------------------------------------------------------
bool CmsisrtosThread::setPriority(mframe::lang::sys::ThreadPriority priority) {
  osThreadId_t id = reinterpret_cast<osThreadId_t>(this->mThreadID);
  return (osThreadSetPriority(id, static_cast<osPriority_t>(priority)) == osOK);
}

/* ****************************************************************************************
 * Public Method
 */

//-----------------------------------------------------------------------------------------
void CmsisrtosThread::entry(void) {
  this->mTask.run();
  this->mThreadID = 0;
  return;
}

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

//-----------------------------------------------------------------------------------------
void CmsisrtosThread::entryPoint(void* attachment) {
  CmsisrtosThread* thread = static_cast<CmsisrtosThread*>(attachment);
  if (thread != nullptr)
    thread->entry();

  osThreadExit();
}

/* ****************************************************************************************
 * End of file
 */
