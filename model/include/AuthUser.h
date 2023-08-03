#pragma once

#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>

namespace dbo = Wt::Dbo;

class AuthUser;
using AuthInfo = Wt::Auth::Dbo::AuthInfo<AuthUser>;

class AuthUser {
public:

  template<class Action>
  void persist(Action& a)
  {
  }
};


DBO_EXTERN_TEMPLATES(AuthUser)
