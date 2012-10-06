package sim.rental;

import java.util.List;
import java.util.ArrayList;

public class Rental{
	boolean isActive;
	ArrayList<Video> videos;
	int duration;
	int remainingDays;
	int startDates;
	Customer customer;
	double cost;
	int rentalID;

	public Rental(List<Video> vids, int duration, Customer cus, int id){
		this.isActive = true;
		this.videos = new ArrayList<Video>(vids);
		this.duration = duration;
		this.remainingDays = duration;
		this.customer = cus;
		this.cost = 0;
		for(Video vid : this.videos){
			this.cost += vid.getPrice();
		}
		this.rentalID = id;
	}

	public Rental(){
		this.isActive = false;
		this.videos = new ArrayList<Video>();
		this.duration = 0;
		this.remainingDays = 0;
		//this.customer = new Customer("none",new RegularBehavior(),new RentalInterface());
		this.cost = 0;
		this.rentalID = -1;
	}

	public void printRental(){
		System.out.println("---RENTAL--id= "+this.rentalID+"----------");
		if(!isActive){
			System.out.print("in");
		}
		System.out.print("active\n");
		System.out.println(customer.getName());
		System.out.println("Rented "+this.videos.size()+
						" videos for "+this.duration+" days");
		for(Video vid : this.videos){
			vid.printVideo();
		}
		System.out.println("Price of Rental: "+this.cost);
		System.out.println("-----------------------------");
	}

	public boolean isDue(){
		if(this.remainingDays <= 0){
			return true;
		}
		return false;
	}

	public int getID(){
		return this.rentalID;
	}

	public void setAvailabilityOfVideosInRental(boolean val){
		for(Video video : videos){
			video.setAvailability(val);
		}
	}

	public ArrayList<Video> getVideos(){
		return this.videos;
	}

	public double getCost(){
		return this.cost;
	}

	public void deactivateRental(){
		this.isActive = false;
		setAvailabilityOfVideosInRental(true);
	}
	public boolean isActive(){
		return this.isActive;
	}

	public int getRemainingDays(){
		return this.remainingDays;
	}

	public void decrementRemainingDays(){
		//System.out.println("Decremented rentalID "+ this.rentalID);
		this.remainingDays--;
	}
}