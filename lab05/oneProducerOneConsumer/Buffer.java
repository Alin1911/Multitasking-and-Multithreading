package oneProducerOneConsumer;

public class Buffer {
    private int a = -1;

    void put(int value) {
        synchronized(this) {
            while(a > -1){
                try{
                    wait();
                } catch (Exception e){
                    System.out.println("Eroare la Producator");
                }
            }
            a = value;
            notify();
        }

    }

    int get() {
        int aux;
        synchronized (this) {
            while(a == -1){
                try{
                    wait();
                } catch (Exception e){
                    System.out.println("Eroare la Cumparator");
                }
            }
            aux = a;
            a = -1;
            notify();
            return aux;
        }
    }
}
