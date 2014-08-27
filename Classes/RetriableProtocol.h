#ifndef __RETRIABLE_PROTOCOL_H__
#define __RETRIABLE_PROTOCOL_H__

class RetriableProtocol
{
public:
    virtual void retry() = 0;
};

#endif

