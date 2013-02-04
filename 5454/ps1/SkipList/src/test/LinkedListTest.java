package src.test;

import org.junit.*;
import static org.junit.Assert.*;
import org.junit.Assert;
import src.main.LinkedList;
import src.main.Link;
import src.main.Element;

public class LinkedListTest {

    LinkedList<Integer, String> lNull4;    

    /**
     * Sets up the test fixture. 
     * (Called before every test case method.)
     */
    @Before
    public void setUp() {
        lNull4 = new LinkedList<>(4);
    }

    /**
     * Tears down the test fixture. 
     * (Called after every test case method.)
     */
    @After
    public void tearDown() {
        
    }
    
    @Test
    public void testNullLinkedListSetup() {
        //LinkedList<Integer, String> lNull4 = new LinkedList<>(4);
        //Check length
        assertEquals("The list should have 4 elements", 4, lNull4.getLength());
        //Root node should not be null
        assertNotNull("The root node shouldn't be null", lNull4.getRoot());
        assertEquals("Root, current = 4", 4, lNull4.getCurrentLinkNum());
        //Root and current should be the same node right now
        assertSame("The root node should be same as current node", lNull4.getRoot(), lNull4.getCurrentLink());
        lNull4.advanceCurrent(); //now at 1
        assertEquals("Current = 3", 3, lNull4.getCurrentLinkNum());
        assertNotNull("The current node shouldn't be null", lNull4.getCurrentLink());
        assertSame("The next node after root should be the same as current now", lNull4.getRoot().getNextLevelDown(), lNull4.getCurrentLink());
        lNull4.advanceCurrent(); //now at 2
        assertEquals("Current = 2", 2, lNull4.getCurrentLinkNum());
        assertNotNull("The current node shouldn't be null", lNull4.getCurrentLink());
        lNull4.advanceCurrent(); //now at 3
        assertEquals("Current = 1", 1, lNull4.getCurrentLinkNum());
        assertNotNull("The current node shouldn't be null", lNull4.getCurrentLink());
        lNull4.advanceCurrent(); //now at 3
        assertNull("The current node should be null now", lNull4.getCurrentLink());
    }

    @Test
    public void testSetUpNewLinkedListFromOld(){
        LinkedList<Integer, String> lNull4Copy = new LinkedList<Integer, String>(4, lNull4);
        assertSame("root, 1", lNull4.getCurrentLink(), lNull4Copy.getCurrentLink());
        lNull4.advanceCurrent();
        lNull4Copy.advanceCurrent();
        assertSame("2", lNull4.getCurrentLink(), lNull4Copy.getCurrentLink()); 
        lNull4.advanceCurrent();
        lNull4Copy.advanceCurrent();
        assertSame("3", lNull4.getCurrentLink(), lNull4Copy.getCurrentLink()); 
        lNull4.advanceCurrent();
        lNull4Copy.advanceCurrent();
        assertSame("4", lNull4.getCurrentLink(), lNull4Copy.getCurrentLink()); 
        lNull4.advanceCurrent();
        lNull4Copy.advanceCurrent();
        assertSame("null", lNull4.getCurrentLink(), lNull4Copy.getCurrentLink()); 
    }

    // @Test void testSetUpNewLinkedListFromArray(){
    //     Element<Integer, String>[] a = (Element<Integer, String>) new Object[4];
    //     a[0] = null;
    //     Element<Integer, String> _25 = new Element<Integer, String>(25, "25", 3);
    //     a[1] = _25;
    //     a[2] = _25;
    //     a[3] = new Element<Integer, String>(19, "19", 1);
    //     LinkedList<Integer, String> lNull4Array = new LinkedList<>(4, a);

    // }

    // @Test
    // public void testGrowLinkedList(){
    //     Link<Integer, String> initialStartOfList = lNull4.getRoot();
    //     lNull4.growListByNumElementsAtRoot(2);
    //     assertEquals("The list should have 6 elements", 6, lNull4.getLength());
    //     assertNotSame("The new root shouldn't equal the old one", initialStartOfList, lNull4.getRoot());
    //     //Advance two links to see if that will be the root
    //     lNull4.advanceCurrent(); // should be link 1
    //     lNull4.advanceCurrent(); // should be link 2, same as old root
    //     assertSame("The current pointer and the old root should be the same", initialStartOfList, lNull4.getCurrentLink());

    // }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main("src.test.LinkedListTest");
    }

}