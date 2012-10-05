package sim.rental;

public interface CustomerRentalBehavior{
	abstract public RentalRequest howManyToRent();
	abstract public String getBehaviorType();
	abstract public RentalRequest getRentalRequirements();
}