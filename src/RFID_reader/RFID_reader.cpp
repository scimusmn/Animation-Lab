/*
 *  RFID_reader.cpp
 *  arenaBot
 *
 *  Created by Exhibits on 11/17/11.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "RFID_reader.h"
#include "../robotConfig.h"

string tagVal="default";
bool tagAvailable=false;
//#if defined( TARGET_OSX )

static RFIDreader rfident;

RFIDreader & rfid(){
	return rfident;
}

#if ENABLE_RFID

int CCONV AttachHandler(CPhidgetHandle RFID, void *userptr)
{
	int serialNo;
	const char *name;
  
	CPhidget_getDeviceName (RFID, &name);
	CPhidget_getSerialNumber(RFID, &serialNo);
	printf("%s %10d attached!\n", name, serialNo);
  
	return 0;
}

int CCONV DetachHandler(CPhidgetHandle RFID, void *userptr)
{
	int serialNo;
	const char *name;
  
	CPhidget_getDeviceName (RFID, &name);
	CPhidget_getSerialNumber(RFID, &serialNo);
	printf("%s %10d detached!\n", name, serialNo);
  
	return 0;
}

int CCONV ErrorHandler(CPhidgetHandle RFID, void *userptr, int ErrorCode, const char *unknown)
{
	printf("Error handled. %d - %s\n", ErrorCode, unknown);
	return 0;
}

int CCONV OutputChangeHandler(CPhidgetRFIDHandle RFID, void *usrptr, int Index, int State)
{
	if(Index == 0 || Index == 1)
	{
		printf("Output: %d > State: %d\n", Index, State);
	}
	return 0;
}

int CCONV TagHandler(CPhidgetRFIDHandle RFID, void *usrptr, unsigned char *TagVal)
{
	//turn on the Onboard LED
	CPhidgetRFID_setLEDOn(RFID, 1);
  tagVal=ssprintf("%02x%02x%02x%02x%02x",TagVal[0], TagVal[1], TagVal[2], TagVal[3], TagVal[4]);
	cout << "Tag found!" << endl;
  tagAvailable=true;
	return 0;
}

int CCONV TagLostHandler(CPhidgetRFIDHandle RFID, void *usrptr, unsigned char *TagVal)
{
	//turn off the Onboard LED
	CPhidgetRFID_setLEDOn(RFID, 0);
	cout << "Tag lost!" << endl;
  //tagVal="";
  tagAvailable=false;
	return 0;
}

#endif

RFIDreader::RFIDreader()
{

}

void RFIDreader::setup()
{
  #if ENABLE_RFID
	if(cfg().test){
		int result;
		const char *err;
		tagAvailable=false;
		CPhidgetRFID_create(&rfid);
		//Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
		CPhidget_set_OnAttach_Handler((CPhidgetHandle)rfid, AttachHandler, NULL);
		CPhidget_set_OnDetach_Handler((CPhidgetHandle)rfid, DetachHandler, NULL);
		CPhidget_set_OnError_Handler((CPhidgetHandle)rfid, ErrorHandler, NULL);
	  
		//Registers a callback that will run if an output changes.
		//Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
		CPhidgetRFID_set_OnOutputChange_Handler(rfid, OutputChangeHandler, NULL);
	  
		//Registers a callback that will run when a Tag is read.
		//Requires the handle for the PhidgetRFID, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
		CPhidgetRFID_set_OnTag_Handler(rfid, TagHandler, NULL);
	  
		//Registers a callback that will run when a Tag is lost (removed from antenna read range).
		//Requires the handle for the PhidgetRFID, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
		CPhidgetRFID_set_OnTagLost_Handler(rfid, TagLostHandler, NULL);
	
  
	  //open the RFID for device connections
		CPhidget_open((CPhidgetHandle)rfid, -1);
  
	  //get the program to wait for an RFID device to be attached
		printf("Waiting for RFID to be attached....");
		if((result = CPhidget_waitForAttachment((CPhidgetHandle)rfid, 2000)))
		{
			CPhidget_getErrorDescription(result, &err);
			printf("Problem waiting for attachment: %s\n", err);
		}
  
		CPhidgetRFID_setAntennaOn(rfid, 1);
	}
#endif
}

RFIDreader::~RFIDreader()
{
#if ENABLE_RFID
	if(cfg().test){
		CPhidget_close((CPhidgetHandle)rfid);
		CPhidget_delete((CPhidgetHandle)rfid);
	}
#endif
}

bool RFIDreader::available()
{
  return tagAvailable;
}

string RFIDreader::getSerialNumber()
{
  return tagVal;
}
