// WmsRestService (info@francescodirienzo.it)
// Copyright Francesco Di Rienzo 2020
// MIT License

#pragma once

#include "WmsRestService/Configuration.hpp"

#if !WMSRESTSERVICE_DEBUG
#ifdef __clang__
#pragma clang system_header
#elif defined __GNUC__
#pragma GCC system_header
#endif
#endif

#include "WmsRestService/RestService.hpp"

namespace WmsRestService {
  using WMSRESTSERVICE_NAMESPACE::RestService;
}