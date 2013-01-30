package testing;

import org.junit.*;
import static org.junit.Assert.*;

import java.util.*;

public class TestPractice {
    @Test
    public void testAddition() {
        assertEquals(4, 3 + 2);
    }

    public static void main(String args[]) {
      	org.junit.runner.JUnitCore.main("testing.TestPractice");
    }
}