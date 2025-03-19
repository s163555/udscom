//  PCAN-ISO-TP.cs
//
//  ~~~~~~~~~~~~
//
//  PCAN-ISO-TP API
//
//  ~~~~~~~~~~~~
//
//  ------------------------------------------------------------------
//  Author : Fabrice Vergnaud
//	Last changed by:	$Author: Fabrice $
//  Last changed date:	$Date: 2017-09-07 17:08:13 +0200 (Thu, 07 Sep 2017) $
//
//  Language: C#
//  ------------------------------------------------------------------
//
//  Copyright (C) 2015  PEAK-System Technik GmbH, Darmstadt
//  more Info at http://www.peak-system.com 
//
using System;
using System.Text;
using System.Runtime.InteropServices;

namespace Peak.Can.IsoTp
{
    ////////////////////////////////////////////////////////////
    // Inclusion of other needed stuff
    ////////////////////////////////////////////////////////////
    using TPCANTPHandle = System.UInt16;
    using TPCANTPBitrateFD = System.String;

    #region Enumerations
    /// <summary>
    /// Represents a PCAN Baud rate register value
    /// </summary>
    public enum TPCANTPBaudrate : ushort
    {
        /// <summary>
        /// 1 MBit/s
        /// </summary>
        PCANTP_BAUD_1M = 0x0014,
        /// <summary>
        /// 800 kBit/s
        /// </summary>
        PCANTP_BAUD_800K = 0x0016,
        /// <summary>
        /// 500 kBit/s
        /// </summary>
        PCANTP_BAUD_500K = 0x001C,
        /// <summary>
        /// 250 kBit/s
        /// </summary>
        PCANTP_BAUD_250K = 0x011C,
        /// <summary>
        /// 125 kBit/s
        /// </summary>
        PCANTP_BAUD_125K = 0x031C,
        /// <summary>
        /// 100 kBit/s
        /// </summary>
        PCANTP_BAUD_100K = 0x432F,
        /// <summary>
        /// 95,238 kBit/s
        /// </summary>
        PCANTP_BAUD_95K = 0xC34E,
        /// <summary>
        /// 83,333 kBit/s
        /// </summary>
        PCANTP_BAUD_83K = 0x852B,
        /// <summary>
        /// 50 kBit/s
        /// </summary>
        PCANTP_BAUD_50K = 0x472F,
        /// <summary>
        /// 47,619 kBit/s
        /// </summary>
        PCANTP_BAUD_47K = 0x1414,
        /// <summary>
        /// 33,333 kBit/s
        /// </summary>
        PCANTP_BAUD_33K = 0x8B2F,
        /// <summary>
        /// 20 kBit/s
        /// </summary>
        PCANTP_BAUD_20K = 0x532F,
        /// <summary>
        /// 10 kBit/s
        /// </summary>
        PCANTP_BAUD_10K = 0x672F,
        /// <summary>
        /// 5 kBit/s
        /// </summary>
        PCANTP_BAUD_5K = 0x7F7F,
    }

    /// <summary>
    /// Represents the different Not Plug-And-Play PCAN Hardware types
    /// </summary>
    public enum TPCANTPHWType : byte
    {
        /// <summary>
        /// PCAN-ISA 82C200
        /// </summary>
        PCANTP_TYPE_ISA = 0x01,
        /// <summary>
        /// PCAN-ISA SJA1000
        /// </summary>
        PCANTP_TYPE_ISA_SJA = 0x09,
        /// <summary>
        /// PHYTEC ISA 
        /// </summary>
        PCANTP_TYPE_ISA_PHYTEC = 0x04,
        /// <summary>
        /// PCAN-Dongle 82C200
        /// </summary>
        PCANTP_TYPE_DNG = 0x02,
        /// <summary>
        /// PCAN-Dongle EPP 82C200
        /// </summary>
        PCANTP_TYPE_DNG_EPP = 0x03,
        /// <summary>
        /// PCAN-Dongle SJA1000
        /// </summary>
        PCANTP_TYPE_DNG_SJA = 0x05,
        /// <summary>
        /// PCAN-Dongle EPP SJA1000
        /// </summary>
        PCANTP_TYPE_DNG_SJA_EPP = 0x06,
    }

    /// <summary>
    /// Represent the PCANTP error and status codes 
    /// </summary>
    public enum TPCANTPStatus : uint
    {
        /// <summary>
        /// No error 
        /// </summary>
        PCANTP_ERROR_OK = 0x00000,
        /// <summary>
        /// Not Initialized
        /// </summary>
        PCANTP_ERROR_NOT_INITIALIZED = 0x00001,
        /// <summary>
        /// Already Initialized
        /// </summary>
        PCANTP_ERROR_ALREADY_INITIALIZED = 0x00002,
        /// <summary>
        /// Could not obtain memory
        /// </summary>
        PCANTP_ERROR_NO_MEMORY = 0x00003,
        /// <summary>
        /// Input buffer overflow
        /// </summary>
        PCANTP_ERROR_OVERFLOW = 0x00004,
        /// <summary>
        /// No Message available
        /// </summary>
        PCANTP_ERROR_NO_MESSAGE = 0x00007,
        /// <summary>
        /// Wrong message parameters
        /// </summary>
        PCANTP_ERROR_WRONG_PARAM = 0x00008,
        /// <summary>
        /// PCANTP Channel is in BUS-LIGHT error state
        /// </summary>
        PCANTP_ERROR_BUSLIGHT = 0x00009,
        /// <summary>
        /// PCANTP Channel is in BUS-HEAVY error state
        /// </summary>
        PCANTP_ERROR_BUSHEAVY = 0x0000A,
        /// <summary>
        /// PCANTP Channel is in BUS-OFF error state
        /// </summary>
        PCANTP_ERROR_BUSOFF = 0x0000B,
        /// <summary>
        /// Global CAN error, status code for composition of PCANBasic Errors.
        ///	Remove this value to get a PCAN-Basic TPCANStatus error code.
        /// </summary>
        PCANTP_ERROR_CAN_ERROR = 0x80000000,
    }

