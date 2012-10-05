package sim.rental;

import java.util.List;
import java.util.ArrayList;

public class Customer{
	String name;
	int numberRented;
	ArrayList<Rental> activeRentals;
	CustomerRentalBehavior rentalBehavior;
	RentalInterface rentalInterface;


	public Customer(String name, CustomerRentalBehavior behavior, RentalInterface intf){
		this.name = name;
		this.rentalBehavior = behavior;
		this.numberRented = 0;
		this.rentalInterface = intf;
	}

	public Customer(Customer cus){
		this.name = cus.getName();
		this.numberRented = cus.getNumberRented();
		this.activeRentals = cus.getActiveRentals();
		this.rentalBehavior = cus.getCustomerRentalBehavior();
		this.rentalInterface = cus.getRentalInterface();
	}

	public void newDay(){
		if(activeRentals.size()>0){
			for(Rental rental : activeRentals){
				rental.decrementRemainingDays();
			}
		}
	}

	public RentalRequest getRentalRequest(){
		return this.rentalBehavior.howManyToRent();
	}

	public boolean canIRentVideos(){
		if(this.numberRented == 3){
			return false;
		}
		System.out.print(this.getName());
		return rentalInterface.canTheyRentVideos(this.getRentalRequest());
	}

	public void rentVideos(){
		if(canIRentVideos()){
			Rental newR = rentalInterface.createRental(this, this.getRentalRequest());
			activeRentals.add(newR);
		}
	}

	public void returnDueRentals(){
		for(Rental rental : activeRentals){
			if(rental.isDue()){
				this.rentalInterface.returnRental(rental);
				activeRentals.remove(rental);
			}
		}
	}

	public String getRentalBehaviorType(){
		return this.rentalBehavior.getBehaviorType();
	}

	public void incrementRented(int rented){
		numberRented += rented;
		if(numberRented > 3){
			System.out.println("ERROR: Customer " + this.name + " has more than 3 videos.");
      		System.exit(1);
		}
	}

	public void decrementRented(int returned){
		numberRented -= returned;
		if (numberRented < 0){
			System.out.println("ERROR: Customer " + this.name + " has less than 0 videos.");
      		System.exit(1);
		}
	}

	public String getName(){
		return name;
	}

	public void setName(String name){
		this.name = name;
	}

	public int getNumberRented(){
		return this.numberRented;
	}

	public ArrayList<Rental> getActiveRentals(){
		return this.activeRentals;
	}

	public CustomerRentalBehavior getCustomerRentalBehavior(){
		return this.rentalBehavior;
	}

	public RentalInterface getRentalInterface(){
		return this.rentalInterface;
	}
}