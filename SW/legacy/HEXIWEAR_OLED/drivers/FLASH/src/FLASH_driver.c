/**
 * FLASH driver functions
 * Project HEXIWEAR, 2015
 */

#include "FLASH_Driver.h"
#include "FLASH_Defs.h"
#include "generic_spi_driver.h"
#include "generic_spi_types.h"
#include "generic_spi_info.h"
#include "error.h"
#include "fsl_gpio_driver.h"
#include "string.h"
#include "FLASH_Info.h"

/**
 * intern variables
 */

static handleFLASH_t
  self;
static settingsFLASH_t
  settings;
//static modeFLASH_t
//  currentMode;

static uint8_t
  currentStatus;

/**
 * macros
 */

#define FLASH_IsWriteInProgress()    ( 0 != ( ( currentStatus & FLASH_BIT_WIP_MASK ) >> FLASH_BIT_WIP_SHIFT ) )
#define FLASH_IsWriteNotInProgress() ( 0 == ( ( currentStatus & FLASH_BIT_WIP_MASK ) >> FLASH_BIT_WIP_SHIFT ) )
#define FLASH_IsWriteEnabled()       ( 0 != ( ( currentStatus & FLASH_BIT_WEL_MASK ) >> FLASH_BIT_WEL_SHIFT ) )
#define FLASH_IsWriteNotEnabled()    ( 0 == ( ( currentStatus & FLASH_BIT_WEL_MASK ) >> FLASH_BIT_WEL_SHIFT ) )

/**
 * intern functions
 */

/**
 * send command via SPI to FLASH
 */
static statusFLASH_t sendCmd(
                              const uint8_t cmdToSend,
                                    uint8_t  endFlag
                            )
{
  statusFLASH_t
    status = STATUS_FLASH_SUCCESS;

  uint8_t
    txSize    = 1,
    cmdBuf[1] = { cmdToSend };

  GPIO_DRV_ClearPinOutput( settings.CSpin );

  genericSpiHandle_t
    spiProtocol = self.protocol;

  status |= DSPI_DRV_EdmaMasterTransferBlocking (
                                                  spiProtocol.instance,
                                                  NULL,

                                                  cmdBuf,
                                                  NULL,

                                                  txSize,

                                                  spiProtocol.timeout
                                                );

  if ( FLASH_CMD_END == endFlag )
  {
    GPIO_DRV_SetPinOutput( settings.CSpin );
  }

  if ( STATUS_FLASH_SUCCESS != status )
  {
    catch(10);
    status = STATUS_FLASH_PROTOCOL_ERROR;
  }

  return status;
}

/**
 * enable write operation
 */
static statusFLASH_t getWriteReady()
{
  statusFLASH_t
    status = STATUS_FLASH_SUCCESS;

  /**
   * make sure there is no other writing in progress
   */
  status = FLASH_ReadStatus( NULL );
  if ( STATUS_FLASH_SUCCESS != status )
  {
    return STATUS_FLASH_ERROR;
  }

  if FLASH_IsWriteInProgress()
  {
    /**
     * poll untill the writing is enabled
     */

    while (1)
    {
      status = FLASH_ReadStatus( NULL );
      if ( STATUS_FLASH_SUCCESS != status )
      {
        return STATUS_FLASH_ERROR;
      }

      if FLASH_IsWriteNotInProgress()
      {
        break;
      }
    }
  }

  /**
   * make sure WEL bit is HIGH (writing is enabled)
   */
  while (1)
  {
    status = FLASH_ReadStatus( NULL );
    if ( STATUS_FLASH_SUCCESS != status )
    {
      return STATUS_FLASH_ERROR;
    }
    else
    {
      if FLASH_IsWriteEnabled()
      {
        break;
      }

      else
      {
        status = sendCmd( FLASH_CMD_WRITE_ENABLE, FLASH_CMD_END );
        if ( STATUS_FLASH_SUCCESS != status )
        {
          return STATUS_FLASH_ERROR;
        }
      }
    }
  }

  return STATUS_FLASH_SUCCESS;
}

/**
 * send FLASH address
 * it is assumed that CS is LOW
 * @param  flashAddress flash address to be sent
 * @return              status flag
 */