    /// <summary>
    /// Represent message request confirmation values defined in ISO 15765-2
    /// </summary>
    public enum TPCANTPConfirmation : byte
    {
        /// <summary>
        /// No network error
        /// </summary>
        PCANTP_N_OK = 0x00,
        /// <summary>
        /// timeout occured between 2 frames transmission (sender and receiver side)
        /// </summary>
        PCANTP_N_TIMEOUT_A = 0x01,
        /// <summary>
        /// sender side timeout while waiting for flow control frame
        /// </summary>
        PCANTP_N_TIMEOUT_BS = 0x02,
        /// <summary>
        /// receiver side timeout while waiting for consecutive frame
        /// </summary>
        PCANTP_N_TIMEOUT_CR = 0x03,
        /// <summary>
        /// unexpected sequence number
        /// </summary>
        PCANTP_N_WRONG_SN = 0x04,
        /// <summary>
        /// invalid or unknown FlowStatus
        /// </summary>
        PCANTP_N_INVALID_FS = 0x05,
        /// <summary>
        /// unexpected protocol data unit
        /// </summary>
        PCANTP_N_UNEXP_PDU = 0x06,
        /// <summary>
        /// reception of flow control WAIT frame that exceeds the maximum counter defined by PCANTP_PARAM_WFT_MAX
        /// </summary>
        PCANTP_N_WFT_OVRN = 0x07,
        /// <summary>
        /// buffer on the receiver side cannot store the data length (server side only)
        /// </summary>
        PCANTP_N_BUFFER_OVFLW = 0x08,
        /// <summary>
        /// general error
        /// </summary>
        PCANTP_N_ERROR = 0x09,
        /// <summary>
        /// message was invalid and ignored
        /// </summary>
        PCANTP_N_IGNORED = 0x0A,
    }

    /// <summary>
    /// PCANTP parameters
    /// </summary>
    public enum TPCANTPParameter : byte
    {
        /// <summary>
        /// 1 BYTE data describing the block size parameter (BS)
        /// </summary>
        PCANTP_PARAM_BLOCK_SIZE = 0xE1,
        /// <summary>
        /// 1 BYTE data describing the seperation time parameter (STmin)
        /// </summary>
        PCANTP_PARAM_SEPERATION_TIME = 0xE2,
        /// <summary>
        /// 1 BYTE data describing the debug mode 
        /// </summary>
        PCANTP_PARAM_DEBUG = 0xE3,
        /// <summary>
        /// 1 Byte data describing the condition of a channel
        /// </summary>
        PCANTP_PARAM_CHANNEL_CONDITION = 0xE4,
        /// <summary>
        /// Integer data describing the Wait Frame Transmissions parameter. 
        /// </summary>
        PCANTP_PARAM_WFT_MAX = 0xE5,
        /// <summary>
        /// 1 BYTE data stating if pending messages are displayed/hidden 
        /// </summary>
        PCANTP_PARAM_MSG_PENDING = 0xE6,
        /// <summary>
        /// PCAN-ISO-TP API version parameter
        /// </summary>
        PCANTP_PARAM_API_VERSION = 0xE7,
        /// <summary>
        /// 1 BYTE data stating if CAN frame DLC uses padding or not
        /// </summary>
        PCANTP_PARAM_CAN_DATA_PADDING = 0xE8,
        /// <summary>
        ///  1 BYTE data stating if unsegmented (NON-ISO-TP) CAN frames can be received
        /// </summary>
        PCANTP_PARAM_CAN_UNSEGMENTED = 0xE9,
        /// <summary>
        /// PCAN-ISO-TP receive event handler parameter
        /// </summary>
        PCANTP_PARAM_RECEIVE_EVENT = 0xEA,
        /// <summary>
        /// 1 BYTE data stating the value used for CAN data padding
        /// </summary>
        PCANTP_PARAM_PADDING_VALUE = 0xED,
        /// <summary>
        /// 1 BYTE data stating the default priority value for normal fixed, mixed and enhanced addressing (default=6)
        /// </summary>
        PCANTP_PARAM_J1939_PRIORITY = 0xEE,
        /// <summary>
        /// 1 BYTE data stating the default DLC to use when transmitting messages with CAN FD
        /// </summary>
        PCANTP_PARAM_CAN_TX_DL = 0xEF
    }

