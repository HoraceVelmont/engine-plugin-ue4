// Copyright (C) 2013-2016 iFunFactory Inc. All Rights Reserved.
//
// This work is confidential and proprietary to iFunFactory Inc. and
// must not be used, disclosed, copied, or distributed without the prior
// consent of iFunFactory Inc.

#ifndef SRC_FUNAPI_SESSION_H_
#define SRC_FUNAPI_SESSION_H_

#include "funapi_transport.h"

namespace fun {

enum class SessionEventType : int {
  kOpened,
  kClosed,
  kChanged,
};

enum class TransportEventType : int {
  kStarted,
  kStopped,
  kConnectionFailed,
  kConnectionTimedOut,
  kDisconnected,
};


class FunapiSessionImpl;
class FunapiSession : public std::enable_shared_from_this<FunapiSession> {
 public:
  typedef std::function<void(const std::shared_ptr<FunapiSession>&,
                             const TransportProtocol,
                             const std::string&,
                             const std::string&)> JsonRecvHandler;

  typedef std::function<void(const std::shared_ptr<FunapiSession>&,
                             const TransportProtocol,
                             const FunMessage&)> ProtobufRecvHandler;

  typedef std::function<void(const std::shared_ptr<FunapiSession>&,
                             const TransportProtocol,
                             const SessionEventType,
                             const std::string&)> SessionEventHandler;

  typedef std::function<void(const std::shared_ptr<FunapiSession>&,
                             const TransportProtocol,
                             const TransportEventType,
                             const std::shared_ptr<FunapiError>&)> TransportEventHandler;

  FunapiSession() = delete;
  FunapiSession(const char* hostname_or_ip, bool reliability = false);
  ~FunapiSession();

  static std::shared_ptr<FunapiSession> create(const char* hostname_or_ip, bool reliability = false);

  void Connect(const TransportProtocol protocol, int port, FunEncoding encoding, std::shared_ptr<FunapiTransportOption> option = nullptr);
  void Connect(const TransportProtocol protocol);
  void Close();
  void Close(const TransportProtocol protocol);

  void SendMessage(const std::string &msg_type, std::string &json_string, TransportProtocol protocol = TransportProtocol::kDefault);
  void SendMessage(FunMessage &message, TransportProtocol protocol = TransportProtocol::kDefault);

  bool IsConnected(const TransportProtocol protocol = TransportProtocol::kDefault) const;
  bool IsReliableSession() const;

  FunEncoding GetEncoding(const TransportProtocol protocol) const;
  bool HasTransport(const TransportProtocol protocol) const;
  void Update();

  void SetDefaultProtocol(const TransportProtocol protocol);
  TransportProtocol GetDefaultProtocol() const;

  void AddSessionEventCallback(const SessionEventHandler &handler);
  void AddTransportEventCallback(const TransportEventHandler &handler);

  void AddProtobufRecvCallback(const ProtobufRecvHandler &handler);
  void AddJsonRecvCallback(const JsonRecvHandler &handler);

 private:
  std::shared_ptr<FunapiSessionImpl> impl_;
};

}  // namespace fun

#endif  // SRC_FUNAPI_SESSION_H_
