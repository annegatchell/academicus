package src.main;

public class SkipList<K, T>{
	private float p; //p nodes with level i pointers also have level i+1 pointers
	private Element<K, T> header;
	private int maxLevel;

	public SkipList(float probability, int maxLev){
		p = probability;
		maxLevel = maxLevel;
		//header = new Element(null, null);
	}


}