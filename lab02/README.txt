Andrei Alin-Ionuț 333CC

time ./multiply_seq 1000
real    0m18.850s

time ./multiply_outer 1000 12
real    0m13.039s

time ./multiply_middle 1000 12
real    0m12.832s

time ./multiply_inner 1000 12
real    2m36.363s

Am folosit un mutex la multiply_inner deoarece
avem 2 de k in operatia a[i][k] * b[k][j] iar unul ar putea sa ramana cu
valaoarea de la alt thread, totusi un mutex ne costa destul de multa procesare
iar timpul l-a depasit pe cel al algoritmului secvential

ex6
am folosit bariera pentru matricele cu M deoarece am nevoie de ca toate sa
fie gata cand voi trece la calcularea C11, C12, C21, C22

time ./strassen 1000 
real    0m16.242s 


time ./strassen_par 1000 
real    0m11.661s