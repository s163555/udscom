//  PCAN-UDS_2013.h
//
//  ~~~~~~~~~~~~
//
//  PCAN-UDS API 2013 (ISO 14229-1:2013)
//
//  ~~~~~~~~~~~~
//
//  ------------------------------------------------------------------
//  Last changed by:    $Author: Fabrice $
//  Last changed date:  $Date: 2023-09-01 14:21:09 +0200 (Fri, 01 Sep 2023) $
//
//  Language: ANSI-C
//  ------------------------------------------------------------------
//
//  Copyright (C) 2015  PEAK-System Technik GmbH, Darmstadt
//  more Info at http://www.peak-system.com
//
#ifndef __PCANUDS2013_H__
#define __PCANUDS2013_H__

#include "PCAN-ISO-TP_2016.h"

////////////////////////////////////////////////////////////
// Enums definition for UDS API
////////////////////////////////////////////////////////////

// Represents PUDS error codes (used in uds_status)
typedef enum _uds_errstatus {
    PUDS_ERRSTATUS_SERVICE_NO_MESSAGE = 1,
    PUDS_ERRSTATUS_SERVICE_TIMEOUT_CONFIRMATION,
    PUDS_ERRSTATUS_SERVICE_TIMEOUT_RESPONSE,
    PUDS_ERRSTATUS_RESET,
    PUDS_ERRSTATUS_ERROR_WAIT_FOR_P3_TIMING,
    PUDS_ERRSTATUS_SERVICE_ALREADY_PENDING,
    PUDS_ERRSTATUS_SERVICE_TX_ERROR,
    PUDS_ERRSTATUS_SERVICE_RX_ERROR,
    PUDS_ERRSTATUS_SERVICE_RX_OVERFLOW,
    PUDS_ERRSTATUS_MESSAGE_BUFFER_ALREADY_USED
}uds_errstatus;

// Defines constants used by the next enum: uds_status
#define PCANTP_STATUS_OFFSET_BUS    8
#define PCANTP_STATUS_OFFSET_NET    (PCANTP_STATUS_OFFSET_BUS + 5)
#define PCANTP_STATUS_OFFSET_INFO   (PCANTP_STATUS_OFFSET_NET + 5)
#define PCANTP_STATUS_OFFSET_UDS    (PCANTP_STATUS_OFFSET_INFO + 6)
#define PCANTP_STATUS_OFFSET_API    PCANTP_STATUS_OFFSET_UDS

// Represents the PCANTP & UDS error and status codes.
//
// Bits information:
//   32|  28|  24|  20|  16|  12|   8|   4|   0|
//     |    |    |    |    |    |    |    |    |
//      0000 0000 0000 0000 0000 0000 0000 0000
//     |    |    |    |    |         [0000 0000] => PCAN-ISO-TP API errors
//     |    |    |    |    |  [0 0000]           => CAN Bus status
//     |    |    |    | [00 000]                 => Networking message status
//     |    |    [0000 00]                       => PCAN-ISO-TP API extra information
//     |  [0 0000]                               => API Status
//     | [0]                                     => UDS Status
//     |[0]                                      => Reserved
//     [0]                                       => PCANBasic error flag
typedef enum _uds_status {
    PUDS_STATUS_OK = PCANTP_STATUS_OK,                                                     // No error
    PUDS_STATUS_NOT_INITIALIZED = PCANTP_STATUS_NOT_INITIALIZED,                           // Not Initialized.
    PUDS_STATUS_ALREADY_INITIALIZED = PCANTP_STATUS_ALREADY_INITIALIZED,                   // Already Initialized.
    PUDS_STATUS_NO_MEMORY = PCANTP_STATUS_NO_MEMORY,                                       // Could not obtain memory.
    PUDS_STATUS_OVERFLOW = PCANTP_STATUS_OVERFLOW,                                         // Input buffer overflow.
    PUDS_STATUS_NO_MESSAGE = PCANTP_STATUS_NO_MESSAGE,                                     // No message available.
    PUDS_STATUS_PARAM_INVALID_TYPE = PCANTP_STATUS_PARAM_INVALID_TYPE,                     // Wrong message parameters.
    PUDS_STATUS_PARAM_INVALID_VALUE = PCANTP_STATUS_PARAM_INVALID_VALUE,                   // Wrong message parameters.
    PUDS_STATUS_MAPPING_NOT_INITIALIZED = PCANTP_STATUS_MAPPING_NOT_INITIALIZED,           // Mapping not initialized.
    PUDS_STATUS_MAPPING_INVALID = PCANTP_STATUS_MAPPING_INVALID,                           // Mapping parameters are invalid.
    PUDS_STATUS_MAPPING_ALREADY_INITIALIZED = PCANTP_STATUS_MAPPING_ALREADY_INITIALIZED,   // Mapping already defined.
    PUDS_STATUS_PARAM_BUFFER_TOO_SMALL = PCANTP_STATUS_PARAM_BUFFER_TOO_SMALL,             // Buffer is too small.
    PUDS_STATUS_QUEUE_TX_FULL = PCANTP_STATUS_QUEUE_TX_FULL,                               // Tx queue is full.
    PUDS_STATUS_LOCK_TIMEOUT = PCANTP_STATUS_LOCK_TIMEOUT,                                 // Failed to get an access to the internal lock.
    PUDS_STATUS_HANDLE_INVALID = PCANTP_STATUS_HANDLE_INVALID,                             // Invalid cantp_handle.
    PUDS_STATUS_UNKNOWN = PCANTP_STATUS_UNKNOWN,                                           // Unknown/generic error.
    // Bus status flags (bits [8..11])
    PUDS_STATUS_FLAG_BUS_LIGHT = PCANTP_STATUS_FLAG_BUS_LIGHT,                             // Channel is in BUS - LIGHT error state.
    PUDS_STATUS_FLAG_BUS_HEAVY = PCANTP_STATUS_FLAG_BUS_HEAVY,                             // Channel is in BUS - HEAVY error state.
    PUDS_STATUS_FLAG_BUS_WARNING = PCANTP_STATUS_FLAG_BUS_WARNING,                         // Channel is in BUS - HEAVY error state.
    PUDS_STATUS_FLAG_BUS_PASSIVE = PCANTP_STATUS_FLAG_BUS_PASSIVE,                         // Channel is error passive state.
    PUDS_STATUS_FLAG_BUS_OFF = PCANTP_STATUS_FLAG_BUS_OFF,                                 // Channel is in BUS - OFF error state.
    PUDS_STATUS_FLAG_BUS_ANY = PCANTP_STATUS_FLAG_BUS_ANY,                                 // Mask for all bus errors.
    PUDS_STATUS_FLAG_NETWORK_RESULT = PCANTP_STATUS_FLAG_NETWORK_RESULT,                   // This flag states if one of the following network errors occurred with the fetched message.
    // Network status (bits [13..17])
    PUDS_STATUS_NETWORK_TIMEOUT_A = PCANTP_STATUS_NETWORK_TIMEOUT_A,                       // Timeout occurred between 2 frames transmission (sender and receiver side).
    PUDS_STATUS_NETWORK_TIMEOUT_Bs = PCANTP_STATUS_NETWORK_TIMEOUT_Bs,                     // Sender side timeout while waiting for flow control frame.
    PUDS_STATUS_NETWORK_TIMEOUT_Cr = PCANTP_STATUS_NETWORK_TIMEOUT_Cr,                     // Receiver side timeout while waiting for consecutive frame.
    PUDS_STATUS_NETWORK_WRONG_SN = PCANTP_STATUS_NETWORK_WRONG_SN,                         // Unexpected sequence number.
    PUDS_STATUS_NETWORK_INVALID_FS = PCANTP_STATUS_NETWORK_INVALID_FS,                     // Invalid or unknown FlowStatus.
    PUDS_STATUS_NETWORK_UNEXP_PDU = PCANTP_STATUS_NETWORK_UNEXP_PDU,                       // Unexpected protocol data unit.
    PUDS_STATUS_NETWORK_WFT_OVRN = PCANTP_STATUS_NETWORK_WFT_OVRN,                         // Reception of flow control WAIT frame that exceeds the maximum counter defined by PUDS_PARAMETER_WFT_MAX.
    PUDS_STATUS_NETWORK_BUFFER_OVFLW = PCANTP_STATUS_NETWORK_BUFFER_OVFLW,                 // Buffer on the receiver side cannot store the data length (server side only).
    PUDS_STATUS_NETWORK_ERROR = PCANTP_STATUS_NETWORK_ERROR,                               // General error.
    PUDS_STATUS_NETWORK_IGNORED = PCANTP_STATUS_NETWORK_IGNORED,                           // Message was invalid and ignored.
    PUDS_STATUS_NETWORK_TIMEOUT_Ar = PCANTP_STATUS_NETWORK_TIMEOUT_Ar,                     // Receiver side timeout while transmitting.
    PUDS_STATUS_NETWORK_TIMEOUT_As = PCANTP_STATUS_NETWORK_TIMEOUT_As,                     // Sender side timeout while transmitting.
    // Extra information flags
    PUDS_STATUS_CAUTION_INPUT_MODIFIED = PCANTP_STATUS_CAUTION_INPUT_MODIFIED,             // Input was modified by the API.
    PUDS_STATUS_CAUTION_DLC_MODIFIED = PCANTP_STATUS_CAUTION_DLC_MODIFIED,                 // DLC value of the input was modified by the API.
    PUDS_STATUS_CAUTION_DATA_LENGTH_MODIFIED = PCANTP_STATUS_CAUTION_DATA_LENGTH_MODIFIED, // Data Length value of the input was modified by the API.
    PUDS_STATUS_CAUTION_FD_FLAG_MODIFIED = PCANTP_STATUS_CAUTION_FD_FLAG_MODIFIED,         // FD flags of the input was modified by the API.
    PUDS_STATUS_CAUTION_RX_QUEUE_FULL = PCANTP_STATUS_CAUTION_RX_QUEUE_FULL,               // Receive queue is full.
    PUDS_STATUS_CAUTION_BUFFER_IN_USE = PCANTP_STATUS_CAUTION_BUFFER_IN_USE,               // Buffer is used by another thread or API.
    // Lower API status code: see also PCANTP_STATUS_xx macros
    PUDS_STATUS_FLAG_PCAN_STATUS = PCANTP_STATUS_FLAG_PCAN_STATUS,                         // PCAN error flag, remove flag to get a usable PCAN error/status code (cf. PCANBasic API).
    // Masks to merge/retrieve different status by type in a uds_status
    PUDS_STATUS_MASK_ERROR = PCANTP_STATUS_MASK_ERROR,                                     // Filter general error.
    PUDS_STATUS_MASK_BUS = PCANTP_STATUS_MASK_BUS,                                         // Filter bus error.
    PUDS_STATUS_MASK_ISOTP_NET = PCANTP_STATUS_MASK_ISOTP_NET,                             // Filter network error.
    PUDS_STATUS_MASK_INFO = PCANTP_STATUS_MASK_INFO,                                       // Filter extra information.
    PUDS_STATUS_MASK_PCAN = PCANTP_STATUS_MASK_PCAN,                                       // Filter PCAN error (encapsulated PCAN-Basic status).
    // UDS service status.
    PUDS_STATUS_FLAG_UDS_ERROR = 0x20 << PCANTP_STATUS_OFFSET_UDS,                                                                                      // UDS error flag.
    PUDS_STATUS_MASK_UDS_ERROR = 0x3f << PCANTP_STATUS_OFFSET_UDS,                                                                                      // Filter UDS error.
    PUDS_STATUS_SERVICE_NO_MESSAGE = PUDS_STATUS_FLAG_UDS_ERROR | (PUDS_ERRSTATUS_SERVICE_NO_MESSAGE << PCANTP_STATUS_OFFSET_UDS),                      // UDS No message avaiable.
    PUDS_STATUS_SERVICE_TIMEOUT_CONFIRMATION = PUDS_STATUS_FLAG_UDS_ERROR | (PUDS_ERRSTATUS_SERVICE_TIMEOUT_CONFIRMATION << PCANTP_STATUS_OFFSET_UDS),  // Timeout while waiting message confirmation (loopback).
    PUDS_STATUS_SERVICE_TIMEOUT_RESPONSE = PUDS_STATUS_FLAG_UDS_ERROR | (PUDS_ERRSTATUS_SERVICE_TIMEOUT_RESPONSE << PCANTP_STATUS_OFFSET_UDS),          // Timeout while waiting request message response.
    PUDS_STATUS_RESET = PUDS_STATUS_FLAG_UDS_ERROR | (PUDS_ERRSTATUS_RESET << PCANTP_STATUS_OFFSET_UDS),                                                // UDS reset error.
    PUDS_STATUS_ERROR_WAIT_FOR_P3_TIMING = PUDS_STATUS_FLAG_UDS_ERROR | (PUDS_ERRSTATUS_ERROR_WAIT_FOR_P3_TIMING << PCANTP_STATUS_OFFSET_UDS),          // UDS wait for P3 timing error.
    PUDS_STATUS_SERVICE_ALREADY_PENDING = PUDS_STATUS_FLAG_UDS_ERROR | (PUDS_ERRSTATUS_SERVICE_ALREADY_PENDING << PCANTP_STATUS_OFFSET_UDS),            // A message with the same service identifier is already pending in the reception queue, user must read response for his previous request before or clear the reception queues with UDS_Reset_2013.
    PUDS_STATUS_SERVICE_TX_ERROR = PUDS_STATUS_FLAG_UDS_ERROR | (PUDS_ERRSTATUS_SERVICE_TX_ERROR << PCANTP_STATUS_OFFSET_UDS),                          // An error occurred during the transmission of the UDS request message.
    PUDS_STATUS_SERVICE_RX_ERROR = PUDS_STATUS_FLAG_UDS_ERROR | (PUDS_ERRSTATUS_SERVICE_RX_ERROR << PCANTP_STATUS_OFFSET_UDS),                          // An error occurred during the reception of the UDS response message.
    PUDS_STATUS_SERVICE_RX_OVERFLOW = PUDS_STATUS_FLAG_UDS_ERROR | (PUDS_ERRSTATUS_SERVICE_RX_OVERFLOW << PCANTP_STATUS_OFFSET_UDS),                    // Service received more messages than input buffer expected.
    PUDS_STATUS_MESSAGE_BUFFER_ALREADY_USED = PUDS_STATUS_FLAG_UDS_ERROR | (PUDS_ERRSTATUS_MESSAGE_BUFFER_ALREADY_USED << PCANTP_STATUS_OFFSET_UDS),    // Given message buffer was already used, user must release buffer with UDS_MsgFree_2013 before reusing it.
} uds_status;

