package task6;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.RecursiveTask;

class Task extends RecursiveTask<Void> {
    private  final int[] graph;
    private  final int step;

    Task(int[] graph, int step) {
        this.graph = graph;
        this.step = step;
    }

    @Override
    protected Void compute() {
        if (Main.N == step) {
            Main.printQueens(graph);
            return null;
        }
        List<Task> taskList = new ArrayList<>();
        for (int i = 0; i < Main.N; ++i) {
            int[] newGraph = graph.clone();
            newGraph[step] = i;

            if (Main.check(newGraph, step)) {
//                queens(newGraph, step + 1);
                Task task = new Task(newGraph, step + 1);
                taskList.add(task);
                task.fork();
            }
        }
        for (Task t : taskList)
            t.join();
        return null;
    }
}
