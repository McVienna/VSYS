#ifndef LDAPHANDLER_H
#define LDAPHANDLER_H

#define LDAP_HOST "ldap.technikum-wien.at"
#define LDAP_PORT 389
#define LDAP_USERPAD_L "uid="
#define LDAP_USERPAD_R ",ou=People,dc=technikum-wien,dc=at"

//login lock in seconds. Default: 30mins
#define LOGLOCK_TIME 1800

#include <map>
#include <ctime>

#include <ldap.h>
#include "protocols.h"

class Ldap {
  public:
    Ldap();
    ~Ldap();
    int login(LDAP *myldap, Login_prot* &login_protocol, unsigned long clientAddress); //checks, if User is in Database;
    int init(LDAP* &myldap); //returns > 0 on success, otherwise return -1;

    std::map<unsigned long, time_t> lockedClients;

  private:
    std::map<unsigned long, int> clientLoginAttempts;
};

#endif //LDAPHANDLER_H INCLUDED