// List of parameters handled by PCAN-UDS
//  Note: PCAN-ISO-TP and PCAN-Basic parameters (PCANTP_PARAMETER_xxx, PCAN_PARAM_xxx) are compatible via casting.
typedef enum _uds_parameter {
    PUDS_PARAMETER_API_VERSION = 0x201,                                     // (R/ ) uint8_t[]      :   PCAN-UDS API version parameter
    PUDS_PARAMETER_DEBUG = 0x203,                                           // (R/W) uint8_t        :   data describing the debug mode  (use PUDS_DEBUG_LVL_ values)
    PUDS_PARAMETER_RECEIVE_EVENT = 0x204,                                   // (R/W) uintptr_t      :   data is pointer to a HANDLE created by CreateEvent function
    PUDS_PARAMETER_SERVER_ADDRESS = 0x207,                                  // (R/W) uint16_t       :   ISO-TP physical address
    PUDS_PARAMETER_SESSION_INFO = 0x209,                                    // (R/W) uds_sessioninfo:   ECU Session information
    PUDS_PARAMETER_TIMEOUT_REQUEST = 0x20A,                                 // (R/W) uint32_t       :   max time in milliseconds to wait to receive the request loopback
    PUDS_PARAMETER_TIMEOUT_RESPONSE = 0x20B,                                // (R/W) uint32_t       :   max time in milliseconds to wait to receive the message response indication
    PUDS_PARAMETER_AUTOMATIC_TESTER_PRESENT = 0x20C,                        // (R/W) bool           :   Automatic tester present. Default value: true
    PUDS_PARAMETER_USE_NO_RESPONSE_AUTOMATIC_TESTER_PRESENT = 0x213,        // (R/W) bool           :   Use no response flag for automatic tester present. Default value: true
    PUDS_PARAMETER_AUTO_P3_TIMING_MANAGEMENT = 0x20D,                       // (R/W) bool           :   Wait for P3 timing. Default value: true (ISO-14229-2_2013 §8.3 Minimum time between client request messages, p.36)

    PUDS_PARAMETER_LISTENED_ADDRESSES = 0x210,                              // (R/ ) uint16_t[size] :   List of pysical addresses to listen to.
                                                                            //                          NOTE: for the parameter PUDS_PARAMETER_LISTENED_ADDRESSES the size of the array must
                                                                            //                          be specified in the "buffer_size" parameter of the "UDS_GetValue_2013" function
    PUDS_PARAMETER_ADD_LISTENED_ADDRESS = 0x211,                            // ( /W) uint16_t       :   Add a listening address to the list of physical addresses to listen to
    PUDS_PARAMETER_REMOVE_LISTENED_ADDRESS = 0x212,                         // ( /W) uint16_t       :   Remove a listening address from the list of physical addresses to listen to

    PUDS_PARAMETER_CHANNEL_CONDITION = PCANTP_PARAMETER_CHANNEL_CONDITION,  // (R/ ) uint8_t        :   data describing the condition of a channel.
    PUDS_PARAMETER_CAN_TX_DL = PCANTP_PARAMETER_CAN_TX_DL,                  // (R/W) uint8_t        :   data stating the default DLC to use when transmitting messages with CAN FD
    PUDS_PARAMETER_CAN_DATA_PADDING = PCANTP_PARAMETER_CAN_DATA_PADDING,    // (R/W) uint8_t        :   data stating if CAN frame DLC uses padding or not
    PUDS_PARAMETER_CAN_PADDING_VALUE = PCANTP_PARAMETER_CAN_PADDING_VALUE,  // (R/W) uint8_t        :   data stating the value used for CAN data padding
    PUDS_PARAMETER_J1939_PRIORITY = PCANTP_PARAMETER_J1939_PRIORITY,        // (R/W) uint8_t        :   data stating the default priority value for normal fixed, mixed and enhanced addressing (default=6)
    PUDS_PARAMETER_BLOCK_SIZE = PCANTP_PARAMETER_BLOCK_SIZE,                // (R/W) uint8_t        :   data describing the block size parameter (BS)
    PUDS_PARAMETER_SEPARATION_TIME = PCANTP_PARAMETER_SEPARATION_TIME,      // (R/W) uint8_t        :   data describing the seperation time parameter (STmin)
    PUDS_PARAMETER_WFT_MAX = PCANTP_PARAMETER_WFT_MAX,                      // (R/W) uint8_t[4]     :   data describing the Wait Frame Transmissions parameter.
    PUDS_PARAMETER_ISO_TIMEOUTS = PCANTP_PARAMETER_ISO_TIMEOUTS,            // (R/W) uint8_t        :   data to set predefined ISO values for timeouts (see PCANTP_ISO_TIMEOUTS_*).
    PUDS_PARAMETER_RESET_HARD = PCANTP_PARAMETER_RESET_HARD,                // ( /W) uint8_t        :   data stating to clear Rx/Tx queues and CAN controller (channel is unitialized and re-initialized but settings and mappings are kept)

    PUDS_PARAMETER_HARDWARE_NAME = PCAN_HARDWARE_NAME,                      // PCAN hardware name parameter
    PUDS_PARAMETER_DEVICE_ID = PCAN_DEVICE_ID,                              // PCAN-USB device identifier parameter
    PUDS_PARAMETER_DEVICE_NUMBER = PCAN_DEVICE_ID,                          // deprecated use PCANTP_PARAMETER_DEVICE_ID instead
    PUDS_PARAMETER_CONTROLLER_NUMBER = PCAN_CONTROLLER_NUMBER,              // CAN-Controller number of a PCAN-Channel
    PUDS_PARAMETER_CHANNEL_FEATURES = PCAN_CHANNEL_FEATURES                 // Capabilities of a PCAN device (FEATURE_***)
} uds_parameter;

// Represents type and flags for a usd_msg
typedef enum _uds_msgtype {
    PUDS_MSGTYPE_USDT = 0,                       // Unacknowledge Segmented Data Transfert (ISO-TP message)
    PUDS_MSGTYPE_UUDT = 1,                       // Unacknowledge Unsegmented Data Transfert (msg_physical will use a single CAN/CAN-FD frame without ISO-TP protocol control information)
    PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE = 2,  // ECU(s) shall not reply to the request on positive response.
    PUDS_MSGTYPE_FLAG_LOOPBACK = 4,              // Message is a loopback
    PUDS_MSGTYPE_MASK_TYPE = 0x01,               // Mask to get the type (USDT or UUDT)
} uds_msgtype;

// Represents ISO-TP network addressing information supported in UDS
typedef enum _uds_msgprotocol {
    PUDS_MSGPROTOCOL_NONE = 0x00,                          // Non ISO-TP frame (Unacknowledge Unsegmented Data Transfer)
    PUDS_MSGPROTOCOL_ISO_15765_2_11B_EXTENDED = 0x07,      // ISO-TP 11 bits Extended addressing (mapping required)
    PUDS_MSGPROTOCOL_ISO_15765_2_11B_NORMAL = 0x01,        // ISO-TP 11 bits Normal addressing (mapping required)
    PUDS_MSGPROTOCOL_ISO_15765_2_11B_REMOTE = 0x02,        // ISO-TP 11 bits Mixed addressing (mapping required)
    PUDS_MSGPROTOCOL_ISO_15765_2_29B_EXTENDED = 0x08,      // ISO-TP 29 bits Extended addressing (mapping required)
    PUDS_MSGPROTOCOL_ISO_15765_2_29B_FIXED_NORMAL = 0x03,  // ISO-TP 29 bits Fixed Normal addressing
    PUDS_MSGPROTOCOL_ISO_15765_2_29B_NORMAL = 0x06,        // ISO-TP 29 bits Normal addressing (mapping required)
    PUDS_MSGPROTOCOL_ISO_15765_2_29B_REMOTE = 0x04,        // ISO-TP 29 bits Mixed addressing
    PUDS_MSGPROTOCOL_ISO_15765_3_29B_ENHANCED = 0x05,      // ISO-TP Enhanced addressing
} uds_msgprotocol;

// Represents UDS negative response codes (see ISO 14229-1:2013 §A.1 Negative response codes p.325)
typedef enum _uds_nrc
{
    PUDS_NRC_PR = 0x00,       // Positive Response
    PUDS_NRC_GR = 0x10,       // General Reject
    PUDS_NRC_SNS = 0x11,      // Service Not Supported
    PUDS_NRC_SFNS = 0x12,     // Sub Function Not Supported
    PUDS_NRC_IMLOIF = 0x13,   // Incorrect Message Length Or Invalid Format
    PUDS_NRC_RTL = 0x14,      // Response Too Long
    PUDS_NRC_BRR = 0x21,      // Busy Repeat Request
    PUDS_NRC_CNC = 0x22,      // Conditions Not Correct
    PUDS_NRC_RSE = 0x24,      // Request Sequence Error
    PUDS_NRC_NRFSC = 0x25,    // No Response From Subnet Component
    PUDS_NRC_FPEORA = 0x26,   // Failure Prevents Execution Of Requested Action
    PUDS_NRC_ROOR = 0x31,     // Request Out Of Range
    PUDS_NRC_SAD = 0x33,      // Security Access Denied
    PUDS_NRC_AR = 0x34,       // Authentication Required
    PUDS_NRC_IK = 0x35,       // Invalid Key
    PUDS_NRC_ENOA = 0x36,     // Exceeded Number Of Attempts
    PUDS_NRC_RTDNE = 0x37,    // Required Time Delay Not Expired
    PUDS_NRC_SDTR = 0x38,     // Secure Data Transmission Required
    PUDS_NRC_SDTNA = 0x39,    // Secure Data Transmission Not Allowed
    PUDS_NRC_SDTF = 0x3A,     // Secure Data Verification Failed
    PUDS_NRC_CVFITP = 0x50,   // Certificate Verification Failed Invalid Time Period
    PUDS_NRC_CVFISIG = 0x51,  // Certificate Verification Failed Invalid SIGnature
    PUDS_NRC_CVFICOT = 0x52,  // Certificate Verification Failed Invalid Chain of Trust
    PUDS_NRC_CVFIT = 0x53,    // Certificate Verification Failed Invalid Type
    PUDS_NRC_CVFIF = 0x54,    // Certificate Verification Failed Invalid Format
    PUDS_NRC_CVFIC = 0x55,    // Certificate Verification Failed Invalid Content
    PUDS_NRC_CVFISCP = 0x56,  // Certificate Verification Failed Invalid SCoPe
    PUDS_NRC_CVFICERT = 0x57, // Certificate Verification Failed Invalid CERTificate(revoked)
    PUDS_NRC_OVF = 0x58,      // Ownership Verification Failed
    PUDS_NRC_CCF = 0x59,      // Challenge Calculation Failed
    PUDS_NRC_SARF = 0x5A,     // Setting Access Rights Failed
    PUDS_NRC_SKCDF = 0x5B,    // Session Key Creation / Derivation Failed
    PUDS_NRC_CDUF = 0x5C,     // Configuration Data Usage Failed
    PUDS_NRC_DAF = 0x5D,      // DeAuthentication Failed
    PUDS_NRC_UDNA = 0x70,     // Upload Download Not Accepted
    PUDS_NRC_TDS = 0x71,      // Transfer Data Suspended
    PUDS_NRC_GPF = 0x72,      // General Programming Failure
    PUDS_NRC_WBSC = 0x73,     // Wrong Block Sequence Counter
    PUDS_NRC_RCRRP = 0x78,    // Request Correctly Received - Response Pending
    PUDS_NRC_SFNSIAS = 0x7E,  // Sub Function Not Supported In Active Session
    PUDS_NRC_SNSIAS = 0x7F,   // Service Not Supported In Active Session
    PUDS_NRC_RPMTH = 0x81,    // RPM Too High
    PUDS_NRC_RPMTL = 0x82,    // RPM Too Low
    PUDS_NRC_EIR = 0x83,      // Engine Is Running
    PUDS_NRC_EINR = 0x84,     // Engine Is Not Running
    PUDS_NRC_ERTTL = 0x85,    // Engine Run Time Too Low
    PUDS_NRC_TEMPTH = 0x86,   // TEMPerature Too High
    PUDS_NRC_TEMPTL = 0x87,   // TEMPerature Too Low
    PUDS_NRC_VSTH = 0x88,     // Vehicle Speed Too High
    PUDS_NRC_VSTL = 0x89,     // Vehicle Speed Too Low
    PUDS_NRC_TPTH = 0x8A,     // Throttle / Pedal Too High
    PUDS_NRC_TPTL = 0x8B,     // Throttle / Pedal Too Low
    PUDS_NRC_TRNIN = 0x8C,    // Transmission Range Not In Neutral
    PUDS_NRC_TRNIG = 0x8D,    // Transmission Range Not In Gear
    PUDS_NRC_BSNC = 0x8F,     // Brake Switch(es) Not Closed(brake pedal not pressed or not applied)
    PUDS_NRC_SLNIP = 0x90,    // Shifter Lever Not In Park
    PUDS_NRC_TCCL = 0x91,     // Torque Converter Clutch Locked
    PUDS_NRC_VTH = 0x92,      // Voltage Too High
    PUDS_NRC_VTL = 0x93,      // Voltage Too Low
    PUDS_NRC_RTNA = 0x94      // Resource Temporarily Not Available
} uds_nrc;

// PUDS ISO_15765_4 11 bit CAN ID definitions
#define PUDS_ISO_15765_4_CAN_ID_FUNCTIONAL_REQUEST      0x7DF        // CAN ID for functionally addressed request messages sent by external test equipment
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_REQUEST_1      0x7E0        // physical request CAN ID from external test equipment to ECU #1
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_RESPONSE_1     0x7E8        // physical response CAN ID from ECU #1 to external test equipment
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_REQUEST_2      0x7E1        // physical request CAN ID from external test equipment to ECU #2
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_RESPONSE_2     0x7E9        // physical response CAN ID from ECU #2 to external test equipment
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_REQUEST_3      0x7E2        // physical request CAN ID from external test equipment to ECU #3
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_RESPONSE_3     0x7EA        // physical response CAN ID from ECU #3 to external test equipment
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_REQUEST_4      0x7E3        // physical request CAN ID from external test equipment to ECU #4
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_RESPONSE_4     0x7EB        // physical response CAN ID from ECU #4 to external test equipment
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_REQUEST_5      0x7E4        // physical request CAN ID from external test equipment to ECU #5
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_RESPONSE_5     0x7EC        // physical response CAN ID from ECU #5 to external test equipment
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_REQUEST_6      0x7E5        // physical request CAN ID from external test equipment to ECU #6
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_RESPONSE_6     0x7ED        // physical response CAN ID from ECU #6 to external test equipment
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_REQUEST_7      0x7E6        // physical request CAN ID from external test equipment to ECU #7
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_RESPONSE_7     0x7EE        // physical response CAN ID from ECU #7 to external test equipment
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_REQUEST_8      0x7E7        // physical request CAN ID from external test equipment to ECU #8
#define PUDS_ISO_15765_4_CAN_ID_PHYSICAL_RESPONSE_8     0x7EF        // physical response CAN ID from ECU #8 to external test equipment

// PUDS ISO_15765_4 address definitions
#define PUDS_ISO_15765_4_ADDR_TEST_EQUIPMENT            0xF1     // external test equipment
#define PUDS_ISO_15765_4_ADDR_OBD_FUNCTIONAL            0x33     // OBD funtional system
#define PUDS_ISO_15765_4_ADDR_ECU_1                     0x01     // ECU 1
#define PUDS_ISO_15765_4_ADDR_ECU_2                     0x02     // ECU 2
#define PUDS_ISO_15765_4_ADDR_ECU_3                     0x03     // ECU 3
#define PUDS_ISO_15765_4_ADDR_ECU_4                     0x04     // ECU 4
#define PUDS_ISO_15765_4_ADDR_ECU_5                     0x05     // ECU 5
#define PUDS_ISO_15765_4_ADDR_ECU_6                     0x06     // ECU 6
#define PUDS_ISO_15765_4_ADDR_ECU_7                     0x07     // ECU 7
#define PUDS_ISO_15765_4_ADDR_ECU_8                     0x08     // ECU 8

