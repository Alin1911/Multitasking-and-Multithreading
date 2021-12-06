package task1;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

class MyRunnable implements Runnable{
    private final ExecutorService executorService;
    private  final AtomicInteger count;
    private final ArrayList<Integer> partialPath;
    private final int destination;

    public MyRunnable(ExecutorService executorService, AtomicInteger count, ArrayList<Integer> partialPath, int destination) {
        this.executorService = executorService;
        this.count = count;
        this.partialPath = partialPath;
        this.destination = destination;
    }


    @Override
    public void run() {
        if (partialPath.get(partialPath.size() - 1) == destination) {
            System.out.println(partialPath);
            int taskLeft = count.decrementAndGet();
            if (taskLeft == 0)
                executorService.shutdown();
        }

        // se verifica nodurile pentru a evita ciclarea in graf
        int lastNodeInPath = partialPath.get(partialPath.size() - 1);
        for (int[] ints : Main.graph) {
            if (ints[0] == lastNodeInPath) {
                if (partialPath.contains(ints[1]))
                    continue;
                ArrayList<Integer> newPartialPath = new ArrayList<>(partialPath);
                newPartialPath.add(ints[1]);
                count.incrementAndGet();
                executorService.submit(new MyRunnable(executorService, count, newPartialPath, destination));
            }
        }
        int taskLeft = count.decrementAndGet();
        if (taskLeft == 0)
            executorService.shutdown();
    }
}
