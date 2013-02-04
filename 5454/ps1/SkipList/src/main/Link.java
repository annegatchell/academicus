package src.main;

public class Link<K extends Comparable<K>, T>{
	private Element<K, T> nextElement;
	private Link<K, T> nextLevelDown;
	private int num;

	public Link(int n){
		num = n;
		nextElement = null;
		nextLevelDown = null;
	}

	// public void Link(){
	// 	nextElement = null;
	// 	nextLevelDown = null;
	// }

	public Link(int n, Element<K, T> nextE){
		nextElement = nextE;
		num = n;
	}

	public Link(Element<K, T> nextE, Link<K, T> nextL, int n){
		nextElement = nextE;
		nextLevelDown = nextL;
		num = n;
	}

	public void setNextElement(Element<K, T> nextE){
		nextElement = nextE;
	}

	public Element<K, T> getNextElement(){
		return nextElement;
	}

	public K getNextElementKey(){
		return nextElement.getKey();
	}

	public void setNextLevelDown(Link<K, T> l){
		nextLevelDown = l;
	}

	public Link<K, T> getNextLevelDown(){
		return nextLevelDown;
	}

	public int getNum(){
		return num;
	}

	public void setNum(int n){
		num = n;
	}
}