package JAVA_PRO;
import JAVA_PRO.Array;
public class Func{
	public static void f_s(){
		int  j = 1, imin = Array.n-1, imax = Array.n-1, jmin = 0, jmax = 0;
		double min = Array.A[Array.n-1][0], max = Array.A[Array.n-1][0], temp = 0;
		for(int i = Array.n-2; i >= 0; i--){
			if(Array.A[i][j] > max){
				max = Array.A[i][j];
				imax = i;
				jmax = j;
			}
			else if(Array.A[i][j] < min){
				min = Array.A[i][j];
				imin = i;
				jmin = j;
			}
			j++;
		}
		temp = Array.A[imax][jmax];
		Array.A[imax][jmax] = Array.A[imin][jmin];
		Array.A[imin][jmin] = temp;
	}
}