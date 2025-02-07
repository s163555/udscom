#include "mex.h"
#include <unistd.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/can.h>
#include <linux/can/isotp.h>
#include <cstring>
#include <cerrno>
#include <iostream>
#include <vector>
#include <string>

// Global socket variable
static int sock = -1;

// Helper to parse a hex string (like "18DAF101") into a 32-bit ID with optional CAN_EFF_FLAG
static unsigned long parseHexID(const std::string &hexStr)
{
    unsigned long val = std::stoul(hexStr, nullptr, 16);
    // If > 0x7FF, we assume extended (29-bit) ID. Add CAN_EFF_FLAG:
    if (val > 0x7FF) {
        val |= CAN_EFF_FLAG;
    }
    return val;
}

// Function to initialize the ISO-TP socket
static bool initializeSocket(const std::string &canIf, unsigned long rxId, unsigned long txId)
{
    if (sock >= 0) {
        // Socket is already open
        return true;
    }

    // Create ISO-TP socket
    sock = socket(PF_CAN, SOCK_DGRAM, CAN_ISOTP);
    if (sock < 0) {
        mexErrMsgTxt("socket(CAN_ISOTP) failed. Check permissions or kernel support.");
        return false;
    }

    // Optional: set a small read timeout
    struct timeval tv;
    tv.tv_sec = 1;  // 1 second timeout
    tv.tv_usec = 0;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        // Not fatal on older kernels, continue
    }

    // Bind to canIf with rxId, txId
    struct sockaddr_can addr;
    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;

    // Convert interface name to ifindex
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, canIf.c_str(), IFNAMSIZ-1);

    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        close(sock);
        sock = -1;
        mexErrMsgTxt("ioctl(SIOCGIFINDEX) failed. Invalid interface name?");
        return false;
    }
    addr.can_ifindex = ifr.ifr_ifindex;

    // Fill in isotp IDs
    addr.can_addr.tp.rx_id = rxId;
    addr.can_addr.tp.tx_id = txId;

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(sock);
        sock = -1;
        mexErrMsgTxt("bind() failed. Possibly interface down or perms missing?");
        return false;
    }

    return true;
}

// Function to close the ISO-TP socket
static void closeSocket()
{
    if (sock >= 0) {
        close(sock);
        sock = -1;
    }
}

// MEX entry point
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs < 4) {
        mexErrMsgTxt("Usage: resp = isotp_mex(canIf, rxHex, txHex, reqBytes)");
        return;
    }

    // Parse inputs
    char canIfCstr[128], rxHexCstr[64], txHexCstr[64];
    mxGetString(prhs[0], canIfCstr, sizeof(canIfCstr));
    mxGetString(prhs[1], rxHexCstr, sizeof(rxHexCstr));
    mxGetString(prhs[2], txHexCstr, sizeof(txHexCstr));
    std::string canIfStr(canIfCstr), rxHexStr(rxHexCstr), txHexStr(txHexCstr);

    if (!mxIsUint8(prhs[3])) {
        mexErrMsgTxt("reqBytes must be a uint8 array");
    }
    size_t reqLen = mxGetNumberOfElements(prhs[3]);
    unsigned char *reqData = (unsigned char *)mxGetData(prhs[3]);

    // Convert hex ID strings
    unsigned long rxId = parseHexID(rxHexStr);
    unsigned long txId = parseHexID(txHexStr);

    // Initialize the socket if not already open
    if (!initializeSocket(canIfStr, rxId, txId)) {
        return;
    }

    // Send request
    ssize_t nSent = write(sock, reqData, reqLen);
    if (nSent < 0) {
        mexErrMsgTxt("write() failed on ISO-TP socket");
        return;
    }

    // Read response (multi-frame automatically reassembled by kernel)
    unsigned char rxBuf[4096];
    memset(rxBuf, 0, sizeof(rxBuf));
    ssize_t nRead = read(sock, rxBuf, sizeof(rxBuf));
    if (nRead < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // Timeout => return empty
            nRead = 0;
        } else {
            mexErrMsgTxt("read() failed on ISO-TP socket. Possibly no response?");
            return;
        }
    }

    // Return data as uint8 row vector
    plhs[0] = mxCreateNumericMatrix(1, nRead, mxUINT8_CLASS, mxREAL);
    unsigned char *outData = (unsigned char *)mxGetData(plhs[0]);
    memcpy(outData, rxBuf, nRead);
}

// Cleanup function to close the socket when the MEX function is unloaded
void mexExit(void)
{
    closeSocket();
}

// Register the cleanup function
void mexAtExit(void)
{
    mexAtExit(mexExit);
}