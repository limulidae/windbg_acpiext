// acpiext.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ms_acpi.h"

#include <iostream>
#include <fstream>
using namespace std;

WINDBG_EXTENSION_APIS ExtensionApis = {0};
EXT_API_VERSION g_ExtApiVersion = {1, 1, EXT_API_VERSION_NUMBER, 0} ;

IDebugClient     *gDebugClient  = NULL;
IDebugDataSpaces *gDebugDataSpaces = NULL;
IDebugSymbols    *gDebugSymbols = NULL;

#define ACPI_BUF_SIZE		(0x100)
#define DSDT_OUT_BIN_PATH	("C:\\tmp\\dsdt.bin")


/***********************************************************
 * ExtensionApiVersion
 *
 * Purpose: WINDBG will call this function to get the version
 *          of the API
 *
 *  Parameters:
 *     Void
 *
 *  Return Values:
 *     Pointer to a EXT_API_VERSION structure.
 *
 ***********************************************************/   
LPEXT_API_VERSION ExtensionApiVersion(void)
{
	return &g_ExtApiVersion;
}

/***********************************************************
 * WinDbgExtensionDllInit
 *
 * Purpose: WINDBG will call this function to initialize
 *          the API
 *
 *  Parameters:
 *     Pointer to the API functions, Major Version, Minor Version
 *
 *  Return Values:
 *     Nothing
 *
 ***********************************************************/  
VOID WinDbgExtensionDllInit(
  PWINDBG_EXTENSION_APIS lpExtensionApis,
  USHORT MajorVersion,
  USHORT MinorVersion
)
{
	ExtensionApis = *lpExtensionApis;
	HRESULT hResult = S_FALSE;
	ULONG acpi_index;
	ULONG64 acpi_base;
	ULONG64 acpi_info_offset; // offset of ACPI!gAcpiInformation
	ACPIInformation acpi_info;
	ULONG BytesRead;
	ULONG64 dsdt_addr, i;
	unsigned long dsdt_length, bytes_left;
	_DSDT dsdt;
	char buf[ACPI_BUF_SIZE];
	ofstream dsdt_ofs;

	//
	// IDebugClient
	//

	hResult = DebugCreate(__uuidof(IDebugClient), (void**) &gDebugClient);
	if (hResult != S_OK)
	{
		dprintf("Acuqiring IDebugClient* Failled\n");
		return;
	}

	//
	// IDebugDataSpaces
	//

	hResult = gDebugClient->QueryInterface(__uuidof(IDebugDataSpaces), (void**) &gDebugDataSpaces);
	if (hResult != S_OK)
	{
		dprintf("Acuqiring IDebugDataSpaces* Failled\n");
		return;
	}

	//
	// IDebugSymbols
	//

	hResult = gDebugClient->QueryInterface(__uuidof(IDebugSymbols), (void**) &gDebugSymbols);
	if (hResult != S_OK)
	{
		dprintf("Acuqiring IDebugSymbols* Failled\n");
		return;
	}

	//
	// search acpi.sys
	//

	hResult = gDebugSymbols->GetModuleByModuleName("ACPI", 0, &acpi_index, &acpi_base);
	if (hResult != S_OK)
	{
		dprintf("GetModuleByModuleName Failled\n");
		return;
	}
	else
	{
		dprintf("ACPI: index=%lu, base=%x\n", acpi_index, acpi_base);
	}

	//
	// search ACPI!gAcpiInformation
	// read the structure into local acpi_info
	//

	hResult = gDebugSymbols->GetOffsetByName("ACPI!gAcpiInformation", &acpi_info_offset);
	if (hResult != S_OK)
	{
		dprintf("GetOffsetByName Failled\n");
		return;
	}
	else
	{
		dprintf("ACPI!gAcpiInformation: offset=%x\n", acpi_info_offset);
	}

	hResult = gDebugDataSpaces->ReadVirtual(acpi_info_offset, &acpi_info, sizeof(acpi_info), &BytesRead);
	if (hResult != S_OK)
	{
		dprintf("ReadVirtual Failled\n");
		return;
	}

	dprintf("\n");
	dprintf("RSDT  = %x\n", acpi_info.RootSystemDescTable);
	dprintf("FADT  = %x\n", acpi_info.FixedACPIDescTable);
	dprintf("FACS  = %x\n", acpi_info.FirmwareACPIControlStructure);
	dprintf("DSDT  = %x\n", acpi_info.DiffSystemDescTable);
	dprintf("MAPIC = %x\n", acpi_info.MultipleApicTable);

	//
	// read DSDT
	//

	dsdt_addr = (ULONG64)(__int64)(__int32) acpi_info.DiffSystemDescTable;
	hResult = gDebugDataSpaces->ReadVirtual(dsdt_addr, &dsdt, sizeof(dsdt), &BytesRead);
	if (hResult != S_OK)
	{
		dprintf("ReadVirtual Failled\n");
		return;
	}
	dprintf("\n");
	dsdt_length = dsdt.Header.Length;
	dprintf("DSDT: ");
	dprintf("address = %llx, ", dsdt_addr);
	dprintf("length = %x\n\n", dsdt_length);

	//
	// write DSDT to a temp file
	//

	bytes_left = dsdt_length;
	i=0;
	dsdt_ofs.open(DSDT_OUT_BIN_PATH, ios::binary|ios::out|ios::trunc);
	do
	{
		if(bytes_left > ACPI_BUF_SIZE)
			gDebugDataSpaces->ReadVirtual(dsdt_addr+i, buf, ACPI_BUF_SIZE, &BytesRead);
		else
			gDebugDataSpaces->ReadVirtual(dsdt_addr+i, buf, bytes_left, &BytesRead);

		dsdt_ofs.write(buf, BytesRead);
		i += BytesRead;
		bytes_left -= BytesRead;
	} while(bytes_left);
	dprintf("Dump DSDT to %s\n", DSDT_OUT_BIN_PATH);
	dprintf("Go to https://acpica.org/, download iASL.exe to disassembly DSDT.\n");
	dprintf("iASL.exe -d %s\n\n", DSDT_OUT_BIN_PATH);
	dsdt_ofs.close();
}

/***********************************************************
 * !help
 *
 * Purpose: WINDBG will call this API when the user types !help
 *          
 *
 *  Parameters:
 *     N/A
 *
 *  Return Values:
 *     N/A
 *
 ***********************************************************/
DECLARE_API (help)
{
    dprintf("!dumpstrings <address/register> - Dumps 20 strings in ANSI/UNICODE using this address as a pointer to strings (useful for dumping strings on the stack) \n");
}


