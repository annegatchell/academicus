package src.main;

public class Link<K, T>{
	private Element<K, T> nextElement;
	private Link<K, T> nextLevelDown;

	public void Link(){
		nextElement = null;
		nextLevelDown = null;
	}

	public void Link(Element<K, T> nextE, Link<K, T> nextL){
		nextElement = nextE;
		nextLevelDown = nextL;
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
}