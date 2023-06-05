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
#include "rtx/cmsis_os2.h"
#include "rtx/rtx_os.h"

//-----------------------------------------------------------------------------------------
#include "cmsisrtos/CmsisrtosThread.h"


/* ****************************************************************************************
 * Macro
 */
#define CMSISRTOS_MEMORY_ALIGN 0x00000008
/* ****************************************************************************************
 * Using
 */

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
using cmsisrtos::CmsisrtosThread;

/* ****************************************************************************************
 * Variable <Static>
 */

/* ****************************************************************************************
 * Construct Method
 */
/**
 *
 */
CmsisrtosThread::CmsisrtosThread(lang::Runnable& task, uint32_t stackSize) : 
mStack(stackSize), 
mTask(task){
  
  this->mThreadID = 0;
  return;
}

/**
 *
 */
CmsisrtosThread::CmsisrtosThread(lang::Runnable& task, lang::Data& stackMemory) : 
mStack(stackMemory), 
mTask(task){
  
  this->mThreadID = 0;
  return;
}

/**
 *
 */
CmsisrtosThread::~CmsisrtosThread(void){
  if(this->mThreadID != 0)
    System::error(this, lang::ErrorCode::SYSTEM_ERROR);
  
  return;
}

/* ****************************************************************************************
 * Operator Method
 */

/* ****************************************************************************************
 * Public Method <Static>
 */

/* ****************************************************************************************
 * Public Method <Override> - lang::Thread
 */

/**
 * @brief Get the Thread Name object
 * 
 * @return const char* 
 */
const char* CmsisrtosThread::getThreadName(void) const{
  osRtxThread_t *handler  = reinterpret_cast<osRtxThread_t*>(this->mThreadID);
  if(handler != nullptr)
    return handler->name;
  
  return nullptr;
}
  
/**
 * @brief Get the Priority object
 * 
 * @return lang::ThreadPriority 
 */
lang::ThreadPriority CmsisrtosThread::getPriority(void) const{
  if(this->mThreadID == 0)
    return lang::ThreadPriority::ERROR;

  osThreadId_t id = reinterpret_cast<osThreadId_t>(this->mThreadID);
  return static_cast<lang::ThreadPriority>(osThreadGetPriority(id));
}

/**
 * @brief Get the State object
 * 
 * @return lang::ThreadState 
 */
lang::ThreadState CmsisrtosThread::getState(void) const{
  if(this->mThreadID == 0)
    return lang::ThreadState::INACTIVE;
  
  osThreadId_t id = reinterpret_cast<osThreadId_t>(this->mThreadID);
  
  switch(osThreadGetState(id)){
    case osThreadInactive:
      return lang::ThreadState::INACTIVE;
    
    case osThreadReady:
      return lang::ThreadState::READY;
    
    case osThreadRunning:
      return lang::ThreadState::RUNNING;
    
    case osThreadBlocked:
      return lang::ThreadState::BLOCKED;
    
    case osThreadTerminated:
      return lang::ThreadState::TERMINATED;
    
    case osThreadError:
      return lang::ThreadState::ERROR;
    
    case osThreadReserved:
      return lang::ThreadState::ERROR;
  }
  
  return lang::ThreadState::ERROR;
}
  
/**
 * @brief Get the Stack Size object
 * 
 * @return uint32_t 
 */
int CmsisrtosThread::getStackSize(void) const{
  osRtxThread_t *handler  = reinterpret_cast<osRtxThread_t*>(this->mThreadID);

  if(handler != nullptr)
    return static_cast<int>(handler->stack_size);
  
  return 0;
}
  

  
/**
 * @brief 
 * 
 * @param priority 
 * @return true 
 * @return false 
 */
bool CmsisrtosThread::start(const char* name, lang::ThreadPriority priority){
  osThreadAttr_t attr;
  
  if(this->mStack.length() < 256)
    return false;
  
  if(true){
    uint32_t shift = this->mStack.getAddress() & 0x00000007;
    
    if(shift)
      shift -= 0x00000008;
    
    uint32_t *thread = this->mStack.pointer(shift, Class<uint32_t>::cast());
    Thread* thisThread = this;
    *thread = Pointers::castUnsigned(thisThread);
    
    shift += 8;
    
    attr.cb_mem = this->mStack.pointer(shift);
    attr.cb_size = sizeof(osRtxThread_t);
    
    if(attr.cb_size & 0x00000007)
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
  
  if(id == nullptr)
    return false;
  
  this->mThreadID = reinterpret_cast<uint32_t>(id);
  return true;
}
  
/**
 * @brief 
 * 
 */
void CmsisrtosThread::notify(void){
  osThreadId_t id = reinterpret_cast<osThreadId_t>(this->mThreadID);
  osThreadFlagsSet(id, 0x00000001U);
}
  
/**
 * @brief Set the Priority object
 * 
 * @param priority 
 * @return true 
 * @return false 
 */
bool CmsisrtosThread::setPriority(lang::ThreadPriority priority){
  osThreadId_t id = reinterpret_cast<osThreadId_t>(this->mThreadID);
  return (osThreadSetPriority(id, static_cast<osPriority_t>(priority)) == osOK);
}

/* ****************************************************************************************
 * Public Method
 */

/**
 *
 */
void CmsisrtosThread::entry(void){
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

/**
 * @brief 
 * 
 * @param attachment 
 */
void CmsisrtosThread::entryPoint(void* attachment){
  CmsisrtosThread* thread = static_cast<CmsisrtosThread*>(attachment);
  if(thread != nullptr)
  thread->entry();
  osThreadExit();
}

/* ****************************************************************************************
 * End of file
 */
