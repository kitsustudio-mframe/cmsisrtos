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

//-----------------------------------------------------------------------------------------
#include "./CmsisrtosKernel.h"
#include "./CmsisrtosThread.h"


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
 * Variable <Static>
 */

/* ****************************************************************************************
 * Construct Method
 */
 
/** ---------------------------------------------------------------------------------------
 *
 */
CmsisrtosKernel::CmsisrtosKernel(cmsisrtos::CmsisrtosConfig& config) : 
mConfig(config){
  this->mLockStack = 0;
  return;
}
  
/** ---------------------------------------------------------------------------------------
 *
 */
CmsisrtosKernel::~CmsisrtosKernel(void){
  return;
}
/* ****************************************************************************************
 * Operator Method
 */

/* ****************************************************************************************
 * Public Method <Static>
 */

/* ****************************************************************************************
 * Public Method <Override> - lang::Kernel
 */

/** ---------------------------------------------------------------------------------------
 *
 */
bool CmsisrtosKernel::kernelInitialize(void){
  return (osKernelInitialize() == osOK);
}

/** ---------------------------------------------------------------------------------------
 *
 */
void CmsisrtosKernel::kernelStart(lang::Runnable& runnable, uint32_t stackSize){
  lang::Thread* thread = new cmsisrtos::CmsisrtosThread(runnable, stackSize);
  if(thread->start("svchost") == false)
    System::error(this, lang::ErrorCode::SYSTEM_ERROR);
    
  osKernelStart();
}

/** ---------------------------------------------------------------------------------------
 *
 */
int CmsisrtosKernel::kernelLock(void){
  switch(osKernelGetState()){
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
int CmsisrtosKernel::kernelUnlock(void){
  switch(osKernelGetState()){
    //----------------------------------------
    case osKernelRunning:
      break;
    
    //----------------------------------------
    case osKernelLocked:
      if(this->mLockStack == 1)
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
uint32_t CmsisrtosKernel::kernelGetTickCount(void){
  return osKernelGetTickCount();
}

/** ---------------------------------------------------------------------------------------
 *
 */
uint32_t CmsisrtosKernel::kernelGetTickFreq(void){
  return osKernelGetSysTimerCount();
}

/** ---------------------------------------------------------------------------------------
 *
 */
bool CmsisrtosKernel::kernelDelay(uint32_t milliseconds) const{
  if(milliseconds <= 0)
    return false;
  
  return (osDelay(static_cast<uint32_t>(milliseconds)) == osOK);
}

/** ---------------------------------------------------------------------------------------
 *
 */
bool CmsisrtosKernel::kernelWait(uint32_t timeout) const {
  osThreadFlagsClear(0x00000001U);
  
  if(timeout){
    if(osThreadFlagsWait(0x00000001U, osFlagsWaitAny, timeout) == osFlagsErrorTimeout)
      return true;
  }else{
    osThreadFlagsWait(0x00000001U, osFlagsWaitAny, osWaitForever);
    return true;
  }

  
  return false;
}

/** ---------------------------------------------------------------------------------------
 *
 */
void CmsisrtosKernel::kernelReboot(void){
  this->mConfig.coreReboot();
}

/** ---------------------------------------------------------------------------------------
 *
 */
lang::Thread* CmsisrtosKernel::kernelAllocThread(lang::Runnable& task, lang::Data& stackMemory){
  return new cmsisrtos::CmsisrtosThread(task, stackMemory);
}

/** ---------------------------------------------------------------------------------------
 *
 */
lang::Thread* CmsisrtosKernel::kernelAllocThread(lang::Runnable& task, uint32_t stackSize){
  return new cmsisrtos::CmsisrtosThread(task, stackSize);
}

/** ---------------------------------------------------------------------------------------
 *
 */
lang::Thread* CmsisrtosKernel::kernelGetCurrentThread(void){
  uint32_t *id = Pointers::pointCast(osThreadGetId(), Class<uint32_t>::cast());
  
  if(id == nullptr)
    return nullptr;
  
  id = Pointers::pointShift(id, -8);
  lang::Thread* result = Pointers::pointCast(*id, Class<lang::Thread>::cast());
  
  if(result == nullptr)
    return nullptr;
  
  return result;
}

/** ---------------------------------------------------------------------------------------
 *
 */
lang::OutputStream* CmsisrtosKernel::kernelGetOutputStream(void){
  return this->mConfig.coreGetOutputStream();
}

/** ---------------------------------------------------------------------------------------
 *
 */
lang::InputStream* CmsisrtosKernel::kernelGetInputStream(void){
  return this->mConfig.coreGetInputStream();
}

/** ---------------------------------------------------------------------------------------
 *
 */
uint32_t CmsisrtosKernel::kernelGetCoreClock(void){
  return this->mConfig.coreGetClock();
}

/** ---------------------------------------------------------------------------------------
 *
 */
bool CmsisrtosKernel::kenrelYield(void){
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
