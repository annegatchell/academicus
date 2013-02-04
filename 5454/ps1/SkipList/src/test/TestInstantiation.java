package src.test;

import org.junit.*;
import static org.junit.Assert.*;

import java.util.*;

public class TestInstantiation{
    @Test
    public void testAddition() {
        assertEquals(4, 2 + 2);
    }

    public static void main(String args[]) {
      	org.junit.runner.JUnitCore.main("src.test.TestInstantiation");
    }
}