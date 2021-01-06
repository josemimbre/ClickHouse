#pragma once

#include <Poco/Net/Socket.h>

#include <IO/ReadBuffer.h>
#include <IO/BufferWithOwnMemory.h>

namespace DB
{

/** Works with the ready Poco::Net::Socket. Blocking operations.
  */
class ReadBufferFromPocoSocket : public BufferWithOwnMemory<ReadBuffer>
{
protected:
    Poco::Net::Socket & socket;

    /** For error messages. It is necessary to receive this address in advance, because,
      *  for example, if the connection is broken, the address will not be received anymore
      *  (getpeername will return an error).
      */
    Poco::Net::SocketAddress peer_address;

    bool nextImpl() override;

public:
    ReadBufferFromPocoSocket(Poco::Net::Socket & socket_, size_t buf_size = DBMS_DEFAULT_BUFFER_SIZE);

    bool poll(size_t timeout_microseconds);

    void setAsyncCallback(std::function<void(Poco::Net::Socket &)> async_callback_) { async_callback = std::move(async_callback_); }

private:
    std::function<void(Poco::Net::Socket &)> async_callback;
};

}
