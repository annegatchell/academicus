package sim.rental;

public interface RentalInterface{
	abstract public boolean canTheyRentVideos(RentalRequest request);
	abstract public Rental createRental(Customer cus, RentalRequest req);
	abstract public void returnRental(Rental rental);
}