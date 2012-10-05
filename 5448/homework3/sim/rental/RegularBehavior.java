package sim.rental;

import java.util.Random;

public class RegularBehavior implements CustomerRentalBehavior{
	int minVideos = 1;
	int maxVideos = 3;
	int minDuration = 3;
	int maxDuration = 5;

	public RentalRequest howManyToRent(){
		Random generator = new Random(17239987);
		return (new RentalRequest(generator.nextInt(maxVideos-minVideos)+ minVideos, 
								generator.nextInt(maxDuration-minDuration)+ minDuration,
								minVideos));
	}

	public RentalRequest getRentalRequirements(){
		return(new RentalRequest(maxVideos, maxDuration, minVideos));
	}

	public String getBehaviorType(){
		return "Regular";
	}
}