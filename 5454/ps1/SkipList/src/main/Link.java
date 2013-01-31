public class Link{
	private Element nextElement;
	private Link nextLevelDown;

	public void Link(Element nextE, Link nextL){
		nextElement = nextE;
		nextLevelDown = nextL;
	}

	public void setNextElement(Element nextE){
		nextElement = nextE;
	}

	public Element getNextElement(){
		return nextElement;
	}

	public <K> K getNextElementKey(){
		return nextElement.getKey();
	}
}