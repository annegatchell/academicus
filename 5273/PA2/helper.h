
#include <time.h>
#include <ctype.h>
#include <semaphore.h>
#include <string.h>

/*int sem_init(sem_t *sem, int pshared, unsigned int value);
sem - points to a semaphore object to initialize
pshared - is a flag indicating whether or not the semaphore should be shared with fork()ed processes. LinuxThreads does not currently support shared semaphores
value - is an initial value to set the semaphore to*/

//@@@ Header flag: 0 is for data, 1 is for ACK @@@@//

#define SWS 1
#define RWS 1

typedef u_char SwpSeqno;
typedef struct Msg {u_char m[512];} Msg;
typedef struct Data {u_char d[509];} Data;

typedef struct {
  SwpSeqno SeqNum; /*sequence number of this frame */
  SwpSeqno AckNum; /* ack of received frame */
  u_char Flags; /* up to 8 bits worth of flags 0 when data, 1 when ACKR*/
} SwpHdr;

typedef struct{
  /* sender side state */
  SwpSeqno  LAR;      /*seqno of last ACK*/
  SwpSeqno  LFS;      /*last frame sent*/
  sem_t sendWindowNotFull;
  int       newSend;
  SwpHdr    hdr;      /*pre-initialized header*/
  
  struct sendQ_slot{
    struct timeval timeout;  /*event associated with send timeout*/
    char    acked; //Has this slot been acked already? 0 is false, 1 is true
    Msg     msg;
  } sendQ[SWS];
  
  int smallestTime; /* ticker of SWS with the least time left */
  SwpSeqno * receivedACK_ptr; /*pointer to the array of received acks */

  /* receiver side state*/
  SwpSeqno  NFE;      /* seqno of next frame expected */
  SwpSeqno  LFR;      /* seqno of last frame received */
  SwpSeqno  LAF;      /* seqno of largest acceptable frame */

  struct recvQ_slot{
    int valid;
    SwpSeqno seqno;     /*is msg valid?*/
    Data     data;
  } recvQ[RWS];
  SwpSeqno * receivedPCKT_ptr; /*pointer to received packets*/

} SwpState;