static statusFLASH_t sendAddress(
                                  uint32_t flashAddress
                                )
{
  statusFLASH_t
    status = STATUS_FLASH_SUCCESS;

  uint32_t
    fmtAddr = 0;

  // if the address is bigger than 24 bytes, exit
  if ( flashAddress > 0x00FFFFFF)
  {
    return STATUS_FLASH_ERROR;
  }

  else
  {
    // arrange bytes
    fmtAddr |= ( flashAddress & 0xFF0000 ) >> 16;
    fmtAddr |= ( flashAddress & 0x00FF00 );
    fmtAddr |= ( flashAddress & 0x0000FF ) << 16;

    genericSpiHandle_t
      spiProtocol = self.protocol;

    status = DSPI_DRV_EdmaMasterTransferBlocking(
                                                  spiProtocol.instance,
                                                  NULL,

                                                  (uint8_t*)&fmtAddr,
                                                  NULL,

                                                  3,

                                                  spiProtocol.timeout
                                                );
  }

  if ( STATUS_FLASH_SUCCESS != status )
  {
    catch(10);
    status = STATUS_FLASH_PROTOCOL_ERROR;
  }

  return status;
}

/**
 * initialize FLASH
 */
statusFLASH_t FLASH_Init(
                            handleFLASH_t* flashHandle,
                          settingsFLASH_t* flashSettings
                        )
{
  statusSPI_t
    status = SPI_Init( &(flashHandle->protocol) );

  if ( STATUS_SPI_SUCCESS != status )
  {
    catch(10);
    status = STATUS_FLASH_PROTOCOL_ERROR;
  }

  else
  {
    /**
     * inititalize intern FLASH structures,
     * which will be used from now on
     */

    memcpy( (void*)&self,     (void*)flashHandle,   sizeof(self) );
    memcpy( (void*)&settings, (void*)flashSettings, sizeof(settings) );

    GPIO_DRV_SetPinOutput( settings.CSpin );
    status = STATUS_FLASH_SUCCESS;
  }

  return status;
}

/**
 * deinitialize the protocol FLASH uses
 */
statusFLASH_t FLASH_Deinit()
{
  statusSPI_t
    status = SPI_Deinit( &(self.protocol) );

  if ( STATUS_SPI_SUCCESS == status )
  {
    return STATUS_FLASH_SUCCESS;
  }

  else
  {
    catch(10);
    return STATUS_FLASH_PROTOCOL_ERROR;
  }
}

/**
 * write data to FLASH
 */
statusFLASH_t FLASH_WriteData (
                                const uint32_t flashAddress,
                                const uint8_t* dataToSend,
                                      uint32_t dataSize
                              )
{
  statusFLASH_t
    status = STATUS_FLASH_SUCCESS;

  // make sure the write process is ready
  status = getWriteReady();

  if ( STATUS_FLASH_SUCCESS != status )
  {
    return STATUS_FLASH_ERROR;
  }

  /**
   * write data
   */

  if ( dataSize > FLASH_PAGE_SIZE )
  {
    return STATUS_FLASH_ERROR;
  }

  else
  {
    // send the PAGE_PROGRAM command
    status |= sendCmd( FLASH_CMD_PROGRAM_PAGE, FLASH_CMD_CNT );
    // send the address
    status |= sendAddress( flashAddress );

    /**
     * send data
     */

    GPIO_DRV_ClearPinOutput( settings.CSpin );

    genericSpiHandle_t
      spiProtocol = self.protocol;

    status |= DSPI_DRV_EdmaMasterTransferBlocking(
                                                  spiProtocol.instance,
                                                  NULL,

                                                  dataToSend,
                                                  NULL,

                                                  dataSize,

                                                  spiProtocol.timeout
                                                );

  /**
   * mark the end of sending data
   * as soon as CS is driven HIGH, the self-timed WRITE STATUS REGISTER cycle
   * is initiated
   *
   * while the WRITE STATUS REGISTER cycle is in progress,
   * the status register may still be read to check the value of the WIP bit
   * the WIP bit is 1 during the self-timed WRITE STATUS REGISTER cycle, and is 0 when the
   * cycle is completed
   *
   * when the cycle is completed, the WEL bit is reset
   */

    GPIO_DRV_SetPinOutput( settings.CSpin );
  }

  if ( STATUS_FLASH_SUCCESS != status )
  {
    catch(10);
    return STATUS_FLASH_ERROR;
  }

  else
  {
    return status;
  }
}

