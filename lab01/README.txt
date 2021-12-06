Andrei Alin-Ionuț 333CC

ex 6
Pentru un vector de dimensiuni mici am observat ca un singur fir de
executie este suficient valorile fiind destul de apropiate, am 
comentat instructiunile de afisare pentru un timp mai bun,
pentru un vector de dimensiune mai mare folosirea mai multor fire de
executie devine eficienta

1 thread 
time ./a.out 10000000
real    0m0.086s
user    0m0.078s
sys     0m0.000s   

2 thread 
time ./a.out 10000000
real    0m0.075s
user    0m0.031s
sys     0m0.031s

4 thread 
time ./a.out 10000000
real    0m0.069s
user    0m0.094s
sys     0m0.031s