package src.test;

import org.junit.*;
import static org.junit.Assert.*;
import org.junit.Assert;
import src.main.LinkedList;
import src.main.Link;
import src.main.Element;

public class ElementTest {
    Element<Integer, String> nullElemIntString5;

    /**
     * Sets up the test fixture. 
     * (Called before every test case method.)
     */
    @Before
    public void setUp() {
        nullElemIntString5 = new Element<>(2, "My DATA", 5);
    }

    /**
     * Tears down the test fixture. 
     * (Called after every test case method.)
     */
    @After
    public void tearDown() {
        
    }
    
    @Test
    public void testElementForNullList() {
        //Element<Integer, String> nullElemIntString5 = new Element<>(2, "My DATA", 5);
        assertNull("Next element should be null", nullElemIntString5.getNextElementForCurrentLevel());
    }

    @Test
    public void testAdvancedToBottomLevel(){
        //Element<Integer, String> nullElemIntString5 = new Element<>(2, "My DATA", 5);
        nullElemIntString5.advanceToBottomLevel();
        assertEquals("Should be at level 0", 0, nullElemIntString5.getCurrentLevelNum());
        assertNull(nullElemIntString5.advanceCurrentLevel());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main("src.test.ElementTest");
    }

}