/**
 * read FLASH status register
 */
statusFLASH_t FLASH_ReadStatus(
                                uint8_t* flashStatus
                              )
{
  statusFLASH_t
    status = STATUS_FLASH_SUCCESS;

  status |= sendCmd( FLASH_CMD_READ_STATUS, FLASH_CMD_CNT );

  if ( STATUS_FLASH_SUCCESS != status )
  {
    catch(10);
    status = STATUS_FLASH_PROTOCOL_ERROR;
  }

  else
  {
    genericSpiHandle_t
      spiProtocol = self.protocol;

    status |= DSPI_DRV_EdmaMasterTransferBlocking (
                                                    spiProtocol.instance,
                                                    NULL,

                                                    NULL,
                                                    &currentStatus,

                                                    sizeof(currentStatus),

                                                    spiProtocol.timeout
                                                  );
    GPIO_DRV_SetPinOutput( settings.CSpin );
  }

  if ( STATUS_FLASH_SUCCESS != status )
  {
    catch(10);
    status = STATUS_FLASH_PROTOCOL_ERROR;
  }

  else
  {
    if ( NULL != flashStatus )
    {
      *flashStatus = currentStatus;
    }
  }

  return status;
}

/**
 * [FLASH_WriteStatus description]
 * @return [description]
 */
statusFLASH_t FLASH_WriteStatus()
{
  statusFLASH_t
    status = sendCmd( FLASH_CMD_WRITE_STATUS, FLASH_CMD_END );
  return status;
}

/**
 * program FLASH page
 */
statusFLASH_t FLASH_ProgramPage (
                                  uint32_t pageAddr
                                )
{
  statusFLASH_t
    status = sendCmd( FLASH_CMD_PROGRAM_PAGE, FLASH_CMD_END );
  return status;
}

/**
 * erase single FLASH sector
 */
statusFLASH_t FLASH_EraseSector (
                                  uint32_t sectAddr
                                )
{
  statusFLASH_t
    status = STATUS_FLASH_SUCCESS;

  // make sure the write process is ready
  status = getWriteReady();
  if ( STATUS_FLASH_SUCCESS != status )
  {
    return STATUS_FLASH_ERROR;
  }

  else
  {
    // send the command
    status |= sendCmd( FLASH_CMD_ERASE_SECTOR, FLASH_CMD_CNT );
    // send the address
    status |= sendAddress( sectAddr );

    if ( STATUS_FLASH_SUCCESS != status )
    {
      catch(10);
      return STATUS_FLASH_ERROR;
    }
    else
    {
      // drive CS to HIGH
      GPIO_DRV_SetPinOutput( settings.CSpin );
      return status;
    }
  }
}

/**
 * erase all the FLASH
 */
statusFLASH_t FLASH_EraseBulk()
{
  statusFLASH_t
    status = sendCmd( FLASH_CMD_ERASE_BULK, FLASH_CMD_END );

  return status;
}


/**
 * read data from FLASH
 */
statusFLASH_t FLASH_ReadData(
                              uint32_t flashAddress,
                              uint8_t* dataToRead,
                              uint32_t dataSize
                            )
{
  statusFLASH_t
    status = STATUS_FLASH_SUCCESS;

  // send the PAGE_PROGRAM command
  status |= sendCmd( FLASH_CMD_READ_DATA_BYTES, FLASH_CMD_CNT );
  // send the address
  status |= sendAddress( flashAddress );

  if ( STATUS_FLASH_SUCCESS != status )
  {
    catch(10);
    status = STATUS_FLASH_PROTOCOL_ERROR;
  }

  else
  {
    GPIO_DRV_ClearPinOutput( settings.CSpin );

    genericSpiHandle_t
      spiProtocol = self.protocol;

    status |= DSPI_DRV_EdmaMasterTransferBlocking (
                                                    spiProtocol.instance,
                                                    NULL,

                                                    NULL,
                                                    dataToRead,

                                                    dataSize,

                                                    spiProtocol.timeout
                                                  );
    GPIO_DRV_SetPinOutput( settings.CSpin );
  }

  if ( STATUS_FLASH_SUCCESS != status )
  {
    catch(10);
    status = STATUS_FLASH_PROTOCOL_ERROR;
  }

  return status;
}
