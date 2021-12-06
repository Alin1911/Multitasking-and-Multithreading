Andrei Alin-Ionut 333CC

===========
Ex 1  oets||
===========
Deci am avut o problema, am incercat sa implementez
cu while(!sorted) dar imi intra in bucla infinita undeva
dar pentru un singur thred mergea si nu am putut sa imi dau seama unde
e problema apoi am incercat cu (size_t j = 0; j < N; j++) si a
mers pe toate testele, nush de ce

varianta neparalelizata
time ./oets 100000 12
real    0m36.319s


varianta pralelizata
time ./oets 100000 12
real    0m8.702s 

============
Ex 2   oets||
============

Varianta paralelizata
time ./oets 100000 12
real    0m7.951s

Varianta neparalelizata
time ./oets 100000 12
real    0m35.277s

================
Ex 3 si 4 shear||
================


varianta paralelizata
time ./shear 10000 12
real    0m43.965s

varianta neparalelizata
time ./shear 10000 12
real    2m21.755s  :((

===========
Ex 5 merge||
===========


varianta pralelizata
time ./merge 65536 12
real    0m0.012s

time ./merge 131072 12
real    0m0.016s  

varianta neparalelizata
time ./merge 65536 12
real    0m0.014s