    /// <summary>
    /// PCANTP message types
    /// </summary>
    public enum TPCANTPMessageType : byte
    {
        /// <summary>
        /// Unknown (non-ISO-TP) message
        /// </summary>
        PCANTP_MESSAGE_UNKNOWN = 0x00,
        /// <summary>
        /// Diagnostic Request/Confirmation
        /// </summary>
        PCANTP_MESSAGE_DIAGNOSTIC = 0x01,
        /// <summary>
        /// Remote Dignostic Request/Confirmation (uses RA address)
        /// </summary>
        PCANTP_MESSAGE_REMOTE_DIAGNOSTIC = 0x02,
        /// <summary>
        /// Confirms that a message has been sent successfully/ not successfully
        /// </summary>
        PCANTP_MESSAGE_REQUEST_CONFIRMATION = 0x03,
        /// <summary>
        /// Multi-Frame Message is being received
        /// </summary>
        PCANTP_MESSAGE_INDICATION = 0x04,
        /// <summary>
        /// Multi-Frame Message is being transmitted
        /// </summary>
        PCANTP_MESSAGE_INDICATION_TX = 0x05,
    }

    /// <summary>
    /// PCANTP message types
    /// </summary>
    [Flags]
    public enum TPCANTPIdType : byte
    {
        /// <summary>
        /// 11 bits CAN ID (CAN Standard Frame)
        /// </summary>
        PCANTP_ID_CAN_11BIT = 0x01,
        /// <summary>
        /// 29 bits CAN ID (CAN Extended Frame)
        /// </summary>
        PCANTP_ID_CAN_29BIT = 0x02,
        /// <summary>
        /// CAN FD flag
        /// </summary>
        PCANTP_ID_CAN_FD = 0x04,
        /// <summary>
        /// Bitrate Switch flag (only if CAN FD)
        /// </summary>
        PCANTP_ID_CAN_BRS = 0x08,
    }

    /// <summary>
    /// PCANTP message types utility function to handle priority J1939 compatible ISO-TP messages
    /// </summary>
    public static class TPCANTPIdTypePriority
    {
        /// <summary>
        /// Get the 29bits CAN ID type with a specific J1939 priority 
        /// </summary>
        public static TPCANTPIdType PCANTP_ID_CAN_GET_29B(byte j1939_priority)
        {
            return (TPCANTPIdType)((byte)((j1939_priority << 5) | CanTpApi.PCANTP_ID_CAN_IS_PRIORITY_MASK | ((byte)TPCANTPIdType.PCANTP_ID_CAN_29BIT & CanTpApi.PCANTP_ID_CAN_MASK)));
        }
        /// <summary>
        /// Retrieves the priority field from a CAN ID type
        /// </summary>
        public static byte PCANTP_ID_CAN_GET_PRIORIY(byte id_type)
        {
            return (byte)(id_type >> 5);
        }
        /// <summary>
        /// States if the CAN ID Type is 29bits
        /// </summary>
        public static bool PCANTP_ID_CAN_IS_EXTENDED(byte id_type)
        {
            return ((id_type & (byte)TPCANTPIdType.PCANTP_ID_CAN_29BIT) == (int)TPCANTPIdType.PCANTP_ID_CAN_29BIT);
        }
        /// <summary>
        /// States if the id_type contains a J1939 priority field
        /// </summary>
        public static bool PCANTP_ID_CAN_HAS_PRIORITY(byte id_type)
        {
            return (PCANTP_ID_CAN_IS_EXTENDED(id_type) && ((id_type & CanTpApi.PCANTP_ID_CAN_IS_PRIORITY_MASK) == CanTpApi.PCANTP_ID_CAN_IS_PRIORITY_MASK));
        }
    }

    /// <summary>
    /// PCANTP addressing type
    /// </summary>
    public enum TPCANTPFormatType : byte
    {
        /// <summary>
        /// unknown adressing format
        /// </summary>
        PCANTP_FORMAT_UNKNOWN = 0xFF,
        /// <summary>
        /// unsegmented CAN frame
        /// </summary>
        PCANTP_FORMAT_NONE = 0x00,
        /// <summary>
        /// normal adressing format from ISO 15765-2
        /// </summary>
        PCANTP_FORMAT_NORMAL = 0x01,
        /// <summary>
        /// fixed normal adressing format from ISO 15765-2
        /// </summary>
        PCANTP_FORMAT_FIXED_NORMAL = 0x02,
        /// <summary>
        /// extended adressing format from ISO 15765-2
        /// </summary>
        PCANTP_FORMAT_EXTENDED = 0x03,
        /// <summary>
        /// mixed adressing format from ISO 15765-2
        /// </summary>
        PCANTP_FORMAT_MIXED = 0x04,
        /// <summary>
        /// enhanced adressing format from ISO 15765-3
        /// </summary>
        PCANTP_FORMAT_ENHANCED = 0x05,
    }

    /// <summary>
    /// PCANTP addressing type
    /// </summary>
    public enum TPCANTPAddressingType : byte
    {
        /// <summary>
        /// Unknown (non-ISO-TP) message
        /// </summary>
        PCANTP_ADDRESSING_UNKNOWN = 0x00,
        /// <summary>
        /// Physical addressing
        /// </summary>
        PCANTP_ADDRESSING_PHYSICAL = 0x01,
        /// <summary>
        /// Functional addressing
        /// </summary>
        PCANTP_ADDRESSING_FUNCTIONAL = 0x02,
    }
    #endregion

