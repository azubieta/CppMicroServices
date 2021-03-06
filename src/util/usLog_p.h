/*=============================================================================

  Library: CppMicroServices

  Copyright (c) German Cancer Research Center,
    Division of Medical and Biological Informatics

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

#ifndef USLOG_P_H
#define USLOG_P_H

#include <usConfig.h>

#include <iostream>
#include <sstream>

US_BEGIN_NAMESPACE

US_EXPORT void message_output(MsgType, const char* buf);

struct LogMsg {

  LogMsg(int t, const char* file, int ln, const char* func)
    : type(static_cast<MsgType>(t)), enabled(true), buffer()
  { buffer << "In " << func << " at " << file << ":" << ln << " : "; }

  ~LogMsg() { if(enabled) message_output(type, buffer.str().c_str()); }

  template<typename T>
  LogMsg& operator<<(T t)
  {
    if (enabled) buffer << t;
    return *this;
  }

  LogMsg& operator()(bool flag)
  {
    this->enabled = flag;
    return *this;
  }

private:

  MsgType type;
  bool enabled;
  std::stringstream buffer;
};

struct NoLogMsg {

  template<typename T>
  NoLogMsg& operator<<(T)
  {
    return *this;
  }

  NoLogMsg& operator()(bool)
  {
    return *this;
  }

};

US_END_NAMESPACE

#if defined(US_ENABLE_DEBUG_OUTPUT)
  #define US_DEBUG US_PREPEND_NAMESPACE(LogMsg)(0, __FILE__, __LINE__, __FUNCTION__)
#else
  #define US_DEBUG true ? US_PREPEND_NAMESPACE(NoLogMsg)() : US_PREPEND_NAMESPACE(NoLogMsg)()
#endif

#if !defined(US_NO_INFO_OUTPUT)
  #define US_INFO US_PREPEND_NAMESPACE(LogMsg)(1, __FILE__, __LINE__, __FUNCTION__)
#else
  #define US_INFO  true ? US_PREPEND_NAMESPACE(NoLogMsg)() : US_PREPEND_NAMESPACE(NoLogMsg)()
#endif

#if !defined(US_NO_WARNING_OUTPUT)
  #define US_WARN US_PREPEND_NAMESPACE(LogMsg)(2, __FILE__, __LINE__, __FUNCTION__)
#else
  #define US_WARN true ? US_PREPEND_NAMESPACE(LogMsg)() : US_PREPEND_NAMESPACE(LogMsg)()
#endif

#define US_ERROR US_PREPEND_NAMESPACE(LogMsg)(3, __FILE__, __LINE__, __FUNCTION__)

#endif // USLOG_P_H
