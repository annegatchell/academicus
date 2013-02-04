package src.test;

import org.junit.*;
import static org.junit.Assert.*;
import org.junit.Assert;
import src.main.LinkedList;
import src.main.Link;
import src.main.Element;
import src.main.SkipList;

public class SkipListTest{

	SkipList<Integer, String> test1;

	/**
     * Sets up the test fixture. 
     * (Called before every test case method.)
     */
    @Before
    public void setUp() {
        
    }

    /**
     * Tears down the test fixture. 
     * (Called after every test case method.)
     */
    @After
    public void tearDown() {
        
    }

    @Test
    public void testInsertion(){
    	test1 = new SkipList<>(0.5f, 16);
    	Element<Integer, String> inserted2 = test1.insert(2,"2");
    	Element<Integer, String> current = test1.getHeader();
    	System.out.println("inserted element 2 "+inserted2 + " " + inserted2.getKey() + " into " + current);
		
		System.out.println(current.getCurrentLevelNum() + " " + current.getNextElementForCurrentLevel());
		while(current.getCurrentLevelNum() > 1){
			current.advanceCurrentLevel();
			System.out.println(current.getCurrentLevelNum() + " " + current.getNextElementForCurrentLevel());
		}
		current.resetCurrentLevelToRoot();
		current.advanceToBottomLevel();
		current = current.getNextElementForCurrentLevel();
		assertSame("(1) Should be at the new element 2 we inserted first", inserted2, current);
		System.out.println("At current level... "+ current.getCurrentLevelNum());

		current = test1.getHeader();
    	Element<Integer, String> inserted1 = test1.insert(1, "1");
    	System.out.println("inserted element 1 "+inserted1 + " " + inserted1.getKey() + " into " + current);

		System.out.println(current.getCurrentLevelNum() + " " + current.getNextElementForCurrentLevel());
		while(current.getCurrentLevelNum() > 1){
			current.advanceCurrentLevel();
			System.out.println(current.getCurrentLevelNum() + " " + current.getNextElementForCurrentLevel());
		}

		current.resetCurrentLevelToRoot();
		current.advanceToBottomLevel();
		System.out.println(current.getCurrentLevelNum() + " bottom" + current.getNextElementForCurrentLevel());
		current = current.getNextElementForCurrentLevel();

		assertSame("(2)Should be at the new element 1 we inserted second", inserted1, current);
    	assertEquals(Integer.valueOf(1),current.getKey());
    	current = current.getNextElementForCurrentLevel();
    	current.advanceToBottomLevel();
    	assertSame("(2)Should be at the new element 2 we inserted first", inserted2, current);
    	assertEquals(Integer.valueOf(2),current.getKey());
    }

    @Test
    public void testTraverseInOrder(){
        
    }
    
    @Test
    public void testSkipListInitialization() {
        test1 = new SkipList<>(0.5f);
        test1 = new SkipList<>(0.5f, 16);
    }

    @Test
    public void testMaxLevelIncreaseWithAddingElements(){

    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main("src.test.ElementTest");
    }
}