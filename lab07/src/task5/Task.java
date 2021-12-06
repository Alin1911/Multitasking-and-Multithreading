package task5;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.RecursiveTask;

public class Task extends RecursiveTask<Void> {
    private final int[] colors;
    private final int step;

    public Task(int[] colors, int step) {
        this.colors = colors;
        this.step = step;
    }

    @Override
    protected Void compute() {
        if (step == Main.N) {
            Main.printColors(colors);
            return null;
        }
        // for the node at position step try all possible colors
        List<Task> taskList = new ArrayList<>();
        for (int i = 0; i < Main.COLORS; i++) {
            int[] newColors = colors.clone();
            newColors[step] = i;
            if (Main.verifyColors(newColors, step)) {
                //colorGraph(newColors, step + 1);
                Task task = new Task(newColors, step + 1);
                taskList.add(task);
                task.fork();
            }
        }
        for (Task t : taskList)
            t.join();
        return null;
    }
}
