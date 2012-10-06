package sim;

import sim.rental.*;
import java.util.List;
import java.util.ArrayList;

public class Simulation {
	
	public ArrayList<Video> makeVideoList(){
		ArrayList<Video> list = new ArrayList<Video>();
		list.add(new Video("Mean Girls",new ComedyBehavior()));
		list.add(new Video("I Love You Man",new ComedyBehavior()));
		list.add(new Video("Liar Liar",new ComedyBehavior()));
		list.add(new Video("Forgetting Sarah Marshall",new ComedyBehavior()));

		list.add(new Video("The Ring", new HorrorBehavior()));
		list.add(new Video("The Exorcist", new HorrorBehavior()));
		list.add(new Video("Paranormal Activity", new HorrorBehavior()));
		list.add(new Video("The Shining", new HorrorBehavior()));

		list.add(new Video("12 Angry Men", new DramaBehavior()));
		list.add(new Video("To Kill A Mockingbird", new DramaBehavior()));
		list.add(new Video("The Godfather", new DramaBehavior()));
		list.add(new Video("True Grit", new DramaBehavior()));

		list.add(new Video("Titanic", new RomanceBehavior()));
		list.add(new Video("The Notebook", new RomanceBehavior()));
		list.add(new Video("Love Actually", new RomanceBehavior()));
		list.add(new Video("Dirty Dancing", new RomanceBehavior()));

		list.add(new Video("Bridesmaids", new NewReleaseBehavior()));
		list.add(new Video("Ted", new NewReleaseBehavior()));
		list.add(new Video("The Dark Kight Rises", new NewReleaseBehavior()));
		list.add(new Video("The Avengers", new NewReleaseBehavior()));
		return list;
	}

	public ArrayList<Customer> makeCustomerList(RentalInterface intf){
		ArrayList<Customer> list = new ArrayList<Customer>();
		list.add(new Customer("Anne", new BreezyBehavior(), intf));
		list.add(new Customer("Mario", new BreezyBehavior(),intf));
		list.add(new Customer("Ben", new HoarderBehavior(),intf));
		list.add(new Customer("Gary", new HoarderBehavior(),intf));
		list.add(new Customer("Mary", new RegularBehavior(),intf));
		
		list.add(new Customer("Caitlin", new RegularBehavior(),intf));
		list.add(new Customer("Kiki", new BreezyBehavior(),intf));
		list.add(new Customer("Netta", new HoarderBehavior(),intf));
		list.add(new Customer("Libby", new RegularBehavior(),intf));
		list.add(new Customer("Homer", new BreezyBehavior(),intf));
		return list;
	}

	public void run(){
		System.out.println("sim");
		RentalStore store = new RentalStore(makeVideoList());
		store.printVideos();
		CustomerPopulation customers = new CustomerPopulation(makeCustomerList(store));
		customers.printCustomerPop();
		//customers.canWeRent();
		int date = 1;
		for(date = 1; date < 36;date++){
			//System.out.println("DAY "+date+"-----------------------");
			//store.aNewDay();
			customers.newDay();
			customers.customersReturnVideos();
			customers.customersRentVideos();

		}

		System.out.println("SUMMARY-------------------------");
		store.printNumAvailableVideosAndNames();
		store.printRevenue();
		System.out.println("========================================");
		System.out.println("COMPLETED RENTALS-----------------------");
		System.out.println("========================================");
		store.printCompletedRentals();
		System.out.println("========================================");
		System.out.println("ACTIVE RENTALS-----------------------");
		System.out.println("========================================");
		store.printActiveRentals();
		for(Customer cus : customers.getCustomerList()){
			System.out.println(cus.getNumberOfTimesNewDayCalled());
		}
		return;
	}

	public static void main(String[] args){
		Simulation simulation = new Simulation();
		simulation.run();
	}

}