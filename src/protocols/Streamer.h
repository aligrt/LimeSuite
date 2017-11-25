/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Streamer.h
 * Author: ignas
 *
 * Created on November 20, 2017, 12:29 PM
 */

#ifndef STREAMER_H
#define STREAMER_H

#include "dataTypes.h"
#include "fifo.h"

namespace lime
{
   
class ILimeSDRStreaming;
class StreamChannel;

/*!
 * The stream config structure is used with the SetupStream() API.
 */
struct LIME_API StreamConfig
{
    StreamConfig(void);

    //! True for transmit stream, false for receive
    bool isTx;

    /*!
     * A list of channels to use.
     *  - Example ChA on RFIC0: [0]
     *  - Example MIMO on RFIC0: [0, 1]
     *  - Example MIMO on RFIC1: [2, 3]
     */
    uint8_t channelID;

    float performanceLatency;

    //! Possible stream data formats
    enum StreamDataFormat
    {
        FMT_INT16,
        FMT_INT12,
        FMT_FLOAT32,
    };

    /*!
     * The buffer length is a size in samples
     * that used for allocating internal buffers.
     * Default: 0, meaning automatic selection
     */
    size_t bufferLength;

    //! The format of the samples in Read/WriteStream().
    StreamDataFormat format;

    /*!
     * The format of samples over the wire.
     * This is not the format presented to the API caller.
     * Choosing a compressed format can decrease link use
     * at the expense of additional processing on the PC
     * Default: STREAM_12_BIT_IN_16
     */
    StreamDataFormat linkFormat;
};
    
class Streamer
{
public:
    Streamer(ILimeSDRStreaming* port);
    ~Streamer();

    StreamChannel* SetupStream(const StreamConfig& config);
    int CloseStream(StreamChannel* streamID);
    int GetStreamSize(bool tx);

    uint64_t GetHardwareTimestamp(void);
    void SetHardwareTimestamp(const uint64_t now);
    int UpdateThreads(bool stopAll = false);

    std::atomic<uint32_t> rxDataRate_Bps;
    std::atomic<uint32_t> txDataRate_Bps;
    ILimeSDRStreaming* dataPort;
    std::thread rxThread;
    std::thread txThread;
    std::atomic<bool> rxRunning;
    std::atomic<bool> txRunning;
    std::atomic<bool> terminateRx;
    std::atomic<bool> terminateTx;

    StreamChannel* mRxStreams[2];
    StreamChannel* mTxStreams[2];
    std::atomic<uint64_t> rxLastTimestamp;
    std::atomic<uint64_t> txLastLateTime;
    uint64_t mTimestampOffset;
    int mChipID;
    int streamSize;
    unsigned txBatchSize;
    unsigned rxBatchSize;
    void ReceivePacketsLoop();
    void TransmitPacketsLoop();
};

class LIME_API StreamChannel 
{
public:
    struct Frame
    {
        uint64_t timestamp;
        static const uint16_t samplesCount = samples12InPkt;
        complex16_t samples[samplesCount];
    };
    
    struct Metadata
    {
        uint64_t timestamp;
        uint32_t flags;
    };
    
    struct Info
    {
        float sampleRate;
        int fifoSize;
        int fifoItemsCount;
        int overrun;
        int underrun;
        bool active;
        float linkRate;
        int droppedPackets;
        uint64_t timestamp;
    };
    
    StreamChannel(Streamer* streamer, StreamConfig config);
    ~StreamChannel();
    
    int Read(void* samples, const uint32_t count, Metadata* meta, const int32_t timeout_ms = 100);
    int Write(const void* samples, const uint32_t count, const Metadata* meta, const int32_t timeout_ms = 100);
    StreamChannel::Info GetInfo();
    int GetStreamSize();

    bool IsActive() const;
    int Start();
    int Stop();
    StreamConfig config;
    Streamer* mStreamer;
    unsigned overflow;
    unsigned underflow;
    unsigned pktLost;
    bool mActive;
       
protected:
    RingFIFO* fifo;   
    std::atomic<uint64_t> sampleCnt;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
private:
    StreamChannel() = default;
};



}

#endif /* STREAMER_H */
