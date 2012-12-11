#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	
	if(argc < 2){
		printf("Use: ./executable <initial_deviation>\n");
		exit(1);
	}

	int SampleRTT = 1;
	int EstimatedRTT = 4<<3;
	float dev = atof(argv[1])*8;
	int deviation = (int)dev;
	double TimeOut = 100;
	int count = 0;

	while(TimeOut > 4.0){
		
		SampleRTT = 1;
		SampleRTT -= (EstimatedRTT >> 3);
		EstimatedRTT += SampleRTT;
		if(SampleRTT < 0){
			SampleRTT = -SampleRTT;
		}
		SampleRTT -= (deviation >> 3);
		deviation += SampleRTT;
		TimeOut = (EstimatedRTT >> 3) + (deviation >> 1);
		printf("Timeout = %f\n", TimeOut);
		count++;
		if(count > 100){
			TimeOut = 0;
			printf("Count has gone above %d, exiting\n", count);
		}
	}

	printf("Count = %d\n", count);
	return 0;
}



printf("%d\n", EstimatedRTT);
		printf("%d\n", SampleRTT);
		printf("%f\n", dev);
		printf("%d\n", deviation);
		printf("%d\n", SampleRTT);