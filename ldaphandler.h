#ifndef LDAPHANDLER_H
#define LDAPHANDLER_H

#define LDAP_HOST "ldap.technikum-wien.at"
#define LDAP_PORT 389

#include <ldap.h>
#include "protocols.h"


class Ldap {
  public:
    Ldap();
    ~Ldap();
    int login(LDAP *myldap, Protocol *login_protocol); //checks, if User is in Database;
    int init(LDAP* &myldap); //returns > 0 on success, otherwise rturn -1;


};

#endif //LDAPHANDLER_H INCLUDED