swap: EQU 0
teste: EQU 1
testando: EQU 0
SECTION TEXT       ;uwdhcausbdcisip saudchuihsabdcu9bas 
INPUT           OLD_DATA           ;ichauhc
            LOAD OLD_DATA  ;eae manin
L1: DIV     DOIS   ;uasdchuashbdcosanoidcnb
STORE NEW_DATA ;wiudchuhdc9uhsd9ufch
SWAP: MACRO &A
;testeEquDpsSec: EQU 5
MULT        DOIS;iuech9uhdcu udcuducohusnd;dushc9usehc
STORE     TMP_DATA;0iechu0ueh0icuhes0i
STORE     MP_DATA;0iechu0ueh0icuhes0i
ENDMACRO
OIE: MACRO &A
LOAD OLD_DATA
IF testando
SUB TMP_DATA
STORE TMP_DATA
JMP testezao
OUTPUT TMP_DATA
ENDMACRO
OIE X
;testando: EQU 0
testezao: ADD OLD_DATA
SWAP W
IF     teste
    COPY DOIS,OLD_DATA
LOAD      OLD_DATA: 
JMPP L1
STOP
SECTION DATA
DOIS: CONST 10
OLD_DATA: SPACE 16
NEW_DATA: SPACE 2 