// PUDS parameter values
#define PUDS_TIMEOUT_REQUEST                        10000   // Default maximum timeout in milliseconds for UDS transmit confirmation
#define PUDS_TIMEOUT_RESPONSE                       10000   // Default maximum timeout in milliseconds for UDS response reception
#define PUDS_SERVER_ADDR_FLAG_ENHANCED_ISO_15765_3  0x1000  // Flag stating that the address is defined as a ISO-15765-3 address
#define PUDS_SERVER_ADDR_MASK_ENHANCED_ISO_15765_3  0x07FF  // Mask used for the ISO-15765-3 enhanced addresses
#define PUDS_CHANNEL_UNAVAILABLE                    0x00    // The Channel is illegal or not available
#define PUDS_CHANNEL_AVAILABLE                      0x01    // The Channel is available
#define PUDS_CHANNEL_OCCUPIED                       0x02    // The Channel is valid, and is being used
#define PUDS_CAN_DATA_PADDING_NONE                  0x00    // Uses CAN frame data optimization
#define PUDS_CAN_DATA_PADDING_ON                    0x01    // Uses CAN frame data padding (default, i.e. CAN DLC = 8)
#define PUDS_CAN_DATA_PADDING_VALUE                 0x55    // Default value used if CAN data padding is enabled

#define PUDS_P2CAN_SERVER_MAX_DEFAULT               50      // Default server performance requirement in ms (See ISO_14229-2_2013 §7.2 table 4)
#define PUDS_P2CAN_ENHANCED_SERVER_MAX_DEFAULT      5000    // Enhanced server performance requirement in ms (See ISO_14229-2_2013 §7.2 table 4)
#define PUDS_S3_CLIENT_TIMEOUT_RECOMMENDED          2000    // Recommended S3 client timeout in ms (See ISO_14229-2_2013 §7.5 table 5)
#define PUDS_P3CAN_DEFAULT                          PUDS_P2CAN_SERVER_MAX_DEFAULT    // Default P3 timing parameter in ms (See ISO_14229-2_2013 §7.2 table 4)

#define PUDS_DEBUG_LVL_NONE         0x00    // Disable debug messages (default)
#define PUDS_DEBUG_LVL_ERROR        0xF1    // Enable debug messages (only errors)
#define PUDS_DEBUG_LVL_WARNING      0xF2    // Enable debug messages (only warnings, errors)
#define PUDS_DEBUG_LVL_INFORMATION  0xF3    // Enable debug messages (only informations, warnings, errors)
#define PUDS_DEBUG_LVL_NOTICE       0xF4    // Enable debug messages (only notices, informations, warnings, errors)
#define PUDS_DEBUG_LVL_DEBUG        0xF5    // Enable debug messages (only debug, notices, informations, warnings, errors)
#define PUDS_DEBUG_LVL_TRACE        0xF6    // Enable all debug messages

#define PUDS_ONLY_PREPARE_REQUEST PCANTP_HANDLE_NONEBUS // Option that can be used as channel identifier in UDS_Svc* functions: only prepare uds_msg structure and do not send it

// PUDS message data flags
#define PUDS_FLAG_SUPPRESS_POSITIVE_RESPONSE 0x80	// Flag to suppress positive response message

////////////////////////////////////////////////////////////
// Message definitions
////////////////////////////////////////////////////////////

#pragma pack(push, 8)

// Represents a UDS Network Addressing Information
typedef struct _uds_netaddrinfo {
    uds_msgprotocol protocol;           // communication protocol
    cantp_isotp_addressing target_type; // ISO-TP target type
    uint16_t source_addr;               // source address
    uint16_t target_addr;               // target address
    uint8_t extension_addr;             // extension address
} uds_netaddrinfo;

// Represents the diagnostic session's information of a server
typedef struct _uds_sessioninfo {
    uds_netaddrinfo nai;                        // Network address information
    cantp_can_msgtype can_msg_type;             // Types and flags of the CAN/CAN-FD frames
    uint8_t session_type;                       // Activated Diagnostic Session (see PUDS_SVC_PARAM_DSC_xxx values)
    uint16_t timeout_p2can_server_max;          // Default P2Can_Server_Max timing for the activated session (resolution: 1ms)
    uint16_t timeout_enhanced_p2can_server_max; // Enhanced P2Can_Server_Max timing for the activated session (resolution: 10ms)
    uint16_t s3_client_ms;                      // Time between 2 TesterPresents
} uds_sessioninfo;

// Represents the configuration of a PUDS message
typedef struct _uds_msgconfig {
    uds_msgtype type;               // structure specific flags
    uds_netaddrinfo nai;            // Network Addressing Information
    uint32_t can_id;                // (optional) CAN ID (for configuration use either nai or m_can_id)
    cantp_can_msgtype can_msgtype;  // optional flags for the CAN layer (29 bits CAN-ID, FD, BRS)
    uint8_t can_tx_dlc;             // Default CAN DLC value to use with segmented messages
} uds_msgconfig;

// Represents a mapping between an UDS Network Addressing Information and a CAN ID.
typedef struct _uds_mapping {
    uintptr_t uid;                  // Mapping's unique ID
    uint32_t can_id;                // CAN ID mapped to the Network Address Information
    uint32_t can_id_flow_ctrl;      // CAN ID used for the flow control frame (formerly 'can_id_resp')
    cantp_can_msgtype can_msgtype;  // CAN frame msgtype (only PCANTP_CAN_MSGTYPE_STANDARD or PCANTP_CAN_MSGTYPE_EXTENDED is mandatory)
    uint8_t can_tx_dlc;             // Default CAN DLC value to use with segmented messages
    uds_netaddrinfo nai;            // Network Addressing Information
} uds_mapping;
#define PUDS_MAPPING_FLOW_CTRL_NONE -1  // Mapping does not require a Flow Control frame.

// Provides accessors to the corresponding data in the cantp_msg
typedef struct _uds_msgaccess {
    uint8_t* service_id;    // Pointer to the Service ID in message's data.
    uint8_t* param;         // Pointer to the first parameter in message's data.
    uint8_t* nrc;           // Pointer to the Negative Response Code (see uds_nrc enumeration) in message's data (NULL on positive response).
} uds_msgaccess;

// Represents the content of a UDS message.
typedef struct _uds_msg {
    uds_msgtype type;       // structure specific flags
    uds_msgaccess links;    // quick accessors to the cantp_msg data
    cantp_msg msg;          // the PCANTP message encapsulating the UDS data
} uds_msg;

#pragma pack(pop)

////////////////////////////////////////////////////////////
// PCAN-UDS API: Core function declarations
////////////////////////////////////////////////////////////

#ifdef __cplusplus
// extra operator for uds_status
inline uds_status  operator|   (uds_status a, uds_status b) { return static_cast<uds_status>(static_cast<int>(a) | static_cast<int>(b)); }
inline uds_status  operator&   (uds_status a, uds_status b) { return static_cast<uds_status>(static_cast<int>(a)& static_cast<int>(b)); }
inline uds_status& operator&=  (uds_status& a, uds_status b) { return (a = (a & b)); }
inline uds_status  operator~   (uds_status a) { return static_cast<uds_status>(~static_cast<int>(a)); }

// extra operator for cantp_statustype
inline uds_msgtype  operator|   (uds_msgtype a, uds_msgtype b) { return static_cast<uds_msgtype>(static_cast<int>(a) | static_cast<int>(b)); }
inline uds_msgtype& operator|=  (uds_msgtype& a, uds_msgtype b) { return (a = (a | b)); }
inline uds_msgtype  operator&   (uds_msgtype a, uds_msgtype b) { return static_cast<uds_msgtype>(static_cast<int>(a)& static_cast<int>(b)); }
inline uds_msgtype& operator&=  (uds_msgtype& a, uds_msgtype b) { return (a = (a & b)); }
inline uds_msgtype  operator~   (uds_msgtype a) { return static_cast<uds_msgtype>(~static_cast<int>(a)); }

extern "C" {
#define _DEF_ARG =0
#define _DEF_ARG_NULL =nullptr
#ifndef _DEF_ARG_HW
#define _DEF_ARG_HW = static_cast<cantp_hwtype>(0)
#endif
#define _DEF_ARG_UDS_OK = PUDS_STATUS_OK
#define _DEF_ARG_PUDS_SVC_PARAM_TP_ZSUBF = PUDS_SVC_PARAM_TP_ZSUBF
#else
#define _DEF_ARG
#define _DEF_ARG_NULL
#define _DEF_ARG_HW
#define _DEF_ARG_UDS_OK
#define _DEF_ARG_MSGTYPE
#define _DEF_ARG_PUDS_SVC_PARAM_TP_ZSUBF
#endif


/// <summary>
/// Initializes a PUDS channel based on a PCANTP channel handle (without CAN-FD support)
/// </summary>
/// <remarks>Only one PUDS channel can be initialized per CAN-Channel</remarks>
/// <param name="channel">A PCANTP channel handle</param>
/// <param name="baudrate">The CAN Hardware speed</param>
/// <param name="hw_type">NON PLUG-N-PLAY: The type of hardware and operation mode</param>
/// <param name="io_port">NON PLUG-N-PLAY: The I/O address for the parallel port</param>
/// <param name="interrupt">NON PLUG-N-PLAY: Interrupt number of the parallel port</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_Initialize_2013(
    cantp_handle channel,
    cantp_baudrate baudrate,
    cantp_hwtype hw_type _DEF_ARG_HW,
    uint32_t io_port _DEF_ARG,
    uint16_t interrupt _DEF_ARG);

/// <summary>
/// Initializes a PUDS channel based on a PCANTP channel handle (including CAN-FD support)
/// </summary>
/// <param name="channel">The handle of a FD capable PCANTP Channel</param>
/// <param name="bitrate_fd">The speed for the communication (FD bit rate string)</param>
/// <remarks>Only one PUDS channel can be initialized per CAN-Channel.
/// See PCAN_BR_* values
/// * Parameter and values must be separated by '='
/// * Couples of parameter/value must be separated by ','
/// * Following parameter must be filled out: f_clock, data_brp, data_sjw, data_tseg1, data_tseg2,
///   nom_brp, nom_sjw, nom_tseg1, nom_tseg2.
/// * Following parameters are optional (not used yet): data_ssp_offset, nom_samp
/// </remarks>
/// <example>f_clock_mhz=80,nom_brp=0,nom_tseg1=13,nom_tseg2=0,nom_sjw=0,data_brp=0,
/// data_tseg1=13,data_tseg2=0,data_sjw=0</example>
/// <returns>A uds_status error code</returns>
uds_status __stdcall UDS_InitializeFD_2013(
    cantp_handle channel,
    const cantp_bitrate bitrate_fd);

/// <summary>
/// Uninitializes a PUDS channel
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_Uninitialize_2013(
    cantp_handle channel);

/// <summary>
/// Resets the receive and transmit queues of a PUDS channel
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_Reset_2013(
    cantp_handle channel);

/// <summary>
/// Gets information about the internal BUS status of a PUDS channel
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_GetCanBusStatus_2013(
    cantp_handle channel);

/// <summary>
/// Reads a PUDS message from the receive queue of a PUDS channel
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="out_msg_buffer">[out]A uds_msg structure buffer to store the PUDS message</param>
/// <param name="in_msg_request">(Optional) If NULL the first available message is fetched.
///     Otherwise in_msg_request must represent a sent PUDS request.
///     To look for the request confirmation, in_msg_request->type should not have the loopback flag;
///     otherwise a response from the target ECU will be searched.</param>
/// <param name="out_timestamp">A cantp_timestamp structure buffer to get
/// the reception time of the message. If this value is not desired, this parameter
/// should be passed as NULL</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_Read_2013(
    cantp_handle channel,
    uds_msg* out_msg_buffer,
    uds_msg* in_msg_request _DEF_ARG_NULL,
    cantp_timestamp* out_timestamp _DEF_ARG_NULL);

/// <summary>
/// Transmits a PUDS message
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="msg_buffer">A uds_msg buffer with the message to be sent</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_Write_2013(
    cantp_handle channel,
    uds_msg* msg_buffer);

/// <summary>
/// Adds a user-defined UDS mapping (relation between a CAN ID and a UDS Network Address Information)
/// </summary>
/// <remark>
/// Defining a mapping enables ISO-TP communication with opened Addressing Formats
/// (like PCANTP_ISOTP_FORMAT_NORMAL or PCANTP_ISOTP_FORMAT_EXTENDED).
/// </remark>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="mapping">Mapping to be added</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_AddMapping_2013(
    cantp_handle channel,
    uds_mapping* mapping);

/// <summary>
/// Removes all user-defined PUDS mappings corresponding to a CAN ID
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="can_id">The mapped CAN ID to search for</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_RemoveMappingByCanId_2013(
    cantp_handle channel,
    uint32_t can_id);

/// <summary>
/// Removes a user-defined PUDS mapping
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="mapping">The mapping to remove</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_RemoveMapping_2013(
    cantp_handle channel,
    uds_mapping mapping);

/// <summary>
/// Retrieves a mapping matching the given CAN identifier and message type (11bits, 29 bits, FD, etc.)
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="buffer">[out]Buffer to store the searched mapping</param>
/// <param name="can_id">The mapped CAN ID to look for</param>
/// <param name="can_msgtype">The CAN message type to look for (11bits, 29 bits, FD, etc.)</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success,
/// PUDS_STATUS_MAPPING_NOT_INITIALIZED if no mapping was found.</returns>
uds_status __stdcall UDS_GetMapping_2013(
    cantp_handle channel,
    uds_mapping* buffer,
    uint32_t can_id,
    cantp_can_msgtype can_msgtype);

/// <summary>
/// Retrieves all the mappings defined for a PUDS channel
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="buffer">[out]Buffer of mappings</param>
/// <param name="buffer_length">The number of uds_mapping elements the buffer can store.</param>
/// <param name="count">[out]The actual number of elements copied in the buffer.</param>
uds_status __stdcall UDS_GetMappings_2013(
    cantp_handle channel,
    uds_mapping* buffer,
    uint16_t buffer_length,
    uint16_t* count);

/// <summary>
/// Adds a "PASS" filter on a CAN ID
/// </summary>
/// <remark>
/// CAN and CAN FD frames matching this CAN ID will be fetchable by the UDS API with UDS_Read_2013 function.
/// By default all frames are ignored and are available in lower APIs.
/// </remark>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="can_id">CAN identifier to listen to</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_AddCanIdFilter_2013(
    cantp_handle channel,
    uint32_t can_id);

/// <summary>
/// Remove a "PASS" filter on a CAN ID
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="can_id">CAN identifier to remove</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_RemoveCanIdFilter_2013(
    cantp_handle channel,
    uint32_t can_id);

/// <summary>
/// Retrieves a PUDS channel value
/// </summary>
/// <remark>
///  * Parameter PUDS_PARAM_SERVER_ADDRESS uses 2 bytes data to describe
/// the physical address of the equipment, but the first byte is needed only
/// for ISO-15765-3 Enhanced diagnostics 29 bit CAN ID where addresses
/// are 11 bits long.
///  * Parameter PUDS_PARAM_SERVER_FILTER uses 2 bytes data to describe
/// a functional address, but the first byte is needed only
/// for ISO-15765-3 Enhanced diagnostics 29 bit CAN ID where addresses
/// are 11 bits long; the Most Significant Bit is used to define filter
/// status (see PUDS_SERVER_FILTER_LISTEN).
/// </remark>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="parameter">The parameter to get</param>
/// <param name="buffer">Buffer for the parameter value</param>
/// <param name="buffer_size">Size in bytes of the buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_GetValue_2013(
    cantp_handle channel,
    uds_parameter parameter,
    void* buffer,
    uint32_t buffer_size);

