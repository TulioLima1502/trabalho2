section .data
msg db 'Digite o seu nome:',0dH,0ah
msg_size EQU $-msg
boasvindas db 'Olá, ',0
boasvindas_size EQU $-boasvindas
boasvindas2 db ', bem vindo ao programa de CALC IA-32',0dH,0ah
boasvindas_size2 EQU $-boasvindas2

um db '1 ',0dh
dois db '2 ',0dh
tres db '3 ',0dh
quatro db '4 ',0dh
cinco db '5 ',0dh
seis db '6 ',0dh
seven db '7 ',0dh
oito db '8 ',0dh
nove db '9 ',0dh
zero db '0 ',0dh
menos db '- ',0dh

mensagem_soma db 'Vamos fazer a soma de dois números inteiros...',0dH,0ah
mensagem_soma_size EQU $-mensagem_soma
mensagem_subtracao db 'Vamos fazer a subtração de dois números inteiros...',0dH,0ah
mensagem_subtracao_size EQU $-mensagem_subtracao
mensagem_multiplicacao db 'Vamos fazer a multiplicação de dois números inteiros...',0dH,0ah
mensagem_multiplicacao_size EQU $-mensagem_multiplicacao
mensagem_divisao db 'Vamos fazer a divisão de dois números inteiros...',0dH,0ah
mensagem_divisao_size EQU $-mensagem_divisao
mensagem_mod db 'Vamos fazer o mod do dois números inteiros...',0dH,0ah
mensagem_mod_size EQU $-mensagem_mod

mensagem_resultado db 0dH,0ah,'O resultado da operação escolhida foi:',0dH,0ah
mensagem_resultado_size EQU $-mensagem_resultado


msg_primeiro_numero db 'Digite o primeiro valor:',0dH,0ah
msg_primeiro_numero_size EQU $-msg_primeiro_numero
msg_segundo_numero db 'Digite o segundo valor:',0dH,0ah
msg_segundo_numero_size EQU $-msg_segundo_numero


functions db 0dH,0ah,'ESCOLHA UMA OPÇÃO:',0dH,0ah,'- 1: SOMA',0dH,0ah,'- 2: SUBTRAÇÃO',0dH,0ah,'- 3: MULTIPLICAÇÃO',0dH,0ah,'- 4: DIVISÂO',0dH,0ah,'- 5: MOD',0dH,0ah,'- 6: SAIR',0dH,0ah
functions_size EQU $-functions

section .bss
nome resb 100
input_1 resb 15
i resb 15
number_1 resd 1
input_2 resb 15
number_2 resd 1
result resd 1
function resb 1
section .text
global _start

_start: push msg                ;faz o print da mensagem que pede para digitar o nome
        push msg_size
        call print

        ;mov eax,214
        ;push eax
        ;call print_int

        mov edx,100             ;espera e lê o nome digitado pelo usuário até o enter
        push nome
        push edx
        call read

        ;'imprime' na tela os caracteres referente ao 'Ola, '
        push boasvindas
        push boasvindas_size
        call print
        
        ;inicializando valores de registradores que serão utilizados na rotina a seguir
        mov eax,nome
        mov ebx,nome

        ;laço que conta a quantidade de caracteres inserido no nome, comparando cada byte com o valor de 0
        ;caso o valor no acumulador seja igual a zero, então o pulo é feito para a label contou, onde a diferença de endereços é encontrada
        ;REVISAR essa parte e a parte dos compare dentro de cada função 
        conta_caracteres: cmp byte [eax],0
        jz contou
        inc eax
        jmp conta_caracteres

        contou:
        sub eax,ebx
        sub eax,1

        push nome
        push eax
        call print

        push boasvindas2
        push boasvindas_size2
        call print

        pulo:

        cmp byte [function],10
        jz salto

        push functions
        push functions_size
        call print

        salto:                  ;salto para o caso o caracter enter ser reconhecido como um entrada

        ;espera e lê o nome digitado pelo usuário até o enter
        mov edx,1
        push function
        push edx
        call read

        cmp byte [function],54
        jz saida        ;sair do laço caso seja 6

        cmp byte [function],49
        jz soma
        
        cmp byte [function],50
        jz subtracao
        
        cmp byte [function],51
        jz multiplicacao
        
        cmp byte [function],52
        jz divisao
        
        cmp byte [function],53
        jz mod
        
        jmp pulo


