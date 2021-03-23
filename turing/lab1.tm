#! start S
#! end F
#! fill *

#! write ******010001011110

#поиск
S * * R S
S 1 0 R i
S 0 1 R i 
#инверсия
i 1 0 R i
i 0 1 R i
i * * L a
#прибавление единицы
a 1 0 L a
a 0 1 L F
a * * L F
