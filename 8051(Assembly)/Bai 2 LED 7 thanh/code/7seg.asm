//  ; hien thi 1 so
//  ; dem tu 1 -9 
//num equ p0
//led bit p1.0
//
//org 00h
//main:
//mov p1,#000h
//reset:
//mov r1,#0
//loop:
//mov dptr,#ma7seg
//mov a,r1;
//movc a, @a+dptr
//mov num,a
//setb led
//lcall delay
//inc r1
//cjne r1,#10	,loop
//sjmp reset
//
//
//delay:
//mov r2,#16	 ; 16 <=> 1s
//loop1: mov r3,#250
//loop2: mov r4,#250
//djnz r4,$
//djnz r3,loop2
//djnz r2,loop1
//ret
//
//
//ma7seg:
//db 03fh,006h,05bh,04fh,066h,06dh,07dh,007h,07fh,06fh
//end



//; hien hai so57
//; start
//;===========================
//num equ p0
//donvi bit p1.0
//chuc bit p1.1
//org 00h
//main: ;hien so 75 theo pp quet led
//mov p1,#000h
//mov dptr,#ma7seg
//setb donvi ;enable 7seg no1
//mov a,#5
//movc a,@a+dptr
//mov num,a ;so 5
//lcall delay ;delay 250us
//clr donvi ;disable 7seg no1
//setb chuc
//mov a,#7
//movc a,@a+dptr
//mov num,a ;so 7
//lcall delay
//clr chuc
//sjmp main
//delay:
//mov r7,#250
//djnz r7,$
//ret
//ma7seg:
//db 03fh,006h,05bh,04fh,066h,06dh,07dh,007h,07fh,06fh
//end






;======================
; dem tu 1 den 99
;======================
num equ p0
donvi bit p1.0
chuc bit p1.1
org 00h
main:
mov p1,#000h
mov r5,#100 ;1 so quet 100 lan
reset:
mov R6,#0
loop:
mov a,r6
cjne a,#100,skip ;dem len 100 thi quay ve 0
sjmp reset
skip:
mov b,#10
div ab ;tach chu so hang chuc luu torng a, don vi luu trong b
mov dptr,#ma7seg

setb chuc;enable 7seg no1
movc a,@a+dptr
mov num,a ;chu so hang chuc
lcall delay;delay 250us
clr chuc ;disable 7seg no1

setb donvi
mov a,b
movc a,@a+dptr
mov num,a ; chu so hang don vi
lcall delay
clr donvi
djnz r5,loop ; r5=0 tang r6 len 1
inc r6
mov r5,#100
sjmp loop
;==================
delay:
mov r2,#16	 
loop1: mov r3,#250			   // chua tinh ro dc thoi gian tre??
djnz r3,$
djnz r2,loop1
ret
;=================
ma7seg:
db 03fh,006h,05bh,04fh,066h,06dh,07dh,007h,07fh,06fh
end