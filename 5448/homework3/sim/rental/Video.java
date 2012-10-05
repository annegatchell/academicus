package sim.rental;

public class Video{
	boolean available;
	String name;
	VideoBehavior behavior;

	public Video(String name, VideoBehavior behavior){
		this.name = name;
		this.available = true;
		this.behavior = behavior;
	}

	public void printVideo(){
		System.out.println(this);
	}

	public double getPrice(){
		return this.behavior.getPrice();
	}

	public String getCategory(){
		return this.behavior.getCategory();
	}

	public void setBehavior(VideoBehavior behavior){
		this.behavior = behavior;
	}

	public VideoBehavior getBehavior(){
		return this.behavior;
	}

	public void setName(String name){
		this.name = name;
	}

	public String getName(){
		return this.name;
	}

	public void setAvailability(boolean available){
		this.available = available;
	}

	public boolean getAvailability(){
		return this.available;
	}

}