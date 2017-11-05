#include <cstdio>
#include <cstring>
#include <ldap.h>

#include <string>

#include "ldaphandler.h"
#include "protocols.h"


Ldap::Ldap(){
}

Ldap::~Ldap(){
}

//return value > 0 on success, otherwise -1;
int Ldap::init(LDAP* &myldap)
{
  if ((myldap = ldap_init(LDAP_HOST, LDAP_PORT)) == NULL) return 1;
  else                                                    return -1;
} 

int Ldap::login(LDAP *myldap, Protocol *login_protocol)
{
  Login_prot *login_prot = static_cast<Login_prot* >(login_protocol);
  /* anonymous bind */
  int rc = ldap_simple_bind_s(myldap, login_prot->get_username().c_str(), login_prot->get_password().c_str());

  if (rc != LDAP_SUCCESS)
  {
    printf("Client %s logged in successfully", login_prot->get_username().c_str());
  }
  
  delete login_prot;
  return rc;
}