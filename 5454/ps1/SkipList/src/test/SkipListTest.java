package src.main;

import org.junit.*;
import static org.junit.Assert.*;
import org.junit.Assert;
import src.main.LinkedList;
import src.main.Link;
import src.main.Element;
import src.main.SkipListTest;

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
    public void testSkipListInitialization() {
        test1 = new SkipList<>(0.5f);
    }

    @Test
    public void testMaxLevelIncreaseWithAddingElements(){

    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main("src.test.ElementTest");
    }
}