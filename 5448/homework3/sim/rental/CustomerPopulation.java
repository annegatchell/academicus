package sim.rental;

import java.util.List;
import java.util.ArrayList;
import java.util.Random;

public class CustomerPopulation{
	ArrayList<Customer> customers;

	public CustomerPopulation(ArrayList<Customer> customers){
		this.customers = new ArrayList<Customer>(customers);
	}

	public void printCustomerPop(){
		for(Customer cus : customers){
			System.out.println(cus.getName() + " :"+cus.getRentalBehaviorType()+": ");
		}
	}

	public void canWeRent(){
		for(Customer cus : customers){
			System.out.println(cus.canIRentVideos());
		}
	}

	public void customersReturnVideos(){
		for(Customer cust : customers){
			cust.returnDueRentals();
		}
	}

	public void customersRentVideos(){
		Random generator = new Random(192102);
		for(Customer cus : customers){
				if(cus.canIRentVideos() && generator.nextInt(6) == 3){
					cus.rentVideos();
				}
		}
	}

	public void newDay(){
		for(Customer cus : customers){
			cus.newDay();
		}
	}
}