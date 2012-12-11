#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[]){
	
	if(argc < 2){
		printf("Use: ./executable <initial_deviation>\n");
		exit(1);
	}

	double SampleRTT = 1.0;
	double EstimatedRTT = 4.0;
	double difference;
	float deviation = atof(argv[1]);
	double TimeOut = 100;
	int count = 0;

	while(TimeOut > 4.0){
		
		SampleRTT = 1.0;
		difference = SampleRTT - EstimatedRTT;
		printf("differece %f\n", difference);
		EstimatedRTT = EstimatedRTT + (1.0/8)*difference;
		printf("EstimatedRTT %f\n", EstimatedRTT);
		printf("deviation %f\n", deviation);
		deviation = deviation + (1.0/8)*(abs(difference) - deviation);
		printf("deviation %f\n", deviation);
		TimeOut = EstimatedRTT + 4*deviation;
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