/// <summary>
/// Configures or sets a PUDS channel value
/// </summary>
/// <remark>
///  * Parameter PUDS_PARAM_SERVER_ADDRESS uses 2 bytes data to describe
/// the physical address of the equipment, but the first byte is needed only
/// for ISO-15765-3 Enhanced diagnostics 29 bit CAN ID where addresses
/// are 11 bits long.
///  * Parameter PUDS_PARAM_SERVER_FILTER uses 2 bytes data to describe
/// a functional address, but the first byte is needed only
/// for ISO-15765-3 Enhanced diagnostics 29 bit CAN ID where addresses
/// are 11 bits long; the Most Significant Bit is used to define filter
/// status.
/// </remark>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="parameter">The parameter to set</param>
/// <param name="buffer">Buffer with the value to be set</param>
/// <param name="buffer_size">Size in bytes of the buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SetValue_2013(
    cantp_handle channel,
    uds_parameter parameter,
    void* buffer,
    uint32_t buffer_size);

/// <summary>
/// Returns a descriptive text of a given cantp_status error
/// code, in any desired language
/// </summary>
/// <remarks>The current languages available for translation are:
/// Neutral (0x00), German (0x07), English (0x09), Spanish (0x0A),
/// Italian (0x10) and French (0x0C)</remarks>
/// <param name="error_code">A uds_status error code</param>
/// <param name="language">Indicates a 'Primary language ID'</param>
/// <param name="buffer">Buffer for a null terminated char array</param>
/// <param name="buffer_size">Buffer size</param>
/// <returns>A uds_status error code</returns>
uds_status __stdcall UDS_GetErrorText_2013(
    uds_status error_code,
    uint16_t language,
    char* buffer,
    uint32_t buffer_size);

/// <summary>
/// Gets the session information known by the API
/// </summary>
/// <remark>
/// session_info must be initialized a network address information associated to an ECU.
/// Note that the session's information within the API may be different to the actual session of the corresponding ECU.
/// </remark>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="session_info">[in|out] The session is filled if an ECU session, matching session_info->nai, exists</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_GetSessionInformation_2013(
    cantp_handle channel,
    uds_sessioninfo* session_info);

/// <summary>
/// Checks if a status matches an expected result (default is PUDS_STATUS_OK).
/// </summary>
/// <param name="status">The status to analyze.</param>
/// <param name="status_expected">The expected status (default is PUDS_STATUS_OK).</param>
/// <param name="strict_mode">Enable strict mode (default is false). Strict mode ensures that bus or extra information are the same.</param>
/// <returns>Returns true if the status matches expected parameter.</returns>
bool __stdcall UDS_StatusIsOk_2013(
    const uds_status status,
    const uds_status status_expected _DEF_ARG_UDS_OK,
    bool strict_mode _DEF_ARG);


////////////////////////////////////////////////////////////
// PCAN-UDS API: PUDS Message initialization function declarations
////////////////////////////////////////////////////////////

/// <summary>
/// Allocates a PUDS message based on the given configuration
/// </summary>
/// <param name="msg_buffer">A uds_msg structure buffer (it will be freed if required)</param>
/// <param name="msg_configuration">Configuration of the PUDS message to allocate</param>
/// <param name="msg_data_length">Length of the message's data</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_MsgAlloc_2013(
    uds_msg* msg_buffer,
    uds_msgconfig msg_configuration,
    uint32_t msg_data_length);

/// <summary>
/// Deallocates a PUDS message
/// </summary>
/// <param name="msg_buffer">An allocated uds_msg structure buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_MsgFree_2013(
    uds_msg* msg_buffer);

/// <summary>
/// Copies a PUDS message to another buffer.
/// </summary>
/// <param name="msg_buffer_dst">A uds_msg structure buffer to store the copied message.</param>
/// <param name="msg_buffer_src">The uds_msg structure buffer to copy.</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_MsgCopy_2013(
    uds_msg* msg_buffer_dst,
    const uds_msg* msg_buffer_src);

/// <summary>
/// Moves a PUDS message to another buffer (and cleans the original message structure).
/// </summary>
/// <param name="msg_buffer_dst">A uds_msg structure buffer to store the message.</param>
/// <param name="msg_buffer_src">The uds_msg structure buffer used as the source (will be cleaned).</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_MsgMove_2013(
    uds_msg* msg_buffer_dst,
    uds_msg* msg_buffer_src);

////////////////////////////////////////////////////////////
// PCAN-UDS API: Utility function declarations
////////////////////////////////////////////////////////////

/// <summary>
/// Waits for a message (a response or a transmit confirmation) based on a UDS request
/// </summary>
/// <remarks>
/// Warning: The order of the parameters has changed in PCAN-UDS 2.0 API.
/// </remarks>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="msg_request">A sent uds_msg message used as a reference to find the waited message</param>
/// <param name="is_waiting_for_tx">States if the message to wait for is a transmit confirmation</param>
/// <param name="timeout">Maximum time to wait (in milliseconds) for a message indication corresponding to the message request</param>
/// <param name="timeout_enhanced">Maximum time to wait for a message indication if the server requests more time</param>
/// <param name="out_msg_response">A uds_msg structure buffer to store the PUDS response</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_WaitForSingleMessage_2013(
    cantp_handle channel,
    uds_msg* msg_request,
    bool is_waiting_for_tx,
    uint32_t timeout,
    uint32_t timeout_enhanced,
    uds_msg* out_msg_response);

/// <summary>
/// Waits for multiple responses (from a functional request for instance) based on a PUDS message request.
/// </summary>
/// <remarks>
/// Warning: The order of the parameters has changed in PCAN-UDS 2.0 API.
/// </remarks>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="msg_request">A sent uds_msg message used as a reference to find the waited message</param>
/// <param name="timeout">Maximum time to wait (in milliseconds) for a message indication corresponding to the message request.</param>
/// <param name="timeout_enhanced">Maximum time to wait for a message indication if the server requested more time</param>
/// <param name="wait_until_timeout">if <code>FALSE</code> the function is interrupted if out_msg_count reaches max_msg_count.</param>
/// <param name="max_msg_count">Length of the buffer array (max. messages that can be received)</param>
/// <param name="out_msg_responses">Buffer must be an array of 'max_msg_count' entries (must have at least
/// a size of max_msg_count * sizeof(uds_msg) bytes</param>
/// <param name="out_msg_count">Actual number of messages read</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success,
/// PUDS_ERROR_OVERFLOW indicates success but buffer was too small to hold all responses.</returns>
uds_status __stdcall UDS_WaitForFunctionalResponses_2013(
    cantp_handle channel,
    uds_msg* msg_request,
    uint32_t timeout,
    uint32_t timeout_enhanced,
    bool wait_until_timeout,
    uint32_t max_msg_count,
    uds_msg* out_msg_responses,
    uint32_t* out_msg_count
);

/// <summary>
/// Handles the communication workflow for a UDS service expecting a single response.
/// </summary>
/// <remark>
/// 1) Warning: The order of the parameters has changed in PCAN-UDS 2.0 API.
/// 2) The function waits for a transmit confirmation then for a message response.
/// Even if the SuppressPositiveResponseMessage flag is set, the function will still wait
/// for an eventual Negative Response.
/// </remark>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="msg_request">A sent uds_msg message used as a reference to manage the UDS service</param>
/// <param name="out_msg_response">A uds_msg structure buffer to store the PUDS response</param>
/// <param name="out_msg_request_confirmation">A uds_msg structure buffer to store the PUDS request confirmation</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_WaitForService_2013(
    cantp_handle channel,
    uds_msg* msg_request,
    uds_msg* out_msg_response,
    uds_msg* out_msg_request_confirmation);

/// <summary>
/// Handles the communication workflow for a UDS service expecting multiple responses.
/// </summary>
/// <remark>
/// 1) Warning: The order of the parameters has changed in PCAN-UDS 2.0 API.
/// 2) The function waits for a transmit confirmation then for N message responses.
/// Even if the SuppressPositiveResponseMessage flag is set, the function will still wait
/// for eventual Negative Responses.
/// </remark>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="msg_request">sent uds_msg message</param>
/// <param name="max_msg_count">Length of the buffer array (max. messages that can be received)</param>
/// <param name="wait_until_timeout">if <code>FALSE</code> the function is interrupted if out_msg_count reaches max_msg_count.</param>
/// <param name="out_msg_responses">Buffer must be an array of 'max_msg_count' entries (must have at least
/// a size of max_msg_count * sizeof(uds_msg) bytes</param>
/// <param name="out_msg_count">Actual number of messages read</param>
/// <param name="out_msg_request_confirmation">A uds_msg structure buffer to store the PUDS request confirmation</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success,
/// PUDS_ERROR_OVERFLOW indicates success but buffer was too small to hold all responses.</returns>
uds_status __stdcall UDS_WaitForServiceFunctional_2013(
    cantp_handle channel,
    uds_msg* msg_request,
    uint32_t max_msg_count,
    bool wait_until_timeout,
    uds_msg* out_msg_responses,
    uint32_t* out_msg_count,
    uds_msg* out_msg_request_confirmation);

////////////////////////////////////////////////////////////
// PCAN-UDS API: UDS Service function declarations
////////////////////////////////////////////////////////////

// PUDS Service ids defined in ISO 14229-1:2013
//
#define PUDS_SI_DiagnosticSessionControl           0x10     // see ISO 14229-1:2013
#define PUDS_SI_ECUReset                           0x11     // see ISO 14229-1:2013
#define PUDS_SI_SecurityAccess                     0x27     // see ISO 14229-1:2013
#define PUDS_SI_CommunicationControl               0x28     // see ISO 14229-1:2013
#define PUDS_SI_TesterPresent                      0x3E     // see ISO 14229-1:2013
#define PUDS_SI_AccessTimingParameter              0x83     // see ISO 14229-1:2013
#define PUDS_SI_SecuredDataTransmission            0x84     // see ISO 14229-1:2013
#define PUDS_SI_ControlDTCSetting                  0x85     // see ISO 14229-1:2013
#define PUDS_SI_ResponseOnEvent                    0x86     // see ISO 14229-1:2013
#define PUDS_SI_LinkControl                        0x87     // see ISO 14229-1:2013
#define PUDS_SI_ReadDataByIdentifier               0x22     // see ISO 14229-1:2013
#define PUDS_SI_ReadMemoryByAddress                0x23     // see ISO 14229-1:2013
#define PUDS_SI_ReadScalingDataByIdentifier        0x24     // see ISO 14229-1:2013
#define PUDS_SI_ReadDataByPeriodicIdentifier       0x2A     // see ISO 14229-1:2013
#define PUDS_SI_DynamicallyDefineDataIdentifier    0x2C     // see ISO 14229-1:2013
#define PUDS_SI_WriteDataByIdentifier              0x2E     // see ISO 14229-1:2013
#define PUDS_SI_WriteMemoryByAddress               0x3D     // see ISO 14229-1:2013
#define PUDS_SI_ClearDiagnosticInformation         0x14     // see ISO 14229-1:2013
#define PUDS_SI_ReadDTCInformation                 0x19     // see ISO 14229-1:2013
#define PUDS_SI_InputOutputControlByIdentifier     0x2F     // see ISO 14229-1:2013
#define PUDS_SI_RoutineControl                     0x31     // see ISO 14229-1:2013
#define PUDS_SI_RequestDownload                    0x34     // see ISO 14229-1:2013
#define PUDS_SI_RequestUpload                      0x35     // see ISO 14229-1:2013
#define PUDS_SI_TransferData                       0x36     // see ISO 14229-1:2013
#define PUDS_SI_RequestTransferExit                0x37     // see ISO 14229-1:2013
#define PUDS_SI_RequestFileTransfer                0x38     // see ISO 14229-1:2013
#define PUDS_SI_Authentication                     0x29     // see ISO 14229-1:2020
#define PUDS_NR_SI                                 0x7F     // negative response
#define PUDS_NRC_EXTENDED_TIMING                   0x78     // server wants more time
#define PUDS_SI_POSITIVE_RESPONSE                  0x40     // positive response offset

// ISO-14229-1:2013 §9.2.2.2 p.39
#define PUDS_SVC_PARAM_DSC_DS           0x01    // Default Session
#define PUDS_SVC_PARAM_DSC_ECUPS        0x02    // ECU Programming Session
#define PUDS_SVC_PARAM_DSC_ECUEDS       0x03    // ECU Extended Diagnostic Session
#define PUDS_SVC_PARAM_DSC_SSDS         0x04    // Safety System Diagnostic Session
/// <summary>
/// The DiagnosticSessionControl service is used to enable different diagnostic sessions in the server.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="session_type">Subfunction parameter: type of the session (see PUDS_SVC_PARAM_DSC_xxx)</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcDiagnosticSessionControl_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t session_type);


// ISO-14229-1:2013 §9.3.2.2 p.43
#define PUDS_SVC_PARAM_ER_HR            0x01    // Hard Reset
#define PUDS_SVC_PARAM_ER_KOFFONR       0x02    // Key Off on Reset
#define PUDS_SVC_PARAM_ER_SR            0x03    // Soft Reset
#define PUDS_SVC_PARAM_ER_ERPSD         0x04    // Enable Rapid Power Shutdown
#define PUDS_SVC_PARAM_ER_DRPSD         0x05    // Disable Rapid Power Shutdown
/// <summary>
/// The ECUReset service is used by the client to request a server reset.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="reset_type">Subfunction parameter: type of Reset (see PUDS_SVC_PARAM_ER_xxx)</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcECUReset_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t reset_type);

// ISO-14229-1:2013 §9.4.2.2 p.49
#define PUDS_SVC_PARAM_SA_RSD_1         0x01    // Request Seed
#define PUDS_SVC_PARAM_SA_RSD_3         0x03    // Request Seed
#define PUDS_SVC_PARAM_SA_RSD_5         0x05    // Request Seed
#define PUDS_SVC_PARAM_SA_RSD_MIN       0x07    // Request Seed (odd numbers)
#define PUDS_SVC_PARAM_SA_RSD_MAX       0x5F    // Request Seed (odd numbers)
#define PUDS_SVC_PARAM_SA_SK_2          0x02    // Send Key
#define PUDS_SVC_PARAM_SA_SK_4          0x04    // Send Key
#define PUDS_SVC_PARAM_SA_SK_6          0x06    // Send Key
#define PUDS_SVC_PARAM_SA_SK_MIN        0x08    // Send Key (even numbers)
#define PUDS_SVC_PARAM_SA_SK_MAX        0x60    // Send Key (even numbers)
/// <summary>
/// SecurityAccess service provides a means to access data and/or diagnostic services which have
/// restricted access for security, emissions or safety reasons.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="security_access_type">Subfunction parameter: type of SecurityAccess (see PUDS_SVC_PARAM_SA_xxx)</param>
/// <param name="security_access_data">If Requesting Seed, buffer is the optional data to transmit to a server (like identification).
/// If Sending Key, data holds the value generated by the security algorithm corresponding to a specific "seed" value</param>
/// <param name="security_access_data_size">Size in bytes of the buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcSecurityAccess_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t security_access_type,
    uint8_t* security_access_data,
    uint32_t security_access_data_size);