    #region Structures    
    /// <summary>
    /// A CAN-TP Message
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct TPCANTPMsg
    {
        /// <summary>
        /// Represents the origin of this message (address from 
        /// where this message was or will be sent)
        /// </summary>
        public byte SA;
        /// <summary>
        /// Represents the destination of this message (address to 
        /// where this message was or will be sent)
        /// </summary>
        public byte TA;
        /// <summary>
        /// Represents the kind of addressing being used for communication
        /// </summary>
        [MarshalAs(UnmanagedType.U1)]
        public TPCANTPAddressingType TA_TYPE;
        /// <summary>
        /// Represents the destination of this message in a remote network 
        /// </summary>
        public byte RA;

        /// <summary>
        /// Identifies the kind of CAN Identifier
        /// </summary>
        [MarshalAs(UnmanagedType.U1)]
        public TPCANTPIdType IDTYPE;
        /// <summary>
        /// Identifies the kind of data represented by this message
        /// </summary>
        [MarshalAs(UnmanagedType.U1)]
        public TPCANTPMessageType MSGTYPE;
        /// <summary>
        /// Represents the kind of format represented by this message
        /// </summary>
        [MarshalAs(UnmanagedType.U1)]
        public TPCANTPFormatType FORMAT;

        /// <summary>
        /// Represents the buffer containing the data of this message
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4095)]
        public byte[] DATA;
        /// <summary>
        /// Indicates the length of the useful data within the DATA buffer
        /// </summary>
        public ushort LEN;
        /// <summary>
        /// Represent message request confirmation
        /// </summary>
        [MarshalAs(UnmanagedType.U1)]
        public TPCANTPConfirmation RESULT;
    }

    /// <summary>
    /// Represents a timestamp of a received PCAN message.
    /// Total Microseconds = micros + 1000 * millis + 0x100000000 * 1000 * millis_overflow
    /// </summary>
    public struct TPCANTPTimestamp
    {
        /// <summary>
        /// Base-value: milliseconds: 0.. 2^32-1
        /// </summary>
        public uint millis;
        /// <summary>
        /// Roll-arounds of millis
        /// </summary>
        public ushort millis_overflow;
        /// <summary>
        /// Microseconds: 0..999
        /// </summary>
        public ushort micros;
    }
    #endregion

    #region PCANTP Api
    /* Summary
       The PCAN-ISO-TP API class
                                 */
    public static class CanTpApi
    {

        #region PCAN-BUS Handles Definition
        /// <summary>
        /// Undefined/default value for a PCAN bus
        /// </summary>
        public const TPCANTPHandle PCANTP_NONEBUS = 0x00;

        /// <summary>
        /// PCAN-ISA interface, channel 1
        /// </summary>
        public const TPCANTPHandle PCANTP_ISABUS1 = 0x21;
        /// <summary>
        /// PCAN-ISA interface, channel 2
        /// </summary>
        public const TPCANTPHandle PCANTP_ISABUS2 = 0x22;
        /// <summary>
        /// PCAN-ISA interface, channel 3
        /// </summary>
        public const TPCANTPHandle PCANTP_ISABUS3 = 0x23;
        /// <summary>
        /// PCAN-ISA interface, channel 4
        /// </summary>
        public const TPCANTPHandle PCANTP_ISABUS4 = 0x24;
        /// <summary>
        /// PCAN-ISA interface, channel 5
        /// </summary>
        public const TPCANTPHandle PCANTP_ISABUS5 = 0x25;
        /// <summary>
        /// PCAN-ISA interface, channel 6
        /// </summary>
        public const TPCANTPHandle PCANTP_ISABUS6 = 0x26;
        /// <summary>
        /// PCAN-ISA interface, channel 7
        /// </summary>
        public const TPCANTPHandle PCANTP_ISABUS7 = 0x27;
        /// <summary>
        /// PCAN-ISA interface, channel 8
        /// </summary>
        public const TPCANTPHandle PCANTP_ISABUS8 = 0x28;

        /// <summary>
        /// PPCAN-Dongle/LPT interface, channel 1 
        /// </summary>
        public const TPCANTPHandle PCANTP_DNGBUS1 = 0x31;

        /// <summary>
        /// PCAN-PCI interface, channel 1
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS1 = 0x41;
        /// <summary>
        /// PCAN-PCI interface, channel 2
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS2 = 0x42;
        /// <summary>
        /// PCAN-PCI interface, channel 3
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS3 = 0x43;
        /// <summary>
        /// PCAN-PCI interface, channel 4
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS4 = 0x44;
        /// <summary>
        /// PCAN-PCI interface, channel 5
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS5 = 0x45;
        /// <summary>
        /// PCAN-PCI interface, channel 6
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS6 = 0x46;
        /// <summary>
        /// PCAN-PCI interface, channel 7
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS7 = 0x47;
        /// <summary>
        /// PCAN-PCI interface, channel 8
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS8 = 0x48;
        /// <summary>
        /// PCAN-PCI interface, channel 9
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS9 = 0x409;
        /// <summary>
        /// PCAN-PCI interface, channel 10
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS10 = 0x40A;
        /// <summary>
        /// PCAN-PCI interface, channel 11
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS11 = 0x40B;
        /// <summary>
        /// PCAN-PCI interface, channel 12
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS12 = 0x40C;
        /// <summary>
        /// PCAN-PCI interface, channel 13
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS13 = 0x40D;
        /// <summary>
        /// PCAN-PCI interface, channel 14
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS14 = 0x40E;
        /// <summary>
        /// PCAN-PCI interface, channel 15
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS15 = 0x40F;
        /// <summary>
        /// PCAN-PCI interface, channel 16
        /// </summary>
        public const TPCANTPHandle PCANTP_PCIBUS16 = 0x410;

        /// <summary>
        /// PCAN-USB interface, channel 1
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS1 = 0x51;
        /// <summary>
        /// PCAN-USB interface, channel 2
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS2 = 0x52;
        /// <summary>
        /// PCAN-USB interface, channel 3
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS3 = 0x53;
        /// <summary>
        /// PCAN-USB interface, channel 4
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS4 = 0x54;
        /// <summary>
        /// PCAN-USB interface, channel 5
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS5 = 0x55;
        /// <summary>
        /// PCAN-USB interface, channel 6
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS6 = 0x56;
        /// <summary>
        /// PCAN-USB interface, channel 7
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS7 = 0x57;
        /// <summary>
        /// PCAN-USB interface, channel 8
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS8 = 0x58;
        /// <summary>
        /// PCAN-USB interface, channel 9
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS9 = 0x509;
        /// <summary>
        /// PCAN-USB interface, channel 10
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS10 = 0x50A;
        /// <summary>
        /// PCAN-USB interface, channel 11
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS11 = 0x50B;
        /// <summary>
        /// PCAN-USB interface, channel 12
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS12 = 0x50C;
        /// <summary>
        /// PCAN-USB interface, channel 13
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS13 = 0x50D;
        /// <summary>
        /// PCAN-USB interface, channel 14
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS14 = 0x50E;
        /// <summary>
        /// PCAN-USB interface, channel 15
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS15 = 0x50F;
        /// <summary>
        /// PCAN-USB interface, channel 16
        /// </summary>
        public const TPCANTPHandle PCANTP_USBBUS16 = 0x510;

        /// <summary>
        /// PCAN-PC Card interface, channel 1
        /// </summary>
        public const TPCANTPHandle PCANTP_PCCBUS1 = 0x61;
        /// <summary>
        /// PCAN-PC Card interface, channel 2
        /// </summary>
        public const TPCANTPHandle PCANTP_PCCBUS2 = 0x62;

        /// <summary>
        /// PCAN-LAN interface, channel 1
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS1 = 0x801;
        /// <summary>
        /// PCAN-LAN interface, channel 2
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS2 = 0x802;
        /// <summary>
        /// PCAN-LAN interface, channel 3
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS3 = 0x803;
        /// <summary>
        /// PCAN-LAN interface, channel 4
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS4 = 0x804;
        /// <summary>
        /// PCAN-LAN interface, channel 5
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS5 = 0x805;
        /// <summary>
        /// PCAN-LAN interface, channel 6
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS6 = 0x806;
        /// <summary>
        /// PCAN-LAN interface, channel 7
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS7 = 0x807;
        /// <summary>
        /// PCAN-LAN interface, channel 8
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS8 = 0x808;
        /// <summary>
        /// PCAN-LAN interface, channel 9
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS9 = 0x809;
        /// <summary>
        /// PCAN-LAN interface, channel 10
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS10 = 0x80A;
        /// <summary>
        /// PCAN-LAN interface, channel 11
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS11 = 0x80B;
        /// <summary>
        /// PCAN-LAN interface, channel 12
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS12 = 0x80C;
        /// <summary>
        /// PCAN-LAN interface, channel 13
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS13 = 0x80D;
        /// <summary>
        /// PCAN-LAN interface, channel 14
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS14 = 0x80E;
        /// <summary>
        /// PCAN-LAN interface, channel 15
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS15 = 0x80F;
        /// <summary>
        /// PCAN-LAN interface, channel 16
        /// </summary>
        public const TPCANTPHandle PCANTP_LANBUS16 = 0x810;
        #endregion

        #region FD Bit rate parameters
        /// <summary>
        /// Clock frequency in Herz (80000000, 60000000, 40000000, 30000000, 24000000, 20000000)
        /// </summary>
        public const string PCANTP_BR_CLOCK = "f_clock";
        /// <summary>
        /// Clock frequency in Megaherz (80, 60, 40, 30, 24, 20)
        /// </summary>
        public const string PCANTP_BR_CLOCK_MHZ = "f_clock_mhz";
        /// <summary>
        /// Clock prescaler for nominal time quantum
        /// </summary>
        public const string PCANTP_BR_NOM_BRP = "nom_brp";
        /// <summary>
        /// TSEG1 segment for nominal bit rate in time quanta
        /// </summary>
        public const string PCANTP_BR_NOM_TSEG1 = "nom_tseg1";
        /// <summary>
        /// TSEG2 segment for nominal bit rate in time quanta
        /// </summary>
        public const string PCANTP_BR_NOM_TSEG2 = "nom_tseg2";
        /// <summary>
        /// Synchronization Jump Width for nominal bit rate in time quanta
        /// </summary>
        public const string PCANTP_BR_NOM_SJW = "nom_sjw";
        /// <summary>
        /// Sample point for nominal bit rate
        /// </summary>
        public const string PCANTP_BR_NOM_SAMPLE = "nom_sam";
        /// <summary>
        /// Clock prescaler for highspeed data time quantum
        /// </summary>
        public const string PCANTP_BR_DATA_BRP = "data_brp";
        /// <summary>
        /// TSEG1 segment for fast data bit rate in time quanta
        /// </summary>
        public const string PCANTP_BR_DATA_TSEG1 = "data_tseg1";
        /// <summary>
        /// TSEG2 segment for fast data bit rate in time quanta
        /// </summary>
        public const string PCANTP_BR_DATA_TSEG2 = "data_tseg2";
        /// <summary>
        /// Synchronization Jump Width for highspeed data bit rate in time quanta
        /// </summary>
        public const string PCANTP_BR_DATA_SJW = "data_sjw";
        /// <summary>
        /// Secondary sample point delay for highspeed data bit rate in cyles
        /// </summary>
        public const string PCANTP_BR_DATA_SAMPLE = "data_ssp_offset";
        #endregion

        #region Parameter values definition
        /// <summary>
        /// No debug messages
        /// </summary>
        public const byte PCANTP_DEBUG_NONE = 0;
        /// <summary>
        /// Puts CAN debug messages to stdout
        /// </summary>
        public const byte PCANTP_DEBUG_CAN = 1;
        /// <summary>
        /// The Channel is illegal or not available
        /// </summary>
        public const byte PCANTP_CHANNEL_UNAVAILABLE = 0;
        /// <summary>
        /// The Channel is available
        /// </summary>
        public const byte PCANTP_CHANNEL_AVAILABLE = 1;
        /// <summary>
        /// The Channel is valid, and is being used
        /// </summary>
        public const byte PCANTP_CHANNEL_OCCUPIED = 2;

        /// <summary>
        /// if set Flow Control frame shall not use the WT flow status value
        /// </summary>
        public const byte PCANTP_WFT_MAX_UNLIMITED = 0x00;

        /// <summary>
        /// An integer describing the Wait Frame Transmissions parameter. 
        /// </summary>
        public const byte PCANTP_WFT_MAX_DEFAULT = 0x10;

        /// <summary>
        /// Hide messages with type PCANTP_MESSAGE_INDICATION from CANTP_Read function
        /// </summary>
        public const byte PCANTP_MSG_PENDING_HIDE = 0x00;
        /// <summary>
        /// Show messages with type PCANTP_MESSAGE_INDICATION from CANTP_Read function
        /// </summary>
        public const byte PCANTP_MSG_PENDING_SHOW = 0x01;

        /// <summary>
        /// Uses CAN frame data optimization
        /// </summary>
        public const byte PCANTP_CAN_DATA_PADDING_NONE = 0x00;
        /// <summary>
        /// Uses CAN frame data padding (default, i.e. CAN DLC = 8)
        /// </summary>
        public const byte PCANTP_CAN_DATA_PADDING_ON = 0x01;

        /// <summary>
        /// Default value used if CAN data padding is enabled
        /// </summary>
        public const byte PCANTP_CAN_DATA_PADDING_VALUE = 0x55;

        /// <summary>
        /// Disable reception of unformatted (NON-ISO-TP) CAN frames (default):
        /// only ISO 15765 messages will be received
        /// </summary>
        public const byte PCANTP_CAN_UNSEGMENTED_OFF = 0x00;
        /// <summary>
        /// Enable reception of unformatted (NON-ISO-TP) CAN frames:
        /// received messages will be treated as either ISO 15765 or as an unformatted CAN frame
        /// </summary>
        public const byte PCANTP_CAN_UNSEGMENTED_ON = 0x01;
        /// <summary>
        /// Enable reception of unformatted (NON-ISO-TP) CAN frames:
        /// received messages will be treated as ISO 15765, unformatted CAN frame, or both (user will able to read fragmented CAN frames
        /// </summary>
        public const byte PCANTP_CAN_UNSEGMENTED_ALL_FRAMES = 0x02;

        /// <summary>
        /// Default priority for ISO-TP messages (only available for normal fixed, mixed and enhanced addressing)
        /// </summary>
        public const byte PCANTP_J1939_PRIORITY_DEFAULT = 0x06;

        /// <summary>
        /// Mask to retrieve the CAN ID type
        /// </summary>
        public const byte PCANTP_ID_CAN_MASK = 0x0F;
        /// <summary>
        /// Mask to check if the priority field is set
        /// </summary>
        public const byte PCANTP_ID_CAN_IS_PRIORITY_MASK = 0x10;
        /// <summary>
        /// Mask to retrieve the J1939 priority
        /// </summary>
        public const byte PCANTP_ID_CAN_PRIORITY_MASK = 0xE0;

        /// <summary>
        /// Constant defining that no CAN ID mapping exists
        /// </summary>
        public const uint CAN_ID_NO_MAPPING = 0xFFFFFFFF;

        /// <summary>
        /// max data length prior ISO 15765-2:2016 update 
        /// </summary>
        public const uint PCANTP_MESSAGE_MAX_LENGTH = 0xfff;
        #endregion

        #region PCAN ISO-TP API Implementation
        /// <summary>
        /// Initializes a PCANTP-Client based on a CANTP Channel
        /// </summary>
        /// <remarks>Only one PCANTP-Client can be initialized per CAN-Channel</remarks>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <param name="Baudrate">The CAN Hardware speed</param>
        /// <param name="HwType">NON PLUG-n-PLAY: The type of hardware and operation mode</param>
        /// <param name="IOPort">NON PLUG-n-PLAY: The I/O address for the parallel port</param>
        /// <param name="Interrupt">NON PLUG-n-PLAY: Interrupt number of the parallel port</param>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_Initialize")]
        public static extern TPCANTPStatus Initialize(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel,
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPBaudrate Baudrate,
            [MarshalAs(UnmanagedType.U1)]
            TPCANTPHWType HwType,
            UInt32 IOPort,
            UInt16 Interrupt);

        /// <summary>
        /// Initializes a PCANTP-Client based on a CANTP Channel
        /// </summary>
        /// <remarks>Only one PCANTP-Client can be initialized per CAN-Channel</remarks>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        public static TPCANTPStatus Initialize(
            TPCANTPHandle CanChannel,
            TPCANTPBaudrate Baudrate)
        {
            return Initialize(CanChannel, Baudrate, (TPCANTPHWType)0, 0, 0);
        }

        /// <summary>
        /// Initializes a PCANTP-Client based on a CANTP Channel with FD support
        /// </summary>
        /// <remarks>Only one PCANTP-Client can be initialized per CAN-Channel</remarks>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <param name="BitrateFD">The speed for the communication (FD bit rate string)</param>
        /// <remarks> See PCANTP_BR_* values
        /// Bit rate string must follow the following construction rules:
        /// * parameters and values must be separated by '='
        /// * Couples of Parameter/value must be separated by ','
        /// * Following Parameter must be filled out: f_clock, data_brp, data_sjw, data_tseg1, data_tseg2,
        ///   nom_brp, nom_sjw, nom_tseg1, nom_tseg2.
        /// * Following Parameters are optional (not used yet): data_ssp_offset, nom_samp</remarks>
        /// <example>f_clock_mhz=80, nom_brp=1, nom_tset1=63, nom_tseg2=16, nom_sjw=7, data_brp=4, data_tset1=12, data_tseg2=7, data_sjw=1</example>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_InitializeFD")]
        public static extern TPCANTPStatus InitializeFD(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel,
            TPCANTPBitrateFD BitrateFD);

        /// <summary>
        /// Uninitializes a PCANTP-Client initialized before
        /// </summary>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_Uninitialize")]
        public static extern TPCANTPStatus Uninitialize(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel);


        /// <summary>
        /// Resets the receive and transmit queues of a PCANTP-Client 
        /// </summary>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_Reset")]
        public static extern TPCANTPStatus Reset(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel);

        /// <summary>
        /// Gets information about the internal BUS status of a PCANTP-Channel.
        /// </summary>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_GetStatus")]
        public static extern TPCANTPStatus GetStatus(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel);


        /// <summary>
        /// Reads a PCANTP message from the receive queue of a PCANTP-Client
        /// </summary>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <param name="MessageBuffer">A TPCANTPMsg structure buffer to store the PUDS message</param>
        /// <param name="TimestampBuffer">"A TPCANTPTimestamp structure buffer to get 
        /// the reception time of the message. If this value is not desired, this parameter
        /// should be passed as NULL"</param>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_Read")]
        public static extern TPCANTPStatus Read(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel,
            out TPCANTPMsg MessageBuffer,
            out TPCANTPTimestamp TimestampBuffer);

        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_Read")]
        private static extern TPCANTPStatus Read(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel,
            out TPCANTPMsg MessageBuffer,
            IntPtr TimestampBuffer);
        /// <summary>
        /// Reads a PCANTP message from the receive queue of a PCANTP-Client
        /// </summary>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <param name="MessageBuffer">A TPCANTPMsg structure buffer to store the PUDS message</param>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        public static TPCANTPStatus Read(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel,
            out TPCANTPMsg MessageBuffer)
        {
            return Read(CanChannel, out MessageBuffer, IntPtr.Zero);
        }

        /// <summary>
        /// Transmits a PCANTP message
        /// </summary>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <param name="MessageBuffer">A TPCANTPMsg buffer with the message to be sent</param>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_Write")]
        public static extern TPCANTPStatus Write(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel,
            ref TPCANTPMsg MessageBuffer);

        /// <summary>
        /// Retrieves a PCANTP-Client value
        /// </summary>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <param name="Parameter">The TPCANTPParameter parameter to get</param>
        /// <param name="StringBuffer">Buffer for the parameter value</param>
        /// <param name="BufferLength">Size in bytes of the buffer</param>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_GetValue")]
        public static extern TPCANTPStatus GetValue(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel,
            [MarshalAs(UnmanagedType.U1)]
            TPCANTPParameter Parameter,
            StringBuilder StringBuffer,
            UInt32 BufferLength);
        /// <summary>
        /// Retrieves a PCANTP-Client value
        /// </summary>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <param name="Parameter">The TPCANTPParameter parameter to get</param>
        /// <param name="NumericBuffer">Buffer for the parameter value</param>
        /// <param name="BufferLength">Size in bytes of the buffer</param>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_GetValue")]
        public static extern TPCANTPStatus GetValue(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel,
            [MarshalAs(UnmanagedType.U1)]
            TPCANTPParameter Parameter,
            out UInt32 NumericBuffer,
            UInt32 BufferLength);
        /// <summary>
        /// Retrieves a PCANTP-Client value
        /// </summary>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <param name="Parameter">The TPCANTPParameter parameter to get</param>
        /// <param name="Buffer">Buffer for the parameter value</param>
        /// <param name="BufferLength">Size in bytes of the buffer</param>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_GetValue")]
        public static extern TPCANTPStatus GetValue(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel,
            [MarshalAs(UnmanagedType.U1)]
            TPCANTPParameter Parameter,
            [MarshalAs(UnmanagedType.LPArray)]
            [Out] Byte[] Buffer,
            UInt32 BufferLength);

        /// <summary>
        /// Configures or sets a PCANTP-Client value
        /// </summary>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <param name="Parameter">The TPCANTPParameter parameter to set</param>
        /// <param name="NumericBuffer">Buffer with the value to be set</param>
        /// <param name="BufferLength">Size in bytes of the buffer</param>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_SetValue")]
        public static extern TPCANTPStatus SetValue(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel,
            [MarshalAs(UnmanagedType.U1)]
            TPCANTPParameter Parameter,
            ref UInt32 NumericBuffer,
            UInt32 BufferLength);
        /// <summary>
        /// Configures or sets a PCANTP-Client value
        /// </summary>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <param name="Parameter">The TPCANTPParameter parameter to set</param>
        /// <param name="StringBuffer">Buffer with the value to be set</param>
        /// <param name="BufferLength">Size in bytes of the buffer</param>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_SetValue")]
        public static extern TPCANTPStatus SetValue(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel,
            [MarshalAs(UnmanagedType.U1)]
            TPCANTPParameter Parameter,
            [MarshalAs(UnmanagedType.LPStr, SizeParamIndex = 3)]
            string StringBuffer,
            UInt32 BufferLength);
        /// <summary>
        /// Configures or sets a PCANTP-Client value
        /// </summary>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <param name="Parameter">The TPCANTPParameter parameter to set</param>
        /// <param name="Buffer">Buffer with the value to be set</param>
        /// <param name="BufferLength">Size in bytes of the buffer</param>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_SetValue")]
        public static extern TPCANTPStatus SetValue(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel,
            [MarshalAs(UnmanagedType.U1)]
            TPCANTPParameter Parameter,
            [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 3)]
            Byte[] Buffer,
            UInt32 BufferLength);

        /// <summary>
        /// Adds a user-defined PCAN-TP mapping between CAN ID and Network Address Information
        /// </summary>
        /// <remark>
        /// Defining a mapping enables ISO-TP communication with 11BITS CAN ID or 
        /// with opened Addressing Formats (like PCANTP_FORMAT_NORMAL or PCANTP_FORMAT_EXTENDED).
        /// </remark>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <param name="canID">The CAN ID to map</param>
        /// <param name="canIDResponse">The CAN ID mapped response</param>
        /// <param name="canIdType">CAN ID Type (11 or 29 bits, see PCANTP_ID_CAN_XXX)</param>
        /// <param name="formatType">Addressing format (see PCANTP_FORMAT_XXX)</param>
        /// <param name="msgType">Message type (remote or diagnostic, see PCANTP_MESSAGE_XXX)</param>
        /// <param name="sourceAddr">Source Address</param>
        /// <param name="targetAddr">Target Address</param>
        /// <param name="targetType">Target Addressing Type (physical or functional, see PCANTP_ADDRESSING_XXX)</param>
        /// <param name="remoteAddr">Address extension or Remote address</param>
        /// <returns>A TPCANTPStatus code : PCANTP_ERROR_OK is returned on success, 
        /// PCANTP_ERROR_WRONG_PARAM states invalid Network Address Information parameters.</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_AddMapping")]
        public static extern TPCANTPStatus AddMapping(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel,
            uint canID,
            uint canIDResponse,
            [MarshalAs(UnmanagedType.U1)]
            TPCANTPIdType canIdType,
            [MarshalAs(UnmanagedType.U1)]
            TPCANTPFormatType formatType,
            [MarshalAs(UnmanagedType.U1)]
            TPCANTPMessageType msgType,
            byte sourceAddr,
            byte targetAddr,
            [MarshalAs(UnmanagedType.U1)]
            TPCANTPAddressingType targetType,
            byte remoteAddr);

        /// <summary>
        /// Removes a user-defined PCAN-TP mapping between CAN ID and Network Address Information
        /// </summary>
        /// <param name="CanChannel">A PCANTP Channel Handle representing a PCANTP-Client</param>
        /// <param name="canID">The mapped CAN ID to remove</param>
        /// <returns>A TPCANTPStatus code. PCANTP_ERROR_OK is returned on success</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_RemoveMapping")]
        public static extern TPCANTPStatus RemoveMapping(
            [MarshalAs(UnmanagedType.U2)]
            TPCANTPHandle CanChannel,
            uint canID);

        /// <summary>
        /// Returns a descriptive text of a given TPCANTPStatus error 
        /// code, in any desired language
        /// </summary>
        /// <remarks>The current languages available for translation are: 
        /// Neutral (0x00), German (0x07), English (0x09), Spanish (0x0A),
        /// Italian (0x10) and French (0x0C)</remarks>
        /// <param name="Error">A TPCANTPStatus error code</param>
        /// <param name="Language">Indicates a 'Primary language ID'</param>
        /// <param name="StringBuffer">Buffer for the text (must be at least 256 in length)</param>
        /// <returns>A TPCANTPStatus error code</returns>
        [DllImport("PCAN-ISO-TP.dll", EntryPoint = "CANTP_GetErrorText")]
        public static extern TPCANTPStatus GetErrorText(
            [MarshalAs(UnmanagedType.U4)]
            TPCANTPStatus Error,
            UInt16 Language,
            StringBuilder StringBuffer);

        #endregion
    }
    #endregion
}