;
;               SOMA
;

        soma:
        call zera_input
        push mensagem_soma
        push mensagem_soma_size
        call print

        
        push msg_primeiro_numero
        push msg_primeiro_numero_size
        call print

        new_soma_um:
        mov edx,15
        push input_1
        push edx
        call readdword

        cmp dword [input_1],10
        jz new_soma_um

        mov edx,15
        push input_1
        push edx
        call string_to_int1

        mov eax,[number_1]
        push eax
        call print_int

        push msg_segundo_numero
        push msg_segundo_numero_size
        call print

        new_soma_dois:
        mov edx,15
        push input_2
        push edx
        call readdword

        cmp dword [input_2],10
        jz new_soma_dois

        mov edx,15
        push input_2
        push edx
        call string_to_int2

        mov eax,[number_2]
        push eax
        call print_int

        push mensagem_resultado
        push mensagem_resultado_size
        call print

        mov eax,[number_1]
        mov ebx,[number_2]
        add eax,ebx
        mov DWORD [result],eax
        
        push eax
        call print_int

        jmp pulo

;
;               SUBTRAÇÃO
;

        subtracao:
        call zera_input
        
        ;mov dword [input_1],10
        ;mov dword [input_2],10
        
        push mensagem_subtracao
        push mensagem_subtracao_size
        call print
        
        push msg_primeiro_numero
        push msg_primeiro_numero_size
        call print
        
        new_subtracao_um:
        mov edx,15
        push input_1
        push edx
        call readdword
       
        cmp dword [input_1],10
        jz new_subtracao_um

        mov edx,15
        push input_1
        push edx
        call string_to_int1

        mov eax,[number_1]
        push eax
        call print_int

        push msg_segundo_numero
        push msg_segundo_numero_size
        call print
        
        new_subtracao_dois:
        mov edx,15
        push input_2
        push edx
        call readdword

        cmp dword [input_2],10
        jz new_subtracao_dois

        mov edx,15
        push input_2
        push edx
        call string_to_int2

        mov eax,[number_2]
        push eax
        call print_int

        push mensagem_resultado
        push mensagem_resultado_size
        call print

        mov eax,[number_1]
        mov ebx,[number_2]
        not ebx
        add ebx,1
        add eax,ebx
        mov DWORD [result],eax
        
        push eax
        call print_int

        jmp pulo


;
;               MULTIPLICAÇÃO
;


        multiplicacao:
        call zera_input

        ;mov dword [input_1],10
        ;mov dword [input_2],10
        
        push mensagem_multiplicacao
        push mensagem_multiplicacao_size
        call print

        push msg_primeiro_numero
        push msg_primeiro_numero_size
        call print
        
        new_multiplicacao_um:
        mov edx,15
        push input_1
        push edx
        call readdword

        cmp dword [input_1],10
        jz new_multiplicacao_um

        mov edx,15
        push input_1
        push edx
        call string_to_int1

        mov eax,[number_1]
        push eax
        call print_int

        push msg_segundo_numero
        push msg_segundo_numero_size
        call print
        
        new_multiplicacao_dois:
        mov edx,15
        push input_2
        push edx
        call readdword

        cmp dword [input_2],10
        jz new_multiplicacao_dois
        
        mov edx,15
        push input_2
        push edx
        call string_to_int2

        mov eax,[number_2]
        push eax
        call print_int

        push mensagem_resultado
        push mensagem_resultado_size
        call print

        mov eax,[number_1]
        mov ebx,[number_2]
        mul ebx
        mov DWORD [result],eax
        
        push eax
        call print_int

        jmp pulo


;
;               DIVISÂO
;


        divisao:
        call zera_input
        
        ;mov dword [input_1],10
        ;mov dword [input_2],10
        
        push mensagem_divisao
        push mensagem_divisao_size
        call print

        push msg_primeiro_numero
        push msg_primeiro_numero_size
        call print

        new_divisao_um:
        mov edx,15
        push input_1
        push edx
        call readdword

        cmp dword [input_1],10
        jz new_divisao_um

        mov edx,15
        push input_1
        push edx
        call string_to_int1

        mov eax,[number_1]
        push eax
        call print_int

        push msg_segundo_numero
        push msg_segundo_numero_size
        call print

        new_divisao_dois:
        mov edx,15
        push input_2
        push edx
        call readdword

        cmp dword [input_2],10
        jz new_divisao_dois

        mov edx,15
        push input_2
        push edx
        call string_to_int2

        mov eax,[number_2]
        push eax
        call print_int

        push mensagem_resultado
        push mensagem_resultado_size
        call print

        mov eax,[number_1]
        mov ebx,[number_2]
        cdq
        div ebx
        mov DWORD [result],eax

        push eax
        call print_int

        jmp pulo