// ISO-14229-1:2013 §9.5.2.2 p.54
#define PUDS_SVC_PARAM_CC_ERXTX                 0x00    // Enable Rx and Tx
#define PUDS_SVC_PARAM_CC_ERXDTX                0x01    // Enable Rx and Disable Tx
#define PUDS_SVC_PARAM_CC_DRXETX                0x02    // Disable Rx and Enable Tx
#define PUDS_SVC_PARAM_CC_DRXTX                 0x03    // Disable Rx and Tx
#define PUDS_SVC_PARAM_CC_ERXDTXWEAI            0x04    // Enable Rx And Disable Tx With Enhanced Address Information
#define PUDS_SVC_PARAM_CC_ERXTXWEAI             0x05    // Enable Rx And Tx With Enhanced Address Information
#define PUDS_SVC_PARAM_CC_FLAG_APPL             0x01    // Application (01b)
#define PUDS_SVC_PARAM_CC_FLAG_NWM              0x02    // NetworkManagement (10b)
#define PUDS_SVC_PARAM_CC_FLAG_DESCTIRNCN       0x00    // Disable/Enable specified communicationType (see Flags APPL/NMW)
// in the receiving node and all connected networks
#define PUDS_SVC_PARAM_CC_FLAG_DENWRIRO         0xF0    // Disable/Enable network which request is received on
#define PUDS_SVC_PARAM_CC_FLAG_DESNIBNN_MIN     0x10    // Disable/Enable specific network identified by network number (minimum value)
#define PUDS_SVC_PARAM_CC_FLAG_DESNIBNN_MAX     0xE0    // Disable/Enable specific network identified by network number (maximum value)
#define PUDS_SVC_PARAM_CC_FLAG_DESNIBNN_MASK    0xF0    // Mask for DESNIBNN bits
/// <summary>
/// CommunicationControl service's purpose is to switch on/off the transmission
/// and/or the reception of certain messages of (a) server(s).
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="control_type">Subfunction parameter: type of CommunicationControl (see PUDS_SVC_PARAM_CC_xxx)</param>
/// <param name="communication_type">a bit-code value to reference the kind of communication to be controlled,
/// See PUDS_SVC_PARAM_CC_FLAG_xxx flags and ISO_14229-1:2013 §B.1 p.333 for bit-encoding</param>
/// <param name="node_identification_number">Identify a node on a sub-network (only used with
/// PUDS_SVC_PARAM_CC_ERXDTXWEAI or PUDS_SVC_PARAM_CC_ERXTXWEAI control type)</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcCommunicationControl_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t control_type,
    uint8_t communication_type,
    uint16_t node_identification_number _DEF_ARG);

// ISO-14229-1:2013 §9.6.2.2 p.59
#define PUDS_SVC_PARAM_TP_ZSUBF        0x00    // Zero SubFunction
/// <summary>
/// TesterPresent service indicates to a server (or servers) that a client is still connected
/// to the vehicle and that certain diagnostic services and/or communications
/// that have been previously activated are to remain active.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="testerpresent_type">No Subfunction parameter by default (PUDS_SVC_PARAM_TP_ZSUBF)</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcTesterPresent_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t testerpresent_type _DEF_ARG_PUDS_SVC_PARAM_TP_ZSUBF);

// ISO-14229-1:2013 §9.8 p.66
/// <summary>
/// SecuredDataTransmission(2013) service's purpose is to transmit data that is protected
/// against attacks from third parties, which could endanger data security.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="security_data_request_record">buffer containing the data as processed by the Security Sub-Layer (See ISO-15764)</param>
/// <param name="security_data_request_record_size">Size in bytes of the buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcSecuredDataTransmission_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t* security_data_request_record,
    uint32_t security_data_request_record_size);

// ISO-14229-1:2020 §16.2 p.358
#define PUDS_SVC_PARAM_APAR_REQUEST_MSG_FLAG 0x1 // The messsage is a request message.
#define PUDS_SVC_PARAM_APAR_PRE_ESTABLISHED_KEY_FLAG 0x8 // A pre - established key is used.
#define PUDS_SVC_PARAM_APAR_ENCRYPTED_MSG_FLAG 0x10 // Message is encrypted.
#define PUDS_SVC_PARAM_APAR_SIGNED_MSG_FLAG 0x20 // Message is signed.
#define PUDS_SVC_PARAM_APAR_REQUEST_RESPONSE_SIGNATURE_FLAG 0x40 // Signature on the response is requested.
/// <summary>
/// SecuredDataTransmission(2020) service's purpose is to transmit data that is protected
/// against attacks from third parties, which could endanger data security.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="administrative_parameter">Security features used in the message (see PUDS_SVC_PARAM_APAR* definitions)</param>
/// <param name="signature_encryption_calculation">Signature or encryption algorithm identifier</param>
/// <param name="anti_replay_counter">Anti-replay counter value</param>
/// <param name="internal_service_identifier">Internal message service request identifier</param>
/// <param name="service_specific_parameters">Buffer that contains internal message service request data</param>
/// <param name="service_specific_parameters_size">Internal message service request data size (in bytes)</param>
/// <param name="signature_mac">Buffer that contains signature used to verify the message</param>
/// <param name="signature_size">Size in bytes of the signature</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcSecuredDataTransmission_2020(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint16_t administrative_parameter,
    uint8_t signature_encryption_calculation,
    uint16_t anti_replay_counter,
    uint8_t internal_service_identifier,
    uint8_t* service_specific_parameters,
    uint32_t service_specific_parameters_size,
    uint8_t* signature_mac,
    uint16_t signature_size);

// ISO-14229-1:2013 §9.9.2.2 p.72
#define PUDS_SVC_PARAM_CDTCS_ON         0x01    // The server(s) shall resume the setting of diagnostic trouble codes
#define PUDS_SVC_PARAM_CDTCS_OFF        0x02    // The server(s) shall stop the setting of diagnostic trouble codes
/// <summary>
/// ControlDTCSetting service shall be used by a client to stop or resume the setting of
/// diagnostic trouble codes (DTCs) in the server(s).
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="dtc_setting_type">Subfunction parameter (see PUDS_SVC_PARAM_CDTCS_xxx)</param>
/// <param name="dtc_setting_control_option_record">This parameter record is user-optional and transmits data to a server when controlling the DTC setting.
/// It can contain a list of DTCs to be turned on or off.</param>
/// <param name="dtc_setting_control_option_record_size">Size in bytes of the buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcControlDTCSetting_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t dtc_setting_type,
    uint8_t* dtc_setting_control_option_record,
    uint32_t dtc_setting_control_option_record_size);

// ISO-14229-1:2013 §9.10.2.2.1 p.78
#define PUDS_SVC_PARAM_ROE_STPROE           0x00    // Stop Response On Event
#define PUDS_SVC_PARAM_ROE_ONDTCS           0x01    // On DTC Status Change
#define PUDS_SVC_PARAM_ROE_OTI              0x02    // On Timer Interrupt
#define PUDS_SVC_PARAM_ROE_OCODID           0x03    // On Change Of Data Identifier
#define PUDS_SVC_PARAM_ROE_RAE              0x04    // Report Activated Events
#define PUDS_SVC_PARAM_ROE_STRTROE          0x05    // Start Response On Event
#define PUDS_SVC_PARAM_ROE_CLRROE           0x06    // Clear Response On Event
#define PUDS_SVC_PARAM_ROE_OCOV             0x07    // On Comparison Of Values
#define PUDS_SVC_PARAM_ROE_RMRDOSC          0x08    // Report Most Recent Dtc On Status Change (ISO 14229-1:2020 10.9.2.2 p.121)
#define PUDS_SVC_PARAM_ROE_RDRIODSC         0x09    // Report Dtc Record Information On Dtc Status Change (ISO 14229-1:2020 10.9.2.2 p.121)
#define PUDS_SVC_PARAM_ROE_STPROE_LEN       0       // Expected size of event type record for ROE_STPROE
#define PUDS_SVC_PARAM_ROE_ONDTCS_LEN       1       // Expected size of event type record for ROE_ONDTCS
#define PUDS_SVC_PARAM_ROE_OTI_LEN          1       // Expected size of event type record for ROE_OTI
#define PUDS_SVC_PARAM_ROE_OCODID_LEN       2       // Expected size of event type record for ROE_OCODID
#define PUDS_SVC_PARAM_ROE_RAE_LEN          0       // Expected size of event type record for ROE_RAE
#define PUDS_SVC_PARAM_ROE_STRTROE_LEN      0       // Expected size of event type record for ROE_STRTROE
#define PUDS_SVC_PARAM_ROE_CLRROE_LEN       0       // Expected size of event type record for ROE_CLRROE
#define PUDS_SVC_PARAM_ROE_OCOV_LEN         10      // Expected size of event type record for ROE_OCOV
#define PUDS_SVC_PARAM_ROE_RMRDOSC_LEN      1       // Expected size of event type record for ROE_RMRDOSC
#define PUDS_SVC_PARAM_ROE_EWT_ITTR         0x02    // Infinite Time To Response (eventWindowTime parameter)
#define PUDS_SVC_PARAM_ROE_EWT_SEWT         0x03    // Short event window time (eventWindowTime parameter)
#define PUDS_SVC_PARAM_ROE_EWT_MEWT         0x04    // Medium event window time (eventWindowTime parameter)
#define PUDS_SVC_PARAM_ROE_EWT_LEWT         0x05    // Long event window time (eventWindowTime parameter)
#define PUDS_SVC_PARAM_ROE_EWT_PWT          0x06    // Power window time (eventWindowTime parameter)
#define PUDS_SVC_PARAM_ROE_EWT_IWT          0x07    // Ignition window time (eventWindowTime parameter)
#define PUDS_SVC_PARAM_ROE_EWT_MTEWT        0x08    // Manufacturer trigger event window time (eventWindowTime parameter)
#define PUDS_SVC_PARAM_ROE_OTI_SLOW_RATE    0x01    // Slow rate (onTimerInterrupt parameter)
#define PUDS_SVC_PARAM_ROE_OTI_MEDIUM_RATE  0x02    // Medium rate (onTimerInterrupt parameter)
#define PUDS_SVC_PARAM_ROE_OTI_FAST_RATE    0x03    // Fast rate (onTimerInterrupt parameter)
#define PUDS_SVC_PARAM_ROE_STRT_SI_RDBI     PUDS_SI_ReadDataByIdentifier            // Recommended service (first byte of service to respond to record)
#define PUDS_SVC_PARAM_ROE_STRT_SI_RDTCI    PUDS_SI_ReadDTCInformation              // Recommended service (first byte of service to respond to record)
#define PUDS_SVC_PARAM_ROE_STRT_SI_RC       PUDS_SI_RoutineControl                  // Recommended service (first byte of service to respond to record)
#define PUDS_SVC_PARAM_ROE_STRT_SI_IOCBI    PUDS_SI_InputOutputControlByIdentifier  // Recommended service (first byte of service to respond to record)
/// <summary>
/// The ResponseOnEvent service requests a server to
/// start or stop transmission of responses on a specified event.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="event_type">Subfunction parameter: event type (see PUDS_SVC_PARAM_ROE_xxx)</param>
/// <param name="store_event">Storage State (TRUE = Store Event, FALSE = Do Not Store Event)</param>
/// <param name="event_window_time">Specify a window for the event logic to be active in the server (see PUDS_SVC_PARAM_ROE_EWT_ITTR)</param>
/// <param name="event_type_record">Additional parameters for the specified event type</param>
/// <param name="event_type_record_size">Size in bytes of the event type record (see PUDS_SVC_PARAM_ROE_xxx_LEN)</param>
/// <param name="service_to_respond_to_record">Service parameters, with first byte as service Id (see PUDS_SVC_PARAM_ROE_STRT_SI_xxx)</param>
/// <param name="service_to_respond_to_record_size">Size in bytes of the service to respond to record</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcResponseOnEvent_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t event_type,
    bool store_event,
    uint8_t event_window_time,
    uint8_t* event_type_record _DEF_ARG_NULL,
    uint32_t event_type_record_size _DEF_ARG,
    uint8_t* service_to_respond_to_record _DEF_ARG_NULL,
    uint32_t service_to_respond_to_record_size _DEF_ARG);

// ISO-14229-1:2013 §9.11.2.2 p.101
#define PUDS_SVC_PARAM_LC_VBTWFBR               0x01    // Verify Baudrate Transition With Fixed Baudrate
#define PUDS_SVC_PARAM_LC_VBTWSBR               0x02    // Verify Baudrate Transition With Specific Baudrate
#define PUDS_SVC_PARAM_LC_TB                    0x03    // Transition Baudrate
#define PUDS_SVC_PARAM_LC_BAUDRATE_PC_9600      0x01    // standard PC baud rate of 9.6 KBaud
#define PUDS_SVC_PARAM_LC_BAUDRATE_PC_19200     0x02    // standard PC baud rate of 19.2 KBaud
#define PUDS_SVC_PARAM_LC_BAUDRATE_PC_38400     0x03    // standard PC baud rate of 38.4 KBaud
#define PUDS_SVC_PARAM_LC_BAUDRATE_PC_57600     0x04    // standard PC baud rate of 57.6 KBaud
#define PUDS_SVC_PARAM_LC_BAUDRATE_PC_115200    0x05    // standard PC baud rate of 115.2 KBaud
#define PUDS_SVC_PARAM_LC_BAUDRATE_CAN_125K     0x10    // standard CAN baud rate of 125 KBaud
#define PUDS_SVC_PARAM_LC_BAUDRATE_CAN_250K     0x11    // standard CAN baud rate of 250 KBaud
#define PUDS_SVC_PARAM_LC_BAUDRATE_CAN_500K     0x12    // standard CAN baud rate of 500 KBaud
#define PUDS_SVC_PARAM_LC_BAUDRATE_CAN_1M       0x13    // standard CAN baud rate of 1 MBaud
#define PUDS_SVC_PARAM_LC_BAUDRATE_PROGSU       0x20    // Programming setup
/// <summary>
/// The LinkControl service is used to control the communication link baud rate
/// between the client and the server(s) for the exchange of diagnostic data.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="link_control_type">Subfunction parameter: Link Control Type (see PUDS_SVC_PARAM_LC_xxx)</param>
/// <param name="baudrate_identifier">defined baud rate identifier (see PUDS_SVC_PARAM_LC_BAUDRATE_xxx)</param>
/// <param name="link_baudrate">used only with PUDS_SVC_PARAM_LC_VBTWSBR parameter:
/// a three-byte value baud rate (baudrate High, Middle and Low bytes).</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success </returns>
uds_status __stdcall UDS_SvcLinkControl_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t link_control_type,
    uint8_t baudrate_identifier,
    uint32_t link_baudrate _DEF_ARG);

