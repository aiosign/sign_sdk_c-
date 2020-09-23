// **********************************************************************
//
// Copyright (c) 2003-2005 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#pragma once

#include "HICEException.h"

namespace HICEUtil
{

class HUTIL_API ThreadSyscallException : public Exception
{
public:

    ThreadSyscallException(const char*, int, int);
    virtual const std::string& ice_name() const;
    virtual void ice_print(std::ostream&) const;
    virtual Exception* ice_clone() const;
    virtual void ice_throw() const;

    int error() const;
private:

    const int _error;
    static ::std::string _name;
};

class HUTIL_API ThreadLockedException : public Exception
{
public:

    ThreadLockedException(const char*, int);
    virtual const std::string& ice_name() const;
    virtual Exception* ice_clone() const;
    virtual void ice_throw() const;

private:

    static ::std::string _name;
};

class HUTIL_API ThreadStartedException : public Exception
{
public:

    ThreadStartedException(const char*, int);
    virtual const std::string& ice_name() const;
    virtual Exception* ice_clone() const;
    virtual void ice_throw() const;

private:

    static ::std::string _name;
};

class HUTIL_API ThreadNotStartedException : public Exception
{
public:

    ThreadNotStartedException(const char*, int);
    virtual const std::string& ice_name() const;
    virtual Exception* ice_clone() const;
    virtual void ice_throw() const;

private:

    static ::std::string _name;
};
    
}

