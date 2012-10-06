package sim.rental;

import java.util.List;
import java.util.ArrayList;
import java.util.Random;
import java.util.Vector;

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
			//System.out.println(cus.canIRentVideos());
		}
	}

	public void customersReturnVideos(){
		for(Customer cust : customers){
			cust.returnDueRentals();
		}
	}

	public void customersRentVideos(){
		Random generator = new Random();
		int randomNumberOfRenters = 1;
		int randomIndex =0;
		int numCustomers = this.customers.size();
		
		randomNumberOfRenters = generator.nextInt(numCustomers);
		
		randomIndex = generator.nextInt(customers.size());
		ArrayList<Integer> usedIndeces = new ArrayList<Integer>();
		usedIndeces.add(randomIndex);
		for(int i = 1; i < randomNumberOfRenters; i++){
			do{
				randomIndex = generator.nextInt(customers.size());
			}
			while(usedIndeces.contains(randomIndex));
			usedIndeces.add(randomIndex);
		}
		System.out.println("Renters are: "+usedIndeces);
		int num;
		for(int i=0;i<usedIndeces.size();i++){
			num = usedIndeces.get(i);
			if(this.customers.get(num).canIRentVideos()){
				this.customers.get(i).rentVideos();
			}
		}





		// Random generator = new Random();
		// int i = 0;
		// int index
		// int numRenters = generator.nextInt(4)+1;
		// System.out.println(numRenters);
		// Vector usedIndeces = new Vector();
		// for(i = 0; i < numRenters; i++){
		// 	index = generator.nextInt(this.customers.size());
		// 	if()
		// 	usedIndeces.add(index);
		// 	this.customers
		// }


		// for(i = 0; i < numRenters; i++){
		// 	if(this.customers.get(i).canIRentVideos()){
		// 		this.customers.get(i).rentVideos();
		// 	}
		// 	else{
		// 		if(i < this.customers.size()-1){
		// 			i++;
		// 		}
		// 	}
		// }
		// for(Customer cus : customers){
		// 		if(cus.canIRentVideos() && i<numRenters){
		// 			cus.rentVideos();
		// 			i++;
		// 		}
		// }
	}

	public ArrayList<Customer> getCustomerList(){
			return this.customers;
	}

	public void newDay(){
		for(Customer cus : customers){
			cus.newDay();
		}
	}
}