// ISO-14229-1:2013 §C.1 p337
#define PUDS_SVC_PARAM_DI_BSIDID        0xF180  // bootSoftwareIdentificationDataIdentifier
#define PUDS_SVC_PARAM_DI_ASIDID        0xF181  // applicationSoftwareIdentificationDataIdentifier
#define PUDS_SVC_PARAM_DI_ADIDID        0xF182  // applicationDataIdentificationDataIdentifier
#define PUDS_SVC_PARAM_DI_BSFPDID       0xF183  // bootSoftwareIdentificationDataIdentifier
#define PUDS_SVC_PARAM_DI_ASFPDID       0xF184  // applicationSoftwareFingerprintDataIdentifier
#define PUDS_SVC_PARAM_DI_ADFPDID       0xF185  // applicationDataFingerprintDataIdentifier
#define PUDS_SVC_PARAM_DI_ADSDID        0xF186  // activeDiagnosticSessionDataIdentifier
#define PUDS_SVC_PARAM_DI_VMSPNDID      0xF187  // vehicleManufacturerSparePartNumberDataIdentifier
#define PUDS_SVC_PARAM_DI_VMECUSNDID    0xF188  // vehicleManufacturerECUSoftwareNumberDataIdentifier
#define PUDS_SVC_PARAM_DI_VMECUSVNDID   0xF189  // vehicleManufacturerECUSoftwareVersionNumberDataIdentifier
#define PUDS_SVC_PARAM_DI_SSIDDID       0xF18A  // systemSupplierIdentifierDataIdentifier
#define PUDS_SVC_PARAM_DI_ECUMDDID      0xF18B  // ECUManufacturingDateDataIdentifier
#define PUDS_SVC_PARAM_DI_ECUSNDID      0xF18C  // ECUSerialNumberDataIdentifier
#define PUDS_SVC_PARAM_DI_SFUDID        0xF18D  // supportedFunctionalUnitsDataIdentifier
#define PUDS_SVC_PARAM_DI_VMKAPNDID     0xF18E  // vehicleManufacturerKitAssemblyPartNumberDataIdentifier
#define PUDS_SVC_PARAM_DI_VINDID        0xF190  // VINDataIdentifier
#define PUDS_SVC_PARAM_DI_VMECUHNDID    0xF191  // vehicleManufacturerECUHardwareNumberDataIdentifier
#define PUDS_SVC_PARAM_DI_SSECUHWNDID   0xF192  // systemSupplierECUHardwareNumberDataIdentifier
#define PUDS_SVC_PARAM_DI_SSECUHWVNDID  0xF193  // systemSupplierECUHardwareVersionNumberDataIdentifier
#define PUDS_SVC_PARAM_DI_SSECUSWNDID   0xF194  // systemSupplierECUSoftwareNumberDataIdentifier
#define PUDS_SVC_PARAM_DI_SSECUSWVNDID  0xF195  // systemSupplierECUSoftwareVersionNumberDataIdentifier
#define PUDS_SVC_PARAM_DI_EROTANDID     0xF196  // exhaustRegulationOrTypeApprovalNumberDataIdentifier
#define PUDS_SVC_PARAM_DI_SNOETDID      0xF197  // systemNameOrEngineTypeDataIdentifier
#define PUDS_SVC_PARAM_DI_RSCOTSNDID    0xF198  // repairShopCodeOrTesterSerialNumberDataIdentifier
#define PUDS_SVC_PARAM_DI_PDDID         0xF199  // programmingDateDataIdentifier
#define PUDS_SVC_PARAM_DI_CRSCOCESNDID  0xF19A  // calibrationRepairShopCodeOrCalibrationEquipmentSerialNumberDataIdentifier
#define PUDS_SVC_PARAM_DI_CDDID         0xF19B  // calibrationDateDataIdentifier
#define PUDS_SVC_PARAM_DI_CESWNDID      0xF19C  // calibrationEquipmentSoftwareNumberDataIdentifier
#define PUDS_SVC_PARAM_DI_EIDDID        0xF19D  // ECUInstallationDateDataIdentifier
#define PUDS_SVC_PARAM_DI_ODXFDID       0xF19E  // ODXFileDataIdentifier
#define PUDS_SVC_PARAM_DI_EDID          0xF19F  // entityDataIdentifier

// ISO-14229-1:2013 §10.2 p.106
/// <summary>
/// The ReadDataByIdentifier service allows the client to request data record values
/// from the server identified by one or more dataIdentifiers.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="data_identifier">buffer containing a list of two-byte Data Identifiers (see PUDS_SVC_PARAM_DI_xxx)</param>
/// <param name="data_identifier_length">Number of elements in the buffer (size in uint16_t of the buffer)</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDataByIdentifier_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint16_t* data_identifier,
    uint32_t data_identifier_length);

// ISO-14229-1:2013 §10.3 p.113
/// <summary>
/// The ReadMemoryByAddress service allows the client to request memory data from the server
/// via a provided starting address and to specify the size of memory to be read.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="memory_address_buffer">starting address buffer of server memory from which data is to be retrieved</param>
/// <param name="memory_address_size">Size in bytes of the memory_address_buffer (max.: 0xF)</param>
/// <param name="memory_size_buffer">number of bytes to be read starting at the address specified by memory_address_buffer</param>
/// <param name="memory_size_size">Size in bytes of the memory_size_buffer (max.: 0xF)</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadMemoryByAddress_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t* memory_address_buffer,
    uint8_t memory_address_size,
    uint8_t* memory_size_buffer,
    uint8_t memory_size_size);

// ISO-14229-1:2013 §10.4 p.119
/// <summary>
/// The ReadScalingDataByIdentifier service allows the client to request
/// scaling data record information from the server identified by a dataIdentifier.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="data_identifier">a two-byte Data Identifier (see PUDS_SVC_PARAM_DI_xxx)</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadScalingDataByIdentifier_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint16_t data_identifier);

// ISO-14229-1:2013 §C.4 p.351
#define PUDS_SVC_PARAM_RDBPI_SASR       0x01    // Send At Slow Rate
#define PUDS_SVC_PARAM_RDBPI_SAMR       0x02    // Send At Medium Rate
#define PUDS_SVC_PARAM_RDBPI_SAFR       0x03    // Send At Fast Rate
#define PUDS_SVC_PARAM_RDBPI_SS         0x04    // Stop Sending
/// <summary>
/// The ReadDataByPeriodicIdentifier service allows the client to request the periodic transmission
/// of data record values from the server identified by one or more periodicDataIdentifiers.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="transmission_mode">transmission rate code (see PUDS_SVC_PARAM_RDBPI_xxx)</param>
/// <param name="periodic_data_identifier">buffer containing a list of Periodic Data Identifiers</param>
/// <param name="periodic_data_identifier_size">Number of elements in the buffer (size in bytes of the buffer)</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDataByPeriodicIdentifier_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t transmission_mode,
    uint8_t* periodic_data_identifier,
    uint32_t periodic_data_identifier_size);

// ISO-14229-1:2013 §10.6.2.2 p.144
#define PUDS_SVC_PARAM_DDDI_DBID        0x01    // Define By Identifier
#define PUDS_SVC_PARAM_DDDI_DBMA        0x02    // Define By Memory Address
#define PUDS_SVC_PARAM_DDDI_CDDDI       0x03    // Clear Dynamically Defined Data Identifier
/// <summary>
/// The DynamicallyDefineDataIdentifier service allows the client to dynamically define
/// in a server a data identifier that can be read via the ReadDataByIdentifier service at a later time.
/// The Define By Identifier subfunction specifies that definition of the dynamic data
/// identifier shall occur via a data identifier reference.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="dynamically_defined_data_identifier">a two-byte Data Identifier (see PUDS_SVC_PARAM_DI_xxx)</param>
/// <param name="source_data_identifier">buffer containing the sources of information to be included into the dynamic data record</param>
/// <param name="memory_size">buffer containing the total numbers of bytes from the source data record address</param>
/// <param name="position_in_source_data_record">buffer containing the starting byte positions of the excerpt of the source data record</param>
/// <param name="number_of_elements">Number of elements in SourceDataIdentifier/position_in_source_data_record/memory_size triplet.</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcDynamicallyDefineDataIdentifierDBID_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint16_t dynamically_defined_data_identifier,
    uint16_t* source_data_identifier,
    uint8_t* memory_size,
    uint8_t* position_in_source_data_record,
    uint32_t number_of_elements);

/// <summary>
/// The DynamicallyDefineDataIdentifier service allows the client to dynamically define
/// in a server a data identifier that can be read via the ReadDataByIdentifier service at a later time.
/// The Define By Memory Address subfunction specifies that definition of the dynamic data
/// identifier shall occur via an address reference.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="dynamically_defined_data_identifier">a two-byte Data Identifier (see PUDS_SVC_PARAM_DI_xxx)</param>
/// <param name="memory_address_size">Size in bytes of the memory address items in the memory_address_buffer (max.: 0xF)</param>
/// <param name="memory_size_size">Size in bytes of the memory size items in the memory_size_buffer (max.: 0xF)</param>
/// <param name="memory_address_buffer">buffer containing the memory address buffer,
/// must be an array of 'number_of_elements' items whose size is 'memory_address_size'
/// (size is 'number_of_elements * memory_address_size' bytes)</param>
/// <param name="memory_size_buffer">buffer containing the memory size buffer,
/// must be an array of 'number_of_elements' items whose size is 'memory_size_size'
/// (size is 'number_of_elements * memory_size_size' bytes)</param>
/// <param name="number_of_elements">Number of elements in memory_address_buffer/memory_size_buffer couple.</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcDynamicallyDefineDataIdentifierDBMA_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint16_t dynamically_defined_data_identifier,
    uint8_t memory_address_size,
    uint8_t memory_size_size,
    uint8_t* memory_address_buffer,
    uint8_t* memory_size_buffer,
    uint32_t number_of_elements);

/// <summary>
/// The Clear Dynamically Defined Data Identifier subfunction shall be used to clear
/// the specified dynamic data identifier.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="dynamically_defined_data_identifier">a two-byte Data Identifier (see PUDS_SVC_PARAM_DI_xxx)</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcDynamicallyDefineDataIdentifierCDDDI_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint16_t dynamically_defined_data_identifier);

/// <summary>
/// The Clear All Dynamically Defined Data Identifier function shall be used to clear
/// all dynamic data identifier declared in the server.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcDynamicallyDefineDataIdentifierClearAllDDDI_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request);

// ISO-14229-1:2013 §10.7 p.162
/// <summary>
/// The WriteDataByIdentifier service allows the client to write information into the server at an internal location
/// specified by the provided data identifier.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="data_identifier">a two-byte Data Identifier (see PUDS_SVC_PARAM_DI_xxx)</param>
/// <param name="data_record">buffer containing the data to write</param>
/// <param name="data_record_size">Size in bytes of the buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcWriteDataByIdentifier_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint16_t data_identifier,
    uint8_t* data_record,
    uint32_t data_record_size);

// ISO-14229-1:2013 §10.8 p.167
/// <summary>
/// The WriteMemoryByAddress service allows the client to write
/// information into the server at one or more contiguous memory locations.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="memory_address_buffer">Starting address buffer of server memory to which data is to be written</param>
/// <param name="memory_address_size">Size in bytes of the memory_address_buffer (max.: 0xF)</param>
/// <param name="memory_size_buffer">number of bytes to be written starting at the address specified by memory_address_buffer</param>
/// <param name="memory_size_size">Size in bytes of the memory_size_buffer (max.: 0xF)</param>
/// <param name="data_record">buffer containing the data to write</param>
/// <param name="data_record_size">Size in bytes of the buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcWriteMemoryByAddress_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t* memory_address_buffer,
    uint8_t memory_address_size,
    uint8_t* memory_size_buffer,
    uint8_t memory_size_size,
    uint8_t* data_record,
    uint32_t data_record_size);

// ISO-14229-1:2013 §11.2 p.175
#define PUDS_SVC_PARAM_CDI_ERS      0x000000        // Emissions-related systems group of DTCs
#define PUDS_SVC_PARAM_CDI_AGDTC    0xFFFFFF        // All Groups of DTCs
/// <summary>
/// The ClearDiagnosticInformation service is used by the client to clear diagnostic information
/// in one server's or multiple servers' memory.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="group_of_dtc">a three-byte value indicating the group of DTCs (e.g. powertrain, body, chassis)
/// or the particular DTC to be cleared (see PUDS_SVC_PARAM_CDI_xxx)</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcClearDiagnosticInformation_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint32_t group_of_dtc);

/// <summary>
/// The ClearDiagnosticInformation service is used by the client to clear diagnostic information
/// in one server's or multiple servers' memory.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="group_of_dtc">a three-byte value indicating the group of DTCs (e.g. powertrain, body, chassis)
/// or the particular DTC to be cleared (see PUDS_SVC_PARAM_CDI_xxx)</param>
/// <param name="memory_selection">User defined DTC memory</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcClearDiagnosticInformation_2020(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint32_t group_of_dtc,
    uint8_t memory_selection);


// ISO-14229-1:2013 §11.3.2.2 p.194
#define PUDS_SVC_PARAM_RDTCI_RNODTCBSM          0x01    // report Number Of DTC By Status Mask
#define PUDS_SVC_PARAM_RDTCI_RDTCBSM            0x02    // report DTC By Status Mask
#define PUDS_SVC_PARAM_RDTCI_RDTCSSI            0x03    // report DTC Snapshot Identification
#define PUDS_SVC_PARAM_RDTCI_RDTCSSBDTC         0x04    // report DTC Snapshot Record By DTC Number
#define PUDS_SVC_PARAM_RDTCI_RDTCSSBRN          0x05    // report DTC Snapshot Record By Record Number
#define PUDS_SVC_PARAM_RDTCI_RDTCEDRBDN         0x06    // report DTC Extended Data Record By DTC Number
#define PUDS_SVC_PARAM_RDTCI_RNODTCBSMR         0x07    // report Number Of DTC By Severity Mask Record
#define PUDS_SVC_PARAM_RDTCI_RDTCBSMR           0x08    // report DTC By Severity Mask Record
#define PUDS_SVC_PARAM_RDTCI_RSIODTC            0x09    // report Severity Information Of DTC
#define PUDS_SVC_PARAM_RDTCI_RSUPDTC            0x0A    // report Supported DTC
#define PUDS_SVC_PARAM_RDTCI_RFTFDTC            0x0B    // report First Test Failed DTC
#define PUDS_SVC_PARAM_RDTCI_RFCDTC             0x0C    // report First Confirmed DTC
#define PUDS_SVC_PARAM_RDTCI_RMRTFDTC           0x0D    // report Most Recent Test Failed DTC
#define PUDS_SVC_PARAM_RDTCI_RMRCDTC            0x0E    // report Most Recent Confirmed DTC
#define PUDS_SVC_PARAM_RDTCI_RMMDTCBSM          0x0F    // report Mirror Memory DTC By Status Mask
#define PUDS_SVC_PARAM_RDTCI_RMMDEDRBDN         0x10    // report Mirror Memory DTC Extended Data Record By DTC Number
#define PUDS_SVC_PARAM_RDTCI_RNOMMDTCBSM        0x11    // report Number Of Mirror MemoryDTC By Status Mask
#define PUDS_SVC_PARAM_RDTCI_RNOOBDDTCBSM       0x12    // report Number Of Emissions Related OBD DTC By Status Mask
#define PUDS_SVC_PARAM_RDTCI_ROBDDTCBSM         0x13    // report Emissions Related OBD DTC By Status Mask
#define PUDS_SVC_PARAM_RDTCI_RDTCEDBR           0x16    // report DTC Ext Data Record By Record Number
#define PUDS_SVC_PARAM_RDTCI_RUDMDTCBSM         0x17    // report User Def Memory DTC By Status Mask
#define PUDS_SVC_PARAM_RDTCI_RUDMDTCSSBDTC      0x18    // report User Def Memory DTC Snapshot Record By DTC Number
#define PUDS_SVC_PARAM_RDTCI_RUDMDTCEDRBDN      0x19    // report User Def Memory DTC Ext Data Record By DTC Number
#define PUDS_SVC_PARAM_RDTCI_RDTCEDI            0x1A    // report report DTC Extended Data Record Identification (ISO_14229-1 2020)
#define PUDS_SVC_PARAM_RDTCI_RWWHOBDDTCBMR      0x42    // report WWHOBD DTC By Mask Record
#define PUDS_SVC_PARAM_RDTCI_RWWHOBDDTCWPS      0x55    // report WWHOBD DTC With Permanent Status
#define PUDS_SVC_PARAM_RDTCI_RDTCBRGI           0x56    // report DTC Information By DTC Readiness Group Identifier (ISO_14229-1 2020)
// Reminder: following parameters were not defined as they are NOT in ISO-15765-3 :
#define PUDS_SVC_PARAM_RDTCI_RDTCFDC            0x14    // report DTC Fault Detection Counter
#define PUDS_SVC_PARAM_RDTCI_RDTCWPS            0x15    // report DTC With Permanent Status

