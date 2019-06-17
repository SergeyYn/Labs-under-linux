package JAVA_PRO;
import JAVA_PRO.Func;
import JAVA_PRO.In_out;

public class Main{
	public static void main(String[] argc){
		In_out.input_rand();
		System.out.println("BEFORE SEARCH");
		System.out.println();
		In_out.output();
		Func.f_s();
		System.out.println();
		System.out.println("AFTER SEARCH");
		System.out.println();
		In_out.output();
	}
}