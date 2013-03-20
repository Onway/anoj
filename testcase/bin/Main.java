import java.io.*;
import java.util.*;

public class Main
{
    public static void main(String args[])
    {
        /*
        int [] s;
        int len = 768 * 1024 / 4;
        s = new int[len];
        int i;
        for (i = 0; i < len ; ++i)
            s[i] = i;
        */

        //long useMemory = (Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory()) / 1024;
//        long[] use = new long[(65536-((int)useMemory))/8 * 1024];
	//System.out.println("Exception in thread \"main\" java.lang.OutOfMemoryError: Java heap space at Main.main(Main.java:19)");
        //int[] use = new int[1024 * 1024 * 16];

        Scanner cin = new Scanner(System.in);
        int a, b;
        while(cin.hasNextInt())
        {
            a = cin.nextInt();
            b = cin.nextInt();
            System.out.println(a + b);          
        }
    }
}