// DTCSeverityMask (DTCSVM): ISO-14229-1:2013 §D.3 p.366
#define PUDS_SVC_PARAM_RDTCI_DTCSVM_NSA         0x00    // DTC severity bit definitions: no SeverityAvailable
#define PUDS_SVC_PARAM_RDTCI_DTCSVM_MO          0x20    // DTC severity bit definitions: maintenance Only
#define PUDS_SVC_PARAM_RDTCI_DTCSVM_CHKANH      0x40    // DTC severity bit definitions: check At Next Halt
#define PUDS_SVC_PARAM_RDTCI_DTCSVM_CHKI        0x80    // DTC severity bit definitions: check Immediately

/// <summary>
/// This service allows a client to read the status of server-resident Diagnostic Trouble Code (DTC) information.
/// Only reportNumberOfDTCByStatusMask, reportDTCByStatusMask, reportMirrorMemoryDTCByStatusMask,
/// reportNumberOfMirrorMemoryDTCByStatusMask, reportNumberOfEmissionsRelatedOBDDTCByStatusMask,
/// reportEmissionsRelatedOBDDTCByStatusMask Sub-functions are allowed.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="PUDS_SVC_PARAM_RDTCI_Type">Subfunction parameter: ReadDTCInformation type, use one of the following:
/// PUDS_SVC_PARAM_RDTCI_RNODTCBSM, PUDS_SVC_PARAM_RDTCI_RDTCBSM,
/// PUDS_SVC_PARAM_RDTCI_RMMDTCBSM, PUDS_SVC_PARAM_RDTCI_RNOMMDTCBSM,
/// PUDS_SVC_PARAM_RDTCI_RNOOBDDTCBSM, PUDS_SVC_PARAM_RDTCI_ROBDDTCBSM</param>
/// <param name="dtc_status_mask">Contains eight DTC status bit.</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDTCInformation_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t PUDS_SVC_PARAM_RDTCI_Type,
    uint8_t dtc_status_mask);

/// <summary>
/// This service allows a client to read the status of server-resident Diagnostic Trouble Code (DTC) information.
/// The sub-function reportDTCSnapshotRecordByDTCNumber (PUDS_SVC_PARAM_RDTCI_RDTCSSBDTC) is implicit.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="dtc_mask">a unique identification number (three byte value) for a specific diagnostic trouble code</param>
/// <param name="dtc_snapshot_record_number">the number of the specific DTCSnapshot data records</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDTCInformationRDTCSSBDTC_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint32_t dtc_mask,
    uint8_t dtc_snapshot_record_number);

/// <summary>
/// This service allows a client to read the status of server-resident Diagnostic Trouble Code (DTC) information.
/// The sub-function reportDTCSnapshotByRecordNumber (PUDS_SVC_PARAM_RDTCI_RDTCSSBRN) is implicit.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="dtc_snapshot_record_number">the number of the specific DTCSnapshot data records</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDTCInformationRDTCSSBRN_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t dtc_snapshot_record_number);

/// <summary>
/// This service allows a client to read the status of server-resident Diagnostic Trouble Code (DTC) information.
/// Only reportDTCExtendedDataRecordByDTCNumber and reportMirrorMemoryDTCExtendedDataRecordByDTCNumber Sub-functions are allowed.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="PUDS_SVC_PARAM_RDTCI_Type">Subfunction parameter: ReadDTCInformation type, use one of the following:
/// PUDS_SVC_PARAM_RDTCI_RDTCEDRBDN, PUDS_SVC_PARAM_RDTCI_RMMDEDRBDN</param>
/// <param name="dtc_mask">a unique identification number (three byte value) for a specific diagnostic trouble code</param>
/// <param name="dtc_extended_data_record_number">the number of the specific DTCExtendedData record requested.</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDTCInformationReportExtended_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t PUDS_SVC_PARAM_RDTCI_Type,
    uint32_t dtc_mask,
    uint8_t dtc_extended_data_record_number);

/// <summary>
/// This service allows a client to read the status of server-resident Diagnostic Trouble Code (DTC) information.
/// Only reportNumberOfDTCBySeverityMaskRecord and reportDTCSeverityInformation Sub-functions are allowed.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="PUDS_SVC_PARAM_RDTCI_Type">Subfunction parameter: ReadDTCInformation type, use one of the following:
/// PUDS_SVC_PARAM_RDTCI_RNODTCBSMR, PUDS_SVC_PARAM_RDTCI_RDTCBSMR</param>
/// <param name="dtc_severity_mask">a mask of eight (8) DTC severity bits (see PUDS_SVC_PARAM_RDTCI_DTCSVM_xxx)</param>
/// <param name="dtc_status_mask">a mask of eight (8) DTC status bits</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDTCInformationReportSeverity_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t PUDS_SVC_PARAM_RDTCI_Type,
    uint8_t dtc_severity_mask,
    uint8_t dtc_status_mask);

/// <summary>
/// This service allows a client to read the status of server-resident Diagnostic Trouble Code (DTC) information.
/// The sub-function reportSeverityInformationOfDTC (PUDS_SVC_PARAM_RDTCI_RSIODTC) is implicit.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="dtc_mask">a unique identification number for a specific diagnostic trouble code</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDTCInformationRSIODTC_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint32_t dtc_mask);

/// <summary>
/// This service allows a client to read the status of server-resident Diagnostic Trouble Code _2013(DTC) information.
/// Only reportSupportedDTC, reportFirstTestFailedDTC, reportFirstConfirmedDTC, reportMostRecentTestFailedDTC,
/// reportMostRecentConfirmedDTC, reportDTCFaultDetectionCounter, reportDTCWithPermanentStatus,
/// and reportDTCSnapshotIdentification Sub-functions are allowed.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="PUDS_SVC_PARAM_RDTCI_Type">Subfunction parameter: ReadDTCInformation type, use one of the following:
/// PUDS_SVC_PARAM_RDTCI_RFTFDTC, PUDS_SVC_PARAM_RDTCI_RFCDTC,
/// PUDS_SVC_PARAM_RDTCI_RMRTFDTC, PUDS_SVC_PARAM_RDTCI_RMRCDTC,
/// PUDS_SVC_PARAM_RDTCI_RSUPDTC, PUDS_SVC_PARAM_RDTCI_RDTCWPS,
/// PUDS_SVC_PARAM_RDTCI_RDTCSSI, PUDS_SVC_PARAM_RDTCI_RDTCFDC</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDTCInformationNoParam_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t PUDS_SVC_PARAM_RDTCI_Type);

/// <summary>
/// This service allows a client to read the status of server-resident Diagnostic Trouble Code (DTC) information.
/// The sub-function reportDTCExtDataRecordByRecordNumber (PUDS_SVC_PARAM_RDTCI_RDTCEDBR) is implicit.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="dtc_extended_data_record_number">DTC extended data record number</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDTCInformationRDTCEDBR_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t dtc_extended_data_record_number);

/// <summary>
/// This service allows a client to read the status of server-resident Diagnostic Trouble Code (DTC) information.
/// The sub-function reportUserDefMemoryDTCByStatusMask (PUDS_SVC_PARAM_RDTCI_RUDMDTCBSM) is implicit.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="dtc_status_mask">a mask of eight (8) DTC status bits</param>
/// <param name="memory_selection">Memory selection</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDTCInformationRUDMDTCBSM_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t dtc_status_mask,
    uint8_t memory_selection);

/// <summary>
/// This service allows a client to read the status of server-resident Diagnostic Trouble Code (DTC) information.
/// The sub-function reportUserDefMemoryDTCSnapshotRecordByDTCNumber (PUDS_SVC_PARAM_RDTCI_RUDMDTCSSBDTC) is implicit.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="dtc_mask">a unique identification number (three byte value) for a specific diagnostic trouble code</param>
/// <param name="user_def_dtc_snapshot_record_number">User DTC snapshot record number</param>
/// <param name="memory_selection">Memory selection</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDTCInformationRUDMDTCSSBDTC_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint32_t dtc_mask,
    uint8_t user_def_dtc_snapshot_record_number,
    uint8_t memory_selection);

/// <summary>
/// This service allows a client to read the status of server-resident Diagnostic Trouble Code (DTC) information.
/// The sub-function reportUserDefMemoryDTCExtDataRecordByDTCNumber (PUDS_SVC_PARAM_RDTCI_RUDMDTCEDRBDN) is implicit.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="dtc_mask">a unique identification number (three byte value) for a specific diagnostic trouble code</param>
/// <param name="dtc_extended_data_record_number">DTC extened data record number</param>
/// <param name="memory_selection">Memory selection</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDTCInformationRUDMDTCEDRBDN_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint32_t dtc_mask,
    uint8_t dtc_extended_data_record_number,
    uint8_t memory_selection);

/// <summary>
/// ISO_14229-1 2020
/// This service allows a client to read the status of server-resident Diagnostic Trouble Code (DTC) information.
/// The sub-function reportSupportedDTCExtDataRecord (PUDS_SVC_PARAM_RDTCI_RDTCEDI) is implicit.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="dtc_extended_data_record_number">DTC extended data record number</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDTCInformationRDTCEDI_2020(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t dtc_extended_data_record_number);

/// <summary>
/// This service allows a client to read the status of server-resident Diagnostic Trouble Code (DTC) information.
/// The sub-function reportWWHOBDDTCByMaskRecord (PUDS_SVC_PARAM_RDTCI_RWWHOBDDTCBMR) is implicit.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="functional_group_identifier">Functional group identifier</param>
/// <param name="dtc_status_mask">a mask of eight (8) DTC status bits</param>
/// <param name="dtc_severity_mask">a mask of eight (8) DTC severity bits (see PUDS_SVC_PARAM_RDTCI_DTCSVM_xxx)</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDTCInformationRWWHOBDDTCBMR_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t functional_group_identifier,
    uint8_t dtc_status_mask,
    uint8_t dtc_severity_mask);

/// <summary>
/// This service allows a client to read the status of server-resident Diagnostic Trouble Code (DTC) information.
/// The sub-function reportWWHOBDDTCWithPermanentStatus (PUDS_SVC_PARAM_RDTCI_RWWHOBDDTCWPS ) is implicit.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="functional_group_identifier">Functional group identifier</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDTCInformationRWWHOBDDTCWPS_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t functional_group_identifier);

/// <summary>
/// ISO_14229-1 2020
/// This service allows a client to read the status of server-resident Diagnostic Trouble Code (DTC) information.
/// The sub-function reportDTCInformationByDTCReadinessGroupIdentifier (PUDS_SVC_PARAM_RDTCI_RDTCBRGI ) is implicit.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="functional_group_identifier">Functional group identifier</param>
/// <param name="dtc_readiness_group_identifier">DTC readiness group identifier</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcReadDTCInformationRDTCBRGI_2020(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t functional_group_identifier,
    uint8_t dtc_readiness_group_identifier);


// ISO-14229-1:2013 §E.1 p.374
#define PUDS_SVC_PARAM_IOCBI_RCTECU         0x00    // inputOutputControlParameter: returnControlToECU (0 controlState bytes in request)
#define PUDS_SVC_PARAM_IOCBI_RTD            0x01    // inputOutputControlParameter: resetToDefault (0 controlState bytes in request)
#define PUDS_SVC_PARAM_IOCBI_FCS            0x02    // inputOutputControlParameter: freezeCurrentState (0 controlState bytes in request)
#define PUDS_SVC_PARAM_IOCBI_STA            0x03    // inputOutputControlParameter: shortTermAdjustment
/// <summary>
/// The InputOutputControlByIdentifier service is used by the client to substitute a value for an input signal,
/// internal server function and/or control an output (actuator) of an electronic system.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="data_identifier">a two-byte Data Identifier (see PUDS_SVC_PARAM_DI_xxx)</param>
/// <param name="control_option_record">First byte can be used as either an inputOutputControlParameter
/// that describes how the server shall control its inputs or outputs (see PUDS_SVC_PARAM_IOCBI_xxx),
/// or as an additional controlState byte</param>
/// <param name="control_option_record_size">Size in bytes of the control_option_record buffer</param>
/// <param name="control_enable_mask_record">The control_enable_mask_record shall only be supported when
/// the inputOutputControlParameter is used (see control_option_record) and the dataIdentifier to be controlled consists
/// of more than one parameter (i.e. the dataIdentifier is bit-mapped or packeted by definition).
/// There shall be one bit in the control_enable_mask_record corresponding to each individual parameter
/// defined within the dataIdentifier.</param>
/// <param name="control_enable_mask_record_size">Size in bytes of the control_enable_mask_record buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcInputOutputControlByIdentifier_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint16_t data_identifier,
    uint8_t* control_option_record,
    uint32_t control_option_record_size,
    uint8_t* control_enable_mask_record _DEF_ARG_NULL,
    uint32_t control_enable_mask_record_size _DEF_ARG);


// ISO-14229-1:2013 §13.2.2.2 p.262
#define PUDS_SVC_PARAM_RC_STR           0x01    // Start Routine
#define PUDS_SVC_PARAM_RC_STPR          0x02    // Stop Routine
#define PUDS_SVC_PARAM_RC_RRR           0x03    // Request Routine Results
// routineIdentifier: ISO-14229-1:2013 §F.1 p.375
#define PUDS_SVC_PARAM_RC_RID_DLRI_     0xE200  // routineIdentifier: DeployLoopRoutineID
#define PUDS_SVC_PARAM_RC_RID_EM_       0xFF00  // routineIdentifier: eraseMemory
#define PUDS_SVC_PARAM_RC_RID_CPD_      0xFF01  // routineIdentifier: checkProgrammingDependencies
#define PUDS_SVC_PARAM_RC_RID_EMMDTC_   0xFF02  // routineIdentifier: eraseMirrorMemoryDTCs
/// <summary>
/// The RoutineControl service is used by the client to start/stop a routine,
/// and request routine results.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="routine_control_type">Subfunction parameter: RoutineControl type (see PUDS_SVC_PARAM_RC_xxx)</param>
/// <param name="routine_identifier">Server Local Routine Identifier (see PUDS_SVC_PARAM_RC_RID_xxx)</param>
/// <param name="routine_control_option_record">buffer containing the Routine Control Options (only with start and stop routine sub-functions)</param>
/// <param name="routine_control_option_record_size">Size in bytes of the buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcRoutineControl_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t routine_control_type,
    uint16_t routine_identifier,
    uint8_t* routine_control_option_record _DEF_ARG_NULL,
    uint32_t routine_control_option_record_size _DEF_ARG);


