public class Element<K, T>{
	private Link topLevel;
	private int height;
	private K key;
	private T data;

	//General constructor
	public Element(K k, T d, int level){
		key = k;
		data = d;
	}

	//Constructor for copying an array of pointers
	//E.g. when using the update temporary node to insert/delete
	//public Element(Element[] ptr){

	//}

	public void setTopLevel(Link tL){
		topLevel = tL;
	}

	public Link getTopLevel(){
		return topLevel;
	}

	public K getKey(){
		return key;
	}
	public void setKey(K k){
		key = k;
	}
}