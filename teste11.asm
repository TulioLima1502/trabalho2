MOD_A: BEGIN
Y: EXTERN
MOD_B: EXTERN
PUBLIC VAL
PUBLIC L1
SECTION TEXT
INPUT Y
LOAD VAL
ADD Y
STORE Y
JMPP MOD_B
SECTION DATA
L1: STOP
VAL: CONST 5
END