package synchronizedSortedList;

import java.util.Collections;
import java.util.List;
import java.util.concurrent.Semaphore;

public class Sort extends Thread {
    private final List<Integer> list;
    private Semaphore sem;
    public Sort(List<Integer> list, Semaphore sem1) {
        super();
        this.sem = sem1;
        this.list = list;
    }

    @Override
    public void run() {
        try{
            sem.acquire();
        } catch (Exception e){
            System.out.println(e.getMessage());
        }
        Collections.sort(list);
    }
}
