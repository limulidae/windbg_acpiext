#ifndef _MS_ACPI_H_
#define _MS_ACPI_H_

#include <winnt.h>

/*
typedef struct _LIST_ENTRY
{
   _LIST_ENTRY * Flink;
   _LIST_ENTRY * Blink;
} LIST_ENTRY;
*/

typedef struct _DESCRIPTION_HEADER
{
   unsigned long Signature;
   unsigned long Length;
   unsigned char Revision;
   unsigned char Checksum;
   char OEMID[6];
   char OEMTableID[8];
   unsigned long OEMRevision;
   char CreatorID[4];
   unsigned long CreatorRev;
} DESCRIPTION_HEADER;

typedef struct _MAPIC
{
   _DESCRIPTION_HEADER Header;
   unsigned long LocalAPICAddress;
   unsigned long Flags;
   unsigned long APICTables[1];
} MAPIC;

typedef struct _DSDT
{
   _DESCRIPTION_HEADER Header;
   unsigned char DiffDefBlock[1];
} DSDT;

typedef struct _FACS
{
   unsigned long Signature;
   unsigned long Length;
   unsigned long HardwareSignature;
   unsigned long pFirmwareWakingVector;
   unsigned long GlobalLock;
   unsigned long Flags;
   _LARGE_INTEGER x_FirmwareWakingVector;
   unsigned char version;
   unsigned char Reserved[31];
} FACS;

#if 0
typedef union _LARGE_INTEGER
{
   unsigned long LowPart;
   long HighPart;
   union {
    unsigned char u[8];
   __int64 QuadPart;
   };
} LARGE_INTEGER;
#endif

typedef struct _GEN_ADDR
{
   unsigned char AddressSpaceID;
   unsigned char BitWidth;
   unsigned char BitOffset;
   unsigned char AccessSize;
   _LARGE_INTEGER Address;
} GEN_ADDR;

typedef struct _FADT
{
   _DESCRIPTION_HEADER Header;
   unsigned long facs;
   unsigned long dsdt;
   unsigned char int_model;
   unsigned char pm_profile;
   short sci_int_vector;
   unsigned long smi_cmd_io_port;
   unsigned char acpi_on_value;
   unsigned char acpi_off_value;
   unsigned char s4bios_req;
   unsigned char pstate_control;
   unsigned long pm1a_evt_blk_io_port;
   unsigned long pm1b_evt_blk_io_port;
   unsigned long pm1a_ctrl_blk_io_port;
   unsigned long pm1b_ctrl_blk_io_port;
   unsigned long pm2_ctrl_blk_io_port;
   unsigned long pm_tmr_blk_io_port;
   unsigned long gp0_blk_io_port;
   unsigned long gp1_blk_io_port;
   unsigned char pm1_evt_len;
   unsigned char pm1_ctrl_len;
   unsigned char pm2_ctrl_len;
   unsigned char pm_tmr_len;
   unsigned char gp0_blk_len;
   unsigned char gp1_blk_len;
   unsigned char gp1_base;
   unsigned char cstate_control;
   short lvl2_latency;
   short lvl3_latency;
   short flush_size;
   short flush_stride;
   unsigned char duty_offset;
   unsigned char duty_width;
   unsigned char day_alarm_index;
   unsigned char month_alarm_index;
   unsigned char century_alarm_index;
   short boot_arch;
   unsigned char reserved3[1];
   unsigned long flags;
   _GEN_ADDR reset_reg;
   unsigned char reset_val;
   unsigned char reserved4[3];
   _LARGE_INTEGER x_firmware_ctrl;
   _LARGE_INTEGER x_dsdt;
   _GEN_ADDR x_pm1a_evt_blk;
   _GEN_ADDR x_pm1b_evt_blk;
   _GEN_ADDR x_pm1a_ctrl_blk;
   _GEN_ADDR x_pm1b_ctrl_blk;
   _GEN_ADDR x_pm2_ctrl_blk;
   _GEN_ADDR x_pm_tmr_blk;
   _GEN_ADDR x_gp0_blk;
   _GEN_ADDR x_gp1_blk;
   _GEN_ADDR sleep_control_reg;
   _GEN_ADDR sleep_status_reg;
} FADT;

typedef struct _RSDT_32
{
   _DESCRIPTION_HEADER Header;
   unsigned long Tables[1];
} RSDT_32;

typedef struct _ACPIInformation
{
#ifndef ACPIEXT_X64
   _RSDT_32 * RootSystemDescTable;
   _FADT * FixedACPIDescTable;
   _FACS * FirmwareACPIControlStructure;
   _DSDT * DiffSystemDescTable;
   _MAPIC * MultipleApicTable;
   unsigned long * GlobalLock;
   _LIST_ENTRY GlobalLockQueue;
   unsigned long GlobalLockQueueLock;
   void * GlobalLockOwnerContext;
#else
   unsigned long RootSystemDescTable;
   unsigned long FixedACPIDescTable;
   unsigned long FirmwareACPIControlStructure;
   unsigned long DiffSystemDescTable;
   unsigned long MultipleApicTable;
   unsigned long GlobalLock;
   _LIST_ENTRY GlobalLockQueue;
   unsigned long GlobalLockQueueLock;
   unsigned long GlobalLockOwnerContext;
#endif
   unsigned long GlobalLockOwnerDepth;
   unsigned char ACPIOnly;
   unsigned char GP0_LEN;
   short Gpe0Size;
   unsigned long GP1_ENABLE;
   unsigned char GP1_LEN;
   unsigned char Padding15;
   short Gpe1Size;
   short GP1_Base_Index;
   short GpeSize;
   unsigned long SMI_CMD;
   short pm1_en_bits;
   short pm1_wake_mask;
   short pm1_wake_status;
   short c2_latency;
   short c3_latency;
   unsigned char Padding25;
   unsigned char Padding26;
   unsigned long ACPI_Flags;
   unsigned long ACPI_Capabilities;
   unsigned char Dockable;
   unsigned char HwReducedAcpiMode;
} ACPIInformation;

#endif