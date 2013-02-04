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
    public void testBasicInsertion(){
    	test1 = new SkipList<>(0.5f, 16);
    	Element<Integer, String> inserted2 = test1.insert(2,"2");
    	Element<Integer, String> current = test1.getHeader();
    	System.out.println("inserted element 2 "+inserted2 + " " + inserted2.getKey() + " into " + current);
		
		System.out.println("FIRST INSERTION " + current);
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

    	System.out.println("SECOND INSERTION, header " + current);
		System.out.println(current.getCurrentLevelNum() + " " + current.getNextElementForCurrentLevel());
		while(current.getCurrentLevelNum() > 1){
			current.advanceCurrentLevel();
			System.out.println(current.getCurrentLevelNum() + " " + current.getNextElementForCurrentLevel());
		}

		current.resetCurrentLevelToRoot();
		current.advanceToBottomLevel();
		// System.out.println(current.getCurrentLevelNum() + " bottom " + current.getNextElementForCurrentLevel());
		current = current.getNextElementForCurrentLevel();

		System.out.println("SECOND INSERTION, element 1 " + current);
		System.out.println(current.getCurrentLevelNum() + " " + current.getNextElementForCurrentLevel());
		while(current.getCurrentLevelNum() > 1){
			current.advanceCurrentLevel();
			System.out.println(current.getCurrentLevelNum() + " " + current.getNextElementForCurrentLevel());
		}

		assertSame("(2)Should be at the new element 1 we inserted second", inserted1, current);
    	assertEquals(Integer.valueOf(1),current.getKey());
    	current.advanceToBottomLevel();
    	// System.out.println(current.getCurrentLevelNum() + " bottom " + current.getNextElementForCurrentLevel());


    	current = current.getNextElementForCurrentLevel();
    	assertSame("(2)Should be at the new element 2 we inserted first", inserted2, current);
    	assertEquals(Integer.valueOf(2),current.getKey());


    }

    @Test
    public void testDeletion(){
    	    	test1 = new SkipList<>(0.5f, 16);
    	Element<Integer, String> inserted2 = test1.insert(2,"2");
    	Element<Integer, String> current = test1.getHeader();
    	System.out.println("inserted element 2 "+inserted2 + " " + inserted2.getKey() + " into " + current);
		
		System.out.println("FIRST INSERTION element 2 " + current);
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

    	System.out.println("SECOND INSERTION, header " + current);
		System.out.println(current.getCurrentLevelNum() + " " + current.getNextElementForCurrentLevel());
		while(current.getCurrentLevelNum() > 1){
			current.advanceCurrentLevel();
			System.out.println(current.getCurrentLevelNum() + " " + current.getNextElementForCurrentLevel());
		}

		current.resetCurrentLevelToRoot();
		current.advanceToBottomLevel();
		// System.out.println(current.getCurrentLevelNum() + " bottom " + current.getNextElementForCurrentLevel());
		current = current.getNextElementForCurrentLevel();

		System.out.println("SECOND INSERTION, element 1 " + current);
		System.out.println(current.getCurrentLevelNum() + " " + current.getNextElementForCurrentLevel());
		while(current.getCurrentLevelNum() > 1){
			current.advanceCurrentLevel();
			System.out.println(current.getCurrentLevelNum() + " " + current.getNextElementForCurrentLevel());
		}

		assertSame("(2)Should be at the new element 1 we inserted second", inserted1, current);
    	assertEquals(Integer.valueOf(1),current.getKey());
    	current.advanceToBottomLevel();
    	// System.out.println(current.getCurrentLevelNum() + " bottom " + current.getNextElementForCurrentLevel());


    	current = current.getNextElementForCurrentLevel();
    	assertSame("(2)Should be at the new element 2 we inserted first", inserted2, current);
    	assertEquals(Integer.valueOf(2),current.getKey());

    	test1.delete(1);
    	current = test1.getHeader();
    	System.out.println("FIRST DELETION of 1, header " + current);
		System.out.println(current.getCurrentLevelNum() + " " + current.getNextElementForCurrentLevel());
		while(current.getCurrentLevelNum() > 1){
			current.advanceCurrentLevel();
			System.out.println(current.getCurrentLevelNum() + " " + current.getNextElementForCurrentLevel());
		}

    }

    @Test
    public void testManyInsertions(){
    	test1 = new SkipList<>(0.5f, 16);
    	Element<Integer, String> insert2 = test1.insert(2, "two");
    	Element<Integer, String> insert5 = test1.insert(5, "five");
    	Element<Integer, String> insert4 = test1.insert(4, "four");
    	Element<Integer, String> insert1 = test1.insert(1, "one");
    	Element<Integer, String> insert3 = test1.insert(3, "three");
    }

    @Test
    public void testSearch(){
		test1 = new SkipList<>(0.5f, 16);
    	Element<Integer, String> insert2 = test1.insert(2, "two");
    	Element<Integer, String> insert5 = test1.insert(5, "five");
    	Element<Integer, String> insert4 = test1.insert(4, "four");
    	Element<Integer, String> insert1 = test1.insert(1, "one");
    	Element<Integer, String> insert3 = test1.insert(3, "three");
    	assertEquals("three",test1.search(3));
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