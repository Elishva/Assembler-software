; this is program check all command

;check mov
main: mov #4,r2
mov x,ww
mov e[0-10],r2
mov r3,r5

;check cmp
e: cmp #5,#-9
cmp len,#4095
cmp r4,r0
cmp main[4-14],#-4096
cmp #-4096,main[	3-7]
cmp #200,x


;check entry &extren
.entry x
.entry yy
.extern ww

;check add
add ww,x
add e[  0-10],r2
add r1,r7
len: add #4,r2

sub ww,x
sub e[0-10],r2
sub r1,r7
lntefr: sub #4,r1

not r5
not x

clr r3
clr x

x: lea x, len
lea x,r2

inc len
dec len
inc r5
dec r0

jmp yy
jmper: jmp r5

bne r5
bne jmper

red r3
red load
bdika: .string "tfdssas $%&_*()"
prn 	  yy[6-	9    ]
yy: prn r2
prn len
prn #780
.data -3,+5,7,-96,6,56,2314,-567
rr: jsr x
jsr r7

rts
end: stop

.entry bdika
.extern load
