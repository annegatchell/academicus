package sim.rental;

import java.util.List;
import java.util.ArrayList;
import java.util.Random;

public class RentalStore implements RentalInterface{
	double revenue;
	ArrayList<Video> catalouge;
	//RentalInterface rentalInterface;
	ArrayList<Rental> rentals;


	public RentalStore(ArrayList<Video> videos){
		this.revenue = 0.00;
		this.catalouge = new ArrayList<Video>(videos);
		this.rentals = new ArrayList<Rental>();
	}

	public boolean canTheyRentVideos(RentalRequest request){
		int vidsToRent = getNumberOfAvailableVideos();
		if(request.getNumVideos() <= vidsToRent && 
			vidsToRent <= request.getMinRequested()){
			return true;
		}
		return false;
	}

	public void aNewDay(){
		for(Rental rental : rentals){
			if(rental.isActive()){
				rental.decrementRemainingDays();
			}
		}
	}

	public void addToRevenue(double money){
		this.revenue += money;
	}

	public Rental createRental(Customer cus, RentalRequest req){
		Rental rental = new Rental();
		if (!canTheyRentVideos(req)){
			System.out.println("ERROR: Invalid Rental Request");
		}
		else{
			rental = new Rental(rentRandomVideos(req.getNumVideos()), 
								req.getNumNights(), 
								cus);
		}
		this.addToRevenue(rental.getCost());
		return rental;
	}

	public void returnRental(Rental rental){
		int index = rentals.indexOf(rental);
		rentals.get(index).deactivateRental();
	}

	private List<Video> rentRandomVideos(int number){
		Random generator = new Random();
		ArrayList<Video> list = new ArrayList<Video>();
		while(list.size() < number){
			int num = generator.nextInt(number-1) + 1;
			if(this.catalouge.get(num).getAvailability()){
				this.catalouge.get(num).setAvailability(false);
				list.add(this.catalouge.get(num));
			}
		}
		return list;
	}

	public int getNumberOfAvailableVideos(){
		int i = 0;
		for(Video vid : this.catalouge){
			if(vid.getAvailability()){
				i++;
			}
		}
		return i;
	}

	public void printVideos(){
		for(Video vid : this.catalouge){
			System.out.println(vid.getName() + " :" + vid.getCategory() + 
				": $" + vid.getPrice());
		}
	}

	public void printNumAvailableVideosAndNames(){
		System.out.print(getNumAvailableVideos()+" videos available:");
		for(Video v : catalouge){
			if(v.getAvailability()){
				v.printVideo();
			}
		}
	}

	public int getNumAvailableVideos(){
		int i = 0;
		for(Video v : catalouge){
			if(v.getAvailability()){
				i++;
			}
		}
		return i;
	}

	public void printCompletedRentals(){
		for(Rental r : rentals){
			if(!r.isActive()){
				r.printRental();
				System.out.println();
			}
		}
	}


	public void printActiveRentals(){
		for(Rental r : rentals){
			if(r.isActive()){
				r.printRental();
				System.out.println();
			}
		}
	}

	public void printRevenue(){
		System.out.println("Revenue: "+revenue);
	}
}