// ISO-14229-1:2013 §14.2 p.270
/// <summary>
/// The requestDownload service is used by the client to initiate a data transfer
/// from the client to the server (download).
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="compression_method">A nibble-value that specifies the "compressionMethod",
/// The value 0x0 specifies that no compressionMethod is used.</param>
/// <param name="encrypting_method">A nibble-value that specifies the "encryptingMethod",
/// The value 0x0 specifies that no encryptingMethod is used.</param>
/// <param name="memory_address_buffer">starting address of server memory to which data is to be written</param>
/// <param name="memory_address_size">Size in bytes of the memory_address_buffer buffer (max.: 0xF)</param>
/// <param name="memory_size_buffer">used by the server to compare the uncompressed memory size with
/// the total amount of data transferred during the TransferData service</param>
/// <param name="memory_size_size">Size in bytes of the memory_size_buffer buffer (max.: 0xF)</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcRequestDownload_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t compression_method,
    uint8_t encrypting_method,
    uint8_t* memory_address_buffer,
    uint8_t memory_address_size,
    uint8_t* memory_size_buffer,
    uint8_t memory_size_size);

// ISO-14229-1:2013 §14.3 p.275
/// <summary>
/// The requestUpload service is used by the client to initiate a data transfer
/// from the server to the client (upload).
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="compression_method">A nibble-value that specifies the "compressionMethod",
/// The value 0x0 specifies that no compressionMethod is used.</param>
/// <param name="encrypting_method">A nibble-value that specifies the "encryptingMethod",
/// The value 0x0 specifies that no encryptingMethod is used.</param>
/// <param name="memory_address_buffer">starting address of server memory from which data is to be retrieved</param>
/// <param name="memory_address_size">Size in bytes of the memory_address_buffer buffer (max.: 0xF)</param>
/// <param name="memory_size_buffer">used by the server to compare the uncompressed memory size with
/// the total amount of data transferred during the TransferData service</param>
/// <param name="memory_size_size">Size in bytes of the memory_size_buffer buffer (max.: 0xF)</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcRequestUpload_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t compression_method,
    uint8_t encrypting_method,
    uint8_t* memory_address_buffer,
    uint8_t memory_address_size,
    uint8_t* memory_size_buffer,
    uint8_t memory_size_size);

// ISO-14229-1:2013 §14.4 p.280
/// <summary>
/// The TransferData service is used by the client to transfer data either from the client
/// to the server (download) or from the server to the client (upload).
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="block_sequence_counter">The blockSequenceCounter parameter value starts at 01 hex
/// with the first TransferData request that follows the RequestDownload (34 hex)
/// or RequestUpload (35 hex) service. Its value is incremented by 1 for each subsequent
/// TransferData request. At the value of FF hex, the blockSequenceCounter rolls over
/// and starts at 00 hex with the next TransferData request message.</param>
/// <param name="transfer_request_parameter_record">buffer containing the required transfer parameters</param>
/// <param name="transfer_request_parameter_record_size">Size in bytes of the buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcTransferData_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t block_sequence_counter,
    uint8_t* transfer_request_parameter_record _DEF_ARG_NULL,
    uint32_t transfer_request_parameter_record_size _DEF_ARG);

// ISO-14229-1:2013 §14.5 p.285
/// <summary>
/// The RequestTransferExit service is used by the client to terminate a data
/// transfer between client and server (upload or download).
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="transfer_request_parameter_record">buffer containing the required transfer parameters</param>
/// <param name="transfer_request_parameter_record_size">Size in bytes of the buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcRequestTransferExit_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t* transfer_request_parameter_record _DEF_ARG_NULL,
    uint32_t transfer_request_parameter_record_size _DEF_ARG);

// See ISO-14229-1:2013 §9.7.2.2 p.62 table 74
#define PUDS_SVC_PARAM_ATP_RETPS    0x01  // Read Extended Timing Parameter Set
#define PUDS_SVC_PARAM_ATP_STPTDV   0x02  // Set Timing Parameters To Default Values
#define PUDS_SVC_PARAM_ATP_RCATP    0x03  // Read Currently Active Timing Parameters
#define PUDS_SVC_PARAM_ATP_STPTGV   0x04  // Set Timing Parameters To Given Values

/// <summary>
///  AccessTimingParameter service.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="access_type">Access type, see PUDS_SVC_PARAM_ATP_* values</param>
/// <param name="request_record">Timing parameter request record</param>
/// <param name="request_record_size">Size in byte of the request record</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcAccessTimingParameter_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t access_type,
    uint8_t* request_record,
    uint32_t request_record_size);

// See ISO-14229-1:2013 Annex G p.376 table G.1
#define PUDS_SVC_PARAM_RFT_MOOP_ADDFILE    0x1 // Add File
#define PUDS_SVC_PARAM_RFT_MOOP_DELFILE    0x2 // Delete File
#define PUDS_SVC_PARAM_RFT_MOOP_REPLFILE   0x3 // Replace File
#define PUDS_SVC_PARAM_RFT_MOOP_RDFILE     0x4 // Read File
#define PUDS_SVC_PARAM_RFT_MOOP_RDDIR      0x5 // Read Dir
#define PUDS_SVC_PARAM_RFT_MOOP_RSFILE     0x6 // Resume File (ISO-14229-1:2020 Annex G p.447 table G.1)
/// <summary>
///  RequestFileTransfer service.
/// </summary>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration (PUDS_MSGTYPE_FLAG_NO_POSITIVE_RESPONSE is ignored)</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="mode_of_operation">Mode of operation (add, delete, replace, read), see PUDS_SVC_PARAM_RFT_MOOP_* values</param>
/// <param name="file_path_and_name_size">Size in bytes of file_path_and_name buffer</param>
/// <param name="file_path_and_name">File path and name string</param>
/// <param name="compression_method">A nibble-value that specifies the "compressionMethod", the value 0x0 specifies that no compressionMethod is used.</param>
/// <param name="encrypting_method">A nibble-value that specifies the "encryptingMethod", the value 0x0 specifies that no encryptingMethod is used.</param>
/// <param name="file_size_parameter_size">Size in byte of file_size_uncompressed and file_size_compressed parameters</param>
/// <param name="file_size_uncompressed">Uncompressed file size</param>
/// <param name="file_size_compressed">Compressed file size</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcRequestFileTransfer_2013(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t mode_of_operation,
    uint16_t file_path_and_name_size,
    uint8_t* file_path_and_name,
    uint8_t compression_method _DEF_ARG,
    uint8_t encrypting_method _DEF_ARG,
    uint8_t file_size_parameter_size _DEF_ARG,
    uint8_t* file_size_uncompressed _DEF_ARG_NULL,
    uint8_t* file_size_compressed _DEF_ARG_NULL);

// Represents the subfunction parameter for UDS service Authentication (see ISO 14229-1:2020 §10.6.5.2 Table 74 Request message SubFunction parameter definition p.76)
typedef enum _uds_svc_authentication_subfunction
{
    PUDS_SVC_PARAM_AT_DA = 0x00,     // DeAuthenticate
    PUDS_SVC_PARAM_AT_VCU = 0x01,    // VerifyCertificateUnidirectional
    PUDS_SVC_PARAM_AT_VCB = 0x02,    // VerifyCertificateBidirectional
    PUDS_SVC_PARAM_AT_POWN = 0x03,   // ProofOfOwnership
    PUDS_SVC_PARAM_AT_TC = 0x04,     // TransmitCertificate
    PUDS_SVC_PARAM_AT_RCFA = 0x05,   // RequestChallengeForAuthentication
    PUDS_SVC_PARAM_AT_VPOWNU = 0x06, // VerifyProofOfOwnershipUnidirectional
    PUDS_SVC_PARAM_AT_VPOWNB = 0x07, // VerifyProofOfOwnershipBidirectional
    PUDS_SVC_PARAM_AT_AC = 0x08      // AuthenticationConfiguration
} uds_svc_authentication_subfunction;

// Represents the return parameter for UDS service Authentication (see ISO 14229-1:2020 §B.5 AuthenticationReturnParameter definitions p.403)
typedef enum _uds_svc_authentication_return_parameter
{
    PUDS_SVC_PARAM_AT_RV_RA = 0x00,      // Request Accepted
    PUDS_SVC_PARAM_AT_RV_GR = 0x01,      // General Reject
    PUDS_SVC_PARAM_AT_RV_ACAPCE = 0x02,  // Authentication Configuration APCE
    PUDS_SVC_PARAM_AT_RV_ACACRAC = 0x03, // Authentication Configuration ACR with Asymmetric Cryptography
    PUDS_SVC_PARAM_AT_RV_ACACRSC = 0x04, // Authentication Configuration ACR with Symmetric Cryptography
    PUDS_SVC_PARAM_AT_RV_DAS = 0x10,     // DeAuthentication Successful
    PUDS_SVC_PARAM_AT_RV_CVOVN = 0x11,   // Certificate Verified, Ownership Verification Necessary
    PUDS_SVC_PARAM_AT_RV_OVAC = 0x12,    // Ownership Verified, Authentication Complete
    PUDS_SVC_PARAM_AT_RV_CV = 0x13       // Certificate Verified
} uds_svc_authentication_return_parameter;

/// <summary>
///  Sends Authentication service request with deAuthenticate subfunction.
/// </summary>
/// <remarks>
///  API provides uds_svc_authentication_subfunction and uds_svc_authentication_return_parameter
///  enumerations to help user to decode Authentication service responses.
/// </remarks>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcAuthenticationDA_2020(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request);

/// <summary>
///  Sends Authentication service request with verifyCertificateUnidirectional subfunction.
/// </summary>
/// <remarks>
///  API provides uds_svc_authentication_subfunction and uds_svc_authentication_return_parameter
///  enumerations to help user to decode Authentication service responses.
/// </remarks>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="communication_configuration">Configuration information about communication</param>
/// <param name="certificate_client">Buffer containing the certificate of the client</param>
/// <param name="certificate_client_size">Size in bytes of the certificate buffer</param>
/// <param name="challenge_client">Buffer containing the challenge of the client</param>
/// <param name="challenge_client_size">Size in bytes of the challenge buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcAuthenticationVCU_2020(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t communication_configuration,
    uint8_t* certificate_client,
    uint16_t certificate_client_size,
    uint8_t* challenge_client _DEF_ARG_NULL,
    uint16_t challenge_client_size _DEF_ARG);

/// <summary>
///  Sends Authentication service request with verifyCertificateBidirectional subfunction.
/// </summary>
/// <remarks>
///  API provides uds_svc_authentication_subfunction and uds_svc_authentication_return_parameter
///  enumerations to help user to decode Authentication service responses.
/// </remarks>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="communication_configuration">Configuration information about communication</param>
/// <param name="certificate_client">Buffer containing the certificate of the client</param>
/// <param name="certificate_client_size">Size in bytes of the certificate buffer</param>
/// <param name="challenge_client">Buffer containing the challenge of the client</param>
/// <param name="challenge_client_size">Size in bytes of the challenge buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcAuthenticationVCB_2020(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t communication_configuration,
    uint8_t* certificate_client,
    uint16_t certificate_client_size,
    uint8_t* challenge_client,
    uint16_t challenge_client_size);

/// <summary>
///  Sends Authentication service request with proofOfOwnership subfunction.
/// </summary>
/// <remarks>
///  API provides uds_svc_authentication_subfunction and uds_svc_authentication_return_parameter
///  enumerations to help user to decode Authentication service responses.
/// </remarks>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="proof_of_ownership_client">Buffer containing the proof of ownership of the client</param>
/// <param name="proof_of_ownership_client_size">Size in bytes of the proof of ownership buffer</param>
/// <param name="ephemeral_public_key_client">Buffer containing the ephemeral public key of the client</param>
/// <param name="ephemeral_public_key_client_size">Size in bytes of the ephemeral public key buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcAuthenticationPOWN_2020(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t* proof_of_ownership_client,
    uint16_t proof_of_ownership_client_size,
    uint8_t* ephemeral_public_key_client _DEF_ARG_NULL,
    uint16_t ephemeral_public_key_client_size _DEF_ARG);

/// <summary>
///  Sends Authentication service request with requestChallengeForAuthentication subfunction.
/// </summary>
/// <remarks>
///  API provides uds_svc_authentication_subfunction and uds_svc_authentication_return_parameter
///  enumerations to help user to decode Authentication service responses.
/// </remarks>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="communication_configuration">Configuration information about communication</param>
/// <param name="algorithm_indicator">Buffer of 16 bytes containing the algorithm indicator</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcAuthenticationRCFA_2020(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t communication_configuration,
    uint8_t* algorithm_indicator);

/// <summary>
///  Sends Authentication service request with verifyProofOfOwnershipUnidirectional subfunction.
/// </summary>
/// <remarks>
///  API provides uds_svc_authentication_subfunction and uds_svc_authentication_return_parameter
///  enumerations to help user to decode Authentication service responses.
/// </remarks>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="algorithm_indicator">Buffer of 16 bytes containing the algorithm indicator</param>
/// <param name="proof_of_ownership_client">Buffer containing the proof of ownership of the client</param>
/// <param name="proof_of_ownership_client_size">Size in bytes of the proof of ownership buffer</param>
/// <param name="challenge_client">Buffer containing the challenge of the client</param>
/// <param name="challenge_client_size">Size in bytes of the challenge buffer</param>
/// <param name="additional_parameter">Buffer containing additional parameters</param>
/// <param name="additional_parameter_size">Size in bytes of the additional parameter buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcAuthenticationVPOWNU_2020(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t* algorithm_indicator,
    uint8_t* proof_of_ownership_client,
    uint16_t proof_of_ownership_client_size,
    uint8_t* challenge_client _DEF_ARG_NULL,
    uint16_t challenge_client_size _DEF_ARG,
    uint8_t* additional_parameter _DEF_ARG_NULL,
    uint16_t additional_parameter_size _DEF_ARG);

/// <summary>
///  Sends Authentication service request with verifyProofOfOwnershipBidirectional subfunction.
/// </summary>
/// <remarks>
///  API provides uds_svc_authentication_subfunction and uds_svc_authentication_return_parameter
///  enumerations to help user to decode Authentication service responses.
/// </remarks>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <param name="algorithm_indicator">Buffer of 16 bytes containing the algorithm indicator</param>
/// <param name="proof_of_ownership_client">Buffer containing the proof of ownership of the client</param>
/// <param name="proof_of_ownership_client_size">Size in bytes of the proof of ownership buffer</param>
/// <param name="challenge_client">Buffer containing the challenge of the client</param>
/// <param name="challenge_client_size">Size in bytes of the challenge buffer</param>
/// <param name="additional_parameter">Buffer containing additional parameters</param>
/// <param name="additional_parameter_size">Size in bytes of the additional parameter buffer</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcAuthenticationVPOWNB_2020(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request,
    uint8_t* algorithm_indicator,
    uint8_t* proof_of_ownership_client,
    uint16_t proof_of_ownership_client_size,
    uint8_t* challenge_client,
    uint16_t challenge_client_size,
    uint8_t* additional_parameter _DEF_ARG_NULL,
    uint16_t additional_parameter_size _DEF_ARG);

/// <summary>
///  Sends Authentication service request with authenticationConfiguration subfunction.
/// </summary>
/// <remarks>
///  API provides uds_svc_authentication_subfunction and uds_svc_authentication_return_parameter
///  enumerations to help user to decode Authentication service responses.
/// </remarks>
/// <param name="channel">A PCANTP channel handle representing a PUDS channel</param>
/// <param name="request_config">Request configuration</param>
/// <param name="out_msg_request">(out) request message created and sent by the function</param>
/// <returns>A uds_status code. PUDS_STATUS_OK is returned on success</returns>
uds_status __stdcall UDS_SvcAuthenticationAC_2020(
    cantp_handle channel,
    uds_msgconfig request_config,
    uds_msg* out_msg_request);


#ifdef __cplusplus
}
#endif

#endif
