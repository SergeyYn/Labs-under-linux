package pro5;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.*;
import java.util.Random;


class CR1{
	public int size = 0;
	private int newestItem = 0;
	private int oldestItem = 0;
	private int buffer[];
	public int del_count = 0;
	static Random random = new Random();

	private int nextInd(int i) {
    	return (i + 1) % size;
	}
	public CR1(int inSize) {
        buffer = new int[inSize];
        this.size = inSize;
    }
	synchronized void add_buf() {
		System.out.println("CR1 element creation");
		buffer[newestItem] = random.nextInt();
	    newestItem = nextInd(newestItem);
	}
	synchronized void del_buf() {
		if(buffer[oldestItem] == -1){
			//System.out.println("CR1: Can't delete empty element");
			oldestItem = nextInd(oldestItem);
		}
		else{
			System.out.println("CR1 element deletion");
		    buffer[oldestItem] = -1;
		    del_count++;
		    oldestItem = nextInd(oldestItem);
		}
	}
}

class CR2{
	private int i1 = 0;
	private float f1 = 0;
	private double d1 = 0;
	private boolean b1 = true;
	private long l1 = 0;
	private char c1 = '!';
	static Random random = new Random();
	public CR2(){}
	public void consume(){
		System.out.println("CR2 consume function: ");
        System.out.println(i1 + f1 + d1 + l1);
	}
	public void modify(){
        i1 = random.nextInt();
        f1 = random.nextFloat();
        d1 = random.nextDouble();
        b1 = random.nextBoolean();
        l1 = random.nextLong();
        c1++;
        System.out.println("CR2 modify function");
        System.out.println(i1 + f1 + d1 + l1);
	}
}

class Thread1 implements Runnable{
	public Thread t;
	public Thread1() {
        t = new Thread(this,"THREAD 1");
        t.start();
    }
    public void run() {
        System.out.println("THREAD 1 STARTED");
        while (Main.buf.del_count<Main.stop_count) {
            Main.buf.del_buf();
        }
        System.out.println("THREAD 1 FINISHED");
    }
} 

class Thread2 implements Runnable{
	public Thread t;
	public Thread2() {
        t = new Thread(this,"THREAD 2");
        t.start();
    }
    public void run() {
        System.out.println("THREAD 2 STARTED");
        while (Main.buf.del_count<Main.stop_count && Main.f3) {
        	Main.f2s = true;
        	System.out.println("THREAD 2 BEFORE SEMAFORE SYNC");
        	Main.sem1.release();
        	try {
                Main.sem2.acquire();
            } catch(InterruptedException e) {
                e.printStackTrace();
            }
            Main.f2s = false;
            System.out.println("THREAD 2 AFTER SEMAFORE SYNC");
            Main.buf.add_buf();
        }
        Main.f2 = false;
        Main.sem1.release();
        if(Main.br.getNumberWaiting()>0)
            	Main.br.reset();
        System.out.println("THREAD 2 FINISHED");
    }
}

class Thread3 implements Runnable{
	public Thread t;
	public Thread3() {
        t = new Thread(this,"THREAD 3");
        t.start();
    }
    public void run() {
        System.out.println("THREAD 3 STARTED");
        while (Main.buf.del_count<Main.stop_count && Main.f2 && Main.f6) {
        	Main.mutex.lock();
        	System.out.println("THREAD 3 LOCKED MUTEX FOR MODIFYING");
        	Main.vars.modify();
        	Main.mutex.unlock();
        	System.out.println("THREAD 3 UNLOCKED MUTEX AFTER MODIFYING");
        	System.out.println("THREAD 3 WAITING FOR BARRIER");
            try {
                Main.br.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (BrokenBarrierException e) {
                e.printStackTrace();
            }
            System.out.println("THREAD 3 AFTER BARRIER");
            Main.mutex.lock();
            System.out.println("THREAD 3 LOCKED MUTEX FOR CONSUMING");
        	Main.vars.consume();
        	Main.mutex.unlock();
        	System.out.println("THREAD 3 UNLOCKED MUTEX AFTER CONSUMING");
        	System.out.println("THREAD 3 BEFORE SEMAFORE SYNC");
        	Main.sem2.release();
        	try {
                Main.sem1.acquire();
            } catch(InterruptedException e) {
                e.printStackTrace();
            }
            Main.f3s = false;
            System.out.println("THREAD 3 AFTER SEMAFORE SYNC");
        }
        Main.f3 = false;
        if(Main.br.getNumberWaiting()>0)
            	Main.br.reset();
        Main.sem2.release();
        System.out.println("THREAD 3 FINISHED");
    }
}

class Thread4 implements Runnable{
	public Thread t;

	public Thread4() {
        t = new Thread(this,"THREAD 4");
        t.start();
    }
    public void run() {
        System.out.println("THREAD 4 STARTED");
        while (Main.buf.del_count<Main.stop_count) {
            Main.buf.del_buf();
        }
        System.out.println("THREAD 4 FINISHED");
    }
}

class Thread5 implements Runnable{
	public Thread t;

	public Thread5() {
        t = new Thread(this,"THREAD 5");
        t.start();
    }
    public void run() {
        System.out.println("THREAD 5 STARTED");
        while (Main.buf.del_count<Main.stop_count) {
            Main.buf.add_buf();
        }
        System.out.println("THREAD 5 FINISHED");
    }
}

class Thread6 implements Runnable{
	public Thread t;
	public Thread6() {
        t = new Thread(this,"THREAD 6");
        t.start();
    }
    public void run() {
        System.out.println("THREAD 6 STARTED");
        while (Main.buf.del_count<Main.stop_count && Main.f3) {
        	System.out.println("THREAD 6 WAITING FOR BARRIER");
            try {
                Main.br.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (BrokenBarrierException e) {
                e.printStackTrace();
            }
            System.out.println("THREAD 6 AFTER BARRIER");
        	Main.mutex.lock();
        	System.out.println("THREAD 6 LOCKED MUTEX");
        	Main.vars.consume();
        	Main.mutex.unlock();
        	System.out.println("THREAD 6 UNLOCKED MUTEX");
        }
        Main.f6 = false;
        if(Main.br.getNumberWaiting()>0)
            	Main.br.reset();
        System.out.println("THREAD 6 FINISHED");
    }
}

class Main {
	static CR1 buf = new CR1(25);
    static CR2 vars = new CR2();
    public static int stop_count = 2*buf.size;
    static CyclicBarrier br = new CyclicBarrier(2);
    public static Semaphore sem1 = new Semaphore(0, true);
    public static Semaphore sem2 = new Semaphore(0, true);
    public static ReentrantLock mutex = new ReentrantLock();
    public static boolean f2 = true, f3 = true, f6 = true, f2s = false, f3s = false;
    public static void main(String[] args) {
        System.out.println("\nMain started");
        Thread1 thread1 = new Thread1();
        Thread2 thread2 = new Thread2();
        Thread3 thread3 = new Thread3();
        Thread4 thread4 = new Thread4();
        Thread5 thread5 = new Thread5();
        Thread6 thread6 = new Thread6();
        try {
            thread1.t.join();
            thread2.t.join();
            if(br.getNumberWaiting()>0)
            	br.reset();
            thread3.t.join();
            thread4.t.join();
            thread5.t.join();
            if(br.getNumberWaiting()>0)
            	br.reset();
            thread6.t.join();
        } catch(InterruptedException e)
        {
            System.out.println(e.getMessage());
        }
        System.out.println("\nMain finished");
    }
}