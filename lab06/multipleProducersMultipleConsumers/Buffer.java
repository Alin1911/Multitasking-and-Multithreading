package multipleProducersMultipleConsumers;

import java.util.concurrent.ArrayBlockingQueue;

public class Buffer {
	private ArrayBlockingQueue<Integer> lista;

	Buffer(){
		lista = new ArrayBlockingQueue<>(5, true);
	}

	void put(int value) {
		try{
			lista.put(value);
		} catch (Exception e){
			System.out.println(e.getMessage());
		}

	}
	int get() {
		int aux = -1;
		try{
			aux = lista.take();
		} catch (Exception e){
			System.out.println(e.getMessage());
		}

		return aux;
	}
}
