package sim.rental;

import java.util.List;
import java.util.ArrayList;
import java.util.Random;

public class RentalStore implements RentalInterface{
	double revenue;
	ArrayList<Video> catalouge;
	ArrayList<Rental> rentals;
	static int numberOfRentals;


	public RentalStore(ArrayList<Video> videos){
		this.revenue = 0.00;
		this.catalouge = new ArrayList<Video>(videos);
		this.rentals = new ArrayList<Rental>();
		this.numberOfRentals = 0;
	}

	public boolean canTheyRentVideos(RentalRequest request){
		int vidsToRent = getNumberOfAvailableVideos();
		//System.out.println(vidsToRent+" Video(s) Avaiable");
		if(request.getNumVideos() <= vidsToRent && 
			vidsToRent >= request.getMinRequested()){
			return true;
		}
		return false;
	}

	// public void aNewDay(){
	// 	for(Rental rental : rentals){
	// 		if(rental.isActive()){
	// 			//rental.decrementRemainingDays();
	// 		}
	// 	}
	// }

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
								cus,this.numberOfRentals);
		}
		this.numberOfRentals++;
		addRentalToList(rental);
		//rental.printRental();
		return rental;
	}

	public void addRentalToList(Rental rental){
		this.rentals.add(rental);
		this.addToRevenue(rental.getCost());

	}

	public void returnRental(Rental rental){
		//System.out.print("SUPPOSED TO RETURN");
		int index = getIndexOfRentalInRentals(rental);
		//System.out.println("return index "+index);
		rentals.get(index).deactivateRental();
		//System.out.print("RETURN:");
		//rentals.get(index).printRental();
	}

	public int getIndexOfRentalInRentals(Rental r){
		//System.out.println("here"+this.rentals.size());

		for(int i = 0; i<this.rentals.size(); i++){
			//System.out.println("ID "+rentals.get(i).getID());
			if(rentals.get(i).getID() == r.getID()){
				return i;
			}
		}
		return -1;
	}

	private List<Video> rentRandomVideos(int number){
		ArrayList<Video> list = new ArrayList<Video>();
		if(number < getNumAvailableVideos()){
			for(Video vid : this.catalouge){
				if(vid.getAvailability() && list.size() < number){
					vid.setAvailability(false);
					list.add(vid);
				}	
			}
		}
		else{
			for(Video vid : this.catalouge){
				if(vid.getAvailability()){
					vid.setAvailability(false);
					list.add(vid);
				}	
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
		System.out.println(getNumAvailableVideos()+" Video(s) Available:");
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
		System.out.println("Revenue: $"+revenue+"0");
	}
}