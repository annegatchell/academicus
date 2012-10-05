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

	public Rental(List<Video> vids, int duration, Customer cus){
		this.isActive = true;
		this.videos = new ArrayList<Video>(vids);
		this.duration = duration;
		this.remainingDays = duration;
		this.customer = new Customer(customer);
		this.cost = 0;
		for(Video vid : this.videos){
			this.cost += vid.getPrice();
		}
	}

	public Rental(){
		this.isActive = false;
		this.videos = new ArrayList<Video>();
		this.duration = 0;
		this.remainingDays = 0;
		//this.customer = new Customer("none",new RegularBehavior(),new RentalInterface());
		this.cost = 0;
	}

	public void printRental(){
		System.out.println(customer.getName());
		System.out.println("Rented "+this.videos.size()+
						" videos for "+this.duration+" days");
		for(Video vid : this.videos){
			vid.printVideo();
		}
		System.out.println("Price of Rental: "+this.cost);
	}

	public boolean isDue(){
		if(this.remainingDays == 0){
			return true;
		}
		return false;
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

	public void decrementRemainingDays(){
		this.remainingDays--;
	}
}