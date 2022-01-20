package multipleProducersMultipleConsumers;

public class Buffer {
    int []a;
    int index;

    Buffer() {
        a = new int[5];
        index = 0;
    }

    void put(int value) {
        synchronized(this) {
            while (index == 5) {
                try {
                    this.wait();
                }catch(Exception ex) {
                    System.out.println("Eroare la Producator");
                }
            }
            a[index] = value;
            index++;
            this.notifyAll();
        }
    }

    int get() {
        int aux;
        synchronized(this) {
            while (index == 0) {
                try {
                    this.wait();
                }catch(Exception ex) {
                    System.out.println("Eroare la Consumator");
                }
            }
            index--;
            aux = a[index];
            this.notifyAll();
            return aux;
        }
    }
}
