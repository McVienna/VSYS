#include <cstdio>
#include <cstring>
#include <ldap.h>

#include <string>
#include <map>

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


//Try to log into LDAP
int Ldap::login(LDAP* &myldap, Login_prot* &login_protocol, unsigned long clientAddress, int &locktime)
{
  //Handle multiple access attempts
  auto it = this->clientLoginAttempts.find(clientAddress);
  time_t timeNow = time(NULL);
  
  //build username
  char username[50];
  strcat(username, (LDAP_USERPAD_L));
  strcat(username, login_protocol->get_username().c_str());
  strcat(username, (LDAP_USERPAD_R));
  
  //try to log in
  int rc = ldap_simple_bind_s(myldap, username, login_protocol->get_password().c_str());


  if (it != this->clientLoginAttempts.end() && this->clientLoginAttempts[clientAddress] >= 3)
  {
    if (difftime(this->lockedClients[clientAddress], timeNow) < 0)
    {
      this->clientLoginAttempts[clientAddress] = 0;
      this->lockedClients.erase (clientAddress);
    }
    else
    {
      locktime = difftime(this->lockedClients[clientAddress], timeNow);
      return -1;
    }
  }

  if (rc == LDAP_SUCCESS)
  {
    printf("Client %s logged in successfully\n", login_protocol->get_username().c_str());
    if (it != this->clientLoginAttempts.end())
    {
      this->clientLoginAttempts.erase(it);
    }
  }
  else
  {
    if (it != clientLoginAttempts.end())
    {
      this->clientLoginAttempts[clientAddress] += 1;
      if (this->clientLoginAttempts[clientAddress] >= 3)
      {
        lockedClients[clientAddress] = time(NULL) + LOGLOCK_TIME;
      }
    }
    else
    {
      this->clientLoginAttempts[clientAddress] = 1;
    }
  }

  return rc;
}