;
;               MOD
;
        
        mod:
        call zera_input
        
        ;mov dword [input_1],10
        ;mov dword [input_2],10
        
        push mensagem_mod
        push mensagem_mod_size
        call print

        push msg_primeiro_numero
        push msg_primeiro_numero_size
        call print

        new_mod_um:
        mov edx,15
        push input_1
        push edx
        call readdword

        cmp dword [input_1],10
        jz new_mod_um

        mov edx,15
        push input_1
        push edx
        call string_to_int1

        mov eax,[number_1]
        push eax
        call print_int

        push msg_segundo_numero
        push msg_segundo_numero_size
        call print

        new_mod_dois:
        mov edx,15
        push input_2
        push edx
        call readdword
        
        cmp dword [input_2],10
        jz new_mod_dois

        mov edx,15
        push input_2
        push edx
        call string_to_int2

        mov eax,[number_2]
        push eax
        call print_int

        push mensagem_resultado
        push mensagem_resultado_size
        call print

        mov eax,[number_1]
        mov ebx,[number_2]
        cdq
        div ebx
        mov DWORD [result],edx

        push edx
        call print_int


        jmp pulo

        saida:
        mov eax,1
        mov ebx,0
        int 80h


        print: 
        push ebp
        mov ebp,esp
        push eax
        push ebx
        push ecx
        push edx
        mov eax,4
        mov ebx,1
        mov ecx,[ebp + 12]
        mov edx,[ebp + 8]
        int 80h
        pop edx
        pop ecx
        pop ebx
        pop eax
        pop ebp
        ret 4

        read:
        push ebp
        mov ebp,esp
        push eax
        push ebx
        push ecx
        push edx
        mov eax,3
        mov ebx,0
        mov ecx,[ebp + 12]
        mov edx,[ebp + 8]
        int 80h
        pop edx
        pop ecx
        pop ebx
        pop eax
        pop ebp
        ret 4

        zera_input:
        mov byte [input_1],0
        mov byte [input_2],0
        mov byte [input_1+1],0
        mov byte [input_2+1],0
        mov byte [input_1+2],0
        mov byte [input_2+2],0
        mov byte [input_1+3],0
        mov byte [input_2+3],0
        mov byte [input_1+4],0
        mov byte [input_2+4],0
        mov byte [input_1+5],0
        mov byte [input_2+5],0
        mov byte [input_1+6],0
        mov byte [input_2+6],0
        mov byte [input_1+7],0
        mov byte [input_2+7],0
        mov byte [input_1+8],0
        mov byte [input_2+8],0
        mov byte [input_1+9],0
        mov byte [input_2+9],0
        mov byte [input_1+10],0
        mov byte [input_2+10],0
        mov byte [input_1+11],0
        mov byte [input_2+11],0
        mov byte [input_1+12],0
        mov byte [input_2+12],0
        mov byte [input_1+13],0
        mov byte [input_2+13],0
        mov byte [input_1+14],0
        mov byte [input_2+14],0
        ret 4


        readdword:
        push ebp
        mov ebp,esp
        push eax
        push ebx
        push ecx
        push edx
        mov eax,3
        mov ebx,0
        mov ecx,[ebp + 12]
        mov edx,[ebp + 8]
        int 80h

        mov eax,[ebp + 12]
        mov ebx,[ebp + 12]

        conta_caracteres_1: cmp byte [eax],0
        jz contou_1
        inc eax
        jmp conta_caracteres_1

        contou_1:
        sub eax,ebx
        sub eax,1

        mov edx,eax
        mov eax,4
        mov ebx,1
        mov ecx,[ebp + 12]
        int 80h

        pop edx
        pop ecx
        pop ebx
        pop eax
        pop ebp

        ret 4

        print_int:
        push ebp
        mov ebp,esp
        push eax
        push ebx
        push ecx
        push edx

        mov eax,[ebp+8]
        cmp eax,0
        jge positivo
        mov eax,4
        mov ebx,1
        mov ecx,menos
        mov edx,1
        int 80h

        mov eax,[ebp+8]
        not eax
        positivo:

        ; fazer as 32 divisões no número e fazer um print em cada caso

        mov ecx,0

        loop:
        inc ecx
        mov edx,0
        mov esi,10
        idiv esi
        push edx
        cmp ecx,32
        jnz loop

        mov eax,0
        loop_print:
        inc eax
        pop edx
        mov ecx,edx
        push eax

        cmp ecx,1
        jne fim_um

        mov eax,4
        mov ebx,1
        mov ecx,um
        mov edx,1
        int 80h
        
        fim_um:
        cmp ecx,2
        jne fim_dois
        
        mov eax,4
        mov ebx,1
        mov ecx,dois
        mov edx,1
        int 80h

        fim_dois:
        cmp ecx,3
        jne fim_tres
        
        mov eax,4
        mov ebx,1
        mov ecx,tres
        mov edx,1
        int 80h

        fim_tres:
        cmp ecx,4
        jne fim_quatro
        
        mov eax,4
        mov ebx,1
        mov ecx,quatro
        mov edx,1
        int 80h

        fim_quatro:
        cmp ecx,5
        jne fim_cinco
        
        mov eax,4
        mov ebx,1
        mov ecx,cinco
        mov edx,1
        int 80h

        fim_cinco:
        cmp ecx,6
        jne fim_seis
        
        mov eax,4
        mov ebx,1
        mov ecx,seis
        mov edx,1
        int 80h

        fim_seis:
        cmp ecx,7
        jne fim_seven
        
        mov eax,4
        mov ebx,1
        mov ecx,seven
        mov edx,1
        int 80h

        fim_seven:
        cmp ecx,8
        jne fim_oito
        
        mov eax,4
        mov ebx,1
        mov ecx,oito
        mov edx,1
        int 80h

        fim_oito:
        cmp ecx,9
        jne fim_nove
        
        mov eax,4
        mov ebx,1
        mov ecx,nove
        mov edx,1
        int 80h

        fim_nove:
        cmp ecx,0
        jne fim_zero
        
        mov eax,4
        mov ebx,1
        mov ecx,zero
        mov edx,1
        int 80h

        fim_zero:

        pop eax
        cmp eax,32
        jne loop_print


        ; fazer um laço que conta 32 vezes e faz o print de alguma coisa
        ; 

        pop edx
        pop ecx
        pop ebx
        pop eax
        pop ebp

        ret 4


        string_to_int1:

        push ebp
        mov ebp,esp
        push eax
        push ebx
        push ecx
        push edx

        mov eax,[ebp+12]
        mov esi,eax
        mov eax,0
        mov ecx,0

        iteracao1:
        xor ebx,ebx
        mov bl,[esi + ecx]
        cmp bl,57
        jg acabou_iteracao1
        cmp bl,48
        jl acabou_iteracao1
        cmp bl,10
        je acabou_iteracao1
        cmp bl,13
        je acabou_iteracao1
        cmp bl,0
        jz acabou_iteracao1
        sub bl,48
        add eax,ebx
        mov ebx,10
        mul ebx
        inc ecx
        
        jmp iteracao1

        acabou_iteracao1:
        mov ebx,10
        div ebx
        mov DWORD [number_1],eax

        pop edx
        pop ecx
        pop ebx
        pop eax
        pop ebp

        ret 4


        string_to_int2:

        push ebp
        mov ebp,esp
        push eax
        push ebx
        push ecx
        push edx

        mov eax,[ebp+12]
        mov esi,eax
        mov eax,0
        mov ecx,0

        iteracao2:
        xor ebx,ebx
        mov bl,[esi + ecx]
        cmp bl,57
        jg acabou_iteracao2
        cmp bl,48
        jl acabou_iteracao2
        cmp bl,10
        je acabou_iteracao2
        cmp bl,13
        je acabou_iteracao2
        cmp bl,0
        jz acabou_iteracao2
        sub bl,48
        add eax,ebx
        mov ebx,10
        mul ebx
        inc ecx
        
        jmp iteracao2

        acabou_iteracao2:
        mov ebx,10
        div ebx
        mov DWORD [number_2],eax

        pop edx
        pop ecx
        pop ebx
        pop eax
        pop ebp

        ret 4

