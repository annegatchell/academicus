
#include <time.h>
#include <ctype.h>
#include <semaphore.h>
#include <string.h>

/*int sem_init(sem_t *sem, int pshared, unsigned int value);
sem - points to a semaphore object to initialize
pshared - is a flag indicating whether or not the semaphore should be shared with fork()ed processes. LinuxThreads does not currently support shared semaphores
value - is an initial value to set the semaphore to*/

#define SWS 1
#define RWS 1

typedef u_char SwpSeqno;
typedef char Msg[512];

typedef struct {
  SwpSeqno SeqNum; /*sequence number of this frame */
  SwpSeqno AckNum; /* ack of received frame */
  u_char Flags; /* up to 8 bits worth of flags */
} SwpHdr;

typedef struct{
  /* sender side state */
  SwpSeqno  LAR;      /*seqno of last ACK*/
  SwpSeqno  LFS;      /*last frame sent*/
  sem_t sendWindowNotFull;
  SwpHdr    hdr;      /*pre-initialized header*/
  
  struct sendQ_slot{
    struct timeval timeout;  /*event associated with send timeout*/
    Msg     msg;
  } sendQ[SWS];
  
  struct timeval smallestTime; /* ticker of SWS with the least time left */

  /* receiver side state*/
  SwpSeqno  NFE;      /* seqno of next frame expected */
  SwpSeqno  LFR;      /* seqno of last frame received */
  SwpSeqno  LAF;      /* seqno of largest acceptable frame */

  struct recvQ_slot{
    int     received;     /*is msg valid?*/
    Msg     msg;
  } recvQ[RWS];
} SwpState;

