package src.main;

import java.util.Random;

public class SkipList<K, T>{
	private float p; //p nodes with level i pointers also have level i+1 pointers
	private Element<K, T> header;
	private int maxLevel;
	//TODO: Make MaxLevel start off at a resonable level and then increase with numElements
	private int numElements;

	public SkipList(float probability){
		p = probability;
		numElements = 0;
		maxLevel = 16;
		header = new Element<K, T>(null, null, maxLevel);
	}

	public SkipList(float probability, int powerOfTwoSize){
		p = probability;
		numElements = 0;
		maxLevel = powerOfTwoSize;
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

	public void insert(K searchKey, T newValue){
		Element<K, T> update = new Element<>(null, null, maxLevel);
		Element<K, T> x = header;
		K key;
		for(int i = maxLevel-1; i > -1; i--){
			key = x.getNextElementKeyForCurrentLevel();
			while(key != null && key < searchKey){
				x = x.getNextElementForCurrentLevel();
			}
			update
		}

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