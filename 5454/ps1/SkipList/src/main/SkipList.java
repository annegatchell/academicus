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
		header.resetCurrentLevelToRoot();
		Element<K, T> x = header;
		int lev = x.getCurrentLevelNum();
		//Go from top level down to level 1
		while(lev > 0){
			//If the next element at this level is not null and is less than the key, advance
			while(x.getNextElementForCurrentLevel() != null && x.getNextElementKeyForCurrentLevel().compareTo(searchKey) < 0){
				x = x.getNextElementForCurrentLevel();
			}
			//System.out.println(x.getCurrentLevelNum() + " " + x.getKey());
			// System.out.println(update.getCurrentLinkNum() + " " + update.getCurrentLink());
			//Advance to the next level down in our update list as well as our current element
			if(lev > 1){
				x.advanceCurrentLevel();
				lev = x.getCurrentLevelNum();
			}else{
				lev--;
			}
		}
		//Go to the next element
		x = x.getNextElementForCurrentLevel();
		System.out.println("HERE!!!");
		if(x != null && x.getKey().compareTo(searchKey) == 0){
			return x.getData();
		} else {
			return null;
		}
	}

	public void delete(K searchKey){
		LinkedList<K, T> update = new LinkedList<>(maxLevel);
		Element<K, T> x = header;
		K key;
		header.resetCurrentLevelToRoot();
		int lev = x.getCurrentLevelNum();
		//Go from top level down to level 1
		while(lev > 0){
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
			if(lev > 1){
				update.advanceCurrent();
				x.advanceCurrentLevel();
				lev = x.getCurrentLevelNum();
			}
			else
				lev--;
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
		//If key == our key to delete, we can delete it
		if(x != null && x.getKey().compareTo(searchKey) == 0){
			
			//Go to the top of the update list, attach the new vector
			update.resetCurrentLinkToRoot();
			lev = update.getCurrentLinkNum();
			while(lev > 0){
				//System.out.println("lev "+ lev);
				//When we hit level v, start splicing in the new element x
				if(update.getCurrentLinkNextElement() == x){
					update.setCurrentLinkNextElement(x.getNextElementForCurrentLevel());
				}
				if(lev > 1){
					update.advanceCurrent();
					lev = update.getCurrentLinkNum();
				}else lev--;
			}
			header.resetCurrentLevelToRoot();
			numElements--;
		}
			
			//Reset current pointers
		header.resetCurrentLevelToRoot();
	}

	public Element<K, T> insert(K searchKey, T newValue){
		LinkedList<K, T> update = new LinkedList<>(maxLevel);
		Element<K, T> x = header;
		K key;
		header.resetCurrentLevelToRoot();
		int lev = x.getCurrentLevelNum();
		//Go from top level down to level 1
		while(lev > 0){
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
			if(lev > 1){
				update.advanceCurrent();
				x.advanceCurrentLevel();
				lev = x.getCurrentLevelNum();
			}
			else
				lev--;
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
			x = new Element<K, T>(searchKey, newValue, v);
			x.resetCurrentLevelToRoot();
			//Go to the top of the update list, attach the new vector
			update.resetCurrentLinkToRoot();
			lev = update.getCurrentLinkNum();
			while(lev > 0){
				// System.out.println("lev "+ lev);
				//When we hit level v, start splicing in the new element x
				if(lev <= v){
					x.setNextElementForCurrentLevel(update.getCurrentLinkNextElement());
					update.setCurrentLinkNextElement(x);
					System.out.println("x is "+ x + " "+ x.getNextElementForCurrentLevel() + " update current "+ update.getCurrentLinkNextElement());
					if(lev > 1)
						x.advanceCurrentLevel();
				}
				if(lev > 1){
					update.advanceCurrent();
					lev = update.getCurrentLinkNum();
				}else lev--;

			}
			// update.advanceCurrent();

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
		int count = 0;
		K k;
		Element<K, T> current = header;
		while(true){
			if(current.getNextElementForCurrentLevel() != null){
				k = current.getNextElementForCurrentLevel().getKey();
			} else{
				k = null;
			}
			System.out.println(count + "-"+current.getCurrentLevelNum() + " " + k);
			while(current.getCurrentLevelNum() > 1){
				current.advanceCurrentLevel();
				if(current.getNextElementForCurrentLevel() != null){
					k = current.getNextElementForCurrentLevel().getKey();
				} else{
					k = null;
				}
				System.out.println(count + "-"+current.getCurrentLevelNum() + " " + k);
			}
			current.advanceToBottomLevel();
			current = current.getNextElementForCurrentLevel();
			if (current == null){
				break;
			}
			current.resetCurrentLevelToRoot();
			count++;
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