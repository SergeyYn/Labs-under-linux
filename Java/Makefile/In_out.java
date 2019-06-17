package JAVA_PRO;
import JAVA_PRO.Array;
import java.util.Random;
public class In_out{
	public static void input_rand(){
		Random rand = new Random();
		for(int i = 0; i < Array.n; i++){
			for(int j = 0; j < Array.n; j++){
				Array.A[i][j] = rand.nextDouble();
			}
		}
	}

	public static void output(){
		for(int i = 0; i < Array.n; i++){
			for(int j = 0; j < Array.n; j++){
				System.out.format("%f ",Array.A[i][j]);
			}
			System.out.println();
		}
	}
}