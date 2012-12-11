/*
 * clientlib.h
 *
 *  Created on: Nov 7, 2012
 *      Author: ari
 */

#ifndef CLIENTLIB_H_
#define CLIENTLIB_H_
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


#endif /* CLIENTLIB_H_ */
