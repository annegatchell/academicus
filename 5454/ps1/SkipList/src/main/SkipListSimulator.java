package src.main;

public class SkipListSimulator{

	public static void main(String[] args){
		SkipList<Integer, String> test1 = new SkipList<>(0.5f, 16);
		test1.insert(2, "2");
		test1.insert(3, "3");
	}
}