public class SkipList{
	private float p; //p nodes with level i pointers also have level i+1 pointers
	private Element header;
	private int maxLevel;

	public SkipList(float probability, int maxLev){
		p = probability;
		maxLevel = maxLevel;
		header = new Element(null, null);
	}


}