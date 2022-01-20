package task3;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class MyRunnable implements Runnable{
    private final ExecutorService executorService;
    private  final AtomicInteger count;
    private  final int[] graph;
    private  final int step;

    public MyRunnable(ExecutorService executorService, AtomicInteger count, int[] graph, int step) {
        this.executorService = executorService;
        this.count = count;
        this.graph = graph;
        this.step = step;
    }

    @Override
    public void run() {
        if (Main.N == step) {
            Main.printQueens(graph);
            int taskLeft = count.decrementAndGet();
            if (taskLeft == 0)
                executorService.shutdown();
        }
        for (int i = 0; i < Main.N; ++i) {
            int[] newGraph = graph.clone();
            newGraph[step] = i;

            if (Main.check(newGraph, step)) {
                //queens(newGraph, step + 1);
                count.incrementAndGet();
                executorService.submit(new MyRunnable(executorService, count, newGraph,  step + 1));
            }
        }
        int taskLeft = count.decrementAndGet();
        if (taskLeft == 0)
            executorService.shutdown();
    }
}
