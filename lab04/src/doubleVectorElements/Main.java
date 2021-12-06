package doubleVectorElements;

import java.util.ArrayList;


class Task implements Runnable {
    private int start;
    private int end;

    public Task(int start, int end) {
        this.start = start;
        this.end = end;
    }

    @Override
    public void run() {
        for (int i = start; i < end; i++)
            Main.v[i] *= 2;
    }
}

public class Main {
    private static int N = 100000013;
    public static int[] v = new int[N];
    private static int P = Runtime.getRuntime().availableProcessors();

    public static void main(String[] args) {

        ArrayList<Thread> threads = new ArrayList<>();
        int start, end;
        for (int i = 0; i < N; i++) {
            v[i] = i;
        }

        for (int i = 0; i < P; i++) {
            start = (int)(i * (double)N / P);
            end = (int)(Math.min((i + 1) * (double)N / P, N));

            threads.add(new Thread(new Task(start, end)));
            threads.get(i).start();
        }

        try {
            for (Thread t : threads) {
                t.join();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        for (int i = 0; i < N; i++) {
            if (v[i] != i * 2) {
                System.out.println("Wrong answer");
                System.out.println(i);
                System.exit(1);
            }
        }
        System.out.println("Correct");
    }
}
