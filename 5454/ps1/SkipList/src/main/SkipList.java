package src.main;

import java.util.Random;

public class SkipList<K extends Comparable<K>, T>{
	private float p; //p nodes with level i pointers also have level i+1 pointers
	private Element<K, T> header;
	private int maxLevel;
	private int level;
	//TODO: Make MaxLevel start off at a resonable level and then increase with numElements
	private int numElements;

	public SkipList(float probability){
		p = probability;
		numElements = 0;
		level = 1;
		maxLevel = 16;
		header = new Element<K, T>(null, null, maxLevel);
	}

	public SkipList(float probability, int powerOfTwoSize){
		p = probability;
		numElements = 0;
		maxLevel = powerOfTwoSize;
		level = 1;
		header = new Element<K, T>(null, null, maxLevel);
	}

	public T search(K searchKey){
		/*Search(list, searchKey)
			x := list→header
			-- loop invariant: x→key < searchKey 
			for i := list→level downto 1 do
				while x→forward[i]→key < searchKey do 
					x := x→forward[i]
			-- x→key < searchKey ≤ x→forward[1]→key
			x := x→forward[1]
			if x→key = searchKey then return x→value
				else return failure*/
		Element<K, T> current = header;
		return header.getData();
	}

	public Element<K, T> insert(K searchKey, T newValue){
		LinkedList<K, T> update = new LinkedList<>(maxLevel);
		Element<K, T> x = header;
		K key;
		header.resetCurrentLevelToRoot();
		//Go from top level down to level 1
		while(x.getCurrentLevelNum() > 1){
			//If the next element at this level is not null and is less than the key, advance
			while(x.getNextElementForCurrentLevel() != null && x.getNextElementKeyForCurrentLevel().compareTo(searchKey) < 0){
				x = x.getNextElementForCurrentLevel();
			}
			//Store the current element in our update save list
			update.setCurrentLink(x.getCurrentLevelLink());
			//Since update is a linked list, we need to set the root if this is the top
			if(x.getCurrentLevelNum() == maxLevel){
				update.setRoot(x.getCurrentLevelLink());
			}
			// System.out.println(x.getCurrentLevelNum() + " " + x.getCurrentLevelLink());
			// System.out.println(update.getCurrentLinkNum() + " " + update.getCurrentLink());
			//Advance to the next level down in our update list as well as our current element
			update.advanceCurrent();
			x.advanceCurrentLevel();
		}
		// System.out.println(x.getCurrentLevelNum() + " " + x.getCurrentLevelLink());


/**** Purely code to test *****/
		// update.resetCurrentLinkToRoot();
		// while(update.getCurrentLinkNum() > 1){
		// 	System.out.println(update.getCurrentLinkNum() + " " + update.getCurrentLink());
		// 	update.advanceCurrent();
		// }
		// System.out.println(update.getCurrentLinkNum() + " " + update.getCurrentLink());
		
/**************/
		//Go to the next element
		x = x.getNextElementForCurrentLevel();
		//If key == our key, update the data
		if(x != null && x.getKey().compareTo(searchKey) == 0){
			x.setData(newValue);
		}else{
		//Otherwise, create a new Element with a level v
			int v = randomLevel();
			System.out.println("Level generated: "+v);
			//Keep track of our list's current level for Search
			if(v > level){
				level = v;
			}
			x = new Element<K, T>(searchKey, newValue, v+1);
			//Go to the top of the update list, attach the new vector
			update.resetCurrentLinkToRoot();
			int lev;
			while((lev = update.getCurrentLinkNum()) > 1){
				// System.out.println("lev "+ lev);
				//When we hit level v, start splicing in the new element x
				if(lev <= v){
					// System.out.println("HEREin");
					x.setNextElementForCurrentLevel(update.getCurrentLinkNextElement());
					update.setCurrentLinkNextElement(x);
					x.advanceCurrentLevel();
				}
				update.advanceCurrent();
			}
			//Set the 1st/bottom level without advancing
			if(lev <= v){
				// System.out.println("HERE lev "+lev);
				x.setNextElementForCurrentLevel(update.getCurrentLinkNextElement());
				update.setCurrentLinkNextElement(x);
			}
			update.advanceCurrent();

			numElements++;
			//Reset current pointers
			x.resetCurrentLevelToRoot();
			header.resetCurrentLevelToRoot();
		}
		return x;

	}

	public int randomLevel(){
		int v = 1;
		Random rand = new Random();
		//random value between [0...1)
		while (rand.nextDouble() < p && v < maxLevel){
			v++;
		}
		return v;

	}

	public void traverseInOrderAndPrintKeys(){
		Element<K, T> current = header;
		current.advanceToBottomLevel();
		while(current.getNextElementKeyForCurrentLevel() != null){
			current = current.getNextElementForCurrentLevel();
			System.out.print(current.getKey());
			current.advanceToBottomLevel();
		}
	}

	public Element<K, T> getHeader(){
		return header;
	}

	public int getMaxLevel(){
		return maxLevel;
	}

	public int getNumElements(){
		return numElements;
	}

}