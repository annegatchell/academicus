public class LinkedList<K, T>{
	private Link<K, T> root;
	private int length;

	//Make a new linked list from an old one
	public LinkedList(int l, LinkedList<K, T> lL){
		root = lL.getRoot();
		length = l;
	}

	//Set up the root of a linked list
	public LinkedList(int l, Link<K, T> lL){
		root = lL;
		length = l;
	}

	//Set up the root of a linked list with the element
	//TODO: WTF
	public LinkedList(int l, Element<K, T> elem){
		length = l; 
		Link<K, T> link= new Link<K, T>();
		root = link;
	}

	public void setRoot(Link<K, T> l){
		root = l;
	}
	public Link<K, T> getRoot(){
		return root;
	}
	public int getLength(){
		return length;
	}
}