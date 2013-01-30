public class Element{
	private Element[] ptrs;
	private int key;
	private Object data;

	//General constructor
	public Element(int k, Object d, int level){
		key = k;
		data = d;
		ptrs = new Element[level];
	}

	//Constructor for copying an array of pointers
	//E.g. when using the update temporary node to insert/delete
	//public Element(Element[] ptr){

	//}
}