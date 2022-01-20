package task2;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

class MyRunnable implements Runnable{
    private final ExecutorService executorService;
    private  final AtomicInteger count;
    private final int[] colors;
    private final int step;

    MyRunnable(ExecutorService executorService, AtomicInteger count, int[] colors, int step) {
        this.executorService = executorService;
        this.count = count;
        this.colors = colors;
        this.step = step;
    }


    @Override
    public void run() {
        if (step == Main.N) {
            Main.printColors(colors);
            int taskLeft = count.decrementAndGet();
            if (taskLeft == 0)
                executorService.shutdown();
        }

        // for the node at position step try all possible colors
        for (int i = 0; i < Main.COLORS; i++) {
            int[] newColors = colors.clone();
            newColors[step] = i;
            if (Main.verifyColors(newColors, step)) {
                count.incrementAndGet();
                executorService.submit(new MyRunnable(executorService, count, newColors, step + 1));
            }
        }
        int taskLeft = count.decrementAndGet();
        if (taskLeft == 0)
            executorService.shutdown();
    }
}
