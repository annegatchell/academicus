package src.main;

public class Element<K, T>{
	private LinkedList<K, T> levels;
	private int height;
	private K key;
	private T data;

	//General constructor for a null list
	public Element(K k, T d, int h){
		key = k;
		data = d;
		height = h;
		levels = new LinkedList<>(h); //null linked list
	}

	//Constructor for copying an array of pointers
	//e.g. when using the update temporary node to insert/delete
	public Element(K k, T d, LinkedList<K, T> lst){
		key = k;
		data = d;
		height = lst.getLength();
		levels = lst;
	}

	public Element<K, T> getNextElementForCurrentLevel(){
		return levels.getCurrentLinkNextElement();
	}

	public void setTopLevel(Link<K, T> tL){
		levels.setRoot(tL);
	}

	public Link<K, T> getTopLevel(){
		return levels.getRoot();
	}

	public K getKey(){
		return key;
	}
	public void setKey(K k){
		key = k;
	}

	public void setData(T newData){
		data = newData;
	}
	public T getData(){
		return data;
	}
}