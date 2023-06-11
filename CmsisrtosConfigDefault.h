/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */
#ifndef CMSISRTOS_249BF9D3_824D_468E_A6BD_F449DA28F521
#define CMSISRTOS_249BF9D3_824D_468E_A6BD_F449DA28F521

/* ****************************************************************************************
 * Include
 */  
#include "mframe.h"

//-----------------------------------------------------------------------------------------
#include "./CmsisrtosConfig.h"

/* ****************************************************************************************
 * Namespace
 */  
namespace cmsisrtos{
  class CmsisrtosConfigDefault;
}


/* ****************************************************************************************
 * Class/Interface/Struct/Enum
 */  
class cmsisrtos::CmsisrtosConfigDefault : public mframe::lang::Object, public cmsisrtos::CmsisrtosConfig{

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
    uint32_t mClock;
    mframe::io::OutputStream* mOutputStream;
    mframe::io::InputStream* mInputStream;
    void (*mReboot)(void);
  
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
    CmsisrtosConfigDefault(void);
  
    /**
     *
     */
    virtual ~CmsisrtosConfigDefault(void) override;

  /* **************************************************************************************
   * Operator Method
   */

  /* **************************************************************************************
   * Public Method <Static>
   */

  /* **************************************************************************************
   * Public Method <Override> - cmsisrtos::CmsisrtosConfig
   */
  public:
    /**
     *
     */
    virtual uint32_t coreGetClock(void) override;
    
    /**
     * @brief 
     *
     * @return
     */
    virtual mframe::io::OutputStream* coreGetOutputStream(void) override;
      
    /**
     * @brief 
     *
     * @return
     */
    virtual mframe::io::InputStream* coreGetInputStream(void) override;
    
    /**
     * @brief 核心重啟
     *
     */
    virtual void coreReboot(void) override;
    
  /* **************************************************************************************
   * Public Method
   */
  public:
    /**
     *
     */
    void setClock(uint32_t clock);
  
    /**
     *
     */
    void setOutputStream(mframe::io::OutputStream* outputStream);
  
    /**
     *
     */
    void setInputStream(mframe::io::InputStream* inputStream);
  
    /**
     *
     */
    void setReboot(void (*rebootMethod)(void));
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

  /* **************************************************************************************
   * Private Method <Override>
   */

  /* **************************************************************************************
   * Private Method
   */

};

/* ****************************************************************************************
 * End of file
 */ 

#endif /* CMSISRTOS_249BF9D3_824D_468E_A6BD_F449DA28F521 */
