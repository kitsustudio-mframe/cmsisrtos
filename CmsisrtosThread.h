/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef CMSISRTOS_1B3FB229_533C_4331_88C3_A1D4D0A698D8
#define CMSISRTOS_1B3FB229_533C_4331_88C3_A1D4D0A698D8

/* ****************************************************************************************
 * Include
 */
#include "mframe.h"

//-----------------------------------------------------------------------------------------

/* ****************************************************************************************
 * Namespace
 */
namespace cmsisrtos {
  class CmsisrtosThread;
}

/* ****************************************************************************************
 * Class/Interface/Struct/Enum
 */
class cmsisrtos::CmsisrtosThread : public mframe::lang::Thread {
  /* **************************************************************************************
   * Variable <Public>
   */

  /* **************************************************************************************
   * Variable <Protected>
   */

  /* **************************************************************************************
   * Variable <Private>
   */
 private:
  mframe::lang::Memory mStack;
  uint32_t mThreadID;
  mframe::lang::Runnable& mTask;

  /* **************************************************************************************
   * Abstract method <Public>
   */

  /* **************************************************************************************
   * Abstract method <Protected>
   */

  /* **************************************************************************************
   * Construct Method
   */
 public:
  /**
   *
   */
  CmsisrtosThread(mframe::lang::Runnable& task, int stackSize);

  /**
   *
   */
  CmsisrtosThread(mframe::lang::Runnable& task, mframe::lang::Data& stackMemory);

  /**
   *
   */
  virtual ~CmsisrtosThread(void) override;

  /* **************************************************************************************
   * Operator Method
   */

  /* **************************************************************************************
   * Public Method <Static>
   */

  /* **************************************************************************************
   * Public Method <Override> - mframe::lang::Thread
   */
 public:
  /**
   * @brief Get the Thread Name object
   *
   * @return const char*
   */
  virtual const char* getThreadName(void) const override;

  /**
   * @brief Get the Priority object
   *
   * @return mframe::lang::ThreadPriority
   */
  virtual mframe::lang::ThreadPriority getPriority(void) const override;

  /**
   * @brief Get the State object
   *
   * @return mframe::lang::ThreadState
   */
  virtual mframe::lang::ThreadState getState(void) const override;

  /**
   * @brief Get the Stack Size object
   *
   * @return uint32_t
   */
  virtual int getStackSize(void) const override;

  /**
   * @brief
   *
   * @param priority
   * @return true
   * @return false
   */
  virtual bool start(const char* name, mframe::lang::ThreadPriority priority) override;

  /**
   * @brief
   *
   */
  virtual void notify(void) override;

  /**
   * @brief Set the Priority object
   *
   * @param priority
   * @return true
   * @return false
   */
  virtual bool setPriority(mframe::lang::ThreadPriority priority) override;

  /* **************************************************************************************
   * Public Method
   */

  /* **************************************************************************************
   * Protected Method <Static>
   */

  /* **************************************************************************************
   * Protected Method <Override>
   */

  /* **************************************************************************************
   * Protected Method
   */

  /* **************************************************************************************
   * Private Method <Static>
   */
 private:
  /**
   *
   */
  static void entryPoint(void* attachment);
  /* **************************************************************************************
   * Private Method <Override>
   */

  /* **************************************************************************************
   * Private Method
   */
 private:
  /**
   *
   */
  void entry(void);
};

/* ****************************************************************************************
 * End of file
 */

#endif /* CMSISRTOS_1B3FB229_533C_4331_88C3_A1D4D0A698D8 */
