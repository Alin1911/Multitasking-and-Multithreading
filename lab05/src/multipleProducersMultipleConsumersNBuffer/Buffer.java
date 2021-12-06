package multipleProducersMultipleConsumersNBuffer;

import java.util.Queue;
import java.util.concurrent.Semaphore;

public class Buffer {

    Queue<Integer> queue;
    static Semaphore gol;
    static Semaphore plin = new Semaphore(0);
    static Semaphore mutex = new Semaphore(1);


    public Buffer(int size) {
        queue = new LimitedQueue<>(size);
        gol = new Semaphore(size);
    }

    public void put(int value) {
        try {
            gol.acquire();
        } catch (InterruptedException e) { }
        synchronized (this) {
            try {
                mutex.acquire();
            } catch (Exception e) {
            }

            queue.add(value);

            mutex.release();
        }
        plin.release();
    }

	public int get() {
        int a = -1;
        try {
            plin.acquire();
        } catch (InterruptedException e) { }
        synchronized (this) {
            Integer result = queue.poll();
            if (result != null) {
                a = result;
            }
        }
        mutex.release();
        gol.release();
        return a;
    }
}
