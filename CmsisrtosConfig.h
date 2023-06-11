/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef CMSISRTOS_C7DF9DDF_DB12_4328_BE36_E767F19D1DA0
#define CMSISRTOS_C7DF9DDF_DB12_4328_BE36_E767F19D1DA0

/* ****************************************************************************************
 * Include
 */
#include "mframe.h"

//-----------------------------------------------------------------------------------------

/* ****************************************************************************************
 * Namespace
 */
namespace cmsisrtos {
  struct CmsisrtosConfig;
}

/* ****************************************************************************************
 * Class/Interface/Struct/Enum
 */
struct cmsisrtos::CmsisrtosConfig : public virtual mframe::lang::Interface {
  /* **************************************************************************************
   * Method
   */
  
  /**
   *
   */
  virtual uint32_t coreGetClock(void) abstract;

  /**
   * @brief
   *
   * @return
   */
  virtual mframe::io::OutputStream* coreGetOutputStream(void) abstract;

  /**
   * @brief
   *
   * @return
   */
  virtual mframe::io::InputStream* coreGetInputStream(void) abstract;

  /**
   * @brief 核心重啟
   *
   */
  virtual void coreReboot(void) abstract;
};

/* ****************************************************************************************
 * End of file
 */

#endif /* CMSISRTOS_C7DF9DDF_DB12_4328_BE36_E767F19D1DA0 */
