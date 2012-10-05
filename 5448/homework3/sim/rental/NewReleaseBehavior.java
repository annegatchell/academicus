package sim.rental;

public class NewReleaseBehavior implements VideoBehavior{
	public double getPrice(){
		return 3.00;
	}

	public String getCategory(){
		return "New Release";
	}
}