//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8UB1_Register_Enums.h>
#include <efm8_usb.h>
#include "descriptors.h"
#include "idle.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Port sbits
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------

void WriteLEDs (uint16_t leds);


//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

uint8_t tmpBuffer;
uint8_t xdata txBuffer[64];
uint8_t xdata rxBuffer[64];

uint8_t keyReportNeeded = 0;
uint8_t keyScanState = 0;
uint8_t xdata thisKeyScan[3];
uint8_t xdata lastKeyScan[3];


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

void USBD_EnterHandler (void)
{
	// nothing
}


void USBD_ExitHandler(void)
{
	// nothing
}


void USBD_ResetCb (void)
{
	// nothing
}


void USBD_SofCb (uint16_t sofNr)
{
	uint8_t i;

	UNREFERENCED_ARGUMENT(sofNr);

	idleTimerTick();

	if (!keyReportNeeded) {
		if (keyScanState < 6) {
			if (!(keyScanState & 1)) {
				// set row bits low one at a time
				P1 |= 0x07;
				P1 &= ~(1 << (keyScanState >> 1));
			} else {
				// read column inputs
				thisKeyScan[keyScanState >> 1] = (~P1 & 0x78) >> 3;
			}
			keyScanState++;
	    } else {
	    	P1 |= 0x07;
		    for (i = 0; i < 3; i++) {
			    if (thisKeyScan[i] != lastKeyScan[i]) {
				    keyReportNeeded = true;
			    }
			    lastKeyScan[i] = thisKeyScan[i];
		    }
		    keyScanState = 0;
	    }
	}

	// Check if the device should send a report
	if (isIdleTimerExpired() == true) {
	    if (USBD_EpIsBusy(EP1IN) == false) {
	    	if (keyReportNeeded) {
			    txBuffer[0] = 0x02;
  			    txBuffer[1] = thisKeyScan[0];
	  		    txBuffer[2] = thisKeyScan[1];
			    txBuffer[3] = thisKeyScan[2];
				USBD_Write (EP1IN, txBuffer, 4, false);
				keyReportNeeded = false;
		    }
	    }
	}
}

void USBD_DeviceStateChangeCb (USBD_State_TypeDef oldState,
							   USBD_State_TypeDef newState)
{
	// not configured or in suspend
	if (newState < USBD_STATE_SUSPENDED)
	{
	}
	// Entering suspend mode, power internal and external blocks down
	else if (newState == USBD_STATE_SUSPENDED)
	{
		// Abort any pending transfer
		USBD_AbortTransfer (EP1IN);
	}
	else if (newState == USBD_STATE_CONFIGURED)
	{
		idleTimerSet(POLL_RATE);
		USBD_Read (EP1OUT, rxBuffer, 64, true);
	}

	// Exiting suspend mode, power internal and external blocks up
	if (oldState == USBD_STATE_SUSPENDED)
	{
	}
}


bool USBD_IsSelfPoweredCb (void)
{
	return true;
}


USB_Status_TypeDef USBD_SetupCmdCb(SI_VARIABLE_SEGMENT_POINTER(
                                     setup,
                                     USB_Setup_TypeDef,
                                     MEM_MODEL_SEG))
{
  USB_Status_TypeDef retVal = USB_STATUS_REQ_UNHANDLED;

  if ((setup->bmRequestType.Type == USB_SETUP_TYPE_STANDARD)
      && (setup->bmRequestType.Direction == USB_SETUP_DIR_IN)
      && (setup->bmRequestType.Recipient == USB_SETUP_RECIPIENT_INTERFACE))
  {
    // A HID device must extend the standard GET_DESCRIPTOR command
    // with support for HID descriptors.
    switch (setup->bRequest)
    {
      case GET_DESCRIPTOR:
        if ((setup->wValue >> 8) == USB_HID_REPORT_DESCRIPTOR)
        {
          switch (setup->wIndex)
          {
            case 0: // Interface 0

              USBD_Write(EP0,
                         (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_GENERIC))ReportDescriptor0,
                         EFM8_MIN(sizeof(ReportDescriptor0), setup->wLength),
                         false);
              retVal = USB_STATUS_OK;
              break;

            default: // Unhandled Interface
              break;
          }
        }
        else if ((setup->wValue >> 8) == USB_HID_DESCRIPTOR)
        {
          switch (setup->wIndex)
          {
            case 0: // Interface 0

              USBD_Write(EP0,
                         (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_GENERIC))(&configDesc[18]),
                         EFM8_MIN(USB_HID_DESCSIZE, setup->wLength),
                         false);
              retVal = USB_STATUS_OK;
              break;

            default: // Unhandled Interface
              break;
          }
        }
        break;
    }
  }
  else if ((setup->bmRequestType.Type == USB_SETUP_TYPE_CLASS)
           && (setup->bmRequestType.Recipient == USB_SETUP_RECIPIENT_INTERFACE)
           && (setup->wIndex == HID_VENDOR_IFC))
  {
    // Implement the necessary HID class specific commands.
    switch (setup->bRequest)
    {
      case USB_HID_SET_IDLE:
        if (((setup->wValue & 0xFF) == 0)             // Report ID
            && (setup->wLength == 0)
            && (setup->bmRequestType.Direction != USB_SETUP_DIR_IN))
        {
          idleTimerSet(setup->wValue >> 8);
          retVal = USB_STATUS_OK;
        }
        break;

      case USB_HID_GET_IDLE:
        if ((setup->wValue == 0)                      // Report ID
            && (setup->wLength == 1)
            && (setup->bmRequestType.Direction == USB_SETUP_DIR_IN))
        {
          tmpBuffer = idleGetRate();
          USBD_Write(EP0, &tmpBuffer, 1, false);
          retVal = USB_STATUS_OK;
        }
        break;
    }
  }

  return retVal;
}


uint16_t USBD_XferCompleteCb(uint8_t epAddr, USB_Status_TypeDef status,
		uint16_t xferred, uint16_t remaining)
{
	uint16_t leds;

	UNREFERENCED_ARGUMENT(status);
    UNREFERENCED_ARGUMENT(xferred);
	UNREFERENCED_ARGUMENT(remaining);

	if (epAddr == EP1OUT) {
		// report 1 is  bytes, set leds
		if (rxBuffer[0] == 0x01) {
			leds = (rxBuffer[1] << 8) | rxBuffer[2];
			WriteLEDs (leds);
		}
		USBD_Read (EP1OUT, rxBuffer, 64, true);
	}

	return 0;
}
