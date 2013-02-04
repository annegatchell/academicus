package src.test;

import org.junit.*;
import static org.junit.Assert.*;
import org.junit.Assert;
import src.main.LinkedList;
import src.main.Link;
import src.main.Element;

public class LinkTest {

    Link<Integer, String> test1;
    /**
     * Sets up the test fixture. 
     * (Called before every test case method.)
     */
    @Before
    public void setUp() {
        test1 = new Link<>(1);
    }

    /**
     * Tears down the test fixture. 
     * (Called after every test case method.)
     */
    @After
    public void tearDown() {
        
    }
    
    @Test
    public void test() {
        
        assertNotNull(test1);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main("src.test.LinkTest");
    }

}