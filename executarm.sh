## compilando e executando o trabalho de maneira automatizada
g++ -std=c++11 trabalho2.cpp -o trabalho2
echo '
*****
Compilando bin.asm'
sleep 5
./trabalho2 -m bin bin
echo '
*****
Compilando triangulo.asm'
sleep 5
./trabalho2 -m triangulo triangulo
echo '
*****
Compilando test_basico.asm'
sleep 5
./trabalho2 -m test_basico test_basico
echo '
*****
Compilando test_erro_assembler.asm'
sleep 5
./trabalho2 -m test_erro_assembler test_erro_assembler
echo '
*****
Compilando test_erro_lexer.asm'
sleep 5
./trabalho2 -m test_erro_lexer test_erro_lexer
echo '
*****
Compilando test_vector_plus.asm'
sleep 5
./trabalho2 -m test_vector_plus test_vector_plus
