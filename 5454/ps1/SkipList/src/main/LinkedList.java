package src.main;

public class LinkedList<K, T>{
	private Link<K, T> root;
	private Link<K, T> current;
	private int length;

	//Make a new linked list from an old one
	public LinkedList(int l, LinkedList<K, T> lL){
		root = lL.getRoot();
		current = root;
		length = l;
	}

	//Set up the root of a linked list
	public LinkedList(int l, Link<K, T> lL){
		root = lL;
		current = root;
		length = l;
	}

	//Set up the root of a linked list with the element
	//TODO: WTF
	public LinkedList(int l, Element<K, T> elem){
		length = l;
		Link<K, T> link= new Link<K, T>();
		root = link;
		current = root;
	}

	//Create a new linked list of nulls
	public LinkedList(int l){
		length = l;
		root = new Link<K, T>();
		current = root;
		for(int i = 1; i < length; i++){
			current.setNextLevelDown(new Link<K, T>());
			current = current.getNextLevelDown();
		}
		current = root;
	}

	public void growListByNumElementsAtRoot(int len){
		current = root;
		Link<K, T> beginning = new Link<K, T>();
		Link<K, T> temp = beginning;
		for(int i = 1; i < len; i++){
			temp.setNextLevelDown(new Link<K, T>());
			temp = temp.getNextLevelDown();
		}
		//Point to the current root
		temp.setNextLevelDown(root);
		//Reset root to be the beginning of the list again
		root = beginning;
		//Set current to be the root
		current = root;
		//Update length
		length = length + len;		
	}

	public void setNextLink(Link<K, T> l){
		//Should I check if null or not to make sure it will be 
		current.setNextLevelDown(l);
	}

	public Link<K, T> getNextLink(){
		return current.getNextLevelDown();
	}

	public Link<K, T> getCurrentLink(){
		return current;
	}

	public Element<K, T> getCurrentLinkNextElement(){
		return current.getNextElement();
	}

	public Link<K, T> advanceCurrent(){
		current = current.getNextLevelDown();
		return current;
	}

	public void resetCurrentLinkToRoot(){
		current = root;
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

	public static void main(String[] args){
		LinkedList<Integer, String> lNull4 = new LinkedList<>(4);
		Link<Integer, String> initialStartOfList = lNull4.getRoot();
        lNull4.growListByNumElementsAtRoot(4);
        //Advance two links to see if that will be the root
        lNull4.advanceCurrent(); // should be link 1
        lNull4.advanceCurrent(); // should be link 2, same as old root
	}

}