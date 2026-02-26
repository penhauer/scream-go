// Copyright (c) 2020 Mathis Engelbart All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "ScreamTxC.h"

#include "include/ScreamTx.h"

ScreamV2Tx* ScreamTxInit(bool isL4s,
                         bool pacing,
                         bool delayBasedCC,
                         float packetPacingHeadroom,
                         float adaptivePaceHeadroom,
                         float bytesInFlightHeadroom,
                         float maxWindowHeadroom,
                         bool relaxedPacing,
                         int initialCwnd) {
  if (initialCwnd <= 0) {
    initialCwnd = 100 * 1000 / 8;
  }

  auto s = new ScreamV2Tx(
    0.7,
    0.7,
    0.06f,
    initialCwnd,
    packetPacingHeadroom,
    adaptivePaceHeadroom,
    bytesInFlightHeadroom,
    0.45f,
    isL4s,
    maxWindowHeadroom
  );

  s->enablePacketPacing(pacing);
  s->enableDelayBasedCongestionControl(delayBasedCC);
  s->enableRelaxedPacing(relaxedPacing);

  return s;
}

void ScreamTxFree(ScreamV2Tx* s) {
  delete s;
}

void ScreamTxRegisterNewStream(ScreamV2Tx* s,
                               RtpQueueC* rtpQueue,
                               uint32_t ssrc,
                               float priority,
                               float minBitrate,
                               float startBitrate,
                               float maxBitrate,
                               float maxRtpQueueDelay) {
  ScreamV2Tx* stx = (ScreamV2Tx*)s;
  RtpQueueIface* rtpq = (RtpQueueIface*)rtpQueue;

  stx->registerNewStream(rtpq, ssrc, priority, minBitrate, startBitrate,
                         maxBitrate, maxRtpQueueDelay, false, 0.0);
}

void ScreamTxNewMediaFrame(ScreamV2Tx* s,
                           uint32_t time_ntp,
                           uint32_t ssrc,
                           int bytesRtp,
                           bool isMarker) {
  s->newMediaFrame(time_ntp, ssrc, bytesRtp, isMarker);
}

float ScreamTxIsOkToTransmit(ScreamV2Tx* s, uint32_t time_ntp, uint32_t ssrc) {
  return s->isOkToTransmit(time_ntp, ssrc);
}

float ScreamTxAddTransmitted(ScreamV2Tx* s,
                             uint32_t time_ntp,
                             uint32_t ssrc,
                             int size,
                             uint16_t seqNr,
                             bool isMark,
                             float rtpQueueDelay,
                             uint32_t ts
                            ) {
  ScreamV2Tx* stx = (ScreamV2Tx*)s;
  return stx->addTransmitted(time_ntp, ssrc, size, seqNr, isMark, rtpQueueDelay, ts);
}

void ScreamTxIncomingStdFeedbackBuf(ScreamV2Tx* s,
                                    uint32_t time_ntp,
                                    unsigned char* buf,
                                    int size) {
  s->incomingStandardizedFeedback(time_ntp, buf, size);
}

void ScreamTxIncomingStdFeedback(ScreamV2Tx* s,
                                 uint32_t time_ntp,
                                 int streamId,
                                 uint32_t timestamp,
                                 uint16_t seqNr,
                                 uint8_t ceBits,
                                 bool isLast) {
  s->incomingStandardizedFeedback(time_ntp, streamId, timestamp, seqNr, ceBits,
                                  isLast);
}

float ScreamTxGetTargetBitrate(ScreamV2Tx* s,
                               uint32_t time_ntp,
                               uint32_t ssrc) {
  return s->getTargetBitrate(time_ntp, ssrc);
}

void ScreamTxGetStatistics(ScreamV2Tx* s, float time_ntp, char* result) {
  s->getStatistics(time_ntp, result);
}
