package ex1;

import java.util.ArrayList;

class Run_thred implements Runnable {
    private int id;

    public Run_thred(int id_thred) {
        id = id_thred;
    }

    @Override
    public void run() {
        System.out.println("#" + id + " Hello, world");
    }
}

public class Main {
    public static void main(String[] args) {

        int N = Runtime.getRuntime().availableProcessors();
        ArrayList<Thread> threads = new ArrayList<>();

        for (int id = 0; id < N; id++) {
            threads.add(new Thread(new Run_thred(id)));
            threads.get(id).start();
        }
        try {
            for (int id = 0; id < N; id++)
                threads.get(id).join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
