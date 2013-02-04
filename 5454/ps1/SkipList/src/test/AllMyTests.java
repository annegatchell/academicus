package src.test;

import org.junit.runners.Suite;
import org.junit.runner.RunWith;

@RunWith(Suite.class)
@Suite.SuiteClasses({
	src.test.TestInstantiation.class, 
	src.test.LinkedListTest.class,
	src.test.LinkTest.class,
	src.test.ElementTest.class,
	src.test.SkipListTest.class
})
public class AllMyTests {
  //nothing
}