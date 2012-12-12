/*
 * hw4lib.h
 *
 *  Created on: Nov 7, 2012
 *      Author: ari
 */

#ifndef HW4LIB_H_
#define HW4LIB_H_


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}



#endif /* HW4LIB_H_ */
