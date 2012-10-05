package sim.rental;

public class RentalRequest{
	int numVideos;
	int numNights;
	int minRequested;

	public RentalRequest(int vids, int nights, int minVids){
		this.numVideos = vids;
		this.numNights = nights;
		this.minRequested = minVids;
	}

	public int getNumNights(){
		return this.numNights;
	}

	public void setNumNights(int nights){
		this.numNights = nights;
	}

	public int getNumVideos(){
		return this.numVideos;
	}

	public void setNumVideos(int vids){
		this.numVideos = vids;
	}

	public int getMinRequested(){
		return this.minRequested;
	}

	public void setMinRequested(int set){
		this.minRequested = set;
	}
}