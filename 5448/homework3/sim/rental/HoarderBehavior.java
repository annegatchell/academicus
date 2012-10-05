package sim.rental;

public class HoarderBehavior implements CustomerRentalBehavior{
	int videos = 3;
	int duration = 7;
	int minVideos = 3;

	public RentalRequest howManyToRent(){
		return (new RentalRequest(videos, duration, minVideos));
	}

	public RentalRequest getRentalRequirements(){
		return(new RentalRequest(videos, duration, minVideos));
	}

	public String getBehaviorType(){
		return "Hoarder